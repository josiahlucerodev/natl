#pragma once

//std
#include <concepts>
#include <type_traits>

//own
#include "basicTypes.h"
#include "compilerDependent.h"

//interface
namespace natl {
	template<typename Type>
	struct TypeArg {};

	//constant 
	template<typename DataType, DataType Value>
	struct IntegralConstant {
		constexpr static DataType value = Value;
		using value_type = DataType;
		using type = IntegralConstant;
		constexpr operator value_type() const noexcept { return value; }
		constexpr value_type operator()() const noexcept { return value; }
	};

	template<Bool BoolValue>
	using BoolConstant = IntegralConstant<Bool, BoolValue>;

	using TrueType = BoolConstant<true>;
	using FalseType = BoolConstant<false>;

	//composition 
	template<Bool BoolValue, typename Type, typename OtherType>
	struct Conditional{ using type = Type; };
	template<typename Type, typename OtherType>
	struct Conditional<false, Type, OtherType> { using type = OtherType; };
	template<Bool value, typename Type, typename OtherType >
	using ConditionalT = typename Conditional<value, Type, OtherType>::type;

	template<typename... Types> struct Conjunction : TrueType {};
	template<typename Type> struct Conjunction<Type> : Type {};
	template<typename Type, typename... OtherTypes> 
	struct Conjunction<Type, OtherTypes...> : ConditionalT<Bool(Type::value), Conjunction<OtherTypes...>, Type> {};
	template<typename... Types> constexpr inline Bool ConjunctionV = Conjunction<Types...>::value;

	template<typename... Types> struct Disjunction : FalseType {};
	template<typename Type> struct Disjunction<Type> : Type {};
	template<typename Type, typename... OtherTypes>
	struct Disjunction<Type, OtherTypes...> : ConditionalT<Bool(Type::value), Type, Disjunction<OtherTypes...>> {};
	template<typename... Types> constexpr inline Bool DisjunctionV = Disjunction<Types...>::value;

	template<typename Type> struct Negation : BoolConstant<!Bool(Type::value)> {};
	template<typename Type> constexpr inline Bool NegationV = Negation<Type>::value;

	template<typename Type> struct TypeIdentity { using type = Type; };
	template<typename Type> using TypeIdentityT = typename TypeIdentity<Type>::type;

	//underlying 
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

	//primary 
	template<class Type> struct IsUnion : BoolConstant<std::is_union_v<Type>> {};
	template<class Type> inline constexpr Bool IsUnionV = IsUnion<Type>::value;

	template<class Type> struct IsNotUnion : Negation<IsUnion<Type>> {};
	template<class Type> inline constexpr Bool IsNotUnionV = IsNotUnion<Type>::value;


	namespace impl {
		template<typename Type> IntegralConstant<Bool, IsNotUnionV<Type>> isClassTest(int Type::*);
		template<typename> FalseType isClassTest(...);
	}
	template<typename Type> struct IsClass : decltype(impl::isClassTest<Type>(0)) {};
	template<typename Type> constexpr inline Bool IsClassV = IsClass<Type>::value;

	template<typename Type> struct IsNotClass : Negation<IsClass<Type>> {};
	template<typename Type> constexpr inline Bool IsNotClassV = IsNotClass<Type>::value;

	template<typename Type> struct IsEnum : BoolConstant<std::is_enum_v<Type>> {};
	template<typename Type> inline constexpr Bool IsEnumV = IsEnum<Type>::value;

	template<typename Type> struct IsNotEnum : Negation<IsEnum<Type>> {};
	template<typename Type> constexpr inline Bool IsNotEnumV = IsNotEnum<Type>::value;

	template<typename FunctionSignature> struct IsFunction : FalseType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunction<ReturnType(ArgTypes...)> : TrueType {};
	template<typename FunctionSignature> constexpr inline Bool IsFunctionV = IsFunction<FunctionSignature>::value;

	template<typename Type> struct IsPointer: FalseType {};
	template<typename Type> struct IsPointer<Type*> : TrueType {};
	template<typename Type> struct IsPointer<Type* const> : TrueType {};
	template<typename Type> struct IsPointer<Type* volatile> : TrueType {};
	template<typename Type> struct IsPointer<Type* const volatile> : TrueType {};
	template<typename Type> constexpr inline Bool IsPointerV = IsPointer<Type>::value;

	template<class DataType> struct IsArrayType : FalseType {};
	template<class DataType> struct IsArrayType<DataType[]> : TrueType {};
	template<class DataType, Size Number> struct IsArrayType<DataType[Number]> : TrueType {};
	template<class DataType> constexpr inline Bool IsArray = IsArrayType<DataType>::value;

	//relationships
	template<typename Type, typename OtherType> struct IsSame : FalseType {};
	template<typename Type> struct IsSame<Type, Type> : TrueType {};
	template<typename Type, typename OtherType> constexpr inline Bool IsSameV = IsSame<Type, OtherType>::value;

	template<typename Type, typename OtherType> struct IsNotSame : TrueType {};
	template<typename Type> struct IsNotSame<Type, Type> : FalseType {};
	template<typename Type, typename OtherType> constexpr inline Bool IsNotSameV = IsNotSame<Type, OtherType>::value;

	namespace impl {
		template<class Type, class OtherType> concept SameAsImpl = IsSameV<Type, OtherType>;
	}
	template<class Type, class OtherType>
	concept SameAs = impl::SameAsImpl<Type, OtherType>&& impl::SameAsImpl<Type, OtherType>;

	template<typename... Types> struct IsSameByteSize : BoolConstant<(sizeof(Types) == ...)> {};
	template<typename... Types> constexpr inline Bool IsSameByteSizeV = IsSameByteSize<Types...>::value;

	//basic type properties
	template<typename Type> struct IsConst : FalseType {};
	template<typename Type> struct IsConst<const Type> : TrueType {};
	template<typename Type> constexpr inline Bool IsConstV = IsConst<Type>::value;

	template<typename Type> struct IsNotConst : TrueType {};
	template<typename Type> struct IsNotConst<const Type> : FalseType {};
	template<typename Type> constexpr inline Bool IsNotConstV = IsNotConst<Type>::value;

	template<typename DataType> struct RemoveConst { using type = DataType; };
	template<typename DataType> struct RemoveConst<const DataType> { using type = DataType; };
	template<typename DataType> using RemoveConstT = typename RemoveConst<DataType>::type;

	template<typename DataType> struct AddConst { using type = const DataType; };
	template<typename DataType> using AddConstT = AddConst<DataType>::type;

	template<typename DataType> struct IsVolatile : FalseType {};
	template<typename DataType> struct IsVolatile<volatile  DataType> : TrueType {};
	template<typename DataType> constexpr inline Bool IsVolatileV = IsVolatile<DataType>::value;

	template<typename DataType> struct RemoveVolatile { using type = DataType; };
	template<typename DataType> struct RemoveVolatile<volatile DataType> { using type = DataType; };
	template<typename DataType> using RemoveVolatileT = typename RemoveVolatile<DataType>::type;

	template<typename DataType> struct AddVolatile { using type = volatile DataType; };
	template<typename DataType> using AddVolatileT = AddVolatile<DataType>::type;

	template<typename DataType> struct RemoveCV { using type = DataType; };
	template<typename DataType> struct RemoveCV<const DataType> { using type = DataType; };
	template<typename DataType> struct RemoveCV<volatile DataType> { using type = DataType; };
	template<typename DataType> struct RemoveCV<const volatile DataType> { using type = DataType; };
	template<typename DataType> using RemoveCVT = typename RemoveCV<DataType>::type;

	template<typename DataType> struct AddCV { using type = const volatile DataType; };
	template<typename DataType> using AddCVT = AddCV<DataType>::type;

	template<typename Type> struct IsEmpty { constexpr static Bool value = std::is_empty_v<Type>; };
	template<typename Type> constexpr inline Bool IsEmptyV = IsEmpty<Type>::value;
	template<typename Type> struct IsNotEmpty { constexpr static Bool value = !IsEmptyV<Type>; };
	template<typename Type> constexpr inline Bool IsNotEmptyV = IsNotEmpty<Type>::value;

	template<typename DataType> struct IsVoid : IsSame<void, RemoveCVT<DataType>> {};
	template<typename DataType> constexpr inline Bool IsVoidV = IsVoid<DataType>::value;

	template<typename... AnyTypes> struct MakeVoid { using type = void; };
	template<typename... AnyTypes> using MakeVoidT = MakeVoid<AnyTypes...>::type;

	namespace impl {
		template<typename DataType, template<typename...> typename Primary>
		struct IsSpecialization : FalseType {};

		template<template<typename...> typename Primary, typename... Args>
		struct IsSpecialization<Primary<Args...>, Primary> : TrueType {};

		template<typename DataType, template<typename...> typename Primary>
		inline constexpr Bool IsSpecializationV = IsSpecialization<DataType, Primary>::value;
	}

	template<typename Test, template<typename...> typename SpecializationType>
	concept IsSpecialization = impl::IsSpecializationV<Test, SpecializationType>;

	//reference 
	template<typename DataType> struct RemoveReference { using type = DataType; };
	template<typename DataType> struct RemoveReference<DataType&> { using type = DataType; };
	template<typename DataType> struct RemoveReference<DataType&&> { using type = DataType; };
	template<typename DataType>
	using RemoveReferenceT = typename RemoveReference<DataType>::type;

	template <class DataType, class = void>
	struct UniversalAddReference {
		using lvalue = DataType;
		using rvalue = DataType;
	};
	template <class DataType>
	struct UniversalAddReference<DataType, MakeVoidT<DataType&>> {
		using lvalue = DataType&;
		using rvalue = DataType&&;
	};
	template<typename DataType> struct AddLValueReference {
		using type = UniversalAddReference<DataType>::lvalue;
	};
	template<typename DataType> struct AddRValueReference {
		using type = UniversalAddReference<DataType>::rvalue;
	};

	template<typename DataType>
	using AddLValueReferenceT = typename AddLValueReference<DataType>::type;
	template<typename DataType>
	using AddRValueReferenceT = typename AddRValueReference<DataType>::type;
	
	template<typename DataType> struct IsLValueReference : FalseType {};
	template<typename DataType> struct IsLValueReference<DataType&> : TrueType {};
	template<typename DataType> constexpr inline Bool IsLValueReferenceV = IsLValueReference<DataType>::value;
	template<typename DataType> struct IsRValueReference : FalseType {};
	template<typename DataType> struct IsRValueReference<DataType&&> : TrueType {};
	template<typename DataType> constexpr inline Bool IsRValueReferenceV = IsRValueReference<DataType>::value;

	template<typename DataType> struct IsReferenceType : FalseType {};
	template<typename DataType> struct IsReferenceType<DataType&> : TrueType {};
	template<typename DataType> struct IsReferenceType<DataType&&> : TrueType {};
	template<typename DataType> constexpr inline Bool IsReference = IsReferenceType<TrueType>::value;

	//pointers
	template<typename DataType> struct RemovePointer { using type = DataType; };
	template<typename DataType> struct RemovePointer<DataType*> { using type = DataType; };
	template<typename DataType> struct RemovePointer<DataType* const> { using type = DataType; };
	template<typename DataType> struct RemovePointer<DataType* volatile> { using type = DataType; };
	template<typename DataType> struct RemovePointer<DataType* const volatile> { using type = DataType; };
	template<typename DataType> using RemovePointerT = RemovePointer<DataType>::type;

	namespace impl {
		template<typename DataType> auto tryAddPointer(int) -> TypeIdentity<RemoveReferenceT<DataType>*> {};
		template<typename DataType> auto tryAddPointer(...) -> TypeIdentity<DataType> {};
	}
	template<typename DataType> struct AddPointer : decltype(impl::tryAddPointer<DataType>(0)) {};
	template<typename DataType> using AddPointerT = typename AddPointer<DataType>::type;

	//misc op
	template<typename DataType> AddRValueReferenceT<DataType> declval() noexcept {
		//static_assert(false, "natl: declval not allowed in an evaluated context");
	}
	template <typename DataType>
	constexpr RemoveReferenceT<DataType>&& move(DataType&& arg) noexcept {
		return static_cast<RemoveReferenceT<DataType>&&>(arg);
	}
	template <typename DataType>
	constexpr DataType&& forward(RemoveReferenceT<DataType>& arg) noexcept {
		return static_cast<DataType&&>(arg);
	}
	template <typename DataType>
	constexpr DataType&& forward(RemoveReferenceT<DataType>&& arg) noexcept {
		static_assert(!IsLValueReferenceV<DataType>, "natl: template argument substituting DataType is an lvalue reference type");
		return static_cast<DataType&&>(arg);
	}
	template <typename DataType>
	constexpr void swap(DataType& a, DataType& b) noexcept {
		DataType temp = move(a);
		a = move(b);
		b = move(temp);
	}

	template<class DataType> struct RemoveExtentType { using type = DataType; };
	template<class DataType> struct RemoveExtentType<DataType[]> { using type = DataType; };
	template<class DataType, std::size_t Number> struct RemoveExtentType<DataType[Number]> { using type = DataType; };
	template<class DataType> using RemoveExtent = RemoveExtentType<DataType>::type;

	template<class Type> struct Decay {
	private:
		using TypeNoReference = RemoveReferenceT<Type>;
	public:
		using type = ConditionalT<
			IsArray<TypeNoReference>,
			AddPointerT<RemoveExtent<TypeNoReference>>,
			ConditionalT<
				IsFunctionV<TypeNoReference>,
				AddPointerT<TypeNoReference>,
				RemoveCVT<TypeNoReference>
			>
		>;
	};
	template<class Type> using DecayT = Decay<Type>::type;

	//convert
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
		(decltype(impl::testReturnable<To>(0))::value && decltype(impl::testImplicitlyConvertible<From, To>(0))::value) ||
		(IsVoidV<From> && IsVoidV<To>)
	> {};

	template<typename From, typename To>
	struct IsNoThrowConvertibleType : Conjunction<IsVoid<From>, IsVoid<To>> {};
	template<typename From, typename To>
		requires (
			requires {
				static_cast<To(*)()>(nullptr);
				{ declval<void(&)(To) noexcept>()(declval<From>()) } noexcept;
		}
	)
	struct IsNoThrowConvertibleType<From, To> : TrueType {};
	template<typename From, typename To>
	constexpr inline Bool IsConvertible = IsConvertibleType<From, To>::value;
	template<typename From, typename To>
	constexpr inline Bool IsNothrowConvertible = IsNoThrowConvertibleType<From, To>::value;

	template<typename From, typename To>
	concept ConvertibleTo = IsConvertible<From, To> && requires { static_cast<To>(declval<From>()); };

	//common
	template<typename... Types> struct CommonReference {};
	template<typename Type> struct CommonReference<Type> { using type = Type; };

	template<typename Type, typename OtherType>
	struct CommonReferenceTwo {
	private:
		template<typename LocalType, typename LocalOtherType>
		static auto commonReferenceTest(int) -> decltype(true ? declval<LocalType>() : declval<LocalOtherType>());
		template<typename, typename>
		static auto commonReferenceTest(...) -> void;
	public:
		using type = decltype(commonReferenceTest<Type, OtherType>(0));
	};

	template<typename FirstType, typename SecondType>
	struct CommonReference<FirstType, SecondType> {
		using type = CommonReferenceTwo<FirstType, SecondType>;
	};
	template<typename FirstType, typename SecondType, typename... Rest>
	struct CommonReference<FirstType, SecondType, Rest...> {
		using type = CommonReferenceTwo<typename CommonReferenceTwo<FirstType, SecondType>::type, Rest...>;
	};

	template<typename... Types> using CommonReferenceT = CommonReference<Types...>::type;

	template<class Type, class OtherType>
	concept CommonReferenceWith =
		SameAs<CommonReferenceT<Type, OtherType>, CommonReferenceT<OtherType, Type>>&&
		ConvertibleTo<Type, CommonReferenceT<Type, OtherType>>&&
		ConvertibleTo<OtherType, CommonReferenceT<Type, OtherType>>;

	//assing
	template<typename LhsDataType, class RhsDataType>
	concept AssignableFrom =
		IsLValueReferenceV<LhsDataType> &&
		CommonReferenceWith<
		const RemoveReferenceT<LhsDataType>&,
		const RemoveReferenceT<RhsDataType>&>&&
		requires(LhsDataType lhs, RhsDataType&& rhs) {
			{ lhs = natl::forward<RhsDataType>(rhs) } -> SameAs<RhsDataType>;
	};

	//redumentary 
	namespace impl {
		template<typename Type> struct IsFundamentalIterger : FalseType {};
		template<> struct IsFundamentalIterger<i8> : TrueType {};
		template<> struct IsFundamentalIterger<i16> : TrueType {};
		template<> struct IsFundamentalIterger<i32> : TrueType {};
		template<> struct IsFundamentalIterger<i64> : TrueType {};
		template<> struct IsFundamentalIterger<ui8> : TrueType {};
		template<> struct IsFundamentalIterger<ui16> : TrueType {};
		template<> struct IsFundamentalIterger<ui32> : TrueType {};
		template<> struct IsFundamentalIterger<ui64> : TrueType {};

#ifdef NATL_COMPILER_EMSCRIPTEN
		template<> struct IsFundamentalIterger<Size> : TrueType {};
#endif // NATL_COMPILER_EMSCRIPTEN
	}
	template<typename Type> struct IsFundamentalIterger : impl::IsFundamentalIterger<DecayT<Type>> {};
	template<typename Type> constexpr inline Bool IsFundamentalItergerV = IsFundamentalIterger<Type>::value;

	namespace impl {
		template<typename Type> struct IsFundamentalSignedIterger : FalseType {};
		template<> struct IsFundamentalSignedIterger<i8> : TrueType {};
		template<> struct IsFundamentalSignedIterger<i16> : TrueType {};
		template<> struct IsFundamentalSignedIterger<i32> : TrueType {};
		template<> struct IsFundamentalSignedIterger<i64> : TrueType {};
	}
	template<typename Type> struct IsFundamentalSignedIterger : impl::IsFundamentalSignedIterger<DecayT<Type>> {};
	template<typename Type> constexpr inline Bool IsFundamentalSignedItergerV = IsFundamentalSignedIterger<Type>::value;

	namespace impl {
		template<typename Type> struct IsFundamentalUnsignedIterger : FalseType {};
		template<> struct IsFundamentalUnsignedIterger<ui8> : TrueType {};
		template<> struct IsFundamentalUnsignedIterger<ui16> : TrueType {};
		template<> struct IsFundamentalUnsignedIterger<ui32> : TrueType {};
		template<> struct IsFundamentalUnsignedIterger<ui64> : TrueType {};
#ifdef NATL_COMPILER_EMSCRIPTEN
		template<> struct IsFundamentalUnsignedIterger<Size> : TrueType {};
#endif // NATL_COMPILER_EMSCRIPTEN
	}
	template<typename Type> struct IsFundamentalUnsignedIterger : impl::IsFundamentalUnsignedIterger<DecayT<Type>> {};
	template<typename Type> constexpr inline Bool IsFundamentalUnsignedItergerV = IsFundamentalUnsignedIterger<Type>::value;

	template<typename Type> struct IsFloatingPoint : BoolConstant<SameAs<DecayT<Type>, f32> || SameAs<DecayT<Type>, f64>> {};
	template<typename Type> constexpr inline Bool IsFloatingPointV = IsFloatingPoint<Type>::value;

	template<typename Type> struct IsFundamentalNumeric : BoolConstant<IsFundamentalItergerV<Type> || IsFloatingPointV<Type>> {};
	template<typename Type> constexpr inline Bool IsFundamentalNumericV = IsFundamentalNumeric<Type>::value;

	namespace impl {
		template<typename Type> struct IsFundamentalCharacter : FalseType {};
		template<> struct IsFundamentalCharacter<Ascii> : TrueType {};
		template<> struct IsFundamentalCharacter<Utf8> : TrueType {};
		template<> struct IsFundamentalCharacter<Utf16> : TrueType {};
		template<> struct IsFundamentalCharacter<Utf32> : TrueType {};
	}
	template<typename Type> struct IsFundamentalCharacter : impl::IsFundamentalCharacter<DecayT<Type>> {};
	template<typename Type> constexpr inline Bool IsFundamentalCharacterV = IsFundamentalCharacter<Type>::value;

	template<typename Type> struct IsBasic : BoolConstant<IsFundamentalItergerV<Type> || IsFloatingPointV<Type> || IsFundamentalCharacterV<Type> || IsPointerV<Type>> {};
	template<typename Type> constexpr inline Bool IsBasicV = IsBasic<Type>::value;

	//type operations
	template <typename DataType>
	concept IsCopyConstructible = std::is_copy_constructible_v<std::decay_t<DataType>>;

	template <typename DataType>
	concept IsCopyAssignable = std::is_copy_assignable_v<std::decay_t<DataType>>;

	template <typename DataType>
	concept IsMoveConstructible = std::is_move_constructible_v<std::decay_t<DataType>>;

	namespace impl {
#define NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(conceptName, memberName) \
        template<typename DataType> \
        consteval Bool customCheckIf##conceptName() noexcept { \
            if constexpr (requires() { { DataType::trivially##memberName } -> std::convertible_to<Bool>; }) { \
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
	concept IsTriviallyCompareable = IsBasicV<DataType> ||
		impl::customCheckIfIsTriviallyCompareable<DataType>();

	template <typename DataType>
	concept IsTriviallyRelocatable = (std::is_trivially_copyable_v<DataType> && std::is_trivially_destructible_v<DataType>) ||
		IsBasicV<DataType> || impl::customCheckIfIsTriviallyRelocatable<DataType>();

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

	//memcopy
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
	
	//cast
	template <typename From, typename To>
	concept IsPolymorphicCastable = std::is_base_of_v<From, To> || std::is_convertible_v<From*, To*>;


	//compare
	/*
	#define NATL_OPERATOR_TESTABLE_CONCEPT(Name, Operator) \
		template<typename LhsDataType, typename RhsDataType = LhsDataType> \
		concept IsOneWay##Name##Testable = requires(LhsDataType lhs, RhsDataType rhs) { \
			{ lhs ##Operator rhs } -> ConvertibleTo<Bool>; \
		}; \
		template<typename LhsDataType, typename RhsDataType = LhsDataType> \
		concept Is##Name##Testable = requires(LhsDataType lhs, RhsDataType rhs) { \
			{ lhs ##Operator rhs } -> ConvertibleTo<Bool>; \
			{ rhs ##Operator lhs } -> ConvertibleTo<Bool>; \
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
		{ lhs == rhs } -> ConvertibleTo<Bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs == rhs } -> ConvertibleTo<Bool>; 
		{ rhs == lhs } -> ConvertibleTo<Bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayNotEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs != rhs } -> ConvertibleTo<Bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsNotEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs != rhs } -> ConvertibleTo<Bool>; 
		{ rhs != lhs } -> ConvertibleTo<Bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayLessThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs < rhs } -> ConvertibleTo<Bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsLessThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs < rhs } -> ConvertibleTo<Bool>; 
		{ rhs < lhs } -> ConvertibleTo<Bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayLessThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs <= rhs } -> ConvertibleTo<Bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsLessThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs <= rhs } -> ConvertibleTo<Bool>; 
		{ rhs <= lhs } -> ConvertibleTo<Bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayGreaterThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs > rhs } -> ConvertibleTo<Bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsGreaterThanTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs > rhs } -> ConvertibleTo<Bool>; 
		{ rhs > lhs } -> ConvertibleTo<Bool>;
	};
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsOneWayGreaterThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs >= rhs } -> ConvertibleTo<Bool>;
	}; 
	template<typename LhsDataType, typename RhsDataType = LhsDataType> concept IsGreaterThanEqualTestable = requires(LhsDataType lhs, RhsDataType rhs) {
		{ lhs >= rhs } -> ConvertibleTo<Bool>; 
		{ rhs >= lhs } -> ConvertibleTo<Bool>;
	};
}