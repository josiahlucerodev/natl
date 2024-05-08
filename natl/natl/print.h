#pragma once 

//own
#include "compilerDependent.h"
#include "basicTypes.h"
#include "stringOperations.h"
#include "typeTraits.h"

//interface
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
}