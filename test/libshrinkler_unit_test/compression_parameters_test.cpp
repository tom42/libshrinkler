// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import libshrinkler;

namespace libshrinkler_unit_test
{

TEST_CASE("compression_parameters_test")
{
    SECTION("construct_with_preset_1")
    {
        const libshrinkler::compression_parameters parameters(1);

        CHECK(parameters.parity_context() == true);
        CHECK(parameters.iterations() == 1);
        CHECK(parameters.length_margin() == 1);
        CHECK(parameters.same_length() == 10);
        CHECK(parameters.effort() == 100);
        CHECK(parameters.skip_length() == 1000);
        CHECK(parameters.references() == 100000);
        CHECK(parameters.verbose() == false);
    }

    SECTION("construct_with_preset_9")
    {
        CHECK(false);
    }
}

}
