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
using libshrinkler::endianness;
using bytevector = std::vector<unsigned char>;

namespace
{

bytevector make_bytevector(const char* s)
{
    return bytevector(s, s + strlen(s));
}

}

// TODO: new tests
//       * Empty input (would have been a natural first test...)
TEST_CASE("encoder_test")
{
    encoder_parameters parameters;
    encoder encoder;

    SECTION("parity, big endian")
    {
        const auto original_data = make_bytevector("foo foo foo foo");
        const bytevector expected_encoded_data{0x99, 0xc8, 0x62, 0xc6, 0x9b, 0x39, 0x00};
        parameters.parity_context(true);
        parameters.endianness(endianness::big);
        encoder.parameters(parameters);

        auto actual_encoded_data = encoder.encode(original_data);

        CHECK(actual_encoded_data == expected_encoded_data);
    }

    SECTION("parity, little endian")
    {
        const auto original_data = make_bytevector("foo foo foo foo");
        const bytevector expected_encoded_data{0xc6, 0x62, 0xc8, 0x99, 0x00, 0x00, 0x39, 0x9b};
        parameters.parity_context(true);
        parameters.endianness(endianness::little);
        encoder.parameters(parameters);

        auto actual_encoded_data = encoder.encode(original_data);

        CHECK(actual_encoded_data == expected_encoded_data);
    }

    SECTION("no parity, little endian")
    {
        const auto original_data = make_bytevector("foo foo foo foo");
        const bytevector expected_encoded_data{0xda, 0x70, 0xc5, 0x99, 0x00, 0x80, 0x59, 0xe3};
        parameters.parity_context(false);
        parameters.endianness(endianness::little);
        encoder.parameters(parameters);

        auto actual_encoded_data = encoder.encode(original_data);

        CHECK(actual_encoded_data == expected_encoded_data);
    }

}

}
