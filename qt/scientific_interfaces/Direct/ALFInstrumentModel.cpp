// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2022 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#include "ALFInstrumentModel.h"

#include "MantidAPI/Algorithm.h"
#include "MantidAPI/AlgorithmManager.h"
#include "MantidAPI/AlgorithmProperties.h"
#include "MantidAPI/AnalysisDataService.h"
#include "MantidAPI/ExperimentInfo.h"
#include "MantidAPI/MatrixWorkspace.h"
#include "MantidAPI/NumericAxis.h"
#include "MantidAPI/Workspace.h"
#include "MantidAPI/WorkspaceOpOverloads.h"
#include "MantidGeometry/Instrument.h"
#include "MantidGeometry/Instrument/ComponentInfo.h"
#include "MantidGeometry/Instrument/DetectorInfo.h"
#include "MantidKernel/Unit.h"
#include "MantidQtWidgets/InstrumentView/InstrumentActor.h"

#include <memory>
#include <utility>

using namespace Mantid::API;

namespace {
auto &ADS = AnalysisDataService::Instance();

std::string const NOT_IN_ADS = "not_stored_in_ads";

bool isAxisDSpacing(MatrixWorkspace_const_sptr const &workspace) {
  return workspace->getAxis(0)->unit()->unitID() == "dSpacing";
}

std::optional<double> getTwoTheta(Mantid::Geometry::Instrument_const_sptr instrument,
                                  Mantid::Geometry::IDetector_const_sptr detector) {
  if (!instrument || !detector) {
    return std::nullopt;
  }
  auto const sample = instrument->getSample()->getPos();
  auto const source = instrument->getSource()->getPos();

  return detector->getTwoTheta(sample, sample - source) * 180.0 / M_PI;
}

void appendTwoThetaClosestToZero(std::vector<double> &twoThetas,
                                 std::optional<std::pair<double, std::size_t>> &workspaceIndexClosestToZero,
                                 MatrixWorkspace_const_sptr const &workspace,
                                 Mantid::Geometry::Instrument_const_sptr instrument) {
  if (workspaceIndexClosestToZero) {
    auto const twoTheta = getTwoTheta(instrument, workspace->getDetector((*workspaceIndexClosestToZero).second));
    if (twoTheta) {
      twoThetas.emplace_back(*twoTheta);
    }
    workspaceIndexClosestToZero = std::nullopt;
  }
}

double calculateError(Mantid::HistogramData::HistogramE const &eValues, std::size_t const binIndexMin,
                      std::size_t const binIndexMax) {
  // Calculate the squared values
  std::vector<double> squaredValues;
  squaredValues.reserve(binIndexMax - binIndexMin);
  std::transform(eValues.cbegin() + binIndexMin, eValues.cbegin() + binIndexMax, eValues.cbegin() + binIndexMin,
                 squaredValues.begin(), std::multiplies<double>());
  // Sum them and then take the sqrt
  return sqrt(std::accumulate(squaredValues.begin(), squaredValues.end(), 0.0));
}

double calculateYCounts(Mantid::HistogramData::HistogramY const &yValues, std::size_t const binIndexMin,
                        std::size_t const binIndexMax) {
  return std::accumulate(yValues.cbegin() + binIndexMin, yValues.cbegin() + binIndexMax, 0.0);
}

double calculateOutOfPlaneAngle(Mantid::Kernel::V3D const &pos, Mantid::Kernel::V3D const &origin,
                                Mantid::Kernel::V3D const &normal) {
  auto const vec = normalize(pos - origin);
  return asin(vec.scalar_prod(normal));
}

void loadEmptyInstrument(std::string const &instrumentName, std::string const &outputName) {
  auto alg = AlgorithmManager::Instance().create("LoadEmptyInstrument");
  alg->initialize();
  alg->setProperty("InstrumentName", instrumentName);
  alg->setProperty("OutputWorkspace", outputName);
  alg->execute();
}

MatrixWorkspace_sptr createWorkspace(std::string const &parentName, std::vector<double> const &x,
                                     std::vector<double> const &y, std::vector<double> const &e,
                                     std::size_t const numberOfSpectra, std::string const &unitX) {
  auto alg = AlgorithmManager::Instance().create("CreateWorkspace");
  alg->initialize();
  alg->setAlwaysStoreInADS(false);
  alg->setProperty("ParentWorkspace", parentName);
  alg->setProperty("DataX", x);
  alg->setProperty("DataY", y);
  alg->setProperty("DataE", e);
  alg->setProperty("NSpec", static_cast<int>(numberOfSpectra));
  alg->setProperty("UnitX", unitX);
  alg->setPropertyValue("OutputWorkspace", NOT_IN_ADS);
  alg->execute();
  MatrixWorkspace_sptr outputWorkspace = alg->getProperty("OutputWorkspace");
  return outputWorkspace;
}

MatrixWorkspace_sptr rebunch(MatrixWorkspace_sptr const &inputWorkspace, std::size_t const nBunch) {
  auto alg = Mantid::API::AlgorithmManager::Instance().create("Rebunch");
  alg->initialize();
  alg->setAlwaysStoreInADS(false);
  alg->setProperty("InputWorkspace", inputWorkspace);
  alg->setProperty("NBunch", static_cast<int>(nBunch));
  alg->setProperty("OutputWorkspace", NOT_IN_ADS);
  alg->execute();
  MatrixWorkspace_sptr outputWorkspace = alg->getProperty("OutputWorkspace");
  return outputWorkspace;
}

MatrixWorkspace_sptr convertUnits(MatrixWorkspace_sptr const &inputWorkspace, std::string const &target) {
  auto alg = AlgorithmManager::Instance().create("ConvertUnits");
  alg->initialize();
  alg->setAlwaysStoreInADS(false);
  alg->setProperty("InputWorkspace", inputWorkspace);
  alg->setProperty("Target", target);
  alg->setProperty("OutputWorkspace", NOT_IN_ADS);
  alg->execute();
  MatrixWorkspace_sptr outputWorkspace = alg->getProperty("OutputWorkspace");
  return outputWorkspace;
}

MatrixWorkspace_sptr rebinToWorkspace(MatrixWorkspace_sptr const &workspaceToRebin,
                                      MatrixWorkspace_sptr const &workspaceToMatch) {
  auto alg = AlgorithmManager::Instance().create("RebinToWorkspace");
  alg->initialize();
  alg->setAlwaysStoreInADS(false);
  alg->setProperty("WorkspaceToRebin", workspaceToRebin);
  alg->setProperty("WorkspaceToMatch", workspaceToMatch);
  alg->setProperty("OutputWorkspace", NOT_IN_ADS);
  alg->execute();
  MatrixWorkspace_sptr outputWorkspace = alg->getProperty("OutputWorkspace");
  return outputWorkspace;
}

MatrixWorkspace_sptr scaleX(MatrixWorkspace_sptr const &inputWorkspace, double const factor) {
  auto alg = AlgorithmManager::Instance().create("ScaleX");
  alg->initialize();
  alg->setAlwaysStoreInADS(false);
  alg->setProperty("InputWorkspace", inputWorkspace);
  alg->setProperty("Factor", factor);
  alg->setProperty("OutputWorkspace", NOT_IN_ADS);
  alg->execute();
  MatrixWorkspace_sptr outputWorkspace = alg->getProperty("OutputWorkspace");
  return outputWorkspace;
}

MatrixWorkspace_sptr replaceSpecialValues(MatrixWorkspace_sptr const &inputWorkspace) {
  auto alg = AlgorithmManager::Instance().create("ReplaceSpecialValues");
  alg->initialize();
  alg->setAlwaysStoreInADS(false);
  alg->setProperty("InputWorkspace", inputWorkspace);
  alg->setProperty("InfinityValue", 0.0);
  alg->setProperty("NaNValue", 1.0);
  alg->setProperty("CheckErrorAxis", true);
  alg->setProperty("OutputWorkspace", NOT_IN_ADS);
  alg->execute();
  MatrixWorkspace_sptr outputWorkspace = alg->getProperty("OutputWorkspace");
  return outputWorkspace;
}

} // namespace

namespace MantidQt::CustomInterfaces {

ALFInstrumentModel::ALFInstrumentModel() : m_sample(), m_vanadium(), m_tubes() {
  loadEmptyInstrument("ALF", loadedWsName());
}

std::unique_ptr<AlgorithmRuntimeProps> ALFInstrumentModel::rebinToWorkspaceProperties() const {
  auto properties = std::make_unique<AlgorithmRuntimeProps>();
  AlgorithmProperties::update("WorkspaceToRebin", m_vanadium, *properties);
  AlgorithmProperties::update("WorkspaceToMatch", m_sample, *properties);
  AlgorithmProperties::update("OutputWorkspace", NOT_IN_ADS, *properties);
  return std::move(properties);
}

std::unique_ptr<AlgorithmRuntimeProps> ALFInstrumentModel::replaceSpecialValuesProperties() const {
  auto properties = std::make_unique<AlgorithmRuntimeProps>();
  AlgorithmProperties::update("InputWorkspace", m_sample / m_vanadium, *properties);
  AlgorithmProperties::update("InfinityValue", 0.0, *properties);
  AlgorithmProperties::update("NaNValue", 1.0, *properties);
  AlgorithmProperties::update("CheckErrorAxis", true, *properties);
  AlgorithmProperties::update("OutputWorkspace", NOT_IN_ADS, *properties);
  return std::move(properties);
}

std::unique_ptr<AlgorithmRuntimeProps>
ALFInstrumentModel::convertUnitsProperties(MatrixWorkspace_sptr const &inputWorkspace) const {
  auto properties = std::make_unique<AlgorithmRuntimeProps>();
  AlgorithmProperties::update("InputWorkspace", inputWorkspace, *properties);
  AlgorithmProperties::update("Target", "dSpacing", *properties);
  AlgorithmProperties::update("OutputWorkspace", NOT_IN_ADS, *properties);
  return std::move(properties);
}

/*
 * Normalises the sample by the vanadium if a vanadium has been loaded. Converts the result to dSpacing if it is not
 * already in these units. Adds the resulting workspace to the ADS.
 */
void ALFInstrumentModel::generateLoadedWorkspace() {
  if (!m_sample) {
    return;
  }

  // Rebin the vanadium to match the sample binning if the bins do not match
  if (m_vanadium && !WorkspaceHelpers::matchingBins(*m_sample, *m_vanadium)) {
    m_vanadium = rebinToWorkspace(m_vanadium, m_sample);
  }

  // Normalise the sample using the vanadium if it is provided
  auto const normalised = m_vanadium ? replaceSpecialValues(m_sample / m_vanadium) : m_sample;
  // Convert the normalisation result to dSpacing, and add it to the ADS
  auto const normalisedDSpacing = !isAxisDSpacing(normalised) ? convertUnits(normalised, "dSpacing") : normalised;
  ADS.addOrReplace(loadedWsName(), normalisedDSpacing);
}

void ALFInstrumentModel::setWorkspace(ALFDataSwitch const &dataSwitch, MatrixWorkspace_sptr const &workspace) {
  switch (dataSwitch) {
  case ALFDataSwitch::SAMPLE:
    setSample(workspace);
    return;
  case ALFDataSwitch::VANADIUM:
    setVanadium(workspace);
    return;
  }
  throw std::invalid_argument("ALFDataSwitch must be one of { SAMPLE, VANADIUM }");
}

void ALFInstrumentModel::setSample(MatrixWorkspace_sptr const &sample) {
  auto const sampleRemoved = m_sample && !sample;
  m_sample = sample;
  if (sampleRemoved) {
    loadEmptyInstrument("ALF", loadedWsName());
  }
}

void ALFInstrumentModel::setVanadium(MatrixWorkspace_sptr const &vanadium) { m_vanadium = vanadium; }

std::size_t ALFInstrumentModel::run(ALFDataSwitch const &dataSwitch) const {
  switch (dataSwitch) {
  case ALFDataSwitch::SAMPLE:
    return runNumber(m_sample);
  case ALFDataSwitch::VANADIUM:
    return runNumber(m_vanadium);
  }
  throw std::invalid_argument("ALFDataSwitch must be one of { SAMPLE, VANADIUM }");
}

std::size_t ALFInstrumentModel::runNumber(Mantid::API::MatrixWorkspace_sptr const &workspace) const {
  if (!workspace) {
    return 0u;
  }
  return static_cast<std::size_t>(workspace->getRunNumber());
}

bool ALFInstrumentModel::setSelectedTubes(std::vector<DetectorTube> tubes) {
  // If the number of tubes is different then we definitely need to update the stored tubes
  if (tubes.size() != m_tubes.size()) {
    m_tubes = std::move(tubes);
    return true;
  }

  // Check if a new tube exists in the provided tubes
  const auto hasNewTube =
      std::any_of(tubes.cbegin(), tubes.cend(), [&](const auto &tube) { return !tubeExists(tube); });
  if (hasNewTube) {
    m_tubes = std::move(tubes);
  }
  return hasNewTube;
}

bool ALFInstrumentModel::addSelectedTube(DetectorTube const &tube) {
  auto const isNewTube = !tubeExists(tube);
  if (isNewTube) {
    m_tubes.emplace_back(tube);
  }

  return isNewTube;
}

bool ALFInstrumentModel::tubeExists(DetectorTube const &tube) const {
  return std::find(m_tubes.cbegin(), m_tubes.cend(), tube) != m_tubes.cend();
}

std::unique_ptr<AlgorithmRuntimeProps>
ALFInstrumentModel::createWorkspaceAlgorithmProperties(MantidQt::MantidWidgets::IInstrumentActor const &actor) const {
  std::vector<double> x, y, e;
  // collectXAndYData(actor, x, y, e, twoThetas);

  auto properties = std::make_unique<AlgorithmRuntimeProps>();
  AlgorithmProperties::update("ParentWorkspace", actor.getWorkspace()->getName(), *properties);
  AlgorithmProperties::update("DataX", x, *properties);
  AlgorithmProperties::update("DataY", y, *properties);
  AlgorithmProperties::update("DataE", e, *properties);
  AlgorithmProperties::update("NSpec", 1, *properties);
  AlgorithmProperties::update("UnitX", "Out of plane angle", *properties);
  AlgorithmProperties::update("OutputWorkspace", NOT_IN_ADS, *properties);
  return std::move(properties);
}

std::tuple<MatrixWorkspace_sptr, std::vector<double>>
ALFInstrumentModel::generateOutOfPlaneAngleWorkspace(MantidQt::MantidWidgets::IInstrumentActor const &actor) const {
  std::vector<double> twoThetas;
  if (m_tubes.empty()) {
    return {nullptr, twoThetas};
  }

  std::vector<double> x, y, e;
  collectXAndYData(actor, x, y, e, twoThetas);

  // Create workspace containing the out of plane angle data in one spectrum
  auto workspace = createWorkspace(actor.getWorkspace()->getName(), x, y, e, 1u, "Out of plane angle");
  // Convert x axis from radians to degrees
  workspace = scaleX(workspace, 180.0 / M_PI);
  // Rebin and average the workspace based on the number of tubes that are selected
  workspace = rebunch(workspace, numberOfTubes());
  return {workspace, twoThetas};
}

void ALFInstrumentModel::collectXAndYData(MantidQt::MantidWidgets::IInstrumentActor const &actor,
                                          std::vector<double> &x, std::vector<double> &y, std::vector<double> &e,
                                          std::vector<double> &twoThetas) const {
  auto const &componentInfo = actor.componentInfo();
  auto const &detectorInfo = actor.detectorInfo();

  Mantid::API::MatrixWorkspace_const_sptr workspace = actor.getWorkspace();

  // Collect and sort Y and E values by X
  std::map<double, double> xymap, xemap;
  collectAndSortYByX(xymap, xemap, twoThetas, actor, workspace, componentInfo, detectorInfo);

  x.reserve(xymap.size());
  y.reserve(xymap.size());
  e.reserve(xemap.size());
  std::transform(xymap.cbegin(), xymap.cend(), std::back_inserter(x), [](auto const &xy) { return xy.first; });
  std::transform(xymap.cbegin(), xymap.cend(), std::back_inserter(y), [](auto const &xy) { return xy.second; });
  std::transform(xemap.cbegin(), xemap.cend(), std::back_inserter(e), [](auto const &xe) { return xe.second; });
}

void ALFInstrumentModel::collectAndSortYByX(std::map<double, double> &xy, std::map<double, double> &xe,
                                            std::vector<double> &twoThetas,
                                            MantidQt::MantidWidgets::IInstrumentActor const &actor,
                                            MatrixWorkspace_const_sptr const &workspace,
                                            Mantid::Geometry::ComponentInfo const &componentInfo,
                                            Mantid::Geometry::DetectorInfo const &detectorInfo) const {
  auto const nDetectorsPerTube = m_tubes.front().size();
  auto const samplePosition = componentInfo.samplePosition();
  auto const instrument = actor.getInstrument();

  Mantid::Kernel::V3D normal;
  std::size_t imin, imax;
  std::optional<std::pair<double, std::size_t>> workspaceIndexClosestToZeroX = std::nullopt;
  for (auto const &tubeDetectorIndices : m_tubes) {
    for (auto i = 0u; i < tubeDetectorIndices.size(); ++i) {
      auto const detectorIndex = tubeDetectorIndices[i];
      auto const workspaceIndex = actor.getWorkspaceIndex(detectorIndex);

      if (i % nDetectorsPerTube == 0) {
        normal = normalize(componentInfo.position(tubeDetectorIndices[i + 1]) - componentInfo.position(detectorIndex));
        actor.getBinMinMaxIndex(workspaceIndex, imin, imax);
        appendTwoThetaClosestToZero(twoThetas, workspaceIndexClosestToZeroX, workspace, instrument);
      }

      if (workspaceIndex != MantidQt::MantidWidgets::InstrumentActor::INVALID_INDEX &&
          componentInfo.isDetector(detectorIndex)) {

        auto const xValue = calculateOutOfPlaneAngle(detectorInfo.position(detectorIndex), samplePosition, normal);
        xy[xValue] = calculateYCounts(workspace->y(workspaceIndex), imin, imax);
        xe[xValue] = calculateError(workspace->e(workspaceIndex), imin, imax);

        auto const absXValue = std::abs(xValue);
        if (!workspaceIndexClosestToZeroX || absXValue < (*workspaceIndexClosestToZeroX).first) {
          workspaceIndexClosestToZeroX = std::make_pair(absXValue, workspaceIndex);
        }
      }
    }
  }
  appendTwoThetaClosestToZero(twoThetas, workspaceIndexClosestToZeroX, workspace, instrument);
}

} // namespace MantidQt::CustomInterfaces
