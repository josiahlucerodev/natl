//header
#pragma once
//own
#include "pch.h"
//endHeader

/*module
export module natl.container;
//std
import std;
//own
import natl.error;
endModule*/

//interface
/*export*/ namespace natl {
	inline void fatalError(const bool conditional, const std::string_view message) {
		if (conditional) {
			std::cout << message;
			std::cin.get();
			assert(true);
		}
	}

	constexpr void fatalError(const bool conditional, const std::string_view message) {
		if (conditional) {
			throw std::logic_error(message.data());
		}
	}
}