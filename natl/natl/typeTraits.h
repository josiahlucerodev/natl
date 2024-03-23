#pragma once

//std
#include <concepts>
#include <type_traits>

//own
#include "basicTypes.h"
#include "compilerDependent.h"

//interface
namespace natl {
	template <typename EnumType>
	using UnderlyingType = std::underlying_type_t<EnumType>;

	template <typename EnumType>
	constexpr UnderlyingType<EnumType> toUnderlying(const EnumType value) {
		return static_cast<UnderlyingType<EnumType>>(value);
	}
	template <typename EnumType>
	constexpr EnumType fromUnderlying(const UnderlyingType<EnumType> value) {
		return static_cast<EnumType>(value);
	}


	template<typename Type>
	concept isNotConst = !std::is_const_v<Type>;
	template<typename Type>
	concept isConst = std::is_const_v<Type>;

	template<typename Type>
	concept IsEmpty = std::is_empty_v<Type>;

	template<typename Type>
	concept IsNotEmpty = !std::is_empty_v<Type>;

	template<typename Type>
	using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

	template<typename LhsType, typename RhsType>
	concept IsTheSame = std::is_same_v<LhsType, RhsType>;

	template<typename LhsType, typename RhsType>
	concept IsNotTheSame = !IsTheSame<LhsType, RhsType>;

	template<typename DataType>
	concept IsItergerType = std::is_integral_v<DataType>;

	template<typename DataType>
	concept IsSignedItergerType = IsItergerType<DataType> && std::is_signed_v<DataType>;

	template<typename DataType>
	concept IsUnsignedItergerType = IsItergerType<DataType> && std::is_unsigned_v<DataType>;

	template<typename DataType>
	concept IsNotItergerType = !IsItergerType<DataType>;

	template<typename DataType>
	concept IsFloatingPointType = std::is_floating_point_v<DataType>;

	template<typename DataType>
	concept IsNotFloatingPointType = !std::is_floating_point_v<DataType>;

	template<typename DataType>
	concept IsNumericType = IsItergerType<DataType> || IsFloatingPointType<DataType>;


	template<typename DataType, DataType v>
	struct IntegralConstant {
		constexpr static DataType value = v;
		using value_type = DataType;
		using type = IntegralConstant;
		constexpr operator value_type() const noexcept { return value; }
		constexpr value_type operator()() const noexcept { return value; }
	};

	template<bool boolValue>
	using BoolConstant = IntegralConstant<bool, boolValue>;

	using TrueType = BoolConstant<true>;
	using FalseType = BoolConstant<false>;

	template<typename DataType> struct RemoveCVType { using type = DataType; };
	template<typename DataType> struct RemoveCVType<const DataType> { using type = DataType; };
	template<typename DataType> struct RemoveCVType<volatile DataType> { using type = DataType; };
	template<typename DataType> struct RemoveCVType<const volatile DataType> { using type = DataType; };

	template<typename DataType> struct RemoveConstType { using type = DataType; };
	template<typename DataType> struct RemoveConstType<const DataType> { using type = DataType; };

	template<typename DataType> struct RemoveVolatileType { using type = DataType; };
	template<typename DataType> struct RemoveVolatileType<volatile DataType> { using type = DataType; };

	template<typename DataType> struct RemoveReferenceType { using type = DataType; };
	template<typename DataType> struct RemoveReferenceType<DataType&> { using type = DataType; };
	template<typename DataType> struct RemoveReferenceType<DataType&&> { using type = DataType; };

	template<bool B, typename DataType, typename F>
	struct ConditionalType { using type = DataType; };
	template<typename DataType, typename F>
	struct ConditionalType<false, DataType, F> { using type = F; };


	template<typename DataType>
	struct TypeIdentityType { using type = DataType; };

	template<typename DataType>
	using RemoveCV = typename RemoveCVType<DataType>::type;
	template<typename DataType>
	using RemoveConst = typename RemoveConstType<DataType>::type;
	template<typename DataType>
	using RemoveVolatile = typename RemoveVolatileType<DataType>::type;

	template<typename DataType>
	using RemoveReference = typename RemoveReferenceType<DataType>::type;

	template<bool B, typename DataType, typename F>
	using Conditional = typename ConditionalType<B, DataType, F>::type;

	template<typename DataType, typename U> struct IsSameType : FalseType {};
	template<typename DataType> struct IsSameType<DataType, DataType> : TrueType {};
	template<typename DataType, typename U> constexpr inline bool IsSame = IsSameType<DataType, U>::value;

	template<typename DataType, typename U> struct IsNotSameType : TrueType {};
	template<typename DataType> struct IsNotSameType<DataType, DataType> : FalseType {};
	template<typename DataType, typename U> constexpr inline bool IsNotSame = IsNotSameType<DataType, U>::value;

	template<typename DataType>
	struct IsVoidType : IsSameType<void, RemoveCV<DataType>> {};
	template<typename DataType>
	constexpr inline bool IsVoid = IsVoidType<DataType>::value;

	template<typename DataType>
	using TypeIdentity = typename TypeIdentityType<DataType>::type;

	namespace impl {
		template<typename DataType> auto tryAddPointer(int) -> TypeIdentityType<RemoveReference<DataType>*> {};
		template<typename DataType> auto tryAddPointer(...) -> TypeIdentityType<DataType> {};
	}

	template<typename DataType> struct AddPointerType : decltype(impl::tryAddPointer<DataType>(0)) {};
	template<typename DataType> using AddPointer = typename AddPointerType<DataType>::type;

	template<typename... AnyTypes>
	struct MakeVoidType { using type = void; };

	template<typename... AnyTypes>
	using MakeVoid = MakeVoidType<AnyTypes...>::type;

	template <class DataType, class = void>
	struct UniversalAddReferencesType {
		using lvalue = DataType;
		using rvalue = DataType;
	};
	template <class DataType>
	struct UniversalAddReferencesType<DataType, MakeVoid<DataType&>> {
		using lvalue = DataType&;
		using rvalue = DataType&&;
	};
	template<typename DataType> struct AddLValueReferenceType {
		using type = UniversalAddReferencesType<DataType>::lvalue;
	};
	template<typename DataType> struct AddRValueReferenceType {
		using type = UniversalAddReferencesType<DataType>::rvalue;
	};

	template<typename DataType>
	using AddLValueReference = typename AddLValueReferenceType<DataType>::type;
	template<typename DataType>
	using AddRValueReference = typename AddRValueReferenceType<DataType>::type;

	template<typename DataType> struct IsRValueReferenceType : FalseType {};
	template<typename DataType> struct IsRValueReferenceType<DataType&&> : TrueType {};
	template<typename DataType> constexpr inline bool IsRValueReference = IsRValueReferenceType<DataType>::value;

	template<typename DataType> struct IsLValueReferenceType : FalseType {};
	template<typename DataType> struct IsLValueReferenceType<DataType&> : TrueType {};
	template<typename DataType> constexpr inline bool IsLValueReference = IsLValueReferenceType<DataType>::value;

	template<typename DataType> struct IsReferenceType : FalseType {};
	template<typename DataType> struct IsReferenceType<DataType&> : TrueType {};
	template<typename DataType> struct IsReferenceType<DataType&&> : TrueType {};
	template<typename DataType> constexpr inline bool IsReference = IsReferenceType<TrueType>::value;

	template<typename DataType>
	//std::add_lvalue_reference_t<DataType> 
	AddRValueReference<DataType>
		declval() noexcept {
		//static_assert(false, "natl: declval not allowed in an evaluated context");
	}

	template<typename...> struct ConjunctionType : TrueType {};
	template<typename B1> struct ConjunctionType<B1> : B1 {};
	template<typename B1, typename... Bn> struct ConjunctionType<B1, Bn...> : ConditionalType<bool(B1::value), ConjunctionType<Bn...>, B1> {};
	template<typename... B> inline constexpr bool Conjunction = ConjunctionType<B...>::value;

	namespace impl {
		template<typename DataType>
		auto testReturnable(int) -> decltype(void(static_cast<DataType(*)()>(nullptr)), TrueType{});
		template<typename> auto testReturnable(...) -> FalseType;
		template<typename From, typename To>
		auto testImplicitlyConvertible(int) -> decltype(void(declval<void(&)(To)>()(declval<From>())), TrueType{});
		template<typename, typename> auto testImplicitlyConvertible(...) -> FalseType;
	}

	template<typename From, typename To>
	struct IsConvertibleType : BoolConstant<
		(decltype(impl::testReturnable<To>(0))::value&&
			decltype(impl::testImplicitlyConvertible<From, To>(0))::value) ||
		(IsVoid<From> && IsVoid<To>)
	> {};

	template<typename From, typename To>
	struct IsNoThrowConvertibleType : ConjunctionType<IsVoidType<From>, IsVoidType<To>> {};
	template<typename From, typename To>
		requires (
			requires {
		static_cast<To(*)()>(nullptr);
		{ declval<void(&)(To) noexcept>()(declval<From>()) } noexcept;
	}
	)
		struct IsNoThrowConvertibleType<From, To> : TrueType {};

	template<typename From, typename To>
	constexpr inline bool IsConvertible = IsConvertibleType<From, To>::value;
	template<typename From, typename To>
	constexpr inline bool IsNothrowConvertible = IsNoThrowConvertibleType<From, To>::value;

	template<typename From, typename To>
	concept ConvertibleTo = IsConvertible<From, To>; //&& requires { static_cast<To>(std::declval<From>()); };

	template<typename T> using IsFunctionType = std::is_function<T>;
	template<typename T> constexpr inline bool IsFunction = IsFunctionType<T>::value;

	template<class DataType> struct RemoveExtentType { using type = DataType; };
	template<class DataType> struct RemoveExtentType<DataType[]> { using type = DataType; };
	template<class DataType, std::size_t Number> struct RemoveExtentType<DataType[Number]> { using type = DataType; };
	template<class DataType> using RemoveExtent = RemoveExtentType<DataType>::type;

	template<class DataType> struct IsArrayType : FalseType {};
	template<class DataType> struct IsArrayType<DataType[]> : TrueType {};
	template<class DataType, std::size_t Number> struct IsArrayType<DataType[Number]> : TrueType {};
	template<class DataType> constexpr inline bool IsArray = IsArrayType<DataType>::value;

	template<class DataType>
	struct DecayType {
	private:
		using U = RemoveReference<DataType>;
	public:
		using type = Conditional<
			IsArray<U>,
			AddPointer<RemoveExtent<U>>,
			Conditional<
			IsFunction<U>,
			AddPointer<U>,
			RemoveCV<U>
			>
		>;
	};

	template<class DataType> using Decay = DecayType<DataType>::type;

	template <typename DataType>
	concept IsCharacterType =
		std::is_same_v<std::remove_cv_t<DataType>, AssciCode> ||
		std::is_same_v<std::remove_cv_t<DataType>, char8_t> ||
		std::is_same_v<std::remove_cv_t<DataType>, char16_t> ||
		std::is_same_v<std::remove_cv_t<DataType>, Utf32>;

	template <typename DataType>
	concept IsBasicType = std::is_integral_v<DataType> || std::is_floating_point_v<DataType> || std::is_pointer_v<DataType> || IsCharacterType<DataType> || std::same_as<DataType, Byte>;

	template <typename DataType>
	concept IsCopyConstructible = std::is_copy_constructible_v<std::decay_t<DataType>>;

	template <typename DataType>
	concept IsCopyAssignable = std::is_copy_assignable_v<std::decay_t<DataType>>;

	template <typename DataType>
	concept IsMoveConstructible = std::is_move_constructible_v<std::decay_t<DataType>>;

	namespace impl {
#define NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(conceptName, memberName) \
        template<typename DataType> \
        consteval bool customCheckIf##conceptName() noexcept { \
            if constexpr (requires() { { DataType::trivially##memberName } -> std::convertible_to<bool>; }) { \
                return DataType::trivially##memberName; \
            } else { \
                return false; \
            } \
        }

		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyCompareable, Compareable);
		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyRelocatable, Relocatable);
		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyDefaultConstructible, DefaultConstructible);
		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyDestructible, Destructible);
		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyMoveConstructible, MoveConstructedable);
		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyConstRefConstructible, ConstRefConstructedable);
		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyMoveAssignable, MoveAssignable);
		NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyConstRefAssignable, ConstRefAssignable);

	}
#undef NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN

	template <typename DataType>
	concept IsTriviallyCompareable = IsBasicType<DataType> ||
		impl::customCheckIfIsTriviallyCompareable<DataType>();

	template <typename DataType>
	concept IsTriviallyRelocatable = (std::is_trivially_copyable_v<DataType> && std::is_trivially_destructible_v<DataType>) ||
		IsBasicType<DataType> || impl::customCheckIfIsTriviallyRelocatable<DataType>();

	template <typename DataType>
	concept IsTriviallyDefaultConstructible = std::is_trivially_default_constructible_v<DataType> ||
		impl::customCheckIfIsTriviallyDefaultConstructible<DataType>();

	template <typename DataType>
	concept IsTriviallyDestructible = std::is_trivially_destructible_v<DataType> ||
		impl::customCheckIfIsTriviallyDestructible<DataType>();

	template <typename DataType>
	concept IsTriviallyMoveConstructible = std::is_trivially_move_constructible_v<DataType> ||
		impl::customCheckIfIsTriviallyMoveConstructible<DataType>();

	template <typename DataType>
	concept IsTriviallyConstRefConstructible = std::is_trivially_constructible_v<DataType, const DataType&> ||
		impl::customCheckIfIsTriviallyConstRefConstructible<DataType>();

	template <typename DataType>
	concept IsTriviallyMoveAssignable = std::is_trivially_move_assignable_v<DataType> ||
		impl::customCheckIfIsTriviallyMoveAssignable<DataType>();

	template <typename DataType>
	concept IsTriviallyConstRefAssignable = std::is_trivially_assignable_v<DataType, const DataType&> ||
		impl::customCheckIfIsTriviallyConstRefAssignable<DataType>();

	template <typename Type>
	concept MemcopyConstructible = IsTriviallyDefaultConstructible<Type> && IsTriviallyRelocatable<Type>;

	template <typename Src, typename Dst, typename SrcRef>
	concept MemcopyConstructibleSrcDst =
		!std::is_same_v<Src, void> &&
		!std::is_same_v<Dst, void> &&
		!std::is_same_v<SrcRef, void> &&
		(
			(std::is_same_v<std::decay_t<Src>, std::decay_t<Dst>>&& MemcopyConstructible<Src>) ||
			(sizeof(Src) == sizeof(Dst) && std::is_trivially_constructible_v<Dst, SrcRef>)
			);

	template<typename Src, typename Dst, typename SrcRef, typename DstRef>
	concept MemcopyAssignableSrcDst =
		(std::is_same_v<Src, Dst> && MemcopyConstructible<Src>) ||
		(sizeof(Src) == sizeof(Dst) && std::is_trivially_assignable_v<DstRef, SrcRef>);

	template <typename Type>
	concept MemcpyCompareable = IsTriviallyCompareable<Type>;

	template<typename Src, typename Dst>
	concept MemcpyCompareableSrcDst = (std::is_same_v<Src, Dst> && MemcpyCompareable<Src>);

	template <typename From, typename To>
	concept IsPolymorphicCastable = std::is_base_of_v<From, To> || std::is_convertible_v<From*, To*>;

	namespace impl {
		template<typename DataType, template<typename...> typename Primary>
		struct IsSpecialization : FalseType {};

		template<template<typename...> typename Primary, typename... Args>
		struct IsSpecialization<Primary<Args...>, Primary> : TrueType {};

		template<typename DataType, template<typename...> typename Primary>
		inline constexpr bool IsSpecializationV = IsSpecialization<DataType, Primary>::value;
	}

	template<typename Test, template<typename...> typename SpecializationType>
	concept IsSpecialization = impl::IsSpecializationV<Test, SpecializationType>;

	template<typename... ArgTypes>
	concept IsSameByteSize = (sizeof(ArgTypes) == ...);

	/*
	#define NATL_OPERATOR_TESTABLE_CONCEPT(Name, Operator) \
		template<typename LhsDataType, typename RhsDataType = LhsDataType> \
		concept IsOneWay##Name##Testable = requires(LhsDataType lhs, RhsDataType rhs) { \
			{ lhs ##Operator rhs } -> ConvertibleTo<bool>; \
		}; \
		template<typename LhsDataType, typename RhsDataType = LhsDataType> \
		concept Is##Name##Testable = requires(LhsDataType lhs, RhsDataType rhs) { \
			{ lhs ##Operator rhs } -> ConvertibleTo<bool>; \
			{ rhs ##Operator lhs } -> ConvertibleTo<bool>; \
		};

		NATL_OPERATOR_TESTABLE_CONCEPT(Equal, ==)
		NATL_OPERATOR_TESTABLE_CONCEPT(NotEqual, !=)
		NATL_OPERATOR_TESTABLE_CONCEPT(LessThan, <)
		NATL_OPERATOR_TESTABLE_CONCEPT(LessThanEqual, <=)
		NATL_OPERATOR_TESTABLE_CONCEPT(GreaterThan, >)
		NATL_OPERATOR_TESTABLE_CONCEPT(GreaterThanEqual, >=)
		NATL_OPERATOR_TESTABLE_CONCEPT(ThreeWay, <=>)
	#undef NATL_OPERATOR_TESTABLE_CONCEPT
	*/

	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs == rhs } -> ConvertibleTo<bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs == rhs } -> ConvertibleTo<bool>; 
		{ rhs == lhs } -> ConvertibleTo<bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayNotEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs != rhs } -> ConvertibleTo<bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsNotEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs != rhs } -> ConvertibleTo<bool>; 
		{ rhs != lhs } -> ConvertibleTo<bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayLessThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs < rhs } -> ConvertibleTo<bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsLessThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs < rhs } -> ConvertibleTo<bool>; 
		{ rhs < lhs } -> ConvertibleTo<bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayLessThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs <= rhs } -> ConvertibleTo<bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsLessThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs <= rhs } -> ConvertibleTo<bool>; 
		{ rhs <= lhs } -> ConvertibleTo<bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayGreaterThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs > rhs } -> ConvertibleTo<bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsGreaterThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs > rhs } -> ConvertibleTo<bool>; 
		{ rhs > lhs } -> ConvertibleTo<bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayGreaterThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs >= rhs } -> ConvertibleTo<bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsGreaterThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs >= rhs } -> ConvertibleTo<bool>; 
		{ rhs >= lhs } -> ConvertibleTo<bool>;
	};
}