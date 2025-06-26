// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import libshrinkler;

namespace libshrinkler_unit_test
{

TEST_CASE("compression_parameters_test")
{
    SECTION("construct_with_preset_2")
    {
        const libshrinkler::compression_parameters parameters(2);

        CHECK(parameters.parity_context() == true);
        CHECK(parameters.iterations() == 2);
        CHECK(parameters.length_margin() == 2);
        CHECK(parameters.same_length() == 20);
        CHECK(parameters.effort() == 200);
        CHECK(parameters.skip_length() == 2000);
        CHECK(parameters.references() == 100000);
        CHECK(parameters.verbose() == false);
    }

    SECTION("construct_with_preset_9")
    {
        CHECK(false);
    }
}

}
