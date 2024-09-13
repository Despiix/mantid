// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "MantidKernel/DllConfig.h"

namespace Mantid {
namespace Kernel {
/// Test for equality of doubles using compiler-defined precision
template <typename T> MANTID_KERNEL_DLL bool equals(T const x, T const y);
/// Test whether x<=y within machine precision
template <typename T> MANTID_KERNEL_DLL bool ltEquals(T const x, T const y);
/// Test whether x>=y within machine precision
template <typename T> MANTID_KERNEL_DLL bool gtEquals(T const x, T const y);
/// Calculate absolute difference between x, y
template <typename T> MANTID_KERNEL_DLL T absoluteDifference(T const x, T const y);
/// Calculate relative difference between x, y
template <typename T> MANTID_KERNEL_DLL T relativeDifference(T const x, T const y);
/// Test whether x, y are within absolute tolerance tol
template <typename T> MANTID_KERNEL_DLL bool withinAbsoluteDifference(T const x, T const y, T const tolerance);
/// Test whether x, y are within relative tolerance tol
template <typename T> MANTID_KERNEL_DLL bool withinRelativeDifference(T const x, T const y, T const tolerance);
} // namespace Kernel
} // namespace Mantid
