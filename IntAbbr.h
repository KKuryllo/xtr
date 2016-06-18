#pragma once
#include <cstdint>
// Abbreviations
namespace int_abbr {
using i8t = std::int8_t;
using i16t = std::int16_t;
using i32t = std::int32_t;
using i64t = std::int64_t;

using i8f = std::int_fast8_t;
using i16f = std::int_fast16_t;
using i32f = std::int_fast32_t;
using i64f = std::int_fast64_t;

using i8l = std::int_least8_t;
using i16l = std::int_least16_t;
using i32l = std::int_least32_t;
using i64l = std::int_least64_t;

using imaxt = std::intmax_t;
using iptrt = std::intptr_t;

using ui8t = std::uint8_t;
using ui16t = std::uint16_t;
using ui32t = std::uint32_t;
using ui64t = std::uint64_t;

using ui8f = std::uint_fast8_t;
using ui16f = std::uint_fast16_t;
using ui32f = std::uint_fast32_t;
using ui64f = std::uint_fast64_t;

using ui8l = std::uint_least8_t;
using ui16l = std::uint_least16_t;
using ui32l = std::uint_least32_t;
using ui64l = std::uint_least64_t;

using uimax = std::uintmax_t;

using uiptr = std::uintptr_t;
}