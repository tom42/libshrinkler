// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <stdexcept>

module libshrinkler;

namespace libshrinkler
{

namespace
{

bool is_in_range(int value, int min, int max)
{
    return (min <= value) && (value <= max);
}

void throw_if_out_of_range(int value, int min, int max, const char* what)
{
    if (!is_in_range(value, min, max))
    {
        throw std::out_of_range(what);
    }
}

bool is_valid(endianness e)
{
    switch (e)
    {
        case endianness::big:
        case endianness::little:
            return true;
        default:
            return false;
    }
}

}

encoder_parameters::encoder_parameters(int preset)
{
    this->preset(preset);
}

void encoder_parameters::preset(int preset)
{
    throw_if_out_of_range(preset, min_preset, max_preset, "preset");

    iterations(1 * preset);
    length_margin(1 * preset);
    same_length(10 * preset);
    effort(100 * preset);
    skip_length(1000 * preset);
}

void encoder_parameters::parity_context(bool parity_context)
{
    m_parity_context = parity_context;
}

void encoder_parameters::references(int references)
{
    throw_if_out_of_range(references, min_references, max_references, "references");
    m_references = references;
}

void encoder_parameters::iterations(int iterations)
{
    throw_if_out_of_range(iterations, min_iterations, max_iterations, "iterations");
    m_iterations = iterations;
}

void encoder_parameters::length_margin(int length_margin)
{
    throw_if_out_of_range(length_margin, min_length_margin, max_length_margin, "length_margin");
    m_length_margin = length_margin;
}

void encoder_parameters::same_length(int same_length)
{
    throw_if_out_of_range(same_length, min_same_length, max_same_length, "same_length");
    m_same_length = same_length;
}

void encoder_parameters::effort(int effort)
{
    throw_if_out_of_range(effort, min_effort, max_effort, "effort");
    m_effort = effort;
}

void encoder_parameters::skip_length(int skip_length)
{
    throw_if_out_of_range(skip_length, min_skip_length, max_skip_length, "skip_length");
    m_skip_length = skip_length;
}

void encoder_parameters::endianness(libshrinkler::endianness endianness)
{
    if (!is_valid(endianness))
    {
        throw std::invalid_argument("invalid endianness");
    }

    m_endianness = endianness;
}

}
