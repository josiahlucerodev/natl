#pragma once 

//own
#include "commonHashs.h"
#include "stringView.h"

//interface 
namespace natl {
	template<typename Type>
	consteval auto getNameOfType() noexcept {
#ifdef NATL_COMPILER_EMSCRIPTEN
		ConstAsciiStringView name = __PRETTY_FUNCTION__;
		const ConstAsciiStringView prefix = "consteval auto natl::getNameOfType() [with Type = ";
		const ConstAsciiStringView suffix = "]";
#endif // NATL_COMPILER_EMSCRIPTEN

#ifdef NATL_COMPILER_CLANG
		ConstAsciiStringView name = __PRETTY_FUNCTION__;
		const ConstAsciiStringView prefix = "auto natl::getNameOfType() [T = ";
		const ConstAsciiStringView suffix = "]";
#endif // NATL_COMPILER_CLANG

#ifdef NATL_COMPILER_GCC 
		ConstAsciiStringView name = __PRETTY_FUNCTION__;
		const ConstAsciiStringView prefix = "consteval auto natl::getNameOfType() [with Type = ";
		const ConstAsciiStringView suffix = "]";
#endif // NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
		ConstAsciiStringView name = __FUNCSIG__;
		const ConstAsciiStringView prefix = "auto __cdecl natl::getNameOfType<";
		const ConstAsciiStringView suffix = ">(void) noexcept";
#endif // NATL_COMPILER_MSVC

		name.remove_prefix(prefix.size());
		name.remove_suffix(suffix.size());
		return name;
	}

	constexpr natl::Size getHashCodeFromNameOfType(const ConstAsciiStringView nameOfType) noexcept {
		return fnv1aHash(nameOfType.c_str(), nameOfType.size());
	}

	template<typename Type>
	consteval natl::Size getHashCodeOfType() noexcept {
		constexpr ConstAsciiStringView nameOfType = getNameOfType<Type>();
		return getHashCodeFromNameOfType(nameOfType);
	}

	class TypeInfo {
	private:
		ConstAsciiStringView nameOfType;
		natl::Size hashCodeOfType;
	public:
		//constructor 
		constexpr TypeInfo() noexcept = default;
		constexpr TypeInfo(const ConstAsciiStringView nameOfTypeIn) noexcept : 
			nameOfType(nameOfTypeIn), hashCodeOfType(getHashCodeFromNameOfType(nameOfTypeIn)) {}

		//destructor 
		constexpr ~TypeInfo() noexcept = default;

		//access
		constexpr ConstAsciiStringView name() const noexcept { return nameOfType; }
		constexpr natl::Size hashCode() const noexcept { return hashCodeOfType; }

		//compare 
		constexpr Bool operator==(const TypeInfo& rhs) const noexcept {
			if (hashCodeOfType != rhs.hashCodeOfType) {
				return false;
			} else {
				return nameOfType == rhs.nameOfType;
			}
		}
		constexpr Bool operator!=(const TypeInfo& rhs) const noexcept {
			return nameOfType != rhs.nameOfType;
		}
	};

	template<typename Type>
	consteval TypeInfo getTypeInfo() noexcept {
		return TypeInfo(getNameOfType<Type>());
	}
	template<typename Type>
	const TypeInfo* getTypeInfoPtr() noexcept {
		constinit static TypeInfo typeInfo = getTypeInfo<Type>();
		return &typeInfo;
	}

	template <typename LhsType, typename RhsType>
	struct TypeCompareTypenameLessThanV {
		constexpr static Bool value = getNameOfType<LhsType>() < getNameOfType<RhsType>();
	};
}