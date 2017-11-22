#ifndef MANTID_CUSTOMINTERFACES_ENGGDIFFFITTINGMODELTEST_H_
#define MANTID_CUSTOMINTERFACES_ENGGDIFFFITTINGMODELTEST_H_

#include <cxxtest/TestSuite.h>

#include "MantidAPI/MatrixWorkspace.h"
#include "MantidAPI/WorkspaceFactory.h"

#include "../EnggDiffraction/EnggDiffFittingModel.h"

#include <vector>

// Lets us have pairs inside assertion macros
typedef std::pair<int, size_t> RunBankPair;

using namespace Mantid;
using namespace MantidQT::CustomInterfaces;

namespace {

void addSampleWorkspaceToModel(const int runNumber, const int bank,
                               EnggDiffFittingModel &model) {
  API::MatrixWorkspace_sptr ws =
      API::WorkspaceFactory::Instance().create("Workspace2D", 1, 10, 10);
  model.addWorkspace(runNumber, bank, ws);
}

} // anonymous namespace

class EnggDiffFittingModelTest : public CxxTest::TestSuite {
public:
  // This pair of boilerplate methods prevent the suite being created statically
  // This means the constructor isn't called when running other tests
  static EnggDiffFittingModelTest *createSuite() {
    return new EnggDiffFittingModelTest();
  }
  static void destroySuite(EnggDiffFittingModelTest *suite) { delete suite; }

  void test_addAndGetWorkspace() {
    auto model = EnggDiffFittingModel();
    API::MatrixWorkspace_sptr ws =
        API::WorkspaceFactory::Instance().create("Workspace2D", 1, 10, 10);
    const int runNumber = 100;
    const int bank = 1;
    TS_ASSERT_THROWS_NOTHING(model.addWorkspace(runNumber, bank, ws));
    const auto retrievedWS = model.getWorkspace(runNumber, bank);

    TS_ASSERT(retrievedWS != nullptr);
    TS_ASSERT_EQUALS(ws, retrievedWS);
  }

  void test_getAllRunNumbers() {
    auto model = EnggDiffFittingModel();

    addSampleWorkspaceToModel(123, 1, model);
    addSampleWorkspaceToModel(456, 2, model);
    addSampleWorkspaceToModel(789, 1, model);
    addSampleWorkspaceToModel(123, 2, model);

    const auto runNumbers = model.getAllRunNumbers();

    TS_ASSERT_EQUALS(runNumbers.size(), 3);
    TS_ASSERT_EQUALS(runNumbers[0], 123);
    TS_ASSERT_EQUALS(runNumbers[1], 456);
    TS_ASSERT_EQUALS(runNumbers[2], 789);
  }

  void test_getRunNumbersAndBankIDs() {
    auto model = EnggDiffFittingModel();

    addSampleWorkspaceToModel(123, 1, model);
    addSampleWorkspaceToModel(456, 2, model);
    addSampleWorkspaceToModel(789, 1, model);
    addSampleWorkspaceToModel(123, 2, model);

    const auto runNoBankPairs = model.getRunNumbersAndBanksIDs();

    TS_ASSERT_EQUALS(runNoBankPairs.size(), 4);
    TS_ASSERT_EQUALS(runNoBankPairs[0], RunBankPair(123, 1));
    TS_ASSERT_EQUALS(runNoBankPairs[1], RunBankPair(123, 2));
    TS_ASSERT_EQUALS(runNoBankPairs[2], RunBankPair(456, 2));
    TS_ASSERT_EQUALS(runNoBankPairs[3], RunBankPair(789, 1));
  }
};

#endif