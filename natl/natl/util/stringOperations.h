#pragma once 

//own
#include "basicTypes.h"
#include "utility.h"
#include "typeTraits.h"
#include "limits.h"

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

	constexpr Size hashStringLessThan8(const Ascii* str, const Size size) noexcept {
		Size hash = 0;
		for (Size i = 0; i < size; i++) {
			hash |= static_cast<Size>(str[i]) << (i * 8);
		}
		return hash;
	}
	constexpr Size hashStringLessThan8(const Ascii* begin, const Ascii* end) noexcept {
		return hashStringLessThan8(begin, end - begin);
	}
	constexpr Size hashStringLessThan8(const Ascii* str) noexcept {
		return hashStringLessThan8(str, cstringLength(str));
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	constexpr Size hashStringLessThan8(const StringViewLike& stringViewLike) noexcept {
		return hashStringLessThan8(stringViewLike.data(), stringViewLike.size());
	}

	inline Size hashStringLessThan8Unbounded(const Ascii* str, const Size size) noexcept {
		const Size hash = *reinterpret_cast<const Size*>(str);
		return hash & ~(natl::Limits<Size>::max() << (size * 8));
	}
}