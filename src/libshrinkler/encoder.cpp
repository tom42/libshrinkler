// SPDX-FileCopyrightText: 1999-present Aske Simon Christensen
// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: LicenseRef-Shrinkler
//
// This file contains mostly slightly adapted code from Shrinkler,
// so it's licensed under the Shrinkler license.

module;

#include <cstddef>
#include <format>
#include <stdexcept>
#include <vector>
#include "shrinkler_headers.hpp"

module libshrinkler;

namespace libshrinkler
{

namespace
{

using byte_vector = std::vector<unsigned char>;

class no_progress final : public LZProgress
{
public:
    virtual void begin(int) override {}

    virtual void update(int) override {}

    virtual void end() override {}
};

// packData() from Pack.h
void pack_data(unsigned char* uncompressed_data, int data_length, int zero_padding, const encoder_parameters& parameters, Coder& result_coder, RefEdgeFactory& edge_factory)
{
    MatchFinder finder(uncompressed_data, data_length, 2, parameters.effort(), parameters.same_length());
    LZParser parser(uncompressed_data, data_length, zero_padding, finder, parameters.length_margin(), parameters.skip_length(), &edge_factory);
    result_size_t real_size = 0;
    result_size_t best_size = result_size_t(1) << (32 + 3 + Coder::BIT_PRECISION);
    std::size_t best_result = 0;
    std::vector<LZParseResult> results(2);
    CountingCoder* counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
    no_progress progress;

    for (int i = 0 ; i < parameters.iterations() ; i++)
    {
        // Parse data into LZ symbols
        LZParseResult& result = results[1 - best_result];
#ifdef LIBSHRINKLER_COMPILER_GNU
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference" // Occurs with g++ and optimizations enabled
#endif
        Coder* measurer = new SizeMeasuringCoder(counting_coder);
#ifdef LIBSHRINKLER_COMPILER_GNU
#pragma GCC diagnostic pop
#endif
        measurer->setNumberContexts(LZEncoder::NUMBER_CONTEXT_OFFSET, LZEncoder::NUM_NUMBER_CONTEXTS, data_length);
        finder.reset();
        result = parser.parse(LZEncoder(measurer, parameters.parity_context()), &progress);
        delete measurer;

        // Encode result using adaptive range coding
        byte_vector dummy_result;
        RangeCoder* range_coder = new RangeCoder(LZEncoder::NUM_CONTEXTS, dummy_result);
        real_size = result.encode(LZEncoder(range_coder, parameters.parity_context()));
        range_coder->finish();
        delete range_coder;

        // Choose if best
        if (real_size < best_size)
        {
            best_result = 1 - best_result;
            best_size = real_size;
        }

        // Count symbol frequencies
        CountingCoder* new_counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
        result.encode(LZEncoder(counting_coder, parameters.parity_context()));

        // New size measurer based on frequencies
        CountingCoder* old_counting_coder = counting_coder;
        counting_coder = new CountingCoder(old_counting_coder, new_counting_coder);
        delete old_counting_coder;
        delete new_counting_coder;
    }

    delete counting_coder;

    results[best_result].encode(LZEncoder(&result_coder, parameters.parity_context()));
}

// DataFile::compress() from DataFile.h
byte_vector compress(byte_vector& non_const_uncompressed_data, const encoder_parameters& parameters, RefEdgeFactory& edge_factory)
{
    byte_vector compressed_data;
    RangeCoder range_coder(LZEncoder::NUM_CONTEXTS + num_reloc_contexts, compressed_data);

    // Crunch the data
    range_coder.reset();
    pack_data(non_const_uncompressed_data.data(), int_cast(non_const_uncompressed_data.size()), 0, parameters, range_coder, edge_factory);
    range_coder.finish();

    return compressed_data;
}

// DataFile::verify() from DataFile.h
// Safety margin calculation omitted, since we currently don't need that value.
void verify(byte_vector& compressed_data, byte_vector& uncompressed_data, const encoder_parameters& parameters)
{
    RangeDecoder decoder(LZEncoder::NUM_CONTEXTS + num_reloc_contexts, compressed_data);
    LZDecoder lzd(&decoder, parameters.parity_context());

    // Verify data
    LZVerifier verifier(0, uncompressed_data.data(), int_cast(uncompressed_data.size()), int_cast(uncompressed_data.size()), 1);
    decoder.reset();
    decoder.setListener(&verifier);
    if (!lzd.decode(verifier))
    {
        throw internal_error("verify error: verification of compressed data failed");
    }

    // Check length
    if (verifier.size() != int_cast(uncompressed_data.size()))
    {
        throw internal_error(std::format("verify error: decompressed data has incorrect length ({}, should have been {})", verifier.size(), uncompressed_data.size()));
    }
}

}

void encoder::parameters(const encoder_parameters& parameters)
{
    m_parameters = parameters;
}

byte_vector encoder::encode(const byte_vector& uncompressed_data) const
{
    compression_info unused;
    return encode(uncompressed_data, unused);
}

// Data file compression from main2 in Shrinkler.cpp
byte_vector encoder::encode(const byte_vector& uncompressed_data, compression_info& compression_info) const
{
    if (uncompressed_data.size() == 0)
    {
        throw std::invalid_argument("cannot compress input of size 0");
    }

    // Shrinkler code uses non-const buffers all over the place, but does not modify them.
    // Still we specify 'const' to callers, so we create a non-const copy of the original data.
    auto non_const_uncompressed_data = uncompressed_data;

    RefEdgeFactory edge_factory(m_parameters.references());
    auto compressed_data = compress(non_const_uncompressed_data, m_parameters, edge_factory);

    verify(compressed_data, non_const_uncompressed_data, m_parameters);

    if (m_parameters.endianness() == endianness::little)
    {
        swap_endianness(compressed_data);
    }

    compression_info.considered_references = edge_factory.max_edge_count;
    compression_info.discarded_references = edge_factory.max_cleaned_edges;
    compression_info.increase_reference_buffer_hint = edge_factory.max_edge_count > m_parameters.references();

    return compressed_data;
}

}
