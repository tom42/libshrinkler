// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <cstddef>

export module libshrinkler:util;

namespace libshrinkler
{

// TODO: test (0 => good, max int => good, max int + 1 => bad)
// TODO: what exception do we throw?
inline int int_cast(std::size_t size)
{
    // TODO: do not cast here. Throw if size does not fit
    return static_cast<int>(size);
}

}
