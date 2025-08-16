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

add_library(libshrinkler)
target_sources(libshrinkler ${libshrinkler_sources})
target_compile_features(libshrinkler PUBLIC cxx_std_23)
target_include_directories(libshrinkler SYSTEM PRIVATE ../shrinkler/cruncher)
vtg_target_enable_warnings(libshrinkler)

if(libshrinkler_INSTALL_STATIC_LIBRARY)
  include(InstallLibshrinkler)
endif()
