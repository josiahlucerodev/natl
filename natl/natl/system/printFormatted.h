#pragma once 

//own
#include "../container/stringView.h"
#include "../processing/format.h"
#include "print.h"
#include "printColor.h"

//@export
namespace natl {
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool print(const StringViewLike& stringViewLike) noexcept {
		return print(stringViewLike.data(), stringViewLike.size());
	}

	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool println(const StringViewLike& stringViewLike) noexcept {
		return println(stringViewLike.data(), stringViewLike.size());
	}

	namespace impl {
		constexpr inline Size defaultPrintfBufferSize = 1024;
	}

	template<typename... ArgTypes>
		requires(IsFormattableC<Decay<ArgTypes>, Ascii> && ...)
	inline Bool printf(ArgTypes&&... args) noexcept {
		using string_type = AsciiStringByteSize<impl::defaultPrintfBufferSize>;
		string_type output = format<string_type>(natl::forward<ArgTypes>(args)...);
		return print(output);
	}
	template<typename... ArgTypes>
		requires(IsFormattableC<Decay<ArgTypes>, Ascii> && ...)
	inline Bool printlnf(ArgTypes&&... args) noexcept {
		using string_type = AsciiStringByteSize<impl::defaultPrintfBufferSize>;
		string_type output = format<string_type>(natl::forward<ArgTypes>(args)...);
		return println(output);
	}

	template<typename... ArgTypes>
		requires(IsFormattableC<Decay<ArgTypes>, Ascii> && ...)
	inline Bool printfc(ArgTypes&&... args) noexcept {
		return printf(natl::forward<ArgTypes>(args)..., natl::PrintAllDefaultColor{});
	}
	template<typename... ArgTypes>
		requires(IsFormattableC<Decay<ArgTypes>, Ascii> && ...)
	inline Bool printlnfc(ArgTypes&&... args) noexcept {
		return printlnf(natl::forward<ArgTypes>(args)..., natl::PrintAllDefaultColor{});
	}
}