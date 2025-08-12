// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// TODO: in the old implementation, we used the shrinkler license for this file since it's mostly shrinkler code
//       consider doing this again. Much less problematic that way.
// TODO: replace vector<unsigned char> by an alias

module;

#include <cstddef>
#include <iostream> // TODO: needed because of shrinkler code - I'd prefer not to have this here
#include <utility>
#include <vector>

// TODO: NUM_RELOC_CONTEXTS:
//       * Document where this comes from and why we duplicate it
//       * Not a macro!
#undef NDEBUG // TODO: yes, that's ugly. Explain why we need to do it?
#define NUM_RELOC_CONTEXTS 256
#include "Pack.h" // TODO: this causes shrinkler's assert macro to be defined. To we really want this?

module libshrinkler;

namespace libshrinkler
{

namespace
{

PackParams create_pack_params(const encoder_parameters& parameters)
{
    // TODO: review: are these all fields?
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
    auto non_const_data = data; // TODO: document why we're doing this?
    auto params = create_pack_params(parameters); // TODO: params => pack_params?
    vector<unsigned char> pack_buffer; // TODO: => compressed_data
    RangeCoder range_coder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);

    // TODO: note: apparently packData uses printf. Teach it not to do this?
    //             note: in the past we fixed this by reimplementing packData too
    // TODO: for starters, show_progress is hardcoded to be true. This needs to be an argument
    range_coder.reset();
    // TODO: no static_cast here. Have a conversion from size_t to to_int which throws (that is a specialized numeric_cast)
    packData(non_const_data.data(), static_cast<int>(non_const_data.size()), 0, &params, &range_coder, &edge_factory, true);
    range_coder.finish();

    return pack_buffer;

    // TODO: reference code below
    /*
    vector<unsigned char> compress(PackParams *params, RefEdgeFactory *edge_factory, bool show_progress) {

        // Crunch the data
        range_coder.reset();
        packData(&data[0], data.size(), 0, params, &range_coder, edge_factory, show_progress);
        range_coder.finish();
        printf("\n\n");
        fflush(stdout);

        return pack_buffer;
    }
     */
}

void verify()
{
    // TODO: verify
}

std::vector<unsigned char> crunch(const std::vector<unsigned char>& data, const encoder_parameters& parameters, RefEdgeFactory& edge_factory)
{
    // TODO: print message regarding safety margin? (Then again, should we print anything?)
    auto compressed_data = compress(data, parameters, edge_factory);
    verify();
    return compressed_data;
}

void make_little_endian(std::vector<unsigned char>& data)
{
    // TODO: that should go into an own function, no?
    while ((data.size() % 4) != 0)
    {
        data.push_back(0);
    }

    // TODO: should that also go into an own function?
    for (std::size_t i = 0; i < data.size(); i += 4)
    {
        std::swap(data[i + 0], data[i + 3]);
        std::swap(data[i + 1], data[i + 2]);
    }
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
    //       * swap endianness if requested (and pad if needed)

    // TODO: print crunching... message?
    RefEdgeFactory edge_factory(m_parameters.references());
    auto compressed_data = crunch(data, m_parameters, edge_factory); // TODO: consider making crunch a member, so less parameter passing?

    if (m_parameters.endianness() == endianness::little)
    {
        make_little_endian(compressed_data);
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
