# SPDX-FileCopyrightText: 2024 Thomas Mathys
# SPDX-License-Identifier: MIT

# Set up Catch2.
# This makes Catch2 available.
# find_package is tried first. If that fails, FetchContent is tried.
# Note that this function does not include CTest. This must be done
# manually, in the top level CMakeLists.txt (as required by CTest).
function(vtg_testing_setup_catch2 version)
  # Try installed Catch2 package first.
  find_package(Catch2 ${version} QUIET)

  # If Catch2 package is not installed, get it using FetchContent.
  if(Catch2_FOUND)
    message(STATUS "Catch2 ${version} found")
  else()
    message(STATUS "Catch2 ${version} not found. Will obtain it using FetchContent")
    include(FetchContent)
    FetchContent_Declare(
      Catch2
      SYSTEM
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v${version}
    )
    FetchContent_MakeAvailable(Catch2)

    # Make Catch.cmake and CatchShardTests.cmake available.
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
  endif()

  # Include Catch.cmake to get the catch_discover_tests function.
  include(Catch)

endfunction()
