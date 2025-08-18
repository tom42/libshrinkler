// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#ifndef LIBSHRINKLER_SHRINKLER_HEADERS_HPP_20250816
#define LIBSHRINKLER_SHRINKLER_HEADERS_HPP_20250816

// Shrinkler redefines the assert macro, but in a way that code using
// assert does not build if NDEBUG is defined, so we undefine it.
#undef NDEBUG

#include <cstdio>
#include <cstdlib>
#include "assert.h"
#include "CountingCoder.h"
#include "LZDecoder.h"
#include "LZParser.h"
#include "RangeCoder.h"
#include "RangeDecoder.h"
#include "SizeMeasuringCoder.h"
#include "Verifier.h"

namespace libshrinkler
{

// This is defined in HunkFile.h, which we do not want to include because that
// would require us to build the 68k decrunchers, which are included by HunkFile.h.
constexpr auto num_reloc_contexts = 256;

}

#endif
