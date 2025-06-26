// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module libshrinkler;

namespace libshrinkler
{

compression_parameters::compression_parameters(int preset)
{
    this->preset(preset);
}

void compression_parameters::preset(int /*preset*/)
{

}

void compression_parameters::verbose(bool verbose)
{
    m_verbose = verbose;
}

void compression_parameters::parity_context(bool parity_context)
{
    m_parity_context = parity_context;
}

}
