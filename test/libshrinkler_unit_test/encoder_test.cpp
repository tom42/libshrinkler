// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import libshrinkler;

namespace libshrinkler_unit_test
{

using libshrinkler::encoder_parameters;
using libshrinkler::encoder;

// TODO: reimplement old tests:
//       * with parity
//       * Without parity
// TODO: new tests
//       * Big endian output
//       * Little endian output
//       * Do we even support the header thing?
TEST_CASE("encoder_test")
{
    encoder_parameters parameters;
    // TODO: implement test
    //       * create encoder
    //       * set parameters on encoder
    //       * encode test data
    //       * compare against expected encoded data
}

}
