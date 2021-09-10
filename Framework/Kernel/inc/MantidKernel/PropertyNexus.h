// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2011 ISIS Rutherford Appleton Laboratory UKRI,
//   NScD Oak Ridge National Laboratory, European Spallation Source,
//   Institut Laue - Langevin & CSNS, Institute of High Energy Physics, CAS
// SPDX - License - Identifier: GPL - 3.0 +
#pragma once

#include "MantidKernel/NexusHDF5Descriptor.h"
#include "MantidKernel/System.h"
#include <memory>
#include <string>

// Forward declare
namespace NeXus {
class File;
}

namespace Mantid {
namespace Kernel {
// Forward declare
class Property;

/** Namespace with helper methods for loading and saving Property's (logs)
 * to NXS files.

  @author Janik Zikovsky
  @date 2011-09-08
*/
namespace PropertyNexus {

DLLExport std::unique_ptr<Property> loadProperty(::NeXus::File *file, const std::string &group,
                                                 const std::shared_ptr<Mantid::Kernel::NexusHDF5Descriptor> &fileInfo,
                                                 const std::string &prefix);

DLLExport std::unique_ptr<Property> loadProperty(::NeXus::File *file, const std::string &group);

} // namespace PropertyNexus

} // namespace Kernel
} // namespace Mantid
