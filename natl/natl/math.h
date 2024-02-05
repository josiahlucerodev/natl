#pragma once 

//std
#include <cmath>

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "compilerDependent.h"
#include "error.h"

//interface
namespace natl {
	namespace impl {
		template <typename Float>
		constexpr Float log2Float(const Float x, const Float base = Float(2)) {
			return (x <= Float(1)) ? Float(0) : Float(1) + log2Float(x / base, base);
		}
	}

	//basic operations
	template<typename NumericType>
		requires(IsNumericType<NumericType>)
	NATL_FORCE_INLINE constexpr NumericType math_abs(const NumericType value) noexcept {
		if constexpr (IsUnsignedItergerType<NumericType>) {
			return value;
		} else {
			if (std::is_constant_evaluated()) {
				return (value < NumericType(0)) ? -value : value;
			} else {
				if constexpr (IsFloatingPointType<NumericType>) {
					return std::abs(value);
				} else {
					return std::fabs(value);
				}
			}
		}
	}

	template<typename Integer>
		requires(IsItergerType<Integer>)
	NATL_FORCE_INLINE constexpr Integer twoToThePower(const Integer value) noexcept {
		if constexpr (IsSignedItergerType<Integer>) {
			return (value >= Integer(0) && value < ((sizeof(Integer) * 8) - 1)) ? Integer(1) << value : Integer(0);
		}
		if constexpr (IsUnsignedItergerType<Integer>) {
			return (value < ((sizeof(Integer) * 8))) ? Integer(1) << value : Integer(0);
		}
	}
}