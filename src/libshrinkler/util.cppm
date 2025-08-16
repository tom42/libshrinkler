// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <cstddef>
#include <format>
#include <limits>
#include <stdexcept>

export module libshrinkler:util;

namespace libshrinkler
{

LIBSHRINKLER_EXPORT_FOR_UNIT_TESTING
constexpr int int_cast(std::size_t size)
{
    if (size > std::numeric_limits<int>::max())
    {
        throw std::overflow_error(std::format("cannot convert {} to int", size));
    }

    return static_cast<int>(size);
}

}
