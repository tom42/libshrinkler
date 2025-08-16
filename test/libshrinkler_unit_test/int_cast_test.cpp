// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <limits>

import libshrinkler;

namespace libshrinkler_unit_test
{

using libshrinkler::int_cast;

TEST_CASE("int_cast_test")
{
    // TODO: check int_max cast to size_t => should throw => this can only be done if size_t is wider than int, no?
    constexpr std::size_t min = 0;
    constexpr std::size_t max = std::numeric_limits<int>::max();

    CHECK(int_cast(min) == min);
    CHECK(int_cast(max) == max);
}

}
