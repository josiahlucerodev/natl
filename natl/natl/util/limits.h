#pragma once 

//own
#include "basicTypes.h"
#include "compilerDependent.h"

//interface
namespace natl {
	template<typename Type>
	struct Limits;

	template<>
	struct Limits<i8> {
		using value_type = i8;
		constexpr static i8 min() noexcept { return -128; }
		constexpr static i8 max() noexcept { return 127; }
	};
	template<>
	struct Limits<i16> {
		using value_type = i16;
		constexpr static i16 min() noexcept { return -32768; }
		constexpr static i16 max() noexcept { return 32767; }
	};
	template<>
	struct Limits<i32> {
		using value_type = i32;
		constexpr static i32 min() noexcept { return -2147483648; }
		constexpr static i32 max() noexcept { return 2147483647; }
	};
	template<>
	struct Limits<i64> {
		using value_type = i64;
		constexpr static i64 min() noexcept { return (-9223372036854775807L) - 1; }
		constexpr static i64 max() noexcept { return 9223372036854775807; }
	};

	template<>
	struct Limits<ui8> {
		using value_type = ui8;
		constexpr static ui8 min() noexcept { return 0; }
		constexpr static ui8 max() noexcept { return 255; }
	};
	template<>
	struct Limits<ui16> {
		using value_type = ui16;
		constexpr static ui16 min() noexcept { return 0; }
		constexpr static ui16 max() noexcept { return 65535; }
	};
	template<>
	struct Limits<ui32> {
		using value_type = ui32;
		constexpr static ui32 min() noexcept { return 0; }
		constexpr static ui32 max() noexcept { return 4294967295; }
	};
	template<>
	struct Limits<ui64> {
		using value_type = ui64;
		constexpr static ui64 min() noexcept { return 0; }
		constexpr static ui64 max() noexcept { return 18446744073709551614ULL + 1; }
	};

#if defined(NATL_COMPILER_EMSCRIPTEN)
	template<>
	struct Limits<Size> {
		using value_type = Size;
		constexpr static Size min() noexcept { return 0; }
		constexpr static Size max() noexcept { return ~Size(0); }
	};
#endif // NATL_COMPILER_EMSCRIPTEN

	template<>
	struct Limits<f32> {
		using value_type = f32;
		constexpr static f32 min() noexcept { return -max(); }
		constexpr static f32 max() noexcept { return 3.40282347e+38f; }
		constexpr static f32 smallest() noexcept { return 1.17549435e-38f; }
		constexpr static f32 epsilon() noexcept { return 1.19209290e-07f; }

#if defined(NATL_COMPILER_EMSCRIPTEN)
		constexpr static f32 infinity() noexcept { return __builtin_huge_valf(); };
		constexpr static f32 quietNaN() noexcept { return __builtin_nanf("0"); };
		constexpr static f32 signalingNaN() noexcept { return __builtin_nansf("1"); };
#elif defined(NATL_COMPILER_GCC)
		constexpr static f32 infinity() noexcept { return __builtin_huge_valf(); };
		constexpr static f32 quietNaN() noexcept { return __builtin_nanf(""); };
		constexpr static f32 signalingNaN() noexcept { return __builtin_nansf(""); };
#elif defined(NATL_COMPILER_MSVC)
		constexpr static f32 infinity() noexcept { return __builtin_huge_valf(); };
		constexpr static f32 quietNaN() noexcept { return __builtin_nanf("0"); };
		constexpr static f32 signalingNaN() noexcept { return __builtin_nansf("1"); };
#else 
		static_assert(false, "natl: f32 limits not implemented");
#endif 

		constexpr static ui32 mantissaBitCount() noexcept { return 23; }
		constexpr static ui32 exponentBitCount() noexcept { return 8; }
		constexpr static i32 exponentBias() noexcept { return 127; };
		constexpr static i32 minExponent() noexcept { return -126; };
		constexpr static i32 maxExponent() noexcept { return 127; };
	};
	
	template<>
	struct Limits<f64> {
		using value_type = f64;
		constexpr static f64 min() noexcept { return -max(); }
		constexpr static f64 max() noexcept { return 1.7976931348623158e+308; }
		constexpr static f64 smallest() noexcept { return 2.2250738585072014e-308; }
		constexpr static f64 epsilon() noexcept { return 2.2204460492503131e-16; }
#if defined(NATL_COMPILER_EMSCRIPTEN)
		constexpr static f64 infinity() noexcept { return __builtin_huge_val(); };
		constexpr static f64 quietNaN() noexcept { return __builtin_nan(""); };
		constexpr static f64 signalingNaN() noexcept { return __builtin_nans(""); };
#elif defined(NATL_COMPILER_GCC)
		constexpr static f64 infinity() noexcept { return __builtin_huge_val(); };
		constexpr static f64 quietNaN() noexcept { return __builtin_nan(""); };
		constexpr static f64 signalingNaN() noexcept { return __builtin_nans(""); };
#elif defined(NATL_COMPILER_MSVC)
		constexpr static f64 infinity() noexcept { return __builtin_huge_val(); };
		constexpr static f64 quietNaN() noexcept { return __builtin_nan("0"); };
		constexpr static f64 signalingNaN() noexcept { return __builtin_nans("1"); };
#else 
		static_assert(false, "natl: f64 limits not implemented");
#endif 
		constexpr static ui64 mantissaBitCount() noexcept { return 52; }
		constexpr static ui64 exponentBitCount() noexcept { return 11; }
		constexpr static i64 exponentBias() noexcept { return 1023; };
		constexpr static i64 minExponent() noexcept { return -1022; };
		constexpr static i64 maxExponent() noexcept { return 1023; };
	};
}