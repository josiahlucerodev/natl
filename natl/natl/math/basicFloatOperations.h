#pragma once 

//@begin_non_modules
//own
#include "../util/compilerDependent.h"
//@end_non_modules

//own
#include "../util/basicTypes.h"
#include "../util/limits.h"
#include "../util/bits.h"
#include "../util/algorithm.h"
#include "constants.h"

//@export
namespace natl::math {
	//getExponentBits
	NATL_FORCE_INLINE constexpr ui32 getExponentBitsF32(const f32 value) noexcept {
		using namespace natl::literals;
		return (bitCast<ui32, f32>(value) >> 23_ui32) & 0xFF_ui32;
	}
	NATL_FORCE_INLINE constexpr ui64 getExponentBitsF64(const f64 value) noexcept {
		using namespace natl::literals;
		return (bitCast<ui64, f64>(value) >> 52_ui64) & 0x7FF_ui64;
	}

	//getExponent
	NATL_FORCE_INLINE constexpr i32 getExponentF32(const f32 value) noexcept {
		return static_cast<i32>(getExponentBitsF32(value) - static_cast<ui32>(Limits<f32>::exponentBias()));
	}
	NATL_FORCE_INLINE constexpr i64 getExponentF64(const f64 value) noexcept {
		return static_cast<i64>(getExponentBitsF64(value) - static_cast<ui64>(Limits<f64>::exponentBias()));
	}

	//getMantissaWithExponent
	NATL_FORCE_INLINE constexpr f32 getMantissaWithExponentF32(const f32 value, const i32 exponent) noexcept {
		return value - static_cast<f32>(exponent << Limits<f32>::exponentBitCount());
	}
	NATL_FORCE_INLINE constexpr f64 getMantissaWithExponentF64(const f64 value, const i64 exponent) noexcept {
		return value - static_cast<f64>(exponent << Limits<f64>::exponentBitCount());
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr UIntOfByteSize<sizeof(FloatType)> getExponentBits(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return getExponentBitsF32(value);
		} else {
			return getExponentBitsF64(value);
		}
	}

	//signbit 
	NATL_FORCE_INLINE constexpr Bool signbitF32(const f32 value) noexcept {
		using namespace natl::literals;
		return (bitCast<ui32, f32>(value) & 0x80000000_ui32) != 0;
	}
	NATL_FORCE_INLINE constexpr Bool signbitF64(const f64 value) noexcept {
		using namespace natl::literals;
		return (bitCast<ui64, f64>(value) & 0x8000000000000000_ui64) != 0;
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr Bool signbit(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return signbitF32(value);
		} else {
			return signbitF64(value);
		}
	}

	//copysign 
	NATL_FORCE_INLINE constexpr f32 copysignF32(const f32 mag, const f32 sgn) noexcept {
		using namespace natl::literals;
		constexpr ui32 signMask = 0x80000000_ui32;
		return bitCast<f32, ui32>((bitCast<ui32, f32>(mag) & ~signMask) | (bitCast<ui32, f32>(sgn) & signMask));
	}
	NATL_FORCE_INLINE constexpr f64 copysignF64(const f64 mag, const f64 sgn) noexcept {
		using namespace natl::literals;
		constexpr ui64 signMask = 0x8000000000000000_ui64;
		return bitCast<f64, ui64>((bitCast<ui64, f64>(mag) & ~signMask) | (bitCast<ui64, f64>(sgn) & signMask));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType copysign(const FloatType mag, const FloatType sgn) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return copysignF32(mag, sgn);
		} else {
			return copysignF64(mag, sgn);
		}
	}

	//isnan 
	NATL_FORCE_INLINE constexpr Bool isnanF32(const f32 value) noexcept {
		using namespace natl::literals;
		const ui32 bits = bitCast<ui32, f32>(value);
		const ui32 exponent = getExponentBitsF32(value);
		const ui32 fraction = bits & 0x7FFFFF_ui32;
		return exponent == 0xFF_ui32 && fraction != 0_ui32;
	}
	NATL_FORCE_INLINE constexpr Bool isnanF64(const f64 value) noexcept {
		using namespace natl::literals;
		const ui64 bits = bitCast<ui64, f64>(value);
		const ui64 exponent = getExponentBitsF64(value);
		const ui64 fraction = bits & 0xFFFFFFFFFFFFF_ui64;
		return exponent == 0x7FF_ui64 && fraction != 0_ui64;
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr Bool isnan(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return isnanF32(value);
		} else {
			return isnanF64(value);
		}
	}

	//is infinity
	NATL_FORCE_INLINE constexpr Bool isInfinityF32(const f32 value) noexcept {
		using namespace natl::literals;
		const ui32 bits = bitCast<ui32, f32>(value);
		const ui32 exponent = getExponentBitsF32(value);
		return exponent == 0xFF_ui64 && ((bits & 0x7FFFFF_ui64) == 0);
	}
	NATL_FORCE_INLINE constexpr Bool isInfinityF64(const f64 value) noexcept {
		using namespace natl::literals;
		const ui64 bits = bitCast<ui64, f64>(value);
		const ui64 exponent = getExponentBitsF64(value);
		return exponent == 0x7FF_ui64 && ((bits & 0xFFFFFFFFFFFF_ui64) == 0);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr Bool isInfinity(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return isInfinityF32(value);
		} else {
			return isInfinityF64(value);
		}
	}

	//fabs 
	NATL_FORCE_INLINE constexpr f32 fabsF32(const f32 value) noexcept {
		using namespace natl::literals;
		return bitCast<f32, ui32>(bitCast<ui32, f32>(value) & 0x7FFFFFFF_ui32);
	}
	NATL_FORCE_INLINE constexpr f64 fabsF64(const f64 value) noexcept {
		using namespace natl::literals;
		return bitCast<f64, ui64>(bitCast<ui64, f64>(value) & 0x7FFFFFFFFFFFFFFF_ui64);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType fabs(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return fabsF32(value);
		} else {
			return fabsF64(value);
		}
	}

	//is roughtly equal
	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<FloatType>)
	NATL_FORCE_INLINE constexpr Bool isRelativelyClose(const FloatType lhs, FloatType rhs, const FloatType relativeTolorance) noexcept {
		const FloatType larger = max<FloatType>(fabs<FloatType>(lhs), fabs<FloatType>(rhs));
		return (fabs<FloatType>(lhs - rhs) / larger) <= relativeTolorance;
	}

	//is relatively close
	NATL_FORCE_INLINE constexpr Bool isRelativelyCloseF32(const f32 lhs, const f32& rhs, const f32 relativeTolorance) noexcept {
		return isRelativelyClose<f32>(lhs, rhs, relativeTolorance);
	}
	NATL_FORCE_INLINE constexpr Bool isRelativelyCloseF64(const f64 lhs, const f64& rhs, const f64 relativeTolorance) noexcept {
		return isRelativelyClose<f64>(lhs, rhs, relativeTolorance);
	}

	//is roughtly equal
	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<FloatType>)
	NATL_FORCE_INLINE constexpr Bool isRoughtlyEqual(const FloatType lhs, const FloatType rhs, const FloatType tolerance) noexcept {
		return fabs<FloatType>(lhs - rhs) <= tolerance || lhs == rhs;
	}
	NATL_FORCE_INLINE constexpr Bool isRoughtlyEqualF32(const f32 lhs, const f32 rhs, const f32 tolorance) noexcept {
		return isRoughtlyEqual<f32>(lhs, rhs, tolorance);
	}
	NATL_FORCE_INLINE constexpr Bool isRoughtlyEqualF64(const f64 lhs, const f64 rhs, const f64 tolorance) noexcept {
		return isRoughtlyEqual<f64>(lhs, rhs, tolorance);
	}

	//basic ceil 
	NATL_FORCE_INLINE constexpr f32 basicCeilF32(const f32 value) noexcept {
		const i64 integerPart = static_cast<i64>(value);
		return (value > 0 && value != static_cast<f32>(integerPart))
			? static_cast<f32>(integerPart + 1)
			: static_cast<f32>(integerPart);
	}
	NATL_FORCE_INLINE constexpr f64 basicCeilF64(const f64 value) noexcept {
		const i64 integerPart = static_cast<i64>(value);
		return (value > 0 && value != static_cast<f64>(integerPart))
			? static_cast<f64>(integerPart + 1)
			: static_cast<f64>(integerPart);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicCeil(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicCeilF32(value);
		} else {
			return basicCeilF64(value);
		}
	}

	//ceil 
	NATL_FORCE_INLINE constexpr f32 ceilF32(const f32 value) noexcept {
		const ui32 exponent = getExponentBitsF32(value);
		if (exponent >= 0x3ff + 23 || value == 0)
			return value;
		return basicCeilF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 ceilF64(const f64 value) noexcept {
		const ui64 exponent = getExponentBitsF64(value);
		if (exponent >= 0x3ff + 52 || value == 0)
			return value;
		return basicCeilF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType ceil(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return ceilF32(value);
		} else {
			return ceilF64(value);
		}
	}

	//basic floor 
	NATL_FORCE_INLINE constexpr f32 basicFloorF32(const f32 value) noexcept {
		const i64 integerPart = static_cast<i64>(value);
		return (value < 0 && value != static_cast<f32>(integerPart))
			? static_cast<f32>(integerPart - 1)
			: static_cast<f32>(integerPart);
	}
	NATL_FORCE_INLINE constexpr f64 basicFloorF64(const f64 value) noexcept {
		const i64 integerPart = static_cast<i64>(value);
		return (value < 0 && value != static_cast<f64>(integerPart))
			? static_cast<f64>(integerPart - 1)
			: static_cast<f64>(integerPart);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicFloor(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicFloorF32(value);
		} else {
			return basicFloorF64(value);
		}
	}

	//floor 
	NATL_FORCE_INLINE constexpr f32 floorF32(const f32 value) noexcept {
		const ui32 exponent = getExponentBitsF32(value);
		if (exponent >= 0x3ff + 23 || value == 0)
			return value;
		return basicFloorF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 floorF64(const f64 value) noexcept {
		const ui64 exponent = getExponentBitsF64(value);
		if (exponent >= 0x3ff + 53 || value == 0)
			return value;
		return basicFloorF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType floor(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return floorF32(value);
		} else {
			return floorF64(value);
		}
	}

	//basic trunc 
	NATL_FORCE_INLINE constexpr f32 basicTruncF32(const f32 value) noexcept {
		return (value >= 0)
			? static_cast<f32>(static_cast<i64>(value))
			: -static_cast<f32>(static_cast<i64>(-value));
	}
	NATL_FORCE_INLINE constexpr f64 basicTruncF64(const f64 value) noexcept {
		return (value >= 0)
			? static_cast<f64>(static_cast<i64>(value))
			: -static_cast<f64>(static_cast<i64>(-value));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicTrunc(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicTruncF32(value);
		} else {
			return basicTruncF64(value);
		}
	}

	//trunc 
	NATL_FORCE_INLINE constexpr f32 truncF32(const f32 value) noexcept {
		const ui32 exponent = getExponentBitsF32(value);
		if (exponent >= 0x3ff + 23 || value == 0)
			return value;
		return basicTruncF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 truncF64(const f64 value) noexcept {
		const ui64 exponent = getExponentBitsF64(value);
		if (exponent >= 0x3ff + 53 || value == 0)
			return value;
		return basicTruncF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType trunc(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return truncF32(value);
		} else {
			return truncF64(value);
		}
	}

	//round 
	NATL_FORCE_INLINE constexpr f32 basicRoundF32(const f32 value) noexcept {
		using namespace natl::literals;
		return basicFloorF32(value + 0.5_f32);
	}
	NATL_FORCE_INLINE constexpr f64 basicRoundF64(const f64 value) noexcept {
		using namespace natl::literals;
		return basicFloorF64(value + 0.5_f64);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicRound(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicRoundF32(value);
		} else {
			return basicRoundF32(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 roundF32(const f32 value) noexcept {
		using namespace natl::literals;
		return floorF32(value + 0.5_f32);
	}
	NATL_FORCE_INLINE constexpr f64 roundF64(const f64 value) noexcept {
		using namespace natl::literals;
		return floorF64(value + 0.5_f64);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType round(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return roundF32(value);
		} else {
			return roundF64(value);
		}
	}

	//reciprocal
	NATL_FORCE_INLINE constexpr f32 reciprocalF32(const f32 value) noexcept {
		return f32(1.0) / value;
	}
	NATL_FORCE_INLINE constexpr f64 reciprocalF64(const f64 value) noexcept {
		return f64(1.0) / value;
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType reciprocal(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return reciprocalF32(value);
		} else {
			return reciprocalF64(value);
		}
	}

	//basicFmod 
	NATL_FORCE_INLINE constexpr f32 basicFmodF32(const f32 dividend, const f32 divisor) noexcept {
		const f32 quotient = static_cast<f32>(static_cast<i64>(dividend / divisor));
		const f32 remainder = dividend - (quotient * divisor);
		return remainder;
	}
	NATL_FORCE_INLINE constexpr f64 basicFmodF64(const f64 dividend, const f64 divisor) noexcept {
		const f64 quotient = static_cast<f64>(static_cast<i64>(dividend / divisor));
		const f64 remainder = dividend - (quotient * divisor);
		return remainder;
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicFmod(const FloatType dividend, const FloatType divisor) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicFmodF32(dividend, divisor);
		} else {
			return basicFmodF64(dividend, divisor);
		}
	}

	//fmod 
	NATL_FORCE_INLINE constexpr f32 fmodF32(const f32 dividend, const f32 divisor) noexcept {
		using namespace natl::literals;
		if (divisor == 0.0_f32) {
			return Limits<f32>::quietNaN();
		}
		if (isInfinityF32(dividend) || isInfinityF32(divisor)) {
			return Limits<f32>::quietNaN();
		}
		if (dividend == 0.0_f32) {
			return dividend;
		}
		const f32 absDividend = fabsF32(dividend);
		const f32 absDivisor = fabsF32(divisor);
		const Bool sign = signbitF32(dividend) != signbitF32(divisor);
		const f32 quotient = floorF32(absDividend / absDivisor);
		const f32 remainder = absDividend - quotient * absDivisor;

		if (sign) {
			return -remainder;
		} else {
			return remainder;
		}
	}
	NATL_FORCE_INLINE constexpr f64 fmodF64(const f64 dividend, const f64 divisor) noexcept {
		if (divisor == 0.0) {
			return Limits<f64>::quietNaN();
		}
		if (isInfinityF64(dividend) || isInfinityF64(divisor)) {
			return Limits<f64>::quietNaN();
		}
		if (dividend == 0.0) {
			return dividend;
		}
		const f64 absDividend = fabsF64(dividend);
		const f64 absDivisor = fabsF64(divisor);
		const Bool sign = signbitF64(dividend) != signbitF64(divisor);
		const f64 quotient = floorF64(absDividend / absDivisor);
		const f64 remainder = absDividend - quotient * absDivisor;

		if (sign) {
			return -remainder;
		} else {
			return remainder;
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType fmod(const FloatType dividend, const FloatType divisor) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return fmodF32(dividend, divisor);
		} else {
			return fmodF64(dividend, divisor);
		}
	}

	//mulFmod 
	NATL_FORCE_INLINE constexpr f32 mulFmodF32(const f32 dividend, const f32 divisor) noexcept {
		const f32 mulDivisorConstant = f32(1.0) / divisor;
		f32 truncated = dividend * mulDivisorConstant;
		truncated = truncated - static_cast<f32>(static_cast<i64>(truncated));
		return truncated * divisor;
	}
	NATL_FORCE_INLINE constexpr f64 mulFmodF64(const f64 dividend, const f64 divisor) noexcept {
		const f64 mulDivisorConstant = 1.0 / divisor;
		f64 truncated = dividend * mulDivisorConstant;
		truncated = truncated - static_cast<f64>(static_cast<i64>(truncated));
		return truncated * divisor;
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<FloatType>)
	NATL_FORCE_INLINE constexpr FloatType mulFmod(const FloatType dividend, const FloatType divisor) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return mulFmodF32(dividend, divisor);
		} else {
			return mulFmodF64(dividend, divisor);
		}
	}

	//mulFmodNoscale

	//return value is mod between  0.0 and 1.0
	NATL_FORCE_INLINE constexpr f32 mulFmodNoscaleF32(const f32 dividend, const f32 divisor) noexcept {
		const f32 mulDivisorConstant = f32(1.0) / divisor;
		f32 truncated = dividend * mulDivisorConstant;
		truncated = truncated - static_cast<f32>(static_cast<i64>(truncated));
		return truncated;
	}
	//return value is mod between  0.0 and 1.0
	NATL_FORCE_INLINE constexpr f64 mulFmodNoscaleF64(const f64 dividend, const f64 divisor) noexcept {
		const f64 mulDivisorConstant = 1.0 / divisor;
		f64 truncated = dividend * mulDivisorConstant;
		truncated = truncated - static_cast<f64>(static_cast<i64>(truncated));
		return truncated;
	}

	//return value is mod between  0.0 and 1.0
	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<FloatType>)
	NATL_FORCE_INLINE constexpr FloatType mulFmodNoscale(const FloatType dividend, const FloatType divisor) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return mulFmodNoscaleF32(dividend, divisor);
		} else {
			return mulFmodNoscaleF64(dividend, divisor);
		}
	}
}