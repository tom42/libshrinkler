// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <cstddef>
#include <format>
#include <limits>
#include <stdexcept>
#include <type_traits>

export module libshrinkler:util;

namespace libshrinkler
{

template <std::signed_integral TSignedIntegral>
constexpr auto make_unsigned(TSignedIntegral x)
{
    return static_cast<std::make_unsigned_t<TSignedIntegral>>(x);
}

LIBSHRINKLER_EXPORT_FOR_UNIT_TESTING
constexpr int int_cast(std::size_t size)
{
    if (size > make_unsigned(std::numeric_limits<int>::max()))
    {
        throw std::overflow_error(std::format("cannot convert {} to int", size));
    }

    return static_cast<int>(size);
}

}
