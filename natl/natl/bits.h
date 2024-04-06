#pragma once 

//own
#include "compilerDependent.h"
#include "typeTraits.h"

//interface 
namespace natl {
    template<typename Integer>
        requires(IsFundamentalUnsignedItergerV<Integer>)
    NATL_FORCE_INLINE constexpr Integer setNthBitToZero(const Integer num, const Integer n) noexcept {
        const Integer mask = ~(Integer(1) << n);
        const Integer result = num & mask;
        return result;
    }

    template<typename Integer>
        requires(IsFundamentalUnsignedItergerV<Integer>)
    NATL_FORCE_INLINE constexpr Integer setNthBitToOne(const Integer num, const Integer n) noexcept {
        const Integer mask = Integer(1) << n;
        const Integer result = num | mask;
        return result;
    }
}