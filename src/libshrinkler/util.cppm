// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <cstddef>
#include <format>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

export module libshrinkler:util;

namespace libshrinkler
{

// Deliberately not exported. Client code is not supposed to handle this exception,
// although it can of course handle base classes of internal_error.
class internal_error final : public std::logic_error
{
public:
    explicit internal_error(const char* message)
        : std::logic_error(std::string("libshrinkler: internal error: ") + message)
    {}
};

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
