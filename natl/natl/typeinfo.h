#pragma once 

//own
#include "stringView.h"

//interface 
namespace natl {
	template<typename Type>
	consteval auto getNameOfType() noexcept {
#ifdef NATL_COMPILER_EMSCRIPTEN
		natl::StringView name = __PRETTY_FUNCTION__;
		const natl::StringView prefix = "consteval auto natl::getNameOfType() [with Type = ";
		const natl::StringView suffix = "]";
#endif // NATL_COMPILER_EMSCRIPTEN

#ifdef NATL_COMPILER_CLANG
		natl::StringView name = __PRETTY_FUNCTION__;
		const natl::StringView prefix = "auto natl::getNameOfType() [T = ";
		const natl::StringView suffix = "]";
#endif // NATL_COMPILER_CLANG

#ifdef NATL_COMPILER_GCC 
		natl::StringView name = __PRETTY_FUNCTION__;
		const natl::StringView prefix = "consteval auto natl::getNameOfType() [with Type = ";
		const natl::StringView suffix = "]";
#endif // NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
		natl::StringView name = __FUNCSIG__;
		const natl::StringView prefix = "auto __cdecl natl::getNameOfType<";
		const natl::StringView suffix = ">(void) noexcept";
#endif // NATL_COMPILER_MSVC

		name.remove_prefix(prefix.size());
		name.remove_suffix(suffix.size());
		return name;
	}

}