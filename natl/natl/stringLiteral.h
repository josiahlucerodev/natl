#pragma once 

//own
#include "basicTypes.h"

//interface 
namespace natl {
	template <Size Number>
	struct StringLiteral {
		char stringStorage[Number];
		constexpr StringLiteral(const char(&str)[Number]) { uninitializedCopyCountNoOverlap<const char*>(str, stringStorage, Number); }
		constexpr const char* c_str() const noexcept { return stringStorage; }
		constexpr Size size() const noexcept { return Number; }
	};

	template <Size Number>
		requires (Number > 0)
	constexpr StringLiteral<Number> makeStringLiteral(const char(&str)[Number]) noexcept {
		return StringLiteral<Number>(str);
	}

	template <StringLiteral name>
	struct StringLiteralType {
		static constexpr const char name[name.size()] = name.stringStorage;
	};
}