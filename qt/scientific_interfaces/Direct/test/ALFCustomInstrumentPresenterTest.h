// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2019 ISIS Rutherford Appleton Laboratory UKRI,
//     NScD Oak Ridge National Laboratory, European Spallation Source
//     & Institut Laue - Langevin
// SPDX - License - Identifier: GPL - 3.0 +
#ifndef MANTIDQT_ALFCUSTOMINSTRUMENTPRESENTERTEST_H_
#define MANTIDQT_ALFCUSTOMINSTRUMENTPRESENTERTEST_H_

#include <cxxtest/TestSuite.h>
#include <gmock/gmock.h>

#include "ALFCustomInstrumentModel.h"
#include "ALFCustomInstrumentView.h"
#include "ALFCustomInstrumentPresenter.h"
#include "MantidQtWidgets/InstrumentView/PlotFitAnalysisPanePresenter.h"
#include "MantidQtWidgets/InstrumentView/PlotFitAnalysisPaneView.h"
#include "MantidQtWidgets/InstrumentView/PlotFitAnalysisPaneModel.h"
#include "MantidQtWidgets/Common/ObserverPattern.h"

#include "MantidAPI/AlgorithmManager.h"
#include "MantidAPI/AnalysisDataService.h"
#include "MantidAPI/FrameworkManager.h"
#include "MantidAPI/MatrixWorkspace_fwd.h"
#include "MantidTestHelpers/WorkspaceCreationHelper.h"
#include "MantidGeometry/Instrument.h"


#include <string>
#include <utility>

using namespace Mantid::API;
using namespace MantidQt::CustomInterfaces;
using namespace testing;
using Mantid::Geometry::Instrument;
using namespace MantidQt::MantidWidgets;


// need to add mock objects..
class ALFModelTest : public ALFCustomInstrumentModel{
public:
    ALFModelTest(){};
    MOCK_METHOD0(extractSingleTube, void());
    MOCK_METHOD0(averageTube, void());
};

//class ALFViewTest : public IALFCustomInstrumentView, public IBaseCustomInstrumentView{
class ALFViewTest : public IALFCustomInstrumentView{//, public IBaseCustomInstrumentView{
public:
    ALFViewTest(const std::string &instrument, QWidget *parent = nullptr) {};
    ~ALFViewTest(){};

    MOCK_METHOD1(observeExtractSingleTube, void(Observer *listner));
    MOCK_METHOD1(observeAverageTube, void(Observer *listner));
    MOCK_METHOD1(addSpectrum, void(std::string name));
    MOCK_METHOD1(setupAnalysisPane, void(PlotFitAnalysisPaneView *analysis));

    MOCK_METHOD0(getFile, std::string());
    MOCK_METHOD1(setRunQuietly, void(const std::string &runNumber));
    MOCK_METHOD1(observeLoadRun, void( Observer *listener));
    MOCK_METHOD1(warningBox, void(const std::string &error));
    MOCK_METHOD1(setInstrumentWidget, void(InstrumentWidget *instrument));
    MOCK_METHOD0(getInstrumentView, InstrumentWidget*());
    MOCK_METHOD2(setUpInstrument,void(const std::string &fileName, std::vector<std::function<bool(std::map<std::string,bool>)>> &binders));
    MOCK_METHOD1(addObserver, void(std::tuple<std::string, Observer *> &listener));
    MOCK_METHOD1(setupInstrumentAnalysisSplitters, void(QWidget *analysis));
    MOCK_METHOD0(setupHelp, void());
   
};

class paneTest : public MantidQt::MantidWidgets::PlotFitAnalysisPanePresenter{
public:
    // define init
    paneTest(PlotFitAnalysisPaneView *view, PlotFitAnalysisPaneModel *model):PlotFitAnalysisPanePresenter(view, model) {}; 
    MOCK_METHOD1(addSpectrum, void (const std::string &name));
};

class paneViewTest : public MantidQt::MantidWidgets::IPlotFitAnalysisPaneView{
public:
   paneViewTest(const double &start=1., const double &end=5., QWidget *parent = nullptr){};
   MOCK_METHOD1(observeFitButton, void(Observer *listener));
   MOCK_METHOD0(getRange, std::pair<double, double>());
   MOCK_METHOD0(getFunction, Mantid::API::IFunction_sptr());
   MOCK_METHOD1(addSpectrum, void(std::string name));
   MOCK_METHOD1(addFitSpectrum, void(std::string name));
   MOCK_METHOD1(addFunction, void(Mantid::API::IFunction_sptr));
   MOCK_METHOD1(updateFunction, void(Mantid::API::IFunction_sptr));
   MOCK_METHOD1(fitWarning, void(const std::string &message));
   
   MOCK_METHOD2(setupPlotFitSplitter, void(const double &start, const double &end)); 
   MOCK_METHOD2(createFitPane, QWidget*(const double &start, const double &end)); 
};
class paneModelTest : public MantidQt::MantidWidgets::PlotFitAnalysisPaneModel{
    void empty(){};
};

class ALFCustomInstrumentPresenterTest : public CxxTest::TestSuite {
public:
  /// WorkflowAlgorithms do not appear in the FrameworkManager without this line
  ALFCustomInstrumentPresenterTest() { FrameworkManager::Instance(); }

  static ALFCustomInstrumentPresenterTest *createSuite() { return new ALFCustomInstrumentPresenterTest(); }

  static void destroySuite(ALFCustomInstrumentPresenterTest *suite) { delete suite; }

  void setUp() override {
    //m_workspace = createWorkspace(4, 3);
    //m_ads = std::make_unique<SetUpADSWithWorkspace>("Name", m_workspace);
  m_model = new NiceMock<ALFModelTest>();
  m_view = new NiceMock<ALFViewTest>("ALF");
  m_paneView = new NiceMock<paneViewTest>();
  m_paneModel = new NiceMock<paneModelTest>();
  m_pane = new NiceMock<paneTest>(m_paneView, m_paneModel);
  m_presenter = new ALFCustomInstrumentPresenter(m_view, m_model,m_pane);
  }

  void tearDown() override {
    AnalysisDataService::Instance().clear();
    delete m_presenter;
    delete m_model;
    delete m_view;
    delete m_pane;
    delete m_paneModel;
    delete m_paneView;
    //m_ads.reset();
    //m_workspace.reset();
    //m_model.reset();
  }

  void test_that_the_model_is_instantiated_and_can_hold_a_workspace() {
    return; //SpectraLegacy const spectra = DiscontinuousSpectra<std::size_t>("0-1");

    //m_model->addWorkspace(m_workspace, spectra);

    //TS_ASSERT_EQUALS(m_model->numberOfWorkspaces(), 1);
  }
  void test_extractSingleTube(){
   m_presenter->extractSingleTube();
   EXPECT_CALL(*m_model, averageTube()).Times(1);
}

private:
  //MatrixWorkspace_sptr m_workspace;
  //std::unique_ptr<SetUpADSWithWorkspace> m_ads;
  NiceMock<ALFModelTest> *m_model;
  NiceMock<ALFViewTest> *m_view;
  NiceMock<paneViewTest> *m_paneView;
  NiceMock<paneModelTest> *m_paneModel;
  NiceMock<paneTest> *m_pane;
  NiceMock<ALFCustomInstrumentPresenter> *m_presenter;
};

#endif /* MANTIDQT_ALFCUSTOMINSTRUMENTPRESENTERTEST_H_ */
