// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2020 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include <cxxtest/TestSuite.h>

#include "MantidAPI/FunctionFactory.h"
#include "MantidCurveFitting/Algorithms/DoublePulseFit.h"

using namespace Mantid::CurveFitting::Algorithms;
using namespace Mantid::API;

class DoublePulseFitTest : public CxxTest::TestSuite {
public:
  void test_doublepulse_function_conversion_for_single_function() {
    auto initialFunction = FunctionFactory::Instance().createInitialized(
        "name=ExpDecay, Height=5, Lifetime=2");
    auto doublePulseFunction = getDoublePulseFunction(
        std::dynamic_pointer_cast<ParamFunction>(initialFunction), 5.0);
    TS_ASSERT_EQUALS(
        "composite=Convolution,FixResolution=false,NumDeriv=true;name=ExpDecay,"
        "Height=5,Lifetime=2;(name=DeltaFunction,"
        "Height=0.110727,Centre=-5,ties=(Height=0.110727,Centre=-5);name="
        "DeltaFunction,Height=0.889273,Centre=0,ties=(Height=0.889273,Centre=0)"
        ")",
        doublePulseFunction->asString());
  }

  void test_doublepulse_function_conversion_for_multiDomain_function() {
    auto initialFunction =
        FunctionFactory::Instance().createInitializedMultiDomainFunction(
            "name=ExpDecay, Height=5, Lifetime=2", 2);
    auto doublePulseFunction = getDoublePulseFunction(initialFunction, 5.0);
    TS_ASSERT_EQUALS(
        "composite=MultiDomainFunction,NumDeriv=true;(composite=Convolution,"
        "FixResolution=false,NumDeriv=true,$domains=i;name=ExpDecay,Height=5,"
        "Lifetime=2;(name=DeltaFunction,Height=0.110727,Centre=-5,ties=(Height="
        "0.110727,Centre=-5);name=DeltaFunction,Height=0.889273,Centre=0,ties=("
        "Height=0.889273,Centre=0)));(composite=Convolution,FixResolution="
        "false,NumDeriv=true,$domains=i;name=ExpDecay,Height=5,Lifetime=2;("
        "name=DeltaFunction,Height=0.110727,Centre=-5,ties=(Height=0.110727,"
        "Centre=-5);name=DeltaFunction,Height=0.889273,Centre=0,ties=(Height=0."
        "889273,Centre=0)))",
        doublePulseFunction->asString());
  }

  void
  test_double_pulse_function_throws_exception_if_inner_function_is_composite() {
    auto initialFunction =
        FunctionFactory::Instance().createInitializedMultiDomainFunction(
            "(name=ExpDecay, Height=5, Lifetime=2; name=ExpDecay, Height=7, "
            "Lifetime=3)",
            5);
    TS_ASSERT_THROWS(getDoublePulseFunction(initialFunction, 5.0),
                     const std::runtime_error &);
  }

  void
  test_converting_from_double_pulse_functions_correctly_for_single_function() {
    auto initialFunction = FunctionFactory::Instance().createInitialized(
        "name=ExpDecay, Height=5, Lifetime=2");
    auto doublePulseFunction = getDoublePulseFunction(
        std::dynamic_pointer_cast<ParamFunction>(initialFunction), 5.0);
    auto restored_function = extractInnerFunction(
        std::dynamic_pointer_cast<Mantid::CurveFitting::Functions::Convolution>(
            doublePulseFunction));

    TS_ASSERT_EQUALS(initialFunction->asString(),
                     restored_function->asString());
  }

  void
  test_converting_from_double_pulse_functions_correctly_for_multidomain_function() {
    auto initialFunction =
        FunctionFactory::Instance().createInitializedMultiDomainFunction(
            "name=ExpDecay, Height=5, Lifetime=2", 2);
    auto doublePulseFunction = getDoublePulseFunction(initialFunction, 5.0);
    auto restored_function = extractInnerFunction(
        std::dynamic_pointer_cast<MultiDomainFunction>(doublePulseFunction));

    TS_ASSERT_EQUALS(initialFunction->asString(),
                     restored_function->asString());
  }

  void
  test_extracting_function_throws_exception_when_passed_non_convolution_multidomainFunction() {
    auto initialFunction =
        FunctionFactory::Instance().createInitializedMultiDomainFunction(
            "(name=ExpDecay, Height=5, Lifetime=2; name=ExpDecay, Height=7, "
            "Lifetime=3)",
            5);
    TS_ASSERT_THROWS(extractInnerFunction(initialFunction),
                     const std::runtime_error &);
  }
};