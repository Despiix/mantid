#ifndef IFUNCTIONTEST_H_
#define IFUNCTIONTEST_H_

#include "MantidAPI/AnalysisDataService.h"
#include "MantidAPI/IFunctionMW.h"
#include "MantidAPI/MatrixWorkspace.h"
#include "MantidAPI/ParamFunction.h"
#include "MantidAPI/FrameworkManager.h"
#include "MantidAPI/WorkspaceFactory.h"
#include <cxxtest/TestSuite.h>

using namespace Mantid;
using namespace Mantid::API;

class IFT_Funct: public ParamFunction, public IFunctionMW
{
public:
  IFT_Funct()
  {
    declareParameter("c0", 0.0, "this is the famous c0 blah...");
    declareParameter("c1");
    declareParameter("c2");
    declareParameter("c3");
  }

  std::string name()const{return "IFT_Funct";}

  void functionMW(double* out, const double* xValues, const size_t nData)const
  {
    double c0 = getParameter("c0");
    double c1 = getParameter("c1");
    double c2 = getParameter("c2");
    double c3 = getParameter("c3");
    for(size_t i=0;i<nData;i++)
    {
      double x = xValues[i];
      out[i] = c0 + x*(c1 + x*(c2 + x*c3));
    }
  }
  void functionDerivMW(Jacobian* out, const double* xValues, const size_t nData)
  {
    for(size_t i=0;i<nData;i++)
    {
      double x = xValues[i];
      out->set(static_cast<int>(i),0,1.);
      out->set(static_cast<int>(i),1,x);
      out->set(static_cast<int>(i),2,x*x);
      out->set(static_cast<int>(i),3,x*x*x);
    }
  }

};

class FunctionTest : public CxxTest::TestSuite
{
public:

  void testIFunction()
  {
    IFT_Funct f;

    TS_ASSERT_EQUALS(f.parameterDescription(0),"this is the famous c0 blah...");

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    TS_ASSERT_EQUALS(f.parameterDescription(0),"this is the famous c0 blah...");

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),4);

    TS_ASSERT_EQUALS(f.getParameter(0),1.0);
    TS_ASSERT_EQUALS(f.getParameter(1),1.1);
    TS_ASSERT_EQUALS(f.getParameter(2),1.2);
    TS_ASSERT_EQUALS(f.getParameter(3),1.3);

    TS_ASSERT_EQUALS(f.parameterName(0),"c0");
    TS_ASSERT_EQUALS(f.parameterName(1),"c1");
    TS_ASSERT_EQUALS(f.parameterName(2),"c2");
    TS_ASSERT_EQUALS(f.parameterName(3),"c3");

    TS_ASSERT_EQUALS(f.getParameter("c0"),1.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),1.1);
    TS_ASSERT_EQUALS(f.getParameter("c2"),1.2);
    TS_ASSERT_EQUALS(f.getParameter("c3"),1.3);

    TS_ASSERT_EQUALS(f.parameterIndex("c0"),0);
    TS_ASSERT_EQUALS(f.parameterIndex("c1"),1);
    TS_ASSERT_EQUALS(f.parameterIndex("c2"),2);
    TS_ASSERT_EQUALS(f.parameterIndex("c3"),3);

    std::string str = "name=IFT_Funct,c0=1,c1=1.1,c2=1.2,c3=1.3";

    TS_ASSERT_EQUALS(f.asString(),str);

    TS_ASSERT_EQUALS(f.activeParameter(0),1.0);
    TS_ASSERT_EQUALS(f.activeParameter(1),1.1);
    TS_ASSERT_EQUALS(f.activeParameter(2),1.2);
    TS_ASSERT_EQUALS(f.activeParameter(3),1.3);

    TS_ASSERT_EQUALS(f.nameOfActive(0),"c0");
    TS_ASSERT_EQUALS(f.nameOfActive(1),"c1");
    TS_ASSERT_EQUALS(f.nameOfActive(2),"c2");
    TS_ASSERT_EQUALS(f.nameOfActive(3),"c3");

    TS_ASSERT_EQUALS(f.indexOfActive(0),0);
    TS_ASSERT_EQUALS(f.indexOfActive(1),1);
    TS_ASSERT_EQUALS(f.indexOfActive(2),2);
    TS_ASSERT_EQUALS(f.indexOfActive(3),3);

    TS_ASSERT(   f.isActive(0));
    TS_ASSERT(   f.isActive(1));
    TS_ASSERT(   f.isActive(2));
    TS_ASSERT(   f.isActive(3));

    TS_ASSERT_EQUALS(f.activeIndex(0),0);
    TS_ASSERT_EQUALS(f.activeIndex(1),1);
    TS_ASSERT_EQUALS(f.activeIndex(2),2);
    TS_ASSERT_EQUALS(f.activeIndex(3),3);

  }

  void testRemoveActive()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.removeActive(1);
    f.removeActive(3);

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),2);

    TS_ASSERT_EQUALS(f.activeParameter(0),1.0);
    TS_ASSERT_EQUALS(f.activeParameter(1),1.2);

    TS_ASSERT_EQUALS(f.nameOfActive(0),"c0");
    TS_ASSERT_EQUALS(f.nameOfActive(1),"c2");

    TS_ASSERT_EQUALS(f.indexOfActive(0),0);
    TS_ASSERT_EQUALS(f.indexOfActive(1),2);

    TS_ASSERT(   f.isActive(0));
    TS_ASSERT( ! f.isActive(1));
    TS_ASSERT(   f.isActive(2));
    TS_ASSERT( ! f.isActive(3));

    TS_ASSERT_EQUALS(f.activeIndex(0),0);
    TS_ASSERT_EQUALS(f.activeIndex(1),-1);
    TS_ASSERT_EQUALS(f.activeIndex(2),1);
    TS_ASSERT_EQUALS(f.activeIndex(3),-1);

  }

  void testRestoreActive()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.removeActive(1);
    f.removeActive(3);

    f.restoreActive(3);

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),3);

    TS_ASSERT_EQUALS(f.activeParameter(0),1.0);
    TS_ASSERT_EQUALS(f.activeParameter(1),1.2);
    TS_ASSERT_EQUALS(f.activeParameter(2),1.3);

    TS_ASSERT_EQUALS(f.nameOfActive(0),"c0");
    TS_ASSERT_EQUALS(f.nameOfActive(1),"c2");
    TS_ASSERT_EQUALS(f.nameOfActive(2),"c3");

    TS_ASSERT_EQUALS(f.indexOfActive(0),0);
    TS_ASSERT_EQUALS(f.indexOfActive(1),2);
    TS_ASSERT_EQUALS(f.indexOfActive(2),3);

    TS_ASSERT(   f.isActive(0));
    TS_ASSERT( ! f.isActive(1));
    TS_ASSERT(   f.isActive(2));
    TS_ASSERT(   f.isActive(3));

    TS_ASSERT_EQUALS(f.activeIndex(0),0);
    TS_ASSERT_EQUALS(f.activeIndex(1),-1);
    TS_ASSERT_EQUALS(f.activeIndex(2),1);
    TS_ASSERT_EQUALS(f.activeIndex(3),2);

  }

  void testSetActiveParameter()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.removeActive(1);
    f.removeActive(3);

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),2);

    f.setActiveParameter(0,2.0);
    f.setActiveParameter(1,2.1);

    TS_ASSERT_EQUALS(f.activeParameter(0),2.0);
    TS_ASSERT_EQUALS(f.activeParameter(1),2.1);

    TS_ASSERT_EQUALS(f.getParameter(0),2.0);
    TS_ASSERT_EQUALS(f.getParameter(1),1.1);
    TS_ASSERT_EQUALS(f.getParameter(2),2.1);
    TS_ASSERT_EQUALS(f.getParameter(3),1.3);

    TS_ASSERT_EQUALS(f.getParameter("c0"),2.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),1.1);
    TS_ASSERT_EQUALS(f.getParameter("c2"),2.1);
    TS_ASSERT_EQUALS(f.getParameter("c3"),1.3);

  }

  void testTie()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.tie("c1","0");
    f.tie("c3","0");

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),2);

    TS_ASSERT_EQUALS(f.activeParameter(0),1.0);
    TS_ASSERT_EQUALS(f.activeParameter(1),1.2);

    TS_ASSERT_EQUALS(f.nameOfActive(0),"c0");
    TS_ASSERT_EQUALS(f.nameOfActive(1),"c2");

    TS_ASSERT_EQUALS(f.indexOfActive(0),0);
    TS_ASSERT_EQUALS(f.indexOfActive(1),2);

    TS_ASSERT(   f.isActive(0));
    TS_ASSERT( ! f.isActive(1));
    TS_ASSERT(   f.isActive(2));
    TS_ASSERT( ! f.isActive(3));

    TS_ASSERT_EQUALS(f.activeIndex(0),0);
    TS_ASSERT_EQUALS(f.activeIndex(1),-1);
    TS_ASSERT_EQUALS(f.activeIndex(2),1);
    TS_ASSERT_EQUALS(f.activeIndex(3),-1);

  }

  void testApplyTies()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.tie("c1","c0+4");
    f.tie("c3","c2/2");

    f.applyTies();

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),2);

    TS_ASSERT_EQUALS(f.getParameter("c0"),1.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),5.0);
    TS_ASSERT_EQUALS(f.getParameter("c2"),1.2);
    TS_ASSERT_EQUALS(f.getParameter("c3"),0.6);

  }

  void testRemoveTie()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.tie("c1","c0+4");
    f.tie("c3","c2/2");

    f.applyTies();

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),2);

    TS_ASSERT_EQUALS(f.getParameter("c0"),1.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),5.0);
    TS_ASSERT_EQUALS(f.getParameter("c2"),1.2);
    TS_ASSERT_EQUALS(f.getParameter("c3"),0.6);

    f.removeTie("c3");
    f.setParameter("c3",3.3);

    f.applyTies();

    TS_ASSERT_EQUALS(f.nActive(),3);

    TS_ASSERT_EQUALS(f.getParameter("c0"),1.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),5.0);
    TS_ASSERT_EQUALS(f.getParameter("c2"),1.2);
    TS_ASSERT_EQUALS(f.getParameter("c3"),3.3);

    TS_ASSERT(   f.isActive(0));
    TS_ASSERT( ! f.isActive(1));
    TS_ASSERT(   f.isActive(2));
    TS_ASSERT(   f.isActive(3));

    TS_ASSERT_EQUALS(f.activeIndex(0),0);
    TS_ASSERT_EQUALS(f.activeIndex(1),-1);
    TS_ASSERT_EQUALS(f.activeIndex(2),1);
    TS_ASSERT_EQUALS(f.activeIndex(3),2);

  }

  void testClearTies()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.tie("c1","c0+4");
    f.tie("c3","c2/2");

    f.applyTies();

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),2);

    TS_ASSERT_EQUALS(f.getParameter("c0"),1.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),5.0);
    TS_ASSERT_EQUALS(f.getParameter("c2"),1.2);
    TS_ASSERT_EQUALS(f.getParameter("c3"),0.6);

    f.clearTies();
    f.setParameter("c1",3.1);
    f.setParameter("c3",3.3);

    f.applyTies();

    TS_ASSERT_EQUALS(f.nActive(),4);

    TS_ASSERT_EQUALS(f.getParameter("c0"),1.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),3.1);
    TS_ASSERT_EQUALS(f.getParameter("c2"),1.2);
    TS_ASSERT_EQUALS(f.getParameter("c3"),3.3);

    TS_ASSERT(   f.isActive(0));
    TS_ASSERT(   f.isActive(1));
    TS_ASSERT(   f.isActive(2));
    TS_ASSERT(   f.isActive(3));

    TS_ASSERT_EQUALS(f.activeIndex(0),0);
    TS_ASSERT_EQUALS(f.activeIndex(1),1);
    TS_ASSERT_EQUALS(f.activeIndex(2),2);
    TS_ASSERT_EQUALS(f.activeIndex(3),3);

  }

  void testUpdateActive()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    f.setParameter("c2",1.2);
    f.setParameter("c3",1.3);

    f.tie("c1","c0+4");
    f.tie("c3","c2/2");

    double in[] = {6.,22.2};

    f.updateActive(in);

    TS_ASSERT_EQUALS(f.nParams(),4);
    TS_ASSERT_EQUALS(f.nActive(),2);

    TS_ASSERT_EQUALS(f.getParameter("c0"),6.0);
    TS_ASSERT_EQUALS(f.getParameter("c1"),10.0);
    TS_ASSERT_EQUALS(f.getParameter("c2"),22.2);
    TS_ASSERT_EQUALS(f.getParameter("c3"),11.1);

  }

  void testExplicitlySet()
  {
    IFT_Funct f;

    f.setParameter("c0",1.0);
    f.setParameter("c1",1.1);
    TS_ASSERT(f.isExplicitlySet(0));
    TS_ASSERT(f.isExplicitlySet(1));
    TS_ASSERT(!f.isExplicitlySet(2));
    TS_ASSERT(!f.isExplicitlySet(3));
  }

  void test_setWorkspace_works()
  {
    FrameworkManager::Instance();
    MatrixWorkspace_sptr ws = WorkspaceFactory::Instance().create("Workspace2D",10,11,10);

    MantidVec& x = ws->dataX(3);
    MantidVec& y = ws->dataY(3);
    for(size_t i=0; i < y.size(); ++i)
    {
      x[i] = 0.1 * static_cast<double>(i);
      y[i] = static_cast<double>(i);
    }
    x.back() = 0.1 * static_cast<double>(y.size());
    AnalysisDataService::Instance().add("IFT_Test_WS",ws);
    IFT_Funct f;
    TS_ASSERT_THROWS_NOTHING(f.setWorkspace(ws,"WorkspaceIndex=3,StartX=0.2,EndX = 0.8"));
    TS_ASSERT_EQUALS(f.dataSize(),8);
    TS_ASSERT_EQUALS(f.getData(),&y[2]);
    AnalysisDataService::Instance().remove("IFT_Test_WS");
  }

  /** Refs #3003: Test to make sure setMatrix works in parallel */
  void test_setWorkspace_works_inParallel()
  {
    double expected;
    int numpixels = 15000;
    MatrixWorkspace_sptr ws = WorkspaceFactory::Instance().create("Workspace2D",numpixels,11,10);
    for (size_t wi=0; wi<ws->getNumberHistograms(); wi++)
    {
      MantidVec& x = ws->dataX(wi);
      MantidVec& y = ws->dataY(wi);
      for(size_t i=0; i < y.size(); ++i)
      {
        x[i] = 0.1 * double(i);
        y[i] = double(i);
      }
      x.back() = 0.1 * double(y.size());
      expected = y[2];
    }

    // NOTE: In parallel, there is a segfault on SNS build servers ubuntu-10.04 and RHEL6. The rest pass!?
    PARALLEL_FOR_NO_WSP_CHECK()
    for (int i=0; i<numpixels; i++)
    {
      IFT_Funct f;
      std::stringstream mess;
      mess << "WorkspaceIndex=" << i << ",StartX=0.2,EndX = 0.8";
      TS_ASSERT_THROWS_NOTHING(f.setWorkspace(ws, mess.str() ));
      TS_ASSERT_EQUALS(f.dataSize(),8);
      TS_ASSERT_EQUALS(*f.getData(), expected);
    }
  }

  private:
  void interrupt()
  {
    int iii;
    std::cerr<<"Enter a number:";
    std::cin>>iii;
  }

};

#endif /*IFUNCTIONTEST_H_*/
