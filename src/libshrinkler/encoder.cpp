// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <iostream> // TODO: needed because of shrinkler code - I'd prefer not to have this here
#include <vector>
#include "Pack.h" // TODO: this causes shrinkler's assert macro to be defined. To we really want this?

module libshrinkler;

namespace
{

void compress()
{
    // TODO: compress
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
