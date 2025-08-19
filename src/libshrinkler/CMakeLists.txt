# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

set(
  libshrinkler_sources
  PUBLIC FILE_SET CXX_MODULES FILES
  endianness.cppm
  libshrinkler.cppm
  util.cppm
  PRIVATE
  encoder.cpp
  encoder_parameters.cpp
  endianness.cpp)

# Production version of libshrinkler
add_library(libshrinkler)
target_sources(libshrinkler ${libshrinkler_sources})
target_compile_definitions(libshrinkler PRIVATE LIBSHRINKLER_EXPORT_FOR_UNIT_TESTING=)
target_compile_definitions(libshrinkler PRIVATE LIBSHRINKLER_COMPILER_${CMAKE_CXX_COMPILER_ID})
target_compile_features(libshrinkler PUBLIC cxx_std_23)
target_include_directories(libshrinkler SYSTEM PRIVATE ../shrinkler/cruncher)
vtg_target_enable_warnings(libshrinkler)

# Unit testing version of libshrinkler
if(libshrinkler_BUILD_TESTING)
  add_library(libshrinkler_unit_testing)
  target_sources(libshrinkler_unit_testing ${libshrinkler_sources})
  target_compile_definitions(libshrinkler_unit_testing PRIVATE LIBSHRINKLER_EXPORT_FOR_UNIT_TESTING=export)
  target_compile_definitions(libshrinkler_unit_testing PRIVATE LIBSHRINKLER_COMPILER_${CMAKE_CXX_COMPILER_ID})
  target_compile_features(libshrinkler_unit_testing PUBLIC cxx_std_23)
  target_include_directories(libshrinkler_unit_testing SYSTEM PRIVATE ../shrinkler/cruncher)
  vtg_target_enable_warnings(libshrinkler_unit_testing)
endif()

if(libshrinkler_INSTALL_STATIC_LIBRARY)
  include(InstallLibshrinkler)
endif()
