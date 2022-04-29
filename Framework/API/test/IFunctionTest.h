// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include <cxxtest/TestSuite.h>

#include "MantidAPI/IFunction.h"

using namespace Mantid::API;

class MockFunction : public IFunction {
public:
  MockFunction()
      : IFunction(), m_parameterValues(4), m_parameterIndexes{{"A", 0}, {"B", 1}, {"C", 2}, {"D", 3}},
        m_parameterNames{{0, "A"}, {1, "B"}, {2, "C"}, {3, "D"}}, m_parameterStatus{Active, Active, Active, Active} {}
  std::string name() const override { return "MockFunction"; }
  void function(const Mantid::API::FunctionDomain &, Mantid::API::FunctionValues &) const override {}
  void setParameter(const std::string &parName, const double &value, bool = true) override {
    m_parameterValues[m_parameterIndexes.find(parName)->second] = value;
  }
  void setParameter(std::size_t index, const double &value, bool = true) override { m_parameterValues[index] = value; }
  void setParameterDescription(const std::string &, const std::string &) override {}
  void setParameterDescription(std::size_t, const std::string &) override {}
  double getParameter(const std::string &parName) const override {
    return m_parameterValues[m_parameterIndexes.find(parName)->second];
  }
  double getParameter(std::size_t index) const override { return m_parameterValues[index]; }
  bool hasParameter(const std::string &parName) const override { return m_parameterIndexes.count(parName) > 0; }
  size_t nParams() const override { return m_parameterValues.size(); }
  size_t parameterIndex(const std::string &parName) const override { return m_parameterIndexes.find(parName)->second; }
  std::string parameterName(std::size_t index) const override { return m_parameterNames.find(index)->second; }
  std::string parameterDescription(std::size_t) const override { return ""; }
  bool isExplicitlySet(std::size_t) const override { return true; }
  double getError(std::size_t) const override { return 0.0; }
  double getError(const std::string &) const override { return 0.0; }
  void setError(std::size_t, double) override {}
  void setError(const std::string &, double) override {}
  size_t getParameterIndex(const Mantid::API::ParameterReference &ref) const override {
    if (ref.getLocalFunction() == this && ref.getLocalIndex() < nParams()) {
      return ref.getLocalIndex();
    }
    return nParams();
  }
  void setParameterStatus(std::size_t index, ParameterStatus status) override { m_parameterStatus[index] = status; }
  ParameterStatus getParameterStatus(std::size_t index) const override { return m_parameterStatus[index]; }
  void declareParameter(const std::string &, double, const std::string &) override {}

private:
  std::vector<double> m_parameterValues;
  std::map<std::string, size_t> m_parameterIndexes;
  std::map<size_t, std::string> m_parameterNames;
  std::vector<ParameterStatus> m_parameterStatus;
};

class IFunctionTest : public CxxTest::TestSuite {
public:
  // This pair of boilerplate methods prevent the suite being created statically
  // This means the constructor isn't called when running other tests
  static IFunctionTest *createSuite() { return new IFunctionTest(); }
  static void destroySuite(IFunctionTest *suite) { delete suite; }

  void testTie() {
    MockFunction fun;
    fun.tie("A", "2*B");
    auto aTie = fun.getTie(0);
    TS_ASSERT(aTie);
    TS_ASSERT(!fun.isActive(0));
    TS_ASSERT(!fun.isFixed(0));

    fun.tie("C", "3");
    auto cTie = fun.getTie(2);
    TS_ASSERT(!cTie);
    TS_ASSERT(!fun.isActive(2));
    TS_ASSERT(fun.isFixed(2));
    TS_ASSERT_EQUALS(fun.getParameter(2), 3.0);

    fun.setParameter("B", 4.0);
    TS_ASSERT_EQUALS(fun.getParameter("A"), 0.0);
    fun.applyTies();
    TS_ASSERT_EQUALS(fun.getParameter("A"), 8.0);
    TS_ASSERT_EQUALS(fun.getParameter("B"), 4.0);
    TS_ASSERT_EQUALS(fun.getParameter("C"), 3.0);
    TS_ASSERT_EQUALS(fun.getParameter("D"), 0.0);
  }

  void testFixAll() {
    MockFunction fun;
    fun.tie("A", "2*B");
    fun.setParameter("B", 4.0);
    fun.fixAll();
    TS_ASSERT(!fun.isFixed(0));
    TS_ASSERT(!fun.isActive(0));
    TS_ASSERT(fun.isFixed(1));
    TS_ASSERT(fun.isFixed(2));
    TS_ASSERT(fun.isFixed(3));
    fun.applyTies();
    TS_ASSERT_EQUALS(fun.getParameter("A"), 8.0);
    TS_ASSERT_EQUALS(fun.getParameter("B"), 4.0);
    TS_ASSERT_EQUALS(fun.getParameter("C"), 0.0);
    TS_ASSERT_EQUALS(fun.getParameter("D"), 0.0);
  }

  void testUnfixAll() {
    MockFunction fun;
    fun.tie("A", "2*B");
    fun.setParameter("B", 4.0);
    fun.fixAll();
    fun.unfixAll();
    TS_ASSERT(!fun.isFixed(0));
    TS_ASSERT(!fun.isActive(0));
    TS_ASSERT(!fun.isFixed(1));
    TS_ASSERT(!fun.isFixed(2));
    TS_ASSERT(!fun.isFixed(3));
    fun.applyTies();
    TS_ASSERT_EQUALS(fun.getParameter("A"), 8.0);
    TS_ASSERT_EQUALS(fun.getParameter("B"), 4.0);
    TS_ASSERT_EQUALS(fun.getParameter("C"), 0.0);
    TS_ASSERT_EQUALS(fun.getParameter("D"), 0.0);
  }

  void test_default_calculation_of_step_size_with_zero_parameter_value() {
    MockFunction fun;

    const double parameterValue = 0.0;

    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue), std::numeric_limits<double>::epsilon() * 100);
  }

  void test_default_calculation_of_step_size_with_small_parameter_values() {
    MockFunction fun;

    const double parameterValue1 = 100.0 * std::numeric_limits<double>::min();
    const double parameterValue2 = -100.0 * std::numeric_limits<double>::min();

    const double expectedStep = std::numeric_limits<double>::epsilon() * 100;
    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue1), expectedStep);
    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue2), expectedStep);
  }

  void test_default_calculation_of_step_size_with_larger_parameter_values() {
    MockFunction fun;

    const double parameterValue1 = 5.0;
    const double parameterValue2 = -5.0;

    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue1), parameterValue1 * 0.001);
    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue2), parameterValue2 * 0.001);
  }

  void test_sqrt_epsilon_calculation_of_step_size_with_zero_parameter_value() {
    MockFunction fun;
    fun.setStepSizeMethod(MockFunction::StepSizeMethod::SQRT_EPSILON);

    const double parameterValue = 0.0;

    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue), sqrt(std::numeric_limits<double>::epsilon()));
  }

  void test_sqrt_epsilon_calculation_of_step_size_with_small_parameter_values() {
    MockFunction fun;
    fun.setStepSizeMethod(MockFunction::StepSizeMethod::SQRT_EPSILON);

    const double parameterValue1 = 0.9;
    const double parameterValue2 = -0.9;

    const double expectedStep = sqrt(std::numeric_limits<double>::epsilon());
    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue1), expectedStep);
    TS_ASSERT_EQUALS(fun.calculateStepSize(parameterValue2), expectedStep);
  }

  void test_sqrt_epsilon_calculation_of_step_size_with_large_parameter_values() {
    MockFunction fun;
    fun.setStepSizeMethod(MockFunction::StepSizeMethod::SQRT_EPSILON);

    const double parameterValue1 = 1.1;
    const double parameterValue2 = 1.1;

    const double sqrtEpsilon = sqrt(std::numeric_limits<double>::epsilon());
    TS_ASSERT_DELTA(fun.calculateStepSize(parameterValue1), parameterValue1 * sqrtEpsilon);
    TS_ASSERT_DELTA(fun.calculateStepSize(parameterValue2), parameterValue2 * sqrtEpsilon);
  }
};
