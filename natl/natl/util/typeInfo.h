#pragma once 

//@begin_non_modules
//own
#include "compilerDependent.h"
//@end_non_modules

//own
#include "stringLiteral.h"
#include "rawTypeInfo.h"
#include "../container/stringView.h"
#include "../container/stringView.h"

//@export
namespace natl {
	template<typename Type>
	consteval auto getNameOfType() noexcept {
		TypeNameStr name = getTypeNameStrOfType<Type>();
		return ConstAsciiStringView(name.data(), name.length());
	}

	template<typename Type>
		requires(IsStructC<Type>)
	consteval auto getNameOfStruct() noexcept {
		ConstAsciiStringView name = getNameOfType<Type>();

#if defined(NATL_COMPILER_MSVC)
		//prefix: "class " | "struct "
		if (name.startsWith("c")) {
			return name.substr(6);
		} else {
			return name.substr(7);
		}
#else
		static_assert("natl: natl::getNameOfStruct() not supported");
#endif
	}


	constexpr natl::Size getHashCodeFromNameOfType(const ConstAsciiStringView nameOfType) noexcept {
		return getHashCodeOfTypeName(nameOfType.c_str(), nameOfType.size());
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