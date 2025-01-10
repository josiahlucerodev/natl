#pragma once 

//own
#include "compilerDependent.h"
#include "typeTraits.h"
#include "bits.h"

//interface 
namespace natl {
	template<typename Type>
	NATL_FORCE_INLINE [[nodiscard]] constexpr Type implicitCast(TypeIdentity<Type> value) noexcept {
		return value;
	}
	template<typename To, typename From>
		requires(IsSameByteSizeC<To, From>)
	NATL_FORCE_INLINE [[nodiscard]] constexpr To punCast(const From& from) noexcept {
		return bitCast<To, From>(from);
	}
}

#define NATL_TURN_OFF_PRIVATE #define private public 
#define NATL_TURN_ON_PRIVATE #undef private 