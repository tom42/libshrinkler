# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

add_executable(
  libshrinkler_unit_test
  encoder_parameters_test.cpp
  encoder_test.cpp
  int_cast_test.cpp)
vtg_target_enable_warnings(libshrinkler_unit_test)
target_link_libraries(libshrinkler_unit_test PRIVATE libshrinkler_unit_testing Catch2::Catch2WithMain)
catch_discover_tests(libshrinkler_unit_test)
