// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// TODO: in the old implementation, we used the shrinkler license for this file since it's mostly shrinkler code
//       consider doing this again. Much less problematic that way.
// TODO: replace vector<unsigned char> by an alias

module;

#include <cstddef>
#include <iostream> // TODO: needed because of shrinkler code - I'd prefer not to have this here
#include <vector>

// TODO: NUM_RELOC_CONTEXTS:
//       * Document where this comes from and why we duplicate it
//       * Not a macro!
#undef NDEBUG // TODO: yes, that's ugly. Explain why we need to do it?
#define NUM_RELOC_CONTEXTS 256
#include "RangeDecoder.h"
#include "LZDecoder.h"
#include "Verifier.h"
#include "Pack.h" // TODO: this causes shrinkler's assert macro to be defined. To we really want this?

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

// TODO: data => uncompressed_data?
std::vector<unsigned char> compress(const std::vector<unsigned char>& data, const encoder_parameters& parameters, RefEdgeFactory& edge_factory)
{
    // TODO: compress
    auto non_const_data = data; // TODO: document why we're doing this? (respectively do it only once)
    auto params = create_pack_params(parameters); // TODO: params => pack_params?
    vector<unsigned char> pack_buffer; // TODO: => compressed_data
    RangeCoder range_coder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);

    // TODO: note: apparently packData uses printf. Teach it not to do this?
    //             note: in the past we fixed this by reimplementing packData too
    // TODO: for starters, show_progress is hardcoded to be true. This needs to be an argument (or turned off)

    // Crunch the data
    range_coder.reset();
    packData(non_const_data.data(), int_cast(non_const_data.size()), 0, &params, &range_coder, &edge_factory, true);
    range_coder.finish();

    return pack_buffer;
}

// TODO: pack_buffer => compressed_data?
// TODO: data => uncompressed_data?
void verify(std::vector<unsigned char>& pack_buffer, std::vector<unsigned char>& data, const encoder_parameters& parameters)
{
    // TODO: do we print a verbose message here or somesuch?

    RangeDecoder decoder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);
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

// TODO: "data" => uncompressed_data (everyhwere in this file)
std::vector<unsigned char> crunch(const std::vector<unsigned char>& data, const encoder_parameters& parameters, RefEdgeFactory& edge_factory)
{
    // TODO: print message regarding safety margin? (Then again, should we print anything?)
    auto compressed_data = compress(data, parameters, edge_factory);
    auto non_const_data = data; // TODO: document why we're doing this? (respectively do it only once)
    verify(compressed_data, non_const_data, parameters);
    return compressed_data;
}

}

void encoder::parameters(const encoder_parameters& parameters)
{
    m_parameters = parameters;
}

std::vector<unsigned char> encoder::encode(const std::vector<unsigned char>& data)
{
    // TODO: implement (start at shrinkler's main I guess)
    //       * set up parameters
    //       * compress
    //       * verify
    RefEdgeFactory edge_factory(m_parameters.references());
    auto compressed_data = crunch(data, m_parameters, edge_factory); // TODO: consider making crunch a member, so less parameter passing?

    if (m_parameters.endianness() == endianness::little)
    {
        swap_endianness(compressed_data);
    }

    return compressed_data;

    // TODO: for reference, here is the code to port
    // TODO: need to patch DataFile, such that it does not
    //       * Call exit
    //       * Do console I/O
    /*
    if (data.seen) {
        DataFile *crunched = orig->crunch(&params, &edge_factory, !no_progress.seen);
        delete orig;
        printf("References considered:%8d\n",  edge_factory.max_edge_count);
        printf("References discarded:%9d\n\n", edge_factory.max_cleaned_edges);

        printf("Saving file %s...\n\n", outfile);
        crunched->save(outfile, header.seen);

        printf("Final file size: %d\n\n", crunched->size(header.seen));
        delete crunched;

        if (edge_factory.max_edge_count > references.value) {
            printf("Note: compression may benefit from a larger reference buffer (-r option).\n\n");
        }

        return 0;
    }
     */
}

}
