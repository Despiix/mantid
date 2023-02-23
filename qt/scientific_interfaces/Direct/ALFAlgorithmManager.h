// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2021 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include "DllConfig.h"
#include "IALFAlgorithmManagerSubscriber.h"
#include "MantidAPI/IAlgorithm_fwd.h"
#include "MantidAPI/MatrixWorkspace_fwd.h"
#include "MantidQtWidgets/Common/IJobRunner.h"

#include <memory>

namespace MantidQt::CustomInterfaces {

class IALFAlgorithmManagerSubscriber;

class MANTIDQT_DIRECT_DLL IALFAlgorithmManager {

public:
  virtual ~IALFAlgorithmManager() = default;

  virtual void subscribe(IALFAlgorithmManagerSubscriber *subscriber) = 0;

  virtual void loadAndNormalise(ALFDataType const &dataType, std::string const &filename) = 0;
};

class MANTIDQT_DIRECT_DLL ALFAlgorithmManager final : public IALFAlgorithmManager, public API::JobRunnerSubscriber {

public:
  ALFAlgorithmManager(std::unique_ptr<API::IJobRunner> jobRunner);

  void subscribe(IALFAlgorithmManagerSubscriber *subscriber) override;

  void loadAndNormalise(ALFDataType const &dataType, std::string const &filename) override;

  void notifyBatchComplete(bool error) override{};
  void notifyBatchCancelled() override{};
  void notifyAlgorithmStarted(API::IConfiguredAlgorithm_sptr &algorithm) override{};
  void notifyAlgorithmComplete(API::IConfiguredAlgorithm_sptr &algorithm) override;
  void notifyAlgorithmError(API::IConfiguredAlgorithm_sptr algorithm, std::string const &message) override;

private:
  void notifyLoadComplete(Mantid::API::IAlgorithm_sptr const &algorithm);
  void notifyNormaliseComplete(Mantid::API::IAlgorithm_sptr const &algorithm);

  ALFDataType m_currentType;
  std::unique_ptr<API::IJobRunner> m_jobRunner;
  IALFAlgorithmManagerSubscriber *m_subscriber;
};

} // namespace MantidQt::CustomInterfaces