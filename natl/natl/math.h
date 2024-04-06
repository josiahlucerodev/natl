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
		requires(IsFundamentalNumericV<NumericType>)
	NATL_FORCE_INLINE constexpr NumericType math_abs(const NumericType value) noexcept {
		if constexpr (IsFundamentalUnsignedItergerV<NumericType>) {
			return value;
		} else {
			if (isConstantEvaluated()) {
				return (value < NumericType(0)) ? -value : value;
			} else {
				if constexpr (IsFloatingPointV<NumericType>) {
					return std::abs(value);
				} else {
					return std::fabs(value);
				}
			}
		}
	}

	template<typename Integer>
		requires(IsFundamentalItergerV<Integer>)
	NATL_FORCE_INLINE constexpr Integer twoToThePower(const Integer value) noexcept {
		if constexpr (IsFundamentalSignedItergerV<Integer>) {
			return (value >= Integer(0) && value < ((sizeof(Integer) * 8) - 1)) ? Integer(1) << value : Integer(0);
		}
		if constexpr (IsFundamentalUnsignedItergerV<Integer>) {
			return (value < ((sizeof(Integer) * 8))) ? Integer(1) << value : Integer(0);
		}
	}
}