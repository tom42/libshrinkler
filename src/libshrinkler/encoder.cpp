// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// TODO: in the old implementation, we used the shrinkler license for this file since it's mostly shrinkler code
//       consider doing this again. Much less problematic that way.
// TODO: replace vector<unsigned char> by an alias

module;

#include <cstddef>
#include <vector>
#include "shrinkler_headers.hpp"

module libshrinkler;

namespace libshrinkler
{

namespace
{

// TODO: encoder_parameters: rename effort to match_patience?
// TODO: encoder_parameters: rename same_length to max_same_length?

class no_progress final : public LZProgress
{
public:
    virtual void begin(int) override {}

    virtual void update(int) override {}

    virtual void end() override {}
};

// TODO: document what this is
// TODO: fix all warnings
// TODO: remove heap allocations where appropriate
// TODO: make instance member to reduce amount of parameter passing?
void pack_data(unsigned char *data, int data_length, int zero_padding, const encoder_parameters& parameters, Coder& result_coder, RefEdgeFactory& edge_factory)
{
    MatchFinder finder(data, data_length, 2, parameters.effort(), parameters.same_length());
    LZParser parser(data, data_length, zero_padding, finder, parameters.length_margin(), parameters.skip_length(), &edge_factory);
    result_size_t real_size = 0;
    result_size_t best_size = result_size_t(1) << (32 + 3 + Coder::BIT_PRECISION);
    std::size_t best_result = 0;
    std::vector<LZParseResult> results(2);
    CountingCoder *counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
    no_progress progress;

    for (int i = 0 ; i < parameters.iterations() ; i++)
    {
        // Parse data into LZ symbols
        LZParseResult& result = results[1 - best_result];
        Coder *measurer = new SizeMeasuringCoder(counting_coder);
        measurer->setNumberContexts(LZEncoder::NUMBER_CONTEXT_OFFSET, LZEncoder::NUM_NUMBER_CONTEXTS, data_length);
        finder.reset();
        result = parser.parse(LZEncoder(measurer, parameters.parity_context()), &progress);
        delete measurer;

        // Encode result using adaptive range coding
        vector<unsigned char> dummy_result;
        RangeCoder *range_coder = new RangeCoder(LZEncoder::NUM_CONTEXTS, dummy_result);
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
        CountingCoder *new_counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
        result.encode(LZEncoder(counting_coder, parameters.parity_context()));

        // New size measurer based on frequencies
        CountingCoder *old_counting_coder = counting_coder;
        counting_coder = new CountingCoder(old_counting_coder, new_counting_coder);
        delete old_counting_coder;
        delete new_counting_coder;
    }

    delete counting_coder;

    results[best_result].encode(LZEncoder(&result_coder, parameters.parity_context()));
}

std::vector<unsigned char> compress(std::vector<unsigned char>& non_const_uncompressed_data, const encoder_parameters& parameters, RefEdgeFactory& edge_factory)
{
    vector<unsigned char> compressed_data;
    RangeCoder range_coder(LZEncoder::NUM_CONTEXTS + num_reloc_contexts, compressed_data);

    // TODO: note: apparently packData uses printf. Teach it not to do this?
    //             note: in the past we fixed this by reimplementing packData too
    // TODO: for starters, show_progress is hardcoded to be true. This needs to be an argument (or turned off)

    // Crunch the data
    range_coder.reset();
    pack_data(non_const_uncompressed_data.data(), int_cast(non_const_uncompressed_data.size()), 0, parameters, range_coder, edge_factory);
    range_coder.finish();

    return compressed_data;
}

// TODO: pack_buffer => compressed_data? (everywhere)
// TODO: data => uncompressed_data?
void verify(std::vector<unsigned char>& pack_buffer, std::vector<unsigned char>& data, const encoder_parameters& parameters)
{
    RangeDecoder decoder(LZEncoder::NUM_CONTEXTS + num_reloc_contexts, pack_buffer);
    LZDecoder lzd(&decoder, parameters.parity_context());

    // Verify data
    // TODO: well yes, do so. Base it on shrinkler code, but incorporate our own improvements/modifications
    LZVerifier verifier(0, data.data(), int_cast(data.size()), int_cast(data.size()), 1);
    decoder.reset();
    decoder.setListener(&verifier);
    if (!lzd.decode(verifier))
    {
        // TODO: verification failed => Throw an exception, or call internal_error => Well throw, I guess?
    }

    // TODO: verify (reference code from shrinkler below)
    //       * (what about the return value?)
    //       * And what about comparison with original data? Or is this done by the verifier?
    /*
    int verify(PackParams *params, vector<unsigned char>& pack_buffer) {

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

// TODO: "data" => uncompressed_data (everyhwere in this file)
std::vector<unsigned char> encoder::encode(const std::vector<unsigned char>& uncompressed_data) const
{
    // Shrinkler code uses non-const buffers all over the place, but does not modify them.
    // Still we specify 'const' to callers, so we create a non-const copy of the original data.
    auto non_const_uncompressed_data = uncompressed_data;

    RefEdgeFactory edge_factory(m_parameters.references());
    auto compressed_data = compress(non_const_uncompressed_data, m_parameters, edge_factory); // TODO: consider making crunch a member, so less parameter passing?

    verify(compressed_data, non_const_uncompressed_data, m_parameters); // TODO: here too: could be a member, for less parameter passing, no?

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
