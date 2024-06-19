#pragma once 

//own
#include "../simdOf.h"
#include "../simdClass.h"

//interface 
namespace natl::math {
	//relatively close
	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType> && nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMask<FloatType, SimdArchType> isRelativelyClose(
		nasimd::SimdRegister<FloatType, SimdArchType> lhs,
		nasimd::SimdRegister<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {
		nasimd::SimdRegister<FloatType, SimdArchType> lhsAbs = nasimd::simd_abs<FloatType, SimdArchType>(lhs);
		nasimd::SimdRegister<FloatType, SimdArchType> rhsAbs = nasimd::simd_abs<FloatType, SimdArchType>(rhs);
		nasimd::SimdRegister<FloatType, SimdArchType> larger = nasimd::simd_max<FloatType, SimdArchType>(lhsAbs, rhsAbs);

		nasimd::SimdRegister<FloatType, SimdArchType> difference = nasimd::simd_sub<FloatType, SimdArchType>(lhs, rhs);
		nasimd::SimdRegister<FloatType, SimdArchType> absDifference = nasimd::simd_abs<FloatType, SimdArchType>(difference);

		nasimd::SimdRegister<FloatType, SimdArchType> relativeDifference = nasimd::simd_div<FloatType, SimdArchType>(absDifference, larger);

		nasimd::SimdRegister<FloatType, SimdArchType> toleranceR = nasimd::simd_set<FloatType, SimdArchType>(tolerance);
		nasimd::SimdMask<FloatType, SimdArchType> toleranceMask = nasimd::simd_compare_less_than_or_equal<FloatType, SimdArchType>(relativeDifference, toleranceR);
		return toleranceMask;
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMaskF32<SimdArchType> isRelativelyCloseF32(
		nasimd::SimdRegisterF32<SimdArchType> lhs, 
		nasimd::SimdRegisterF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRelativelyClose<f32, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMaskF64<SimdArchType> isRelativelyCloseF64(
		nasimd::SimdRegisterF64<SimdArchType> lhs,
		nasimd::SimdRegisterF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRelativelyClose<f64, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType> && nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMMaskClass<FloatType, SimdArchType> isRelativelyClose(
		nasimd::SimdRegisterClass<FloatType, SimdArchType> lhs,
		nasimd::SimdRegisterClass<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {
		return isRelativelyClose<FloatType, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMMaskClassF32<SimdArchType> isRelativelyCloseF32(
		nasimd::SimdRegisterClassF32<SimdArchType> lhs,
		nasimd::SimdRegisterClassF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRelativelyClose<f32, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMMaskClassF64<SimdArchType> isRelativelyCloseF64(
		nasimd::SimdRegisterClassF64<SimdArchType> lhs,
		nasimd::SimdRegisterClassF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRelativelyClose<f64, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	//roughtly equal
	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType> && nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMask<FloatType, SimdArchType> isRoughtlyEqual(
		nasimd::SimdRegister<FloatType, SimdArchType> lhs,
		nasimd::SimdRegister<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {

		nasimd::SimdRegister<FloatType, SimdArchType> difference = nasimd::simd_sub<FloatType, SimdArchType>(lhs, rhs);
		nasimd::SimdRegister<FloatType, SimdArchType> absDifference = nasimd::simd_abs<FloatType, SimdArchType>(difference);
		nasimd::SimdRegister<FloatType, SimdArchType> toleranceR = nasimd::simd_set<FloatType, SimdArchType>(tolerance);

		nasimd::SimdMask<FloatType, SimdArchType> toleranceMask = nasimd::simd_compare_less_than_or_equal<FloatType, SimdArchType>(absDifference, toleranceR);
		nasimd::SimdMask<FloatType, SimdArchType> equalMask = nasimd::simd_compare_equal<FloatType, SimdArchType>(lhs, rhs);
		return nasimd::simd_mask_bitwise_or<FloatType, SimdArchType>(toleranceMask, equalMask);
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMaskF32<SimdArchType> isRoughtlyEqualF32(
		nasimd::SimdRegisterF32<SimdArchType> lhs,
		nasimd::SimdRegisterF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRoughtlyEqual<f32, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMaskF64<SimdArchType> isRoughtlyEqualF64(
		nasimd::SimdRegisterF64<SimdArchType> lhs,
		nasimd::SimdRegisterF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRoughtlyEqual<f64, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType>&& nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMMaskClass<FloatType, SimdArchType> isRoughtlyEqual(
		nasimd::SimdRegisterClass<FloatType, SimdArchType> lhs,
		nasimd::SimdRegisterClass<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {
		return isRoughtlyEqual<FloatType, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMMaskClassF32<SimdArchType> isRoughtlyEqualF32(
		nasimd::SimdRegisterClassF32<SimdArchType> lhs,
		nasimd::SimdRegisterClassF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRoughtlyEqual<f32, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(nasimd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr nasimd::SimdMMaskClassF64<SimdArchType> isRoughtlyEqualF64(
		nasimd::SimdRegisterClassF64<SimdArchType> lhs,
		nasimd::SimdRegisterClassF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRoughtlyEqual<f64, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}
}
