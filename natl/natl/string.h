#pragma once 

//std
#include <string>

//own
#include "allocator.h"

//interface 
namespace natl {
	template<class CharType, 
		class CharTraits = std::char_traits<CharType>, 
		class Alloc = DefaultAllocator<CharType>>
	class BasicString : public std::basic_string<CharType, CharTraits, Alloc> {
	public:
		using std::basic_string<CharType, CharTraits, Alloc>::basic_string;
	public:
		constexpr BasicString(const std::basic_string<CharType, CharTraits, Alloc>& string) : 
			std::basic_string<CharType, CharTraits, Alloc>(string) {}
		constexpr BasicString(std::basic_string<CharType, CharTraits, Alloc>&& string) :
			std::basic_string<CharType, CharTraits, Alloc>(string) {}

		template<class StringViewLike>
		using IsStringViewLike =
			std::enable_if_t<
			std::negation_v<std::is_convertible<const StringViewLike&, const CharType*>
			>, int>;

		template<class StringViewLike, IsStringViewLike<StringViewLike> = 0>
		constexpr BasicString& operator+(const char* string) noexcept {
			BasicString result = *this;
			result += string;
			return *this;
		}

		template<class StringViewLike, IsStringViewLike<StringViewLike> = 0>
		constexpr BasicString& operator+(const StringViewLike& stringView) noexcept {
			BasicString result = *this;
			result += stringView;
			return *this;
		}

		constexpr BasicString& operator=(const char* string) noexcept {
			this->assign(string);
			return *this;
		}

		template<class StringViewLike, IsStringViewLike<StringViewLike> = 0>
		constexpr BasicString& operator=(const StringViewLike& stringView) noexcept {
			this->assign(stringView.c_str(), stringView.size());
			return *this;
		}

		constexpr BasicString& operator+=(const char* string) noexcept {
			this->append(string);
			return *this;
		}

		template<class StringViewLike, IsStringViewLike<StringViewLike> = 0>
		constexpr BasicString& operator+=(const StringViewLike& stringView) noexcept {
			this->append(stringView.c_str(), stringView.size());
			return *this;
		}
	};

	template<class Alloc = DefaultAllocator<char>>
	using StringCustom = BasicString<char, std::char_traits<char>, Alloc>;
	using String = BasicString<char>;
}