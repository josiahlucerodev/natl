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

	template<Size ByteSize> struct IntOfByteSizeType;
	template<> struct IntOfByteSizeType<sizeof(i8)> { using type = i8; };
	template<> struct IntOfByteSizeType<sizeof(i16)> { using type = i16; };
	template<> struct IntOfByteSizeType<sizeof(i32)> { using type = i32; };
	template<> struct IntOfByteSizeType<sizeof(i64)> { using type = i64; };

	template<Size ByteSize> struct UIntOfByteSizeType;
	template<> struct UIntOfByteSizeType<sizeof(ui8)> { using type = ui8; };
	template<> struct UIntOfByteSizeType<sizeof(ui16)> { using type = ui16; };
	template<> struct UIntOfByteSizeType<sizeof(ui32)> { using type = ui32; };
	template<> struct UIntOfByteSizeType<sizeof(ui64)> { using type = ui64; };

	template<Size ByteSize> using IntOfByteSize = typename IntOfByteSizeType<ByteSize>::type;
	template<Size ByteSize> using UIntOfByteSize = typename UIntOfByteSizeType<ByteSize>::type;

	using SSize = IntOfByteSize<sizeof(Size)>;
}

constexpr natl::i8 operator""_natl_i8(unsigned long long value) noexcept { return static_cast<natl::i8>(value); }
constexpr natl::i16 operator""_natl_i16(unsigned long long value) noexcept { return static_cast<natl::i16>(value); }
constexpr natl::i32 operator""_natl_i32(unsigned long long value) noexcept { return static_cast<natl::i32>(value); }
constexpr natl::i64 operator""_natl_i64(unsigned long long value) noexcept { return static_cast<natl::i64>(value); }

constexpr natl::ui8 operator""_natl_ui8(unsigned long long value) noexcept { return static_cast<natl::ui8>(value); }
constexpr natl::ui16 operator""_natl_ui16(unsigned long long value) noexcept { return static_cast<natl::ui16>(value); }
constexpr natl::ui32 operator""_natl_ui32(unsigned long long value) noexcept { return static_cast<natl::ui32>(value); }
constexpr natl::ui64 operator""_natl_ui64(unsigned long long value) noexcept { return static_cast<natl::ui64>(value); }

constexpr natl::f32 operator ""_natl_f32(long double value) noexcept { return static_cast<natl::f32>(value); }
constexpr natl::f64 operator ""_natl_f64(long double value) noexcept { return static_cast<natl::f64>(value); }

constexpr natl::Size operator ""_natl_size(unsigned long long value) noexcept { return static_cast<natl::Size>(value); }
constexpr natl::SSize operator ""_natl_ssize(unsigned long long value) noexcept { return static_cast<natl::SSize>(value); }

constexpr natl::Char operator ""_natl_char(char value) noexcept { return value; }
constexpr natl::Ascii operator ""_natl_assci_char(char value) noexcept { return value; }
constexpr natl::Utf32 operator ""_natl_utf32_char(char32_t value) noexcept { return value; }