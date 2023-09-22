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
		constexpr ~BasicString() { 
			//delete[] &this->front();
		}
	};

	template<class Alloc = DefaultAllocator<char>>
	using StringCustom = std::basic_string<char, std::char_traits<char>, Alloc>;
	using String = BasicString<char>;
}