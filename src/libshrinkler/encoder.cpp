// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <vector>

module libshrinkler;

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
    return {};

    // TODO: for reference, here is the code to port
    // TODO: need to patch DataFile, such that it does not
    //       * Call exit
    //       * Do console I/O
    /*
    if (data.seen) {
        // Data file compression
        printf("Loading file %s...\n\n", infile);
        DataFile *orig = new DataFile;
        orig->load(infile);

        printf("Crunching...\n\n");
        RefEdgeFactory edge_factory(references.value);
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
