#pragma once

//std
#include <cstdint>

//interface 
namespace natl {
	using i8 = std::int8_t;
	using i16 = std::int16_t;
	using i32 = std::int32_t;
	using i64 = std::int64_t;

	using ui8 = std::uint8_t;
	using ui16 = std::uint16_t;
	using ui32 = std::uint32_t;
	using ui64 = std::uint64_t;

	using f32 = float;
	using f64 = double;

	using Size = std::uint64_t;
	using PtrDiff = std::ptrdiff_t;

	using Utf32 = char32_t;
	using AssciCode = char;

	enum class Byte : ui8 {};
}