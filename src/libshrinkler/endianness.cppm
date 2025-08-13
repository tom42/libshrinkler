// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <vector>

export module libshrinkler:endianness;

namespace libshrinkler
{

void make_little_endian(std::vector<unsigned char>& data);

}
