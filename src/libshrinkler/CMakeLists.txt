# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

set(
  libshrinkler_sources
  PUBLIC FILE_SET CXX_MODULES FILES
  libshrinkler.cppm
  PRIVATE
  compression_parameters.cpp)

add_library(libshrinkler)
target_sources(libshrinkler ${libshrinkler_sources})
target_compile_features(libshrinkler PUBLIC cxx_std_23)
vtg_target_enable_warnings(libshrinkler)

if(libshrinkler_INSTALL_STATIC_LIBRARY)
  include(InstallLibshrinkler)
endif()
