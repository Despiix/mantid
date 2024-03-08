// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include "Analysis/IDAFunctionParameterEstimation.h"
#include "Analysis/ParameterEstimation.h"
#include "DllConfig.h"
#include "MantidAPI/IFunction_fwd.h"
#include "MantidAPI/ITableWorkspace_fwd.h"
#include "MantidQtWidgets/Common/FunctionModel.h"

#include <map>

#include <boost/optional.hpp>

namespace MantidQt {
namespace CustomInterfaces {
namespace IDA {

using namespace Mantid::API;
using namespace MantidWidgets;

class MANTIDQT_INELASTIC_DLL IqtFunctionModel : public IFunctionModel {
public:
  IqtFunctionModel();
  void setFunction(IFunction_sptr fun) override;
  IFunction_sptr getFullFunction() const override;
  IFunction_sptr getFitFunction() const override;
  bool hasFunction() const override;
  void addFunction(std::string const &prefix, std::string const &funStr) override;
  void removeFunction(std::string const &prefix) override;
  void setParameter(std::string const &parameterName, double value) override;
  void setParameterError(std::string const &parameterName, double value) override;
  double getParameter(std::string const &parameterName) const override;
  double getParameterError(std::string const &parameterName) const override;
  std::string getParameterDescription(std::string const &parameterName) const override;
  std::vector<std::string> getParameterNames() const override;
  IFunction_sptr getSingleFunction(int index) const override;
  IFunction_sptr getCurrentFunction() const override;
  void setNumberDomains(int) override;
  void setDatasets(const QList<FunctionModelDataset> &datasets) override;
  QStringList getDatasetNames() const override;
  QStringList getDatasetDomainNames() const override;
  int getNumberDomains() const override;
  void setCurrentDomainIndex(int i) override;
  int currentDomainIndex() const override;
  void changeTie(std::string const &parameterName, std::string const &tie) override;
  void addConstraint(std::string const &functionIndex, std::string const &constraint) override;
  void removeConstraint(std::string const &parameterName) override;
  std::vector<std::string> getGlobalParameters() const override;
  void setGlobalParameters(std::vector<std::string> const &globals) override;
  bool isGlobal(std::string const &parameterName) const override;
  void setGlobal(std::string const &parameterName, bool on) override;
  std::vector<std::string> getLocalParameters() const override;
  void updateMultiDatasetParameters(const IFunction &fun) override;
  void updateMultiDatasetParameters(const ITableWorkspace &paramTable) override;
  void updateParameters(const IFunction &fun) override;

  double getLocalParameterValue(std::string const &parameterName, int i) const override;
  bool isLocalParameterFixed(std::string const &parameterName, int i) const override;
  std::string getLocalParameterTie(std::string const &parameterName, int i) const override;
  std::string getLocalParameterConstraint(std::string const &parameterName, int i) const override;
  void setLocalParameterValue(std::string const &parameterName, int i, double value) override;
  void setLocalParameterValue(std::string const &parameterName, int i, double value, double error) override;
  void setLocalParameterFixed(std::string const &parameterName, int i, bool fixed) override;
  void setLocalParameterTie(std::string const &parameterName, int i, std::string const &tie) override;
  void setLocalParameterConstraint(std::string const &parameterName, int i, std::string const &constraint) override;
  void setGlobalParameterValue(std::string const &parameterName, double value) override;
  std::string setBackgroundA0(double value) override;
  void setNumberOfExponentials(int);
  int getNumberOfExponentials() const;
  void setStretchExponential(bool);
  bool hasStretchExponential() const;
  void setBackground(std::string const &name);
  void removeBackground();
  bool hasBackground() const;
  void tieIntensities(bool on);
  EstimationDataSelector getEstimationDataSelector() const;
  void updateParameterEstimationData(DataForParameterEstimationCollection &&data);
  void estimateFunctionParameters();
  void setResolution(const std::vector<std::pair<std::string, size_t>> &fitResolutions) override;
  void setQValues(const std::vector<double> &qValues) override;

  enum class ParamID {
    EXP1_HEIGHT,
    EXP1_LIFETIME,
    EXP2_HEIGHT,
    EXP2_LIFETIME,
    STRETCH_HEIGHT,
    STRETCH_LIFETIME,
    STRETCH_STRETCHING,
    BG_A0
  };
  std::map<ParamID, double> getCurrentValues() const;
  std::map<ParamID, double> getCurrentErrors() const;
  std::map<int, std::string> getParameterNameMap() const;

private:
  void clearData();
  std::string buildFunctionString() const;
  boost::optional<std::string> getExp1Prefix() const;
  boost::optional<std::string> getExp2Prefix() const;
  boost::optional<std::string> getStretchPrefix() const;
  boost::optional<std::string> getBackgroundPrefix() const;
  void setParameter(ParamID name, double value);
  boost::optional<double> getParameter(ParamID name) const;
  boost::optional<double> getParameterError(ParamID name) const;
  boost::optional<std::string> getParameterName(ParamID name) const;
  boost::optional<std::string> getParameterDescription(ParamID name) const;
  boost::optional<std::string> getPrefix(ParamID name) const;
  void setCurrentValues(const std::map<ParamID, double> &);
  void applyParameterFunction(const std::function<void(ParamID)> &paramFun) const;
  boost::optional<ParamID> getParameterId(std::string const &parameterName);
  std::string buildExpDecayFunctionString() const;
  std::string buildStretchExpFunctionString() const;
  std::string buildBackgroundFunctionString() const;
  void addGlobal(std::string const &parameterName);
  void removeGlobal(std::string const &parameterName);
  std::vector<std::string> makeGlobalList() const;

  FunctionModel m_model;
  int m_numberOfExponentials = 0;
  bool m_hasStretchExponential = false;
  std::string m_background;
  DataForParameterEstimationCollection m_estimationData;
  QList<ParamID> m_globals;
  std::unique_ptr<IDAFunctionParameterEstimation> m_parameterEstimation;
};

} // namespace IDA
} // namespace CustomInterfaces
} // namespace MantidQt
