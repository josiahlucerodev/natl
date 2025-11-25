#pragma once 

//@begin_non_modules
//own
#include "../util/compilerDependent.h"
//@end_non_modules

//own
#include "simdOf.h"

//@export
namespace natl::simd {
	template<typename DataType, typename Arch>
		requires(IsBuiltInNumericC<DataType>&& IsSimdArch<Arch>)
	NATL_FORCE_INLINE constexpr SimdCMask<DataType, Arch> createEvenCMask() noexcept {
		SimdCMask<DataType, Arch> evenCMask{};
		for (natl::Size i = 0; i < SimdCMask<DataType, Arch>::count(); i += 2) {
			evenCMask.setActiveAt(i);
		}
		return evenCMask;
	}
	template<typename DataType, typename Arch>
		requires(IsBuiltInNumericC<DataType> && IsSimdArch<Arch>)
	NATL_FORCE_INLINE constexpr SimdMask<DataType, Arch> createEvenMMask() noexcept {
		constexpr SimdCMask<DataType, Arch> evenCMask = createEvenCMask<DataType, Arch>();
		return simd_cmask_to_mmask<DataType, Arch>(evenCMask);
	}

	template<typename DataType, typename Arch>
		requires(IsBuiltInNumericC<DataType>&& IsSimdArch<Arch>)
	NATL_FORCE_INLINE constexpr SimdCMask<DataType, Arch> createOddCMask() noexcept {
		SimdCMask<DataType, Arch> oddCMask{};
		for (natl::Size i = 1; i < SimdCMask<DataType, Arch>::count(); i += 2) {
			oddCMask.setActiveAt(i);
		}
		return oddCMask;
	}
	template<typename DataType, typename Arch>
		requires(IsBuiltInNumericC<DataType> && IsSimdArch<Arch>)
	NATL_FORCE_INLINE constexpr SimdMask<DataType, Arch> createOddMMask() noexcept {
		constexpr SimdCMask<DataType, Arch> oddCMask = createOddCMask<DataType, Arch>();
		return simd_cmask_to_mmask<DataType, Arch>(oddCMask);
	}
}