#pragma once 

//own
#include "basicTypes.h"
#include "hash.h"
#include "stringOperations.h"

//interface 
namespace natl {
	struct TypeNameStr {
		const Ascii* strPtr;
		Size strLength;

		constexpr const Ascii* data() const noexcept { return strPtr; }
		constexpr const Ascii* c_str() const noexcept { return strPtr; }

		//capacity 
		constexpr Size size() const noexcept { return strLength; }
		constexpr Size length() const noexcept { return strLength; }
		constexpr Size max_size() const noexcept { return ~Size(1); };
		constexpr Bool empty() const noexcept { return !strPtr || strLength == 0; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }
	};

	template<typename Type>
	consteval auto getTypeNameStrOfType() noexcept {

#if defined(NATL_COMPILER_EMSCRIPTEN)
		//prefix: "auto natl::getTypeNameStrOfType() [Type = "
		//suffix: "]"

		TypeNameStr name;
		name.strPtr = __PRETTY_FUNCTION__ + 42;
		name.strLength = cstringLength(name.strPtr) - 1;
		return name;
#elif defined(NATL_COMPILER_GCC)
		//prefix: "consteval auto natl::getTypeNameStrOfType() [with Type = ";
		//suffix: "]";

		TypeNameStr name;
		name.strPtr = __PRETTY_FUNCTION__ + 57;
		name.strLength = cstringLength(name.strPtr) - 1;
		return name;
#elif defined(NATL_COMPILER_MSVC)
		//prefix: "auto __cdecl natl::getTypeNameStrOfType<"
		//suffix:  ">(void) noexcept"

		TypeNameStr name;
		name.strPtr = __FUNCSIG__ + 40;
		name.strLength = cstringLength(name.strPtr) - 16;
		return name;
#else
		static_assert("natl: natl::getTypeNameStrOfType() not supported");
#endif
	}
	constexpr natl::Size getHashCodeOfTypeName(const Ascii* str, const Size size) noexcept {
		return fnv1aHash(str, size);
	}

	constexpr natl::Size getHashCodeFromTypeNameStr(const TypeNameStr str) noexcept {
		return getHashCodeOfTypeName(str.data(), str.size());
	}

	template<typename Type>
	consteval natl::Size getHashCodeOfType() noexcept {
		return getHashCodeFromTypeNameStr(getTypeNameStrOfType<Type>());
	}
}