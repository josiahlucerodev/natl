#pragma once 

//own
#include "basicTypes.h"
#include "utility.h"

//std
#include <cstring>

//natl
namespace natl {
	constexpr Size cstringLength(const Ascii* str) noexcept {
		if (isConstantEvaluated()) {
			if (str == nullptr) { return 0; }
			Size size = 0;
			while (str[size] != '\0') { size += 1; }
			return size;
		} else {
			return std::strlen(str);
		}
	}
	constexpr Size cstringLength(const Utf32* str) noexcept {
		if (str == nullptr) { return 0; }
		Size size = 0;
		while (str[size] != '\0') { size += 1; }
		return size;
	}
}