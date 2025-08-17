// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// TODO: in the old implementation, we used the shrinkler license for this file since it's mostly shrinkler code
//       consider doing this again. Much less problematic that way.
// TODO: replace vector<unsigned char> by an alias

module;

#include <cstddef>
#include <iostream> // TODO: needed because of shrinkler code - I'd prefer not to have this here => try later, possibly we can remove it if we do not do any output
#include <vector>
#include "shrinkler_headers.hpp"

module libshrinkler;

namespace libshrinkler
{

namespace
{

PackParams create_pack_params(const encoder_parameters& parameters)
{
    return
    {
        .parity_context = parameters.parity_context(),
        .iterations = parameters.iterations(),
        .length_margin = parameters.length_margin(),
        .skip_length = parameters.skip_length(),
        .match_patience = parameters.effort(),
        .max_same_length = parameters.same_length()
    };
}

std::vector<unsigned char> compress(std::vector<unsigned char>& non_const_uncompressed_data, const encoder_parameters& parameters, RefEdgeFactory& edge_factory)
{
    auto params = create_pack_params(parameters); // TODO: params => pack_params?
    vector<unsigned char> compressed_data;
    RangeCoder range_coder(LZEncoder::NUM_CONTEXTS + num_reloc_contexts, compressed_data);

    // TODO: note: apparently packData uses printf. Teach it not to do this?
    //             note: in the past we fixed this by reimplementing packData too
    // TODO: for starters, show_progress is hardcoded to be true. This needs to be an argument (or turned off)

    // Crunch the data
    range_coder.reset();
    packData(non_const_uncompressed_data.data(), int_cast(non_const_uncompressed_data.size()), 0, &params, &range_coder, &edge_factory, true);
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
    auto non_const_uncompressed_data = uncompressed_data; // TODO: document why we're doing this? (respectively do it only once)

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
