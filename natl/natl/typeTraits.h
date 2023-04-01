//header
#pragma once
//own
#include "pch.h"
//endHeader


/*module
export module natl.typeTraits;

//std
import std;

endModule*/

//interface
/*export*/ namespace natl {
	template<class Type>
	concept isNotConst = requires {
		!std::is_const<Type>::value;
	};
	template<class Type>
	concept isConst = requires {
		std::is_const<Type>::value;
	};

	
	template<class Type>
	using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

    template<class Type>
    concept NonTrivialIsTriviallyRelocatable = requires {
        typename Type::natl_trivially_relocatable;
        Type::natl_trivially_relocatable == true;
    };

    template<class Type>
    concept IsTriviallyRelocatable = std::is_trivially_copyable_v<Type> || NonTrivialIsTriviallyRelocatable<Type>;

    template <class Type>
    concept MemcopyConstructible = std::is_trivially_copyable_v<Type> && NonTrivialIsTriviallyRelocatable<Type>;

    template <class Src, class Dst, class SrcRef>
    concept MemcopyConstructibleSrcDst =
        !std::is_same_v<Src, void> &&
        !std::is_same_v<Dst, void> &&
        !std::is_same_v<SrcRef, void> &&
        ((std::is_same_v<Src, Dst>&& MemcopyConstructible<Src>) || (sizeof(Src) == sizeof(Dst) && std::is_trivially_constructible_v<Dst, SrcRef>));

    template<class Src, class Dst, class SrcRef, class DstRef>
    concept MemcopyAssignableSrcDst =
        (std::is_same_v<Src, Dst> && MemcopyConstructible<Src>) ||
        (sizeof(Src) == sizeof(Dst) && std::is_trivially_assignable_v<DstRef, SrcRef>);
}