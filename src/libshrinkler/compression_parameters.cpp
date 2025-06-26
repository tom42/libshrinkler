// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module libshrinkler;

namespace libshrinkler
{

compression_parameters::compression_parameters(int preset)
{
    this->preset(preset);
}

void compression_parameters::preset(int preset)
{
    // TODO: range check preset? => yes, probably?
    iterations(1 * preset);
    length_margin(1 * preset);
    same_length(10 * preset);
    effort(100 * preset);
    skip_length(1000 * preset);
}

void compression_parameters::verbose(bool verbose)
{
    m_verbose = verbose;
}

void compression_parameters::parity_context(bool parity_context)
{
    m_parity_context = parity_context;
}

void compression_parameters::references(int references)
{
    // TODO: range check
    m_references = references;
}

void compression_parameters::iterations(int iterations)
{
    // TODO: range check
    m_iterations = iterations;
}

void compression_parameters::length_margin(int length_margin)
{
    // TODO: range check
    m_length_margin = length_margin;
}

void compression_parameters::same_length(int same_length)
{
    // TODO: range check
    m_same_length = same_length;
}

void compression_parameters::effort(int effort)
{
    // TODO: range check
    m_effort = effort;
}

void compression_parameters::skip_length(int skip_length)
{
    // TODO: range check
    m_skip_length = skip_length;
}

}
