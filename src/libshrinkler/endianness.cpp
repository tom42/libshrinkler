// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <utility>
#include <vector>

module libshrinkler;

namespace libshrinkler
{

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
