// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <vector>

export module libshrinkler:endianness;

namespace libshrinkler
{

void swap_endianness(std::vector<unsigned char>& data);

}
