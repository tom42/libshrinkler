// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import libshrinkler;

namespace libshrinkler_unit_test
{

using libshrinkler::encoder_parameters;
using libshrinkler::endianness;

TEST_CASE("encoder_parameters_test")
{
    SECTION("construct_with_preset_1")
    {
        const encoder_parameters parameters(1);

        CHECK(parameters.parity_context() == true);
        CHECK(parameters.iterations() == 1);
        CHECK(parameters.length_margin() == 1);
        CHECK(parameters.same_length() == 10);
        CHECK(parameters.effort() == 100);
        CHECK(parameters.skip_length() == 1000);
        CHECK(parameters.references() == 100000);
        CHECK(parameters.endianness() == endianness::big);
    }

    SECTION("construct_with_preset_9")
    {
        const encoder_parameters parameters(9);

        CHECK(parameters.parity_context() == true);
        CHECK(parameters.iterations() == 9);
        CHECK(parameters.length_margin() == 9);
        CHECK(parameters.same_length() == 90);
        CHECK(parameters.effort() == 900);
        CHECK(parameters.skip_length() == 9000);
        CHECK(parameters.references() == 100000);
        CHECK(parameters.endianness() == endianness::big);
    }
}

}
