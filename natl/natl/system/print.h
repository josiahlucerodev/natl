#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/stringOperations.h"
#include "../util/typeTraits.h"

//@export
namespace natl {
	struct InputReturn {
		Bool successful;
		Size charactersRead;
	};

	Bool print(const Ascii* string, const Size size) noexcept;
	Bool println(const Ascii* string, const Size size) noexcept;
	InputReturn input(Ascii* dst, const Size dstSize) noexcept;

	inline Bool print(const Ascii* string) noexcept {
		return print(string, cstringLength(string));
	}
	inline Bool println(const Ascii* string) noexcept {
		return println(string, cstringLength(string));
	}
	inline Bool println() noexcept {
		return print("\n");
	}
}