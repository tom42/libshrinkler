// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cstddef>
#include <limits>
#include <stdexcept>

import libshrinkler;

namespace libshrinkler_unit_test
{

using libshrinkler::int_cast;
using Catch::Matchers::Matches;
using Catch::Matchers::MessageMatches;

TEST_CASE("int_cast_test")
{
    constexpr std::size_t min = 0;
    constexpr std::size_t max = std::numeric_limits<int>::max();

    CHECK(int_cast(min) == min);
    CHECK(int_cast(max) == max);
    CHECK_THROWS_MATCHES(
        int_cast(max + 1),
        std::overflow_error,
        MessageMatches(Matches("cannot convert .* to int")));
}

}
