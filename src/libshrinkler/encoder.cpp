// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <iostream> // TODO: needed because of shrinkler code - I'd prefer not to have this here
#include <vector>

// TODO: NUM_RELOC_CONTEXTS:
//       * Document where this comes from and why we duplicate it
//       * Not a macro!
#define NUM_RELOC_CONTEXTS 256
#include "Pack.h" // TODO: this causes shrinkler's assert macro to be defined. To we really want this?

module libshrinkler;

namespace
{

void compress()
{
    // TODO: compress
    vector<unsigned char> pack_buffer; // TODO: => compressed_data
    RangeCoder range_coder(LZEncoder::NUM_CONTEXTS + NUM_RELOC_CONTEXTS, pack_buffer);

    // TODO: note: apparently packData uses printf. Teach it not to do this?
    //             note: in the past we fixed this by reimplementing packData too
    range_coder.reset();
    packData(&data[0], data.size(), 0, params, &range_coder, edge_factory, show_progress);
    range_coder.finish();

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

void crunch()
{
    // TODO: print message regarding safety margin? (Then again, should we print anything?)
    compress();
    verify();
}

}

namespace libshrinkler
{

void encoder::parameters(const encoder_parameters& parameters)
{
    m_parameters = parameters;
}

std::vector<unsigned char> encoder::encode(const std::vector<unsigned char>& /*data*/)
{
    // TODO: implement (start at shrinkler's main I guess)
    //       * set up parameters
    //       * compress
    //       * verify
    //       * swap endianness if requested (and pad if needed)

    // TODO: print crunching... message?
    RefEdgeFactory edge_factory(m_parameters.references());
    crunch(); // TODO: pass parameters

    return {};

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
