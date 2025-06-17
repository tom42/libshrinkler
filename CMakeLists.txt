# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.31.5)
project(libshrinkler VERSION 0.0.1 LANGUAGES CXX)
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_LIST_DIR}/cmake")


################################################################################
# Options
################################################################################

option(libshrinkler_BUILD_TESTING "Build libshrinkler tests" OFF)
option(libshrinkler_ENABLE_WARNINGS "Build libshrinkler with warnings enabled" OFF)
option(libshrinkler_INSTALL_STATIC_LIBRARY "Install the libshrinkler static library" ${PROJECT_IS_TOP_LEVEL})


################################################################################
# Set up testing
################################################################################

if(libshrinkler_BUILD_TESTING)
  enable_testing()
  include(VtgTesting)
  vtg_testing_setup_catch2(3.4.0)
endif()
