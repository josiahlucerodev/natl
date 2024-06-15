#pragma once

//own 
#include "../util/bits.h"
#include "../util/compilerDependent.h"
#include "constants.h"
#include "basicFloatOperations.h"

//interface
namespace natl::math {
	//exp

	//very inaccarate 
	NATL_FORCE_INLINE constexpr f32 basicExpDivF32(const f32 value) noexcept {
		using namespace natl::literals;
		const f32 a = (1_i32 << 22_i32) / f32(ln2);
		const i32 b = 127_i32 * (1_i32 << 23_i32);
		const i32 r = static_cast<i32>(a * value);
		const i32 s = b + r;
		const i32 t = b - r;
		return bitCast<f32, i32>(s) / bitCast<f32, i32>(t);
	}

	//very inaccarate 
	NATL_FORCE_INLINE constexpr f64 basicExpDivF64(const f64 value) noexcept {
		using namespace natl::literals;
		const f64 a = (1_i64 << 51_i64) / f64(ln2);
		const i64 b = 1023_i64 * (1_i64 << 52_i64);
		const i64 r = static_cast<i64>(a * value);
		const i64 s = b + r;
		const i64 t = b - r;
		return bitCast<f64, i64>(s) / bitCast<f64, i64>(t);
	}

	//somewhat accarate 
	//value bounds (-87, 88)
	NATL_FORCE_INLINE constexpr f32 basicExpF32(const f32 value) noexcept {
		using namespace natl::literals;
		const f32 t = value * 1.442695041_f32;
		const f32 integerPart = floorF32(t);
		const f32 factionalPart = t - integerPart;
		const i32 integer = static_cast<i32>(integerPart);
		f32 result = (0.3371894346_f32 * factionalPart + 0.657636276_f32) * factionalPart + 1.00172476_f32;
		result = bitCast<f32, ui32>(ui32(integer << 23) + bitCast<ui32, f32>(result));
		return result;
	}

	//somewhat accarate 
	//value bounds (-87, 88)
	NATL_FORCE_INLINE constexpr f64 basicExpF64(const f64 value) noexcept {
		using namespace natl::literals;
		const f64 t = value * 1.442695041_f64;
		const f64 integerPart = floorF64(t);
		const f64 factionalPart = t - integerPart;
		const i64 integer = static_cast<i64>(integerPart);
		f64 result = (0.3371894346_f64 * factionalPart + 0.657636276_f64) * factionalPart + 1.00172476_f64;
		result = bitCast<f64, ui64>(ui64(integer << 52) + bitCast<ui64, f64>(result));
		return result;
	}

	//exp2 
	NATL_FORCE_INLINE constexpr f32 basicExp2F32(const f32 value) noexcept {
		using namespace natl::literals;
		const f32 clampedValue = (value < Limits<f32>::minExponent()) 
			? Limits<f32>::minExponent() 
			: value; 
		const f32 integralPart = floorF32(clampedValue);
		const f32 fractionalPart = clampedValue - integralPart;
		const f32 approx = -5.725926_f32 + 27.728367_f32 / (4.8425903_f32 - fractionalPart) - 0.49013242_f32 * fractionalPart;
		const i32 resultInteger = static_cast<i32>((1_ui32 << Limits<f32>::mantissaBitCount()) * (integralPart + Limits<f32>::exponentBias() + approx));
		return bitCast<f32, i32>(resultInteger);
	}

	NATL_FORCE_INLINE constexpr f64 basicExp2F64(const f64 value) noexcept {
		using namespace natl::literals;
		const f64 clampedValue = (value < Limits<f64>::minExponent())
			? Limits<f64>::minExponent()
			: value;
		const f64 integralPart = floorF64(clampedValue);
		const f64 fractionalPart = clampedValue - integralPart;
		const f64 approx = -5.725926_f64 + 27.728367_f64 / (4.8425903_f64 - fractionalPart) - 0.49016442_f64 * fractionalPart;
		const i64 resultInteger = static_cast<i64>((1_ui64 << Limits<f64>::mantissaBitCount()) * (integralPart + Limits<f64>::exponentBias() + approx));
		return bitCast<f64, i64>(resultInteger);
	}

	//log2 
	NATL_FORCE_INLINE constexpr f32 basicLog2F32(const f32 value) noexcept {
		using namespace natl::literals;
		const f32 x = static_cast<f32>(bitCast<ui32, f32>(value)) * 1.1920928955078e-7_f32;
		const f32 mx = bitCast<f32, ui32>((bitCast<ui32, f32>(value) & 0x007FFFFF) | 0x3F000000);
		return x - 124.2255149_f32 - 1.49803030_f32 * mx - 1.7258799_f32 / (0.352088706_f32 + mx);
	}
	NATL_FORCE_INLINE constexpr f64 basicLog2F64(const f64 value) noexcept {
		using namespace natl::literals;
		const f64 x = static_cast<f64>(bitCast<ui32, f32>(static_cast<f32>(value))) * 1.1920928955078e-7_f64;
		const f64 mx = bitCast<f64, ui64>((bitCast<ui64, f64>(value) & 0x000FFFFFFFFFFFFF) | 0x3FE0000000000000);
		return x - 124.2255149_f64 - 1.49803030_f64 * mx - 1.7258799_f64 / (0.352088706_f64 + mx);
	}

	NATL_FORCE_INLINE constexpr f32 log2F32(const f32 value) noexcept {
		using namespace natl::literals;
		if (value < 0.0_f32 || isnanF32(value)) {
			return Limits<f32>::quietNaN();
		}
		if (value == 0.0_f32) {
			return -Limits<f32>::infinity();
		}
		return basicLog2F32(value);
	}
	NATL_FORCE_INLINE constexpr f64 log2F64(const f64 value) noexcept {
		using namespace natl::literals;
		if (value < 0.0_f64 || isnanF64(value)) {
			return Limits<f64>::quietNaN();
		}
		if (value == 0.0_f64) {
			return -Limits<f64>::infinity();
		}
		return basicLog2F64(value);
	}

	//ln
	NATL_FORCE_INLINE constexpr f32 basicLnF32(const f32 value) noexcept {
		return f32(ln2) * basicLog2F32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicLnF64(const f64 value) noexcept {
		return ln2 * basicLog2F64(value);
	}

	//ln
	NATL_FORCE_INLINE constexpr f32 lnF32(const f32 value) noexcept {
		using namespace natl::literals;
		if (value < 0.0_f32 || isnanF32(value)) {
			return Limits<f32>::quietNaN();
		}
		if (value == 0.0_f32) {
			return -Limits<f32>::infinity();
		}
		return basicLnF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 lnF64(const f64 value) noexcept {
		using namespace natl::literals;
		if (value < 0.0_f64 || isnanF64(value)) {
			return Limits<f64>::quietNaN();
		}
		if (value == 0.0_f64) {
			return -Limits<f64>::infinity();
		}
		return basicLnF64(value);
	}

	//log
	NATL_FORCE_INLINE constexpr f32 basicLogF32(const f32 value) noexcept {
		return f32(logBase10_2) * basicLog2F32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicLogF64(const f64 value) noexcept {
		return logBase10_2 * basicLog2F64(value);
	}

	NATL_FORCE_INLINE constexpr f32 logF32(const f32 value) noexcept {
		using namespace natl::literals;
		if (value < 0.0_f32 || isnanF32(value)) {
			return Limits<f32>::quietNaN();
		}
		if (value == 0.0_f32) {
			return -Limits<f32>::infinity();
		}
		return basicLogF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 logF64(const f64 value) noexcept {
		using namespace natl::literals;
		if (value < 0.0_f64 || isnanF64(value)) {
			return Limits<f64>::quietNaN();
		}
		if (value == 0.0_f64) {
			return -Limits<f64>::infinity();
		}
		return basicLogF64(value);
	}
}