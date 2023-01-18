// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2022 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include "MantidKernel/DllConfig.h"
#include "MantidKernel/TimeSeriesProperty.h"
#include "MantidKernel/TimeSplitter.h"

namespace Mantid {
namespace Kernel {

/** TimeROI : Object that holds information about when the time measurement was active.
 */
class MANTID_KERNEL_DLL TimeROI {
public:
  /// the underlying property needs a name
  static const std::string NAME;

  TimeROI();
  TimeROI(const Types::Core::DateAndTime &startTime, const Types::Core::DateAndTime &stopTime);
  TimeROI(const Kernel::TimeSeriesProperty<bool> &filter);
  double durationInSeconds() const;
  double durationInSeconds(const Types::Core::DateAndTime &startTime, const Types::Core::DateAndTime &stopTime) const;
  std::size_t numBoundaries() const;
  bool empty() const;
  void addROI(const std::string &startTime, const std::string &stopTime);
  void addROI(const Types::Core::DateAndTime &startTime, const Types::Core::DateAndTime &stopTime);
  void addROI(const std::time_t &startTime, const std::time_t &stopTime);
  void addMask(const std::string &startTime, const std::string &stopTime);
  void addMask(const Types::Core::DateAndTime &startTime, const Types::Core::DateAndTime &stopTime);
  void addMask(const std::time_t &startTime, const std::time_t &stopTime);
  bool valueAtTime(const Types::Core::DateAndTime &time) const;
  void replaceROI(const TimeSeriesProperty<bool> *roi);
  void replaceROI(const TimeROI &other);
  void update_union(const TimeROI &other);
  void update_intersection(const TimeROI &other);
  void removeRedundantEntries();
  const std::vector<SplittingInterval> toSplitters() const;
  bool operator==(const TimeROI &other) const;
  void debugPrint() const;
  size_t getMemorySize() const;

  // nexus items
  void saveNexus(::NeXus::File *file) const;

private:
  std::vector<Types::Core::DateAndTime> getAllTimes(const TimeROI &other);
  void replaceValues(const std::vector<Types::Core::DateAndTime> &times, const std::vector<bool> &values);
  bool isCompletelyInROI(const Types::Core::DateAndTime &startTime, const Types::Core::DateAndTime &stopTime) const;
  /**
   * @brief m_roi private member that holds most of the information
   *
   * This handles the details of the ROI and guarantees that views into the underlying structure is sorted by time.
   */
  TimeSeriesProperty<bool> m_roi;
};

} // namespace Kernel
} // namespace Mantid
