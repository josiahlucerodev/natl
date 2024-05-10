#pragma once 

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "compilerDependent.h"
#include "math.h"

//interface
namespace natl {
    template<typename Integer>
        requires(IsBuiltInInterger<Integer>)
    NATL_FORCE_INLINE constexpr Bool isEven(const Integer value) noexcept {
        return value % Integer(2) == Integer(0);
    }

    template<typename Integer>
        requires(IsBuiltInInterger<Integer>)
    NATL_FORCE_INLINE constexpr Integer roundDownToEven(const Integer value) noexcept {
        if (value % Integer(2) == Integer(0)) {
            return value;
        } else {
            return value - Integer(1);
        }
    }

    template<typename Integer>
        requires(IsBuiltInInterger<Integer>)
    NATL_FORCE_INLINE constexpr Integer findLargestEvenDivisor(const Integer number) noexcept {
        if (number % Integer(2) == Integer(0)) {
            return number;
        } else {
            for (Integer i = roundDownToEven<Integer>(number / Integer(2)); i >= Integer(1); i -= 2) {
                if (i % Integer(2) == Integer(0)) {
                    return i;
                }
            }
            return Integer(1);
        }
    }

    template<typename Integer>
        requires(IsBuiltInInterger<Integer>)
    NATL_FORCE_INLINE constexpr Integer findSmallestEvenDivisor(Integer number) noexcept {
        number = natl::math_abs(number);
        for (Integer i = Integer(2); i <= number; i += Integer(2)) {
            if (number % i == Integer(0)) {
                return i;
            }
        }
        return Integer(1);
    }
    template<typename Integer>
        requires(IsBuiltInInterger<Integer>)
    NATL_FORCE_INLINE constexpr Bool isPowerOfTwo(const Integer value) noexcept {
        return (value > Integer(0)) && ((value & (value - Integer(1))) == Integer(0));
    }
}