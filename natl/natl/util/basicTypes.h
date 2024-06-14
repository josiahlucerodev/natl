#pragma once

//std
#include <cstdint>
#include <cstddef>
#include <type_traits>

//interface 
namespace natl {
	using GenericInt = int;

	using Bool = bool;

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

	using UIntPtrSized = std::conditional_t<sizeof(void*) == 4, ui32, ui64>;

	using Size = std::size_t;
	using PtrDiff = std::ptrdiff_t;

	using SChar = signed char;
	using Char = char;
	using Ascii = char;
	using Utf8 = char8_t;
	using Utf16 = char16_t;
	using Utf32 = char32_t;

	using NullptrType = decltype(nullptr);

	template<typename DataType, typename TypeToAlignTo>
	class alignas(TypeToAlignTo) AlignToType {
	private:
		DataType internalValue;
	public:
		constexpr AlignToType() noexcept = default;
		constexpr AlignToType(const DataType& value) noexcept : internalValue(value) {}
		constexpr ~AlignToType() noexcept = default;
		constexpr operator DataType& () noexcept { return internalValue; }
		constexpr operator const DataType& () const noexcept { return internalValue; }
		constexpr DataType* operator->() noexcept { return &internalValue; }
		constexpr DataType& operator=(const DataType& value) noexcept { internalValue = value; return internalValue; }
		constexpr DataType& value() noexcept { return internalValue; }
		constexpr const DataType& value() const noexcept { return internalValue; }
	};

	template<typename DataType, Size Alignment>
	class alignas(Alignment) AlignedType {
	public:
		using value_type = DataType;
	private:
		DataType internalValue;
	public:
		constexpr AlignedType() noexcept = default;
		constexpr AlignedType(const DataType& value) noexcept : internalValue(value) {}
		constexpr ~AlignedType() noexcept = default;
		constexpr operator DataType& () noexcept { return internalValue; }
		constexpr operator const DataType& () const noexcept { return internalValue; }
		constexpr DataType* operator->() noexcept { return &internalValue; }
		constexpr DataType& operator=(const DataType& value) noexcept { internalValue = value; return internalValue; }
		constexpr DataType& value() noexcept { return internalValue; }
		constexpr const DataType& value() const noexcept { return internalValue; }
	};

	enum class Byte : ui8 {};

	template<typename DataType>
	using ByteAlignedToType = AlignToType<Byte, DataType>;

	template<Size Alignment>
	using AlignedByte = AlignedType<Byte, Alignment>;
	
	struct Dummy {};

	struct IgnoreType {
		template <typename AnyType>
		constexpr void operator=(AnyType&&) const noexcept {}
	};
	constexpr inline IgnoreType ignore;

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

	template<Size ByteSize> struct IntOfByteSizeT;
	template<> struct IntOfByteSizeT<sizeof(i8)> { using type = i8; };
	template<> struct IntOfByteSizeT<sizeof(i16)> { using type = i16; };
	template<> struct IntOfByteSizeT<sizeof(i32)> { using type = i32; };
	template<> struct IntOfByteSizeT<sizeof(i64)> { using type = i64; };

	template<Size ByteSize> struct UIntOfByteSizeT;
	template<> struct UIntOfByteSizeT<sizeof(ui8)> { using type = ui8; };
	template<> struct UIntOfByteSizeT<sizeof(ui16)> { using type = ui16; };
	template<> struct UIntOfByteSizeT<sizeof(ui32)> { using type = ui32; };
	template<> struct UIntOfByteSizeT<sizeof(ui64)> { using type = ui64; };

	template<Size ByteSize> using IntOfByteSize = typename IntOfByteSizeT<ByteSize>::type;
	template<Size ByteSize> using UIntOfByteSize = typename UIntOfByteSizeT<ByteSize>::type;

	template<Size ByteSize> struct FloatOfByteSizeT; 
	template<> struct FloatOfByteSizeT<sizeof(f32)> { using type = f32; };
	template<> struct FloatOfByteSizeT<sizeof(f64)> { using type = f64; };

	template<Size ByteSize> using FloatOfByteSize = typename FloatOfByteSizeT<ByteSize>::type;



	using SSize = IntOfByteSize<sizeof(Size)>;
}

namespace natl::literals {
	constexpr i8 operator""_i8(unsigned long long value) noexcept { return static_cast<i8>(value); }
	constexpr i16 operator""_i16(unsigned long long value) noexcept { return static_cast<i16>(value); }
	constexpr i32 operator""_i32(unsigned long long value) noexcept { return static_cast<i32>(value); }
	constexpr i64 operator""_i64(unsigned long long value) noexcept { return static_cast<i64>(value); }
	constexpr ui8 operator""_ui8(unsigned long long value) noexcept { return static_cast<ui8>(value); }
	constexpr ui16 operator""_ui16(unsigned long long value) noexcept { return static_cast<ui16>(value); }
	constexpr ui32 operator""_ui32(unsigned long long value) noexcept { return static_cast<ui32>(value); }
	constexpr ui64 operator""_ui64(unsigned long long value) noexcept { return static_cast<ui64>(value); }
	constexpr f32 operator ""_f32(long double value) noexcept { return static_cast<f32>(value); }
	constexpr f64 operator ""_f64(long double value) noexcept { return static_cast<f64>(value); }

	constexpr Size operator ""_size(unsigned long long value) noexcept { return static_cast<Size>(value); }
	constexpr SSize operator ""_ssize(unsigned long long value) noexcept { return static_cast<SSize>(value); }

	constexpr Char operator ""_char(char value) noexcept { return value; }
	constexpr Ascii operator ""_assci_char(char value) noexcept { return value; }
	constexpr Utf32 operator ""_utf32_char(char32_t value) noexcept { return value; }
}