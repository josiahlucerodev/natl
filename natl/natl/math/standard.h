#pragma once 

//own
#include "../util/basicTypes.h"

//interface 
namespace natl::math {
	//abs 
	template<typename NumericType>
		requires(IsBuiltInNumericC<Decay<NumericType>>)
	NATL_FORCE_INLINE constexpr Decay<NumericType> abs(const NumericType value) noexcept {
		using namespace natl::literals;
		using decayed_type = Decay<NumericType>;
		if constexpr (IsBuiltInSignedIntegerC<decayed_type>) {
			constexpr NumericType mask = Limits<decayed_type>::max() >> 1;
			return value & mask;
		} else if constexpr (IsBuiltInFloatingPointC<decayed_type>) { 
			if constexpr (sizeof(NumericType) == sizeof(f32)) {
				return bitCast<f32, ui32>(bitCast<ui32, f32>(value) & 0x7FFFFFFF_ui32);
			} else {
				return bitCast<f64, ui64>(bitCast<ui64, f64>(value) & 0x7FFFFFFFFFFFFFFF_ui64);
			}
		} else {
			return value;
		}
	}
}