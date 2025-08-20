// SPDX-FileCopyrightText: 1999-present Aske Simon Christensen
// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: LicenseRef-Shrinkler
//
// This file contains mostly slightly adapted code from Shrinkler,
// so it's licensed under the Shrinkler license.

module;

#include <cstddef>
#include <format>
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

    if (verifier.size() != uncompressed_data.size())
    {
        throw internal_error(std::format("verify error: decompressed data has incorrect length ({}, should have been {})", verifier.size(), uncompressed_data.size()));
    }

    // TODO: verify (reference code from shrinkler below)
    //       * (what about the return value?)
    //       * And what about comparison with original data? Or is this done by the verifier?
    /*
    int verify(PackParams *params, byte_vector& pack_buffer) {

        // Check length
        if (!error && verifier.size() != data.size()) {
            printf("Verify error: data has incorrect length (%d, should have been %d)!\n", verifier.size(), (int) data.size());
            error = true;
        }

        if (error) {
            internal_error();
        }

        printf("OK\n\n");

        return verifier.front_overlap_margin + pack_buffer.size() - data.size();
    }
     */
    // TODO: for reference below is also what we earlier did
    /*
    if (!lzd.decode(verifier))
    {
        throw runtime_error("INTERNAL ERROR: could not verify decompressed data");
    }

    // Check length
    if (numeric_cast<size_t>(verifier.size()) != data.size())
    {
        throw runtime_error(std::format("INTERNAL ERROR: decompressed data has incorrect length ({}, should have been {})", verifier.size(), data.size()));
    }

    return verifier.front_overlap_margin + pack_buffer.size() * 4 - data.size(); // TODO: note that the *4 is because in older version of shrinkler, pack_buffer contained 32 bit elements
     */
}

}

void encoder::parameters(const encoder_parameters& parameters)
{
    m_parameters = parameters;
}

// Data file compression from main2 in Shrinkler.cpp
byte_vector encoder::encode(const byte_vector& uncompressed_data) const
{
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

    // TODO: prepare some result structure which can be used to return all sorts of data
    //       * References considered
    //       * References discarded
    //       * That hint to use a larger reference buffer
    //       * Safety margin thing

    return compressed_data;

    // TODO: need to patch DataFile, such that it does not
    //       * Call exit
    //       * Do console I/O
    /*
    if (data.seen) {

        if (edge_factory.max_edge_count > references.value) {
            printf("Note: compression may benefit from a larger reference buffer (-r option).\n\n");
        }

        return 0;
    }
     */
}

}
