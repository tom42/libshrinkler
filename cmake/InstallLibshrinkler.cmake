# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install libshrinkler module and exports
# Based on https://crascit.com/2024/04/04/cxx-modules-cmake-shared-libraries/#h-installing-shared-libraries-with-c-20-modules
install(
  TARGETS libshrinkler
  EXPORT libshrinklerTargets
  FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libshrinkler/src
  FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
  EXPORT libshrinklerTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libshrinkler
  CXX_MODULES_DIRECTORY .)

# Generate and install libshrinklerConfigVersion.cmake
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/libshrinklerConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion)
install(
  FILES
  "${PROJECT_BINARY_DIR}/libshrinklerConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libshrinkler)

# Generate and install libshrinklerConfig.cmake
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/libshrinklerConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/libshrinklerConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libshrinkler)
install(
  FILES
  "${PROJECT_BINARY_DIR}/libshrinklerConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libshrinkler)
