#pragma once 

//own
#include "commonHashs.h"
#include "../util/stringLiteral.h"
#include "../container/stringView.h"

//interface 
namespace natl {
	template<typename Type>
	consteval auto getNameOfType() noexcept {
#ifdef NATL_COMPILER_EMSCRIPTEN
		ConstAsciiStringView name = __PRETTY_FUNCTION__;
		//prefix: "auto natl::getNameOfType() [Type = "
		//suffix: "]"
		return ConstAsciiStringView(__PRETTY_FUNCTION__ + 35).removeSuffix(1); 
#endif // NATL_COMPILER_EMSCRIPTEN

#ifdef NATL_COMPILER_CLANG
		ConstAsciiStringView name = __PRETTY_FUNCTION__;
		const ConstAsciiStringView prefix = "auto natl::getNameOfType() [T = ";
		const ConstAsciiStringView suffix = "]";
#endif // NATL_COMPILER_CLANG

#ifdef NATL_COMPILER_GCC 
		//prefix: "consteval auto natl::getNameOfType() [with Type = ";
		//suffix: "]";
		return ConstAsciiStringView(__PRETTY_FUNCTION__ + 50).removeSuffix(1);
#endif // NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
		//prefix: "auto __cdecl natl::getNameOfType<"
		//suffix:  ">(void) noexcept"
		return ConstAsciiStringView(__FUNCSIG__ + 33).removeSuffix(16);
#endif // NATL_COMPILER_MSVC
	}

	constexpr natl::Size getHashCodeFromNameOfType(const ConstAsciiStringView nameOfType) noexcept {
		return fnv1aHash(nameOfType.c_str(), nameOfType.size());
	}

	template<typename Type>
	consteval natl::Size getHashCodeOfType() noexcept {
		constexpr ConstAsciiStringView nameOfType = getNameOfType<Type>();
		return getHashCodeFromNameOfType(nameOfType);
	}

	struct TypeInfo {
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

	template<typename Type>
	struct NameOfBuiltInNumericV;

	template<> struct NameOfBuiltInNumericV<i8> {
		constexpr static ConstAsciiStringView value = "i8";
	};
	template<> struct NameOfBuiltInNumericV<i16> {
		constexpr static ConstAsciiStringView value = "i16";
	};
	template<> struct NameOfBuiltInNumericV<i32> {
		constexpr static ConstAsciiStringView value = "i32";
	};
	template<> struct NameOfBuiltInNumericV<i64> {
		constexpr static ConstAsciiStringView value = "i64";
	};

	template<> struct NameOfBuiltInNumericV<ui8> {
		constexpr static ConstAsciiStringView value = "ui8";
	};
	template<> struct NameOfBuiltInNumericV<ui16> {
		constexpr static ConstAsciiStringView value = "ui16";
	};
	template<> struct NameOfBuiltInNumericV<ui32> {
		constexpr static ConstAsciiStringView value = "ui32";
	};
	template<> struct NameOfBuiltInNumericV<ui64> {
		constexpr static ConstAsciiStringView value = "ui64";
	};

	template<> struct NameOfBuiltInNumericV<f32> {
		constexpr static ConstAsciiStringView value = "f32";
	};
	template<> struct NameOfBuiltInNumericV<f64> {
		constexpr static ConstAsciiStringView value = "f64";
	};

	template<typename NumericType>
		requires(IsBuiltInNumericC<NumericType>)
	constexpr inline ConstAsciiStringView NameOfBuiltInNumeric = NameOfBuiltInNumericV<NumericType>::value;

	template<typename NumericType>
	constexpr ConstAsciiStringView getNameOfBuiltInNumeric() noexcept {
		return NameOfBuiltInNumeric<NumericType>;
	}
}