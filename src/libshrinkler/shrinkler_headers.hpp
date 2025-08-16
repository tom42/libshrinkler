// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#ifndef LIBSHRINKLER_SHRINKLER_HEADERS_HPP_20250816
#define LIBSHRINKLER_SHRINKLER_HEADERS_HPP_20250816

// Shrinkler redefines the assert macro, but in a way that code using
// assert does not build if NDEBUG is defined, so we undefine it.
#undef NDEBUG

namespace libshrinkler
{

constexpr auto num_reloc_contexts = 256;

}

#endif
