#ifndef SLICE_VIEWER_PEAKS_PRESENTER_TEST_H_
#define SLICE_VIEWER_PEAKS_PRESENTER_TEST_H_

#include <cxxtest/TestSuite.h>
#include "MantidAPI/ExperimentInfo.h"
#include "MantidAPI/IPeak.h"
#include "MantidQtSliceViewer/ConcretePeaksPresenter.h"
#include "MantidQtSliceViewer/PeakOverlayViewFactory.h"
#include "MantidQtSliceViewer/PeakTransformFactory.h"
#include "MantidDataObjects/PeaksWorkspace.h"
#include "MantidTestHelpers/WorkspaceCreationHelper.h"
#include "MockObjects.h"
#include <boost/make_shared.hpp>
#include <string>

using namespace MantidQt::SliceViewer;
using namespace Mantid::API;
using namespace Mantid::Geometry;
using namespace Mantid::Kernel;
using namespace testing;
using boost::regex;

// Alias.
typedef boost::shared_ptr<Mantid::API::MDGeometry> MDGeometry_sptr;

class ConcretePeaksPresenterTest : public CxxTest::TestSuite
{

  /// Helper method to create a good 'Integrated' peaks workspace
  Mantid::API::IPeaksWorkspace_sptr createPeaksWorkspace(const int nPeaks, const double radius=1)
  {
    Mantid::API::IPeaksWorkspace_sptr peaksWS = WorkspaceCreationHelper::createPeaksWorkspace(nPeaks);
    peaksWS->mutableRun().addProperty("PeaksIntegrated", true);
    peaksWS->mutableRun().addProperty("PeakRadius", radius);
    return peaksWS;
  }

  /// Helper method to create a mock MDDimension.
  IMDDimension_sptr createExpectedMDDimension(const std::string returnLabel)
  {
    auto* pDim = new NiceMock<MockIMDDimension>;
    IMDDimension_sptr dim(pDim);
    EXPECT_CALL(*pDim, getName()).WillRepeatedly(Return(returnLabel));
    return dim;
  }

  /// Helper method to create an expected MDGeometry (we call it MDWorkspace here).
  MDGeometry_sptr createExpectedMDWorkspace()
  {
    // Create a mock H Dim
    IMDDimension_sptr HDim = createExpectedMDDimension("H");
    // Create a mock K Dim
    IMDDimension_sptr KDim = createExpectedMDDimension("K");
    // Create a mock L Dim
    IMDDimension_sptr LDim = createExpectedMDDimension("L");

    // Create the mock MD geometry
    MockMDGeometry* pGeometry = new MockMDGeometry;
    EXPECT_CALL(*pGeometry, getNumDims()).WillRepeatedly(Return(3));
    EXPECT_CALL(*pGeometry, getDimension(0)).WillRepeatedly(Return(HDim));
    EXPECT_CALL(*pGeometry, getDimension(1)).WillRepeatedly(Return(KDim));
    EXPECT_CALL(*pGeometry, getDimension(2)).WillRepeatedly(Return(LDim));

    return boost::shared_ptr<MockMDGeometry>(pGeometry);
  }

public:

  void test_constructor_throws_if_either_view_factory_null()
  {
    PeakOverlayViewFactory* pNullViewFactory = NULL; // View factory is null
    PeakOverlayViewFactory_sptr nullViewFactory(pNullViewFactory);

    PeakOverlayViewFactory* pNormalViewFactory = new MockPeakOverlayFactory;
    PeakOverlayViewFactory_sptr normalViewFactory(pNormalViewFactory);

    IPeaksWorkspace_sptr peaksWS = createPeaksWorkspace(1);
  
    PeakTransform_sptr mockTransform(new MockPeakTransform);  
 
    auto pMockTransformFactory = new NiceMock<MockPeakTransformFactory>;
    PeakTransformFactory_sptr peakTransformFactory(pMockTransformFactory);
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillRepeatedly(Return(mockTransform));

    MDGeometry_sptr mdWS = boost::make_shared<MockMDGeometry>();

    TSM_ASSERT_THROWS("Non integrated view factory is null, should throw.", ConcretePeaksPresenter(nullViewFactory, normalViewFactory, peaksWS, mdWS, peakTransformFactory), std::invalid_argument);
    TSM_ASSERT_THROWS("Integrated view factory is null, should throw.", ConcretePeaksPresenter(normalViewFactory, nullViewFactory, peaksWS, mdWS, peakTransformFactory), std::invalid_argument);
  }

  void test_construction()
  {
    // Expected number of peaks to create
    const size_t expectedNumberPeaks = 3;

    // Peaks workspace IS INTEGRATED.
    IPeaksWorkspace_sptr peaksWS = createPeaksWorkspace(expectedNumberPeaks);

    // Create an MDWorkspace
    MDGeometry_sptr mdWS = createExpectedMDWorkspace();

    // Mock View Factory Product
    auto pMockView = new NiceMock<MockPeakOverlayView>;
    auto mockView = boost::shared_ptr<NiceMock<MockPeakOverlayView> >(pMockView);

    // Mock View Factory for NON-integrated peaks. We expect that this will be used.
    auto pMockNonIntegratedViewFactory = new MockPeakOverlayFactory;
    PeakOverlayViewFactory_sptr mockNonIntegratedViewFactory(pMockNonIntegratedViewFactory);

    // Mock View Factory for integrated peaks. We expect that this will never be used.
    auto pMockIntegratedViewFactory = new MockPeakOverlayFactory;
    PeakOverlayViewFactory_sptr mockIntegratedViewFactory(pMockIntegratedViewFactory);
    EXPECT_CALL(*pMockIntegratedViewFactory, setRadius(_)).Times(1);
    EXPECT_CALL(*pMockIntegratedViewFactory, setZRange(_,_)).Times(1);
    EXPECT_CALL(*pMockIntegratedViewFactory, createView(_)).Times(expectedNumberPeaks).WillRepeatedly(Return(mockView));
    EXPECT_CALL(*pMockIntegratedViewFactory, getPlotXLabel()).WillOnce(Return("H"));
    EXPECT_CALL(*pMockIntegratedViewFactory, getPlotYLabel()).WillOnce(Return("K"));

    // Create a mock transform object.
    auto pMockTransform = new NiceMock<MockPeakTransform>;
    PeakTransform_sptr mockTransform(pMockTransform);
    const int numberOfTimesCalled = static_cast<int>(peaksWS->rowCount());// Should be called for every peak i.e. row.
    EXPECT_CALL(*pMockTransform, transformPeak(_)).Times(numberOfTimesCalled).WillRepeatedly(Return(V3D()));

    // Create a mock transform factory.
    auto pMockTransformFactory = new NiceMock<MockPeakTransformFactory>;
    PeakTransformFactory_sptr peakTransformFactory(pMockTransformFactory);
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillOnce(Return(mockTransform));
    EXPECT_CALL(*pMockTransformFactory, createTransform(_,_)).WillOnce(Return(mockTransform));

    // Construct the presenter.
    ConcretePeaksPresenter presenter(mockNonIntegratedViewFactory, mockIntegratedViewFactory, peaksWS, mdWS, peakTransformFactory);

    TSM_ASSERT("Non-Integrated View Factory has not been used as expected", Mock::VerifyAndClearExpectations(pMockNonIntegratedViewFactory));
    TSM_ASSERT("Integrated View Factory has not been used as expected", Mock::VerifyAndClearExpectations(pMockIntegratedViewFactory));
    TSM_ASSERT("MockView not used as expected.", Mock::VerifyAndClearExpectations(pMockView));
    TSM_ASSERT("MockTransformFactory not used as expected", Mock::VerifyAndClearExpectations(pMockTransformFactory));
    TSM_ASSERT("MockTransform not used as expected", Mock::VerifyAndClearExpectations(pMockTransform));
  }

  void test_constructor_swaps_view_factory_if_peaks_workspace_not_integrated()
  {
    // Expected number of peaks to create
    const size_t expectedNumberPeaks = 3;

    // Peaks workspace IS NOT INTEGRATED.
    IPeaksWorkspace_sptr peaksWS = WorkspaceCreationHelper::createPeaksWorkspace(expectedNumberPeaks); 

    // Create an MDWorkspace
    MDGeometry_sptr mdWS = createExpectedMDWorkspace();

    // Mock View Factory Product
    auto pMockView = new NiceMock<MockPeakOverlayView>;
    auto mockView = boost::shared_ptr<NiceMock<MockPeakOverlayView> >(pMockView);

    // Mock View Factory for NON-INTEGRATED peaks. We expect that this will be used.
    auto pMockNonIntegratedViewFactory = new MockPeakOverlayFactory;
    PeakOverlayViewFactory_sptr mockNonIntegratedViewFactory(pMockNonIntegratedViewFactory);
    EXPECT_CALL(*pMockNonIntegratedViewFactory, setRadius(_)).Times(1);
    EXPECT_CALL(*pMockNonIntegratedViewFactory, setZRange(_,_)).Times(1);
    EXPECT_CALL(*pMockNonIntegratedViewFactory, createView(_)).Times(expectedNumberPeaks).WillRepeatedly(Return(mockView));
    EXPECT_CALL(*pMockNonIntegratedViewFactory, getPlotXLabel()).WillOnce(Return("H"));
    EXPECT_CALL(*pMockNonIntegratedViewFactory, getPlotYLabel()).WillOnce(Return("K"));

    // Mock View Factory for integrated peaks. We expect that this will never be used.
    auto pMockIntegratedViewFactory = new MockPeakOverlayFactory;
    PeakOverlayViewFactory_sptr mockIntegratedViewFactory(pMockIntegratedViewFactory);

    // Mock Peaks transform
    auto pMockTransform = new MockPeakTransform;
    PeakTransform_sptr mockTransform(pMockTransform);  
    const int numberOfTimesCalled = static_cast<int>(peaksWS->rowCount());// Should be called for every peak i.e. row.
    EXPECT_CALL(*pMockTransform, transformPeak(_)).Times(numberOfTimesCalled).WillRepeatedly(Return(V3D()));
 
    // Mock Peak transform factory
    auto pMockTransformFactory = new NiceMock<MockPeakTransformFactory>;
    PeakTransformFactory_sptr peakTransformFactory(pMockTransformFactory);
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillRepeatedly(Return(mockTransform));
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillOnce(Return(mockTransform));
    EXPECT_CALL(*pMockTransformFactory, createTransform(_,_)).WillOnce(Return(mockTransform));

    // Construct the presenter.
    ConcretePeaksPresenter presenter(mockNonIntegratedViewFactory, mockIntegratedViewFactory, peaksWS, mdWS, peakTransformFactory);

    TSM_ASSERT("Non-Integrated View Factory has not been used as expected", Mock::VerifyAndClearExpectations(pMockNonIntegratedViewFactory));
    TSM_ASSERT("Integrated View Factory has not been used as expected", Mock::VerifyAndClearExpectations(pMockIntegratedViewFactory));
    TSM_ASSERT("MockTransformFactory not used as expected", Mock::VerifyAndClearExpectations(pMockTransformFactory));
    TSM_ASSERT("MockTransform not used as expected", Mock::VerifyAndClearExpectations(pMockTransform));
  }

  void test_update()
  {
    // Create a non-integrated widget factory mock.
    auto pMockNonIntegratedPeakViewFactory = new MockPeakOverlayFactory;
    PeakOverlayViewFactory_sptr mockNonIntegratedPeakViewFactory(pMockNonIntegratedPeakViewFactory);

    // Create a spherical widget factory mock
    auto pMockIntegratedPeakViewFactory = new MockPeakOverlayFactory;
    PeakOverlayViewFactory_sptr mockIntegratedPeakViewFactory(pMockIntegratedPeakViewFactory);

    // Set the expectation on the number of calls
    const int expectedNumberPeaks = 10;

    // Create a mock view object that will be returned by the mock factory.
    auto pMockView = new NiceMock<MockPeakOverlayView>;
    EXPECT_CALL(*pMockView, updateView()).Times(expectedNumberPeaks);
    auto mockView = boost::shared_ptr<NiceMock<MockPeakOverlayView> >(pMockView);
    
    EXPECT_CALL(*pMockIntegratedPeakViewFactory, setRadius(_)).Times(1);
    EXPECT_CALL(*pMockIntegratedPeakViewFactory, setZRange(_,_)).Times(1);
    EXPECT_CALL(*pMockIntegratedPeakViewFactory, createView(_)).WillRepeatedly(Return(mockView));
    EXPECT_CALL(*pMockIntegratedPeakViewFactory, getPlotXLabel()).WillOnce(Return("H"));
    EXPECT_CALL(*pMockIntegratedPeakViewFactory, getPlotYLabel()).WillOnce(Return("K"));

    // Create an input MODEL Peaks workspace (INTEGRATED)
    Mantid::API::IPeaksWorkspace_sptr peaksWS = createPeaksWorkspace(expectedNumberPeaks);
    // Create an input MODEL IMDWorkspace (Geom)
    MDGeometry_sptr mdWS = createExpectedMDWorkspace();

    // Create a mock transform object.
    auto pMockTransform = new NiceMock<MockPeakTransform>;
    PeakTransform_sptr mockTransform(pMockTransform);
    EXPECT_CALL(*pMockTransform, transformPeak(_)).WillRepeatedly(Return(V3D()));

    // Create a mock transform factory.
    auto pMockTransformFactory = new NiceMock<MockPeakTransformFactory>;
    PeakTransformFactory_sptr peakTransformFactory(pMockTransformFactory);
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillOnce(Return(mockTransform));
    EXPECT_CALL(*pMockTransformFactory, createTransform(_,_)).WillOnce(Return(mockTransform));

    // Construction should cause the widget factory to be used to generate peak overlay objects.
    ConcretePeaksPresenter presenter(mockNonIntegratedPeakViewFactory, mockIntegratedPeakViewFactory, peaksWS, mdWS, peakTransformFactory);

    // Updating should cause all of the held views to be updated too.
    presenter.update();
    
    TSM_ASSERT("MockView not used as expected.", Mock::VerifyAndClearExpectations(pMockView));
    TSM_ASSERT("MockTransformFactory not used as expected", Mock::VerifyAndClearExpectations(pMockTransformFactory));
  }

  void test_set_slice_point()
  {
    // Create a widget factory mock
    auto mockViewFactory = new MockPeakOverlayFactory;

    const double slicePoint = 0.1;
    const int expectedNumberPeaks = 10;

    // Create a mock view object that will be returned by the mock factory.
    auto pMockView = new NiceMock<MockPeakOverlayView>;
    EXPECT_CALL(*pMockView, setSlicePoint(slicePoint)).Times(expectedNumberPeaks);
    auto mockView = boost::shared_ptr<NiceMock<MockPeakOverlayView> >(pMockView);

    EXPECT_CALL(*mockViewFactory, setRadius(_)).Times(1);
    EXPECT_CALL(*mockViewFactory, setZRange(_,_)).Times(1);
    EXPECT_CALL(*mockViewFactory, createView(_)).WillRepeatedly(Return(mockView));
    EXPECT_CALL(*mockViewFactory, getPlotXLabel()).WillOnce(Return("H"));
    EXPECT_CALL(*mockViewFactory, getPlotYLabel()).WillOnce(Return("K"));

    // Create an input MODEL Peaks workspace (INTEGRATED)
    Mantid::API::IPeaksWorkspace_sptr peaksWS = createPeaksWorkspace(expectedNumberPeaks);
    // Create an input MODEL IMDWorkspace (Geom)
    MDGeometry_sptr mdWS = createExpectedMDWorkspace();

    // Create a mock transform object.
    auto pMockTransform = new NiceMock<MockPeakTransform>;
    PeakTransform_sptr mockTransform(pMockTransform);
    EXPECT_CALL(*pMockTransform, transformPeak(_)).WillRepeatedly(Return(V3D()));

    // Create a mock transform factory.
    auto pMockTransformFactory = new NiceMock<MockPeakTransformFactory>;
    PeakTransformFactory_sptr peakTransformFactory(pMockTransformFactory);
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillOnce(Return(mockTransform));
    EXPECT_CALL(*pMockTransformFactory, createTransform(_,_)).WillOnce(Return(mockTransform));

    // Construction should cause the widget factory to be used to generate peak overlay objects.
    ConcretePeaksPresenter presenter(boost::make_shared<MockPeakOverlayFactory>(), PeakOverlayViewFactory_sptr(mockViewFactory), peaksWS, mdWS, peakTransformFactory);

    // Updating should cause all of the held views to be updated too.
    presenter.updateWithSlicePoint(slicePoint);

    TSM_ASSERT("MockView not used as expected.", Mock::VerifyAndClearExpectations(pMockView));
    TSM_ASSERT("MockTransformFactory not used as expected", Mock::VerifyAndClearExpectations(pMockTransformFactory));
  }

  void test_hide_owned_views_on_death()
  {
    // Create a widget factory mock
    auto mockViewFactory = new MockPeakOverlayFactory;

    const int expectedNumberPeaks = 1;

    // Create a mock view object that will be returned by the mock factory.
    auto pMockView = new NiceMock<MockPeakOverlayView>;
    EXPECT_CALL(*pMockView, hideView()).Times(expectedNumberPeaks);
    auto mockView = boost::shared_ptr<NiceMock<MockPeakOverlayView> >(pMockView);

    EXPECT_CALL(*mockViewFactory, setRadius(_)).Times(1);
    EXPECT_CALL(*mockViewFactory, setZRange(_,_)).Times(1);
    EXPECT_CALL(*mockViewFactory, createView(_)).WillRepeatedly(Return(mockView));
    EXPECT_CALL(*mockViewFactory, getPlotXLabel()).WillOnce(Return("H"));
    EXPECT_CALL(*mockViewFactory, getPlotYLabel()).WillOnce(Return("K"));

    // Create an input MODEL Peaks workspace (INTEGRATED)
    Mantid::API::IPeaksWorkspace_sptr peaksWS = createPeaksWorkspace(expectedNumberPeaks);
    // Create an input MODEL IMDWorkspace (Geom)
    MDGeometry_sptr mdWS = createExpectedMDWorkspace();

     // Create a mock transform object.
    auto pMockTransform = new NiceMock<MockPeakTransform>;
    PeakTransform_sptr mockTransform(pMockTransform);
    EXPECT_CALL(*pMockTransform, transformPeak(_)).WillRepeatedly(Return(V3D()));

    // Create a mock transform factory.
    auto pMockTransformFactory = new NiceMock<MockPeakTransformFactory>;
    PeakTransformFactory_sptr peakTransformFactory(pMockTransformFactory);
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillOnce(Return(mockTransform));
    EXPECT_CALL(*pMockTransformFactory, createTransform(_,_)).WillOnce(Return(mockTransform));

    {
      ConcretePeaksPresenter presenter(boost::make_shared<MockPeakOverlayFactory>(), PeakOverlayViewFactory_sptr(mockViewFactory), peaksWS, mdWS, peakTransformFactory);
    } // Guaranteed destruction at this point. Destructor should trigger hide on all owned views.

    TSM_ASSERT("MockView not used as expected.", Mock::VerifyAndClearExpectations(pMockView));
    TSM_ASSERT("MockTransformFactory not used as expected", Mock::VerifyAndClearExpectations(pMockTransformFactory));
  }

  void test_handle_non_hkl_xy_mappings()
  {
    // Create a widget factory mock
    auto mockViewFactory = new MockPeakOverlayFactory;

    const int expectedNumberPeaks = 1;

    // Create a mock view object that will be returned by the mock factory.
    auto pMockView = new NiceMock<MockPeakOverlayView>;
    EXPECT_CALL(*pMockView, hideView()).Times(expectedNumberPeaks); // This will be called automatically because the presenter won't be able to map Qx (below).
    auto mockView = boost::shared_ptr<NiceMock<MockPeakOverlayView> >(pMockView);

    EXPECT_CALL(*mockViewFactory, setRadius(_)).Times(1);
    EXPECT_CALL(*mockViewFactory, setZRange(_,_)).Times(1);
    EXPECT_CALL(*mockViewFactory, createView(_)).WillRepeatedly(Return(mockView));
    EXPECT_CALL(*mockViewFactory, getPlotXLabel()).WillOnce(Return("Qx")); // Not either H, K or L
    EXPECT_CALL(*mockViewFactory, getPlotYLabel()).WillOnce(Return("K"));
    // Create an input MODEL Peaks workspace (INTEGRATED)
    Mantid::API::IPeaksWorkspace_sptr peaksWS = createPeaksWorkspace(expectedNumberPeaks);
    // Create an input MODEL IMDWorkspace (Geom)
    MDGeometry_sptr mdWS = createExpectedMDWorkspace();

     // Create a mock transform object.
    auto pMockTransform = new NiceMock<MockPeakTransform>;
    PeakTransform_sptr mockTransform(pMockTransform);
    EXPECT_CALL(*pMockTransform, transformPeak(_)).WillRepeatedly(Return(V3D()));

    // Create a mock transform factory.
    auto pMockTransformFactory = new NiceMock<MockPeakTransformFactory>;
    PeakTransformFactory_sptr peakTransformFactory(pMockTransformFactory);
    EXPECT_CALL(*pMockTransformFactory, createDefaultTransform()).WillOnce(Return(mockTransform));
    EXPECT_CALL(*pMockTransformFactory, createTransform(_,_)).WillRepeatedly(Throw(PeakTransformException())); // The actual transform will throw if a mix of Qx and Qy were used.

    ConcretePeaksPresenter presenter(boost::make_shared<MockPeakOverlayFactory>(), PeakOverlayViewFactory_sptr(mockViewFactory), peaksWS, mdWS, peakTransformFactory);
    TSM_ASSERT("MockView not used as expected.", Mock::VerifyAndClearExpectations(pMockView));
    TSM_ASSERT("MockTransformFactory not used as expected", Mock::VerifyAndClearExpectations(pMockTransformFactory));
  }
};


#endif
