#pragma once

//std
#include <string>

//own
#include "hash.h"
#include "string.h"

//interface 
namespace natl {
	//fnv1a
	constexpr std::uint64_t fnv1aOffsetBasis = 14695981039346656037ULL;
	constexpr std::uint64_t fnv1aPrime = 1099511628211ULL;
	constexpr std::uint64_t fnv1aHash(const char* strPtr, const std::uint64_t hash = fnv1aOffsetBasis) noexcept {
		return (*strPtr == '\0') ? hash : fnv1aHash(strPtr + 1, (hash ^ static_cast<std::uint64_t>(*strPtr)) * fnv1aPrime);
	}

	template<>
	struct Hash<String> {
		constexpr static std::size_t hash(const String& string) {
			return fnv1aHash(string.c_str());
		}
	};
}