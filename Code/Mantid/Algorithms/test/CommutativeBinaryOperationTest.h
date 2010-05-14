#ifndef COMMUTATIVEBINARYOPERATIONTEST_H_
#define COMMUTATIVEBINARYOPERATIONTEST_H_

#include <cxxtest/TestSuite.h>
#include <cmath>

#include "WorkspaceCreationHelper.hh"
#include "MantidAlgorithms/CommutativeBinaryOperation.h"
#include "MantidAPI/AnalysisDataService.h"
#include "MantidAPI/WorkspaceFactory.h"
#include "MantidAPI/WorkspaceIterator.h"
#include "MantidDataObjects/Workspace2D.h"
#include "MantidDataObjects/Workspace1D.h"

using namespace Mantid;
using namespace Mantid::API;

class CommutativeBinaryOpHelper : public Mantid::Algorithms::CommutativeBinaryOperation
{
public:
  /// Default constructor
  CommutativeBinaryOpHelper() : CommutativeBinaryOperation() {};
  /// Destructor
  virtual ~CommutativeBinaryOpHelper() {};
  const bool checkSizeCompatibility(const MatrixWorkspace_sptr ws1,const MatrixWorkspace_sptr ws2) const
  {
    return CommutativeBinaryOperation::checkSizeCompatibility(ws1,ws2);
  }

private:
  // Overridden BinaryOperation methods
  void performBinaryOperation(const MantidVec& lhsX, const MantidVec& lhsY, const MantidVec& lhsE,
                              const MantidVec& rhsY, const MantidVec& rhsE, MantidVec& YOut, MantidVec& EOut)
  {}
  void performBinaryOperation(const MantidVec& lhsX, const MantidVec& lhsY, const MantidVec& lhsE,
                              const double& rhsY, const double& rhsE, MantidVec& YOut, MantidVec& EOut)
  {}
};

class CommutativeBinaryOperationTest : public CxxTest::TestSuite
{
public:

  void testcheckSizeCompatibility1D1D()
  {
    int sizex = 10;
    // Register the workspace in the data service
    Workspace1D_sptr work_in1 = WorkspaceCreationHelper::Create1DWorkspaceFib(sizex);
    Workspace1D_sptr work_in2 = WorkspaceCreationHelper::Create1DWorkspaceFib(20);
    Workspace1D_sptr work_in3 = WorkspaceCreationHelper::Create1DWorkspaceFib(10);
    Workspace1D_sptr work_in4 = WorkspaceCreationHelper::Create1DWorkspaceFib(5);
    Workspace1D_sptr work_in5 = WorkspaceCreationHelper::Create1DWorkspaceFib(3);
    Workspace1D_sptr work_in6 = WorkspaceCreationHelper::Create1DWorkspaceFib(1);
    CommutativeBinaryOpHelper helper;
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in2));
    TS_ASSERT(helper.checkSizeCompatibility(work_in1,work_in3));
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in4));
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in5));
    TS_ASSERT(helper.checkSizeCompatibility(work_in1,work_in6));
  }

  void testcheckSizeCompatibility2D1D()
  {
    // Register the workspace in the data service
    Workspace2D_sptr work_in1 = WorkspaceCreationHelper::Create2DWorkspace(10,10);
    Workspace1D_sptr work_in2 = WorkspaceCreationHelper::Create1DWorkspaceFib(20);
    Workspace1D_sptr work_in3 = WorkspaceCreationHelper::Create1DWorkspaceFib(10);
    Workspace1D_sptr work_in4 = WorkspaceCreationHelper::Create1DWorkspaceFib(5);
    Workspace1D_sptr work_in5 = WorkspaceCreationHelper::Create1DWorkspaceFib(3);
    Workspace1D_sptr work_in6 = WorkspaceCreationHelper::Create1DWorkspaceFib(1);
    CommutativeBinaryOpHelper helper;
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in2));
    TS_ASSERT(helper.checkSizeCompatibility(work_in1,work_in3));
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in4));
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in5));
    TS_ASSERT(helper.checkSizeCompatibility(work_in1,work_in6));
  }


  void testcheckSizeCompatibility2D2D()
  {
    // Register the workspace in the data service
    Workspace2D_sptr work_in1 = WorkspaceCreationHelper::Create2DWorkspace(10,10);
    Workspace2D_sptr work_in2 = WorkspaceCreationHelper::Create2DWorkspace(20,10);
    Workspace2D_sptr work_in3 = WorkspaceCreationHelper::Create2DWorkspace(10,10);
    Workspace2D_sptr work_in4 = WorkspaceCreationHelper::Create2DWorkspace(5,5);
    Workspace2D_sptr work_in5 = WorkspaceCreationHelper::Create2DWorkspace(3,3);
    Workspace2D_sptr work_in6 = WorkspaceCreationHelper::Create2DWorkspace(1,100);
    CommutativeBinaryOpHelper helper;
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in2));
    TS_ASSERT(helper.checkSizeCompatibility(work_in1,work_in3));
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in4));
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in5));
    TS_ASSERT(!helper.checkSizeCompatibility(work_in1,work_in6));
  }


  void checkOutputWorkspace(MatrixWorkspace_sptr ws, MatrixWorkspace_sptr wsIn1,MatrixWorkspace_sptr wsIn2 ) const
  {
    int targetsize = (wsIn1->size()>wsIn2->size())?wsIn1->size():wsIn2->size();
    TS_ASSERT_EQUALS(ws->size(),targetsize);
    //check they arre all 0
    for(MatrixWorkspace::iterator ti(*ws); ti != ti.end(); ++ti)
    {
      TS_ASSERT_THROWS_NOTHING
      (
        LocatedDataRef tr = *ti;
        TS_ASSERT_DELTA(tr.X(),0,0.0001);
        TS_ASSERT_DELTA(tr.Y(),0,0.0001);
        TS_ASSERT_DELTA(tr.E(),0,0.0001);
      )
    }
  }

};

#endif /*COMMUTATIVEBINARYOPERATIONTEST_H_*/
