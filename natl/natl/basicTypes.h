#pragma once

//std
#include <cstdint>
#include <cstddef>

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

	using Size = ui64;
	using PtrDiff = std::ptrdiff_t;

	using Char = char;
	using AssciCode = char;
	using Utf32 = char32_t;

	enum class Byte : ui8 {};

	static_assert(sizeof(i8) == 1, "natl: i8 should be 1 bytes");
	static_assert(sizeof(i16) == 2, "natl: i16 should be 2 bytes");
	static_assert(sizeof(i32) == 4, "natl: i32 should be 4 bytes");
	static_assert(sizeof(i64) == 8, "natl: i64 should be 8 bytes");

	static_assert(sizeof(ui8) == 1, "natl: ui8 should be 1 bytes");
	static_assert(sizeof(ui16) == 2, "natl: ui16 should be 2 bytes");
	static_assert(sizeof(ui32) == 4, "natl: ui32 should be 4 bytes");
	static_assert(sizeof(ui64) == 8, "natl: ui64 should be 8 bytes");

	static_assert(sizeof(f32) == 4, "natl: f32 should be 4 bytes");
	static_assert(sizeof(f64) == 8, "natl: f64 should be 8 bytes");

	template<Size ByteSize> struct IntOfByteSize;
	template<> struct IntOfByteSize<sizeof(i8)> { using type = i8; };
	template<> struct IntOfByteSize<sizeof(i16)> { using type = i16; };
	template<> struct IntOfByteSize<sizeof(i32)> { using type = i32; };
	template<> struct IntOfByteSize<sizeof(i64)> { using type = i64; };

	template<Size ByteSize> struct UIntOfByteSize;
	template<> struct UIntOfByteSize<sizeof(ui8)> { using type = ui8; };
	template<> struct UIntOfByteSize<sizeof(ui16)> { using type = ui16; };
	template<> struct UIntOfByteSize<sizeof(ui32)> { using type = ui32; };
	template<> struct UIntOfByteSize<sizeof(ui64)> { using type = ui64; };

	template<Size ByteSize> using IntOfByteSize_t = typename IntOfByteSize<ByteSize>::type;
	template<Size ByteSize> using UIntOfByteSize_t = typename UIntOfByteSize<ByteSize>::type;
}