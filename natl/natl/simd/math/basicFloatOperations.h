#pragma once 

//@begin_non_modules
//own
#include "../../util/compilerDependent.h"
//@end_non_modules

//own
#include "../simdOf.h"
#include "../simdClass.h"

//@export 
namespace natl::math {
	//relatively close
	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType> && natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMask<FloatType, SimdArchType> isRelativelyClose(
		natl::simd::SimdRegister<FloatType, SimdArchType> lhs,
		natl::simd::SimdRegister<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {
		natl::simd::SimdRegister<FloatType, SimdArchType> lhsAbs = natl::simd::simd_abs<FloatType, SimdArchType>(lhs);
		natl::simd::SimdRegister<FloatType, SimdArchType> rhsAbs = natl::simd::simd_abs<FloatType, SimdArchType>(rhs);
		natl::simd::SimdRegister<FloatType, SimdArchType> larger = natl::simd::simd_max<FloatType, SimdArchType>(lhsAbs, rhsAbs);

		natl::simd::SimdRegister<FloatType, SimdArchType> difference = natl::simd::simd_sub<FloatType, SimdArchType>(lhs, rhs);
		natl::simd::SimdRegister<FloatType, SimdArchType> absDifference = natl::simd::simd_abs<FloatType, SimdArchType>(difference);

		natl::simd::SimdRegister<FloatType, SimdArchType> relativeDifference = natl::simd::simd_div<FloatType, SimdArchType>(absDifference, larger);

		natl::simd::SimdRegister<FloatType, SimdArchType> toleranceR = natl::simd::simd_set<FloatType, SimdArchType>(tolerance);
		natl::simd::SimdMask<FloatType, SimdArchType> toleranceMask = natl::simd::simd_compare_less_than_or_equal<FloatType, SimdArchType>(relativeDifference, toleranceR);
		return toleranceMask;
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMaskF32<SimdArchType> isRelativelyCloseF32(
		natl::simd::SimdRegisterF32<SimdArchType> lhs, 
		natl::simd::SimdRegisterF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRelativelyClose<f32, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMaskF64<SimdArchType> isRelativelyCloseF64(
		natl::simd::SimdRegisterF64<SimdArchType> lhs,
		natl::simd::SimdRegisterF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRelativelyClose<f64, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType> && natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMMaskstruct<FloatType, SimdArchType> isRelativelyClose(
		natl::simd::SimdRegisterstruct<FloatType, SimdArchType> lhs,
		natl::simd::SimdRegisterstruct<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {
		return isRelativelyClose<FloatType, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMMaskstructF32<SimdArchType> isRelativelyCloseF32(
		natl::simd::SimdRegisterstructF32<SimdArchType> lhs,
		natl::simd::SimdRegisterstructF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRelativelyClose<f32, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMMaskstructF64<SimdArchType> isRelativelyCloseF64(
		natl::simd::SimdRegisterstructF64<SimdArchType> lhs,
		natl::simd::SimdRegisterstructF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRelativelyClose<f64, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	//roughtly equal
	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType> && natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMask<FloatType, SimdArchType> isRoughtlyEqual(
		natl::simd::SimdRegister<FloatType, SimdArchType> lhs,
		natl::simd::SimdRegister<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {

		natl::simd::SimdRegister<FloatType, SimdArchType> difference = natl::simd::simd_sub<FloatType, SimdArchType>(lhs, rhs);
		natl::simd::SimdRegister<FloatType, SimdArchType> absDifference = natl::simd::simd_abs<FloatType, SimdArchType>(difference);
		natl::simd::SimdRegister<FloatType, SimdArchType> toleranceR = natl::simd::simd_set<FloatType, SimdArchType>(tolerance);

		natl::simd::SimdMask<FloatType, SimdArchType> toleranceMask = natl::simd::simd_compare_less_than_or_equal<FloatType, SimdArchType>(absDifference, toleranceR);
		natl::simd::SimdMask<FloatType, SimdArchType> equalMask = natl::simd::simd_compare_equal<FloatType, SimdArchType>(lhs, rhs);
		return natl::simd::simd_mask_bitwise_or<FloatType, SimdArchType>(toleranceMask, equalMask);
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMaskF32<SimdArchType> isRoughtlyEqualF32(
		natl::simd::SimdRegisterF32<SimdArchType> lhs,
		natl::simd::SimdRegisterF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRoughtlyEqual<f32, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMaskF64<SimdArchType> isRoughtlyEqualF64(
		natl::simd::SimdRegisterF64<SimdArchType> lhs,
		natl::simd::SimdRegisterF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRoughtlyEqual<f64, SimdArchType>(lhs, rhs, tolerance);
	}

	template<typename FloatType, typename SimdArchType>
		requires(natl::IsBuiltInFloatingPointC<FloatType>&& natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMMaskstruct<FloatType, SimdArchType> isRoughtlyEqual(
		natl::simd::SimdRegisterstruct<FloatType, SimdArchType> lhs,
		natl::simd::SimdRegisterstruct<FloatType, SimdArchType> rhs,
		FloatType tolerance) noexcept {
		return isRoughtlyEqual<FloatType, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMMaskstructF32<SimdArchType> isRoughtlyEqualF32(
		natl::simd::SimdRegisterstructF32<SimdArchType> lhs,
		natl::simd::SimdRegisterstructF32<SimdArchType> rhs, const f32 tolerance) noexcept {
		return isRoughtlyEqual<f32, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}

	template<typename SimdArchType>
		requires(natl::simd::IsSimdArch<SimdArchType>)
	NATL_FORCE_INLINE constexpr natl::simd::SimdMMaskstructF64<SimdArchType> isRoughtlyEqualF64(
		natl::simd::SimdRegisterstructF64<SimdArchType> lhs,
		natl::simd::SimdRegisterstructF64<SimdArchType> rhs, const f64 tolerance) noexcept {
		return isRoughtlyEqual<f64, SimdArchType>(lhs.getRegister(), rhs.getRegister(), tolerance);
	}
}
