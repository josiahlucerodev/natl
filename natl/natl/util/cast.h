#pragma once 

//@begin_non_modules
//own
#include "compilerDependent.h"
#include "stateful.h"
//@end_non_modules

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "bits.h"
#include "typePack.h"

//@export
namespace natl {
	template<typename Type>
	[[nodiscard]] NATL_FORCE_INLINE constexpr Type implicitCast(TypeIdentity<Type> value) noexcept {
		return value;
	}
	template<typename To, typename From>
		requires(IsSameByteSizeC<To, From>)
	[[nodiscard]] NATL_FORCE_INLINE constexpr To punCast(const From& from) noexcept {
		return bitCast<To, From>(from);
	}

	template <typename Type>
	[[nodiscard]] NATL_FORCE_INLINE constexpr Type& constCast(const Type& value) noexcept {
    	return const_cast<Type&>(value);
	}
}

//@export
namespace natl {
NATL_DEFINE_STATEFUL_TYPEPACK(PublicCastTypePack)
}

