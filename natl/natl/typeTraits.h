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
	concept NonTrivialIsTriviallyRelocatable = requires {
		typename Type::natl_trivially_relocatable;
		Type::natl_trivially_relocatable == true;
	};


	template<class Type> 
	concept IsTriviallyRelocatable = std::is_trivially_copyable_v<Type> || NonTrivialIsTriviallyRelocatable<Type>;
	
	template<class Type>
	using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<Type>>;


	template <class Type> Type noexceptExpression(Type) noexcept;
}