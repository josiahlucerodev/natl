#pragma once

//own
#include "basicTypes.h"

//interface 
namespace natl {
	//fnv1a
	constexpr Size fnv1aOffsetBasis = 14695981039346656037ULL;
	constexpr Size fnv1aPrime = 1099511628211ULL;
	constexpr Size fnv1aHashCstr(const AssciCode* strPtr, Size hash = fnv1aOffsetBasis) noexcept {
		while (*strPtr != '\0') {
			hash ^= static_cast<Size>(*strPtr++);
			hash *= fnv1aPrime;
		}
		return hash;
	}
	constexpr Size fnv1aHashCstr(const Utf32* strPtr, Size hash = fnv1aOffsetBasis) noexcept {
		while (*strPtr != '\0') {
			hash ^= static_cast<Size>(*strPtr++);
			hash *= fnv1aPrime;
		}
		return hash;
	}

	constexpr Size fnv1aHash(const AssciCode* strPtr, const Size size, Size hash = fnv1aOffsetBasis) noexcept {
		const AssciCode* endPtr = strPtr + size;
		for (; strPtr < endPtr; strPtr++) {
			hash ^= static_cast<Size>(*strPtr);
			hash *= fnv1aPrime;
		}
		return hash;
	}
	constexpr Size fnv1aHash(const Utf32* strPtr, const Size size, Size hash = fnv1aOffsetBasis) noexcept {
		const Utf32* endPtr = strPtr + size;
		for (; strPtr < endPtr; strPtr++) {
			hash ^= static_cast<Size>(*strPtr);
			hash *= fnv1aPrime;
		}
		return hash;
	}
}