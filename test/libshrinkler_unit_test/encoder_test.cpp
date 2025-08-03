// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <cstring>

import libshrinkler;

namespace libshrinkler_unit_test
{

using libshrinkler::encoder_parameters;
using libshrinkler::encoder;
using bytevector = std::vector<unsigned char>;

namespace
{

bytevector make_bytevector(const char* s)
{
    return bytevector(s, s + strlen(s));
}

}

// TODO: reimplement old tests:
//       * with parity
//       * Without parity
// TODO: new tests
//       * Big endian output
//       * Little endian output
//       * Do we even support the header thing?
//       * Empty input (would have been a natural first test...)
TEST_CASE("encoder_test")
{
    encoder_parameters parameters;
    encoder encoder;

    SECTION("parity")
    {
        // TODO: the existing old test assumes little endian with padding here, so we might want to adapt this test
        //       accordingly, meaning we'd have to set endianness to 'little'
        const auto original_data = make_bytevector("foo foo foo foo");
        const bytevector expected_encoded_data{0xc6, 0x62, 0xc8, 0x99, 0x00, 0x00, 0x39, 0x9b};
        parameters.parity_context(true);
        encoder.parameters(parameters);

        auto actual_encoded_data = encoder.encode(original_data);

        CHECK(actual_encoded_data == expected_encoded_data);
    }
}

}
