#pragma once

//own
#include "basicTypes.h"

//interface 
namespace natl {
	//fnv1a
	constexpr inline Size fnv1aOffsetBasis = Size(14695981039346656037ULL);
	constexpr inline Size fnv1aPrime = Size(1099511628211ULL);
	constexpr Size fnv1aHashCstr(const Assci* strPtr, Size hash = fnv1aOffsetBasis) noexcept {
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

	constexpr Size fnv1aHash(const Assci* strPtr, const Size size, Size hash = fnv1aOffsetBasis) noexcept {
		const Assci* endPtr = strPtr + size;
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