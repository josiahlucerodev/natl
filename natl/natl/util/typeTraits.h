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
	struct ConditionalT{ using type = Type; };
	template<typename Type, typename OtherType>
	struct ConditionalT<false, Type, OtherType> { using type = OtherType; };
	template<Bool value, typename Type, typename OtherType >
	using Conditional = typename ConditionalT<value, Type, OtherType>::type;

	template<typename... Types> struct ConjunctionV : TrueType {};
	template<typename Type> struct ConjunctionV<Type> : Type {};
	template<typename Type, typename... OtherTypes> 
	struct ConjunctionV<Type, OtherTypes...> : Conditional<Bool(Type::value), ConjunctionV<OtherTypes...>, Type> {};
	template<typename... Types> constexpr inline Bool Conjunction = ConjunctionV<Types...>::value;

	template<typename... Types> struct DisjunctionV : FalseType {};
	template<typename Type> struct DisjunctionV<Type> : Type {};
	template<typename Type, typename... OtherTypes>
	struct DisjunctionV<Type, OtherTypes...> : Conditional<Bool(Type::value), Type, DisjunctionV<OtherTypes...>> {};
	template<typename... Types> constexpr inline Bool Disjunction = DisjunctionV<Types...>::value;

	template<typename Type> struct NegationV : BoolConstant<!Bool(Type::value)> {};
	template<typename Type> constexpr inline Bool Negation = NegationV<Type>::value;

	template<typename Type> struct TypeIdentityT { using type = Type; };
	template<typename Type> using TypeIdentity = typename TypeIdentityT<Type>::type;

	template<auto Value> struct ValueIdentityV { 
		using type = decltype(Value); 
		constexpr static type value = Value;
	};
	template<auto Value> constexpr inline auto ValueIdentity = ValueIdentityV<Value>::value;

	//primary 
	template<class Type> struct IsUnionV : BoolConstant<std::is_union_v<Type>> {};
	template<class Type> inline constexpr Bool IsUnion = IsUnionV<Type>::value;
	template<class Type> concept IsUnionC = IsUnion<Type>;

	template<class Type> struct IsNotUnionV : NegationV<IsUnionV<Type>> {};
	template<class Type> inline constexpr Bool IsNotUnion = IsNotUnionV<Type>::value;
	template<class Type> concept IsNotUnionC = IsNotUnion<Type>;

	namespace impl {
		template<typename Type> IntegralConstant<Bool, IsNotUnion<Type>> isClassTest(int Type::*);
		template<typename> FalseType isClassTest(...);
	}
	template<typename Type> struct IsClassV : decltype(impl::isClassTest<Type>(0)) {};
	template<typename Type> constexpr inline Bool IsClass = IsClassV<Type>::value;
	template<typename Type> concept IsClassC = IsClass<Type>;

	template<typename Type> struct IsNotClassV : NegationV<IsClassV<Type>> {};
	template<typename Type> constexpr inline Bool IsNotClass = IsNotClassV<Type>::value;
	template<typename Type> concept IsNotClassC = IsNotClass<Type>;

	template<typename Type> struct IsEnumV : BoolConstant<std::is_enum_v<Type>> {};
	template<typename Type> inline constexpr Bool IsEnum = IsEnumV<Type>::value;
	template<typename Type> concept IsEnumC = IsEnum<Type>;

	template<typename Type> struct IsNotEnumV : NegationV<IsEnumV<Type>> {};
	template<typename Type> constexpr inline Bool IsNotEnum = IsNotEnumV<Type>::value;
	template<typename Type> concept IsNotEnumC = IsNotEnum<Type>;

	template<typename FunctionSignature> struct IsFunctionV : FalseType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...)> : TrueType {};
	template<typename FunctionSignature> constexpr inline Bool IsFunction = IsFunctionV<FunctionSignature>::value;
	template<typename FunctionSignature> concept IsFunctionC = IsFunction<FunctionSignature>;

	template<typename Type> struct IsPointerV: FalseType {};
	template<typename Type> struct IsPointerV<Type*> : TrueType {};
	template<typename Type> struct IsPointerV<Type* const> : TrueType {};
	template<typename Type> struct IsPointerV<Type* volatile> : TrueType {};
	template<typename Type> struct IsPointerV<Type* const volatile> : TrueType {};
	template<typename Type> constexpr inline Bool IsPointer = IsPointerV<Type>::value;
	template<typename Type> concept IsPointerC = IsPointer<Type>;

	template<typename Type> struct IsArrayT : FalseType {};
	template<typename Type> struct IsArrayT<Type[]> : TrueType {};
	template<typename Type, Size Number> struct IsArrayT<Type[Number]> : TrueType {};
	template<typename Type> constexpr inline Bool IsArray = IsArrayT<Type>::value;
	template<typename Type> concept IsArrayC = IsArray<Type>;

	//underlying
	template <typename EnumType>
	using UnderlyingTypeT = std::underlying_type<EnumType>;
	template <typename EnumType>
	using UnderlyingType = UnderlyingTypeT<EnumType>::type;

	template <typename EnumType>
	constexpr UnderlyingType<EnumType> toUnderlying(const EnumType value) {
		return static_cast<UnderlyingType<EnumType>>(value);
	}
	template <typename EnumType>
	constexpr EnumType fromUnderlying(const UnderlyingType<EnumType> value) {
		return static_cast<EnumType>(value);
	}
	template<typename EnumType>
		requires(IsEnum<EnumType>)
	constexpr Size enumToSize(EnumType value) noexcept {
		return static_cast<Size>(toUnderlying<EnumType>(value));
	}

	//relationships
	template<typename Type, typename OtherType> struct IsSameT : FalseType {};
	template<typename Type> struct IsSameT<Type, Type> : TrueType {};
	template<typename Type, typename OtherType> constexpr inline Bool IsSame = IsSameT<Type, OtherType>::value;
	template<typename Type, typename OtherType> concept IsSameC = IsSame<Type, OtherType>;

	template<typename Type, typename OtherType> struct IsNotSameT : TrueType {};
	template<typename Type> struct IsNotSameT<Type, Type> : FalseType {};
	template<typename Type, typename OtherType> constexpr inline Bool IsNotSame = IsNotSameT<Type, OtherType>::value;
	template<typename Type, typename OtherType> concept IsNotSameC = IsNotSame<Type, OtherType>;

	namespace impl {
		template<class Type, class OtherType> concept SameAsImpl = IsSame<Type, OtherType>;
	}
	template<class Type, class OtherType>
	concept SameAs = impl::SameAsImpl<Type, OtherType>&& impl::SameAsImpl<Type, OtherType>;

	template<typename... Types> struct IsSameByteSizeV : BoolConstant<(sizeof(Types) == ...)> {};
	template<typename... Types> constexpr inline Bool IsSameByteSize = IsSameByteSizeV<Types...>::value;
	template<typename... Types> concept IsSameByteSizeC = IsSameByteSize<Types...>;

	//basic type properties
	template<typename Type> struct IsConstV : FalseType {};
	template<typename Type> struct IsConstV<const Type> : TrueType {};
	template<typename Type> constexpr inline Bool IsConst = IsConstV<Type>::value;
	template<typename Type> concept IsConstC = IsConst<Type>;

	template<typename Type> struct IsNotConstV : TrueType {};
	template<typename Type> struct IsNotConstV<const Type> : FalseType {};
	template<typename Type> constexpr inline Bool IsNotConst = IsNotConstV<Type>::value;
	template<typename Type> concept IsNotConstC = IsNotConst<Type>;

	template<typename DataType> struct RemoveConstT { using type = DataType; };
	template<typename DataType> struct RemoveConstT<const DataType> { using type = DataType; };
	template<typename DataType> using RemoveConst = typename RemoveConstT<DataType>::type;

	template<typename DataType> struct AddConstT { using type = const DataType; };
	template<typename DataType> using AddConst = AddConstT<DataType>::type;

	template<typename DataType> struct IsVolatileV : FalseType {};
	template<typename DataType> struct IsVolatileV<volatile  DataType> : TrueType {};
	template<typename DataType> constexpr inline Bool IsVolatile = IsVolatileV<DataType>::value;
	template<typename DataType> concept IsVolatileC = IsVolatile<DataType>;

	template<typename DataType> struct RemoveVolatileT { using type = DataType; };
	template<typename DataType> struct RemoveVolatileT<volatile DataType> { using type = DataType; };
	template<typename DataType> using RemoveVolatile = typename RemoveVolatileT<DataType>::type;

	template<typename DataType> struct AddVolatileT { using type = volatile DataType; };
	template<typename DataType> using AddVolatile = AddVolatileT<DataType>::type;

	template<typename DataType> struct RemoveConctVolatileT { using type = DataType; };
	template<typename DataType> struct RemoveConctVolatileT<const DataType> { using type = DataType; };
	template<typename DataType> struct RemoveConctVolatileT<volatile DataType> { using type = DataType; };
	template<typename DataType> struct RemoveConctVolatileT<const volatile DataType> { using type = DataType; };
	template<typename DataType> using RemoveConctVolatile = typename RemoveConctVolatileT<DataType>::type;

	template<typename DataType> struct AddConctVolatileT { using type = const volatile DataType; };
	template<typename DataType> using AddConctVolatile = AddConctVolatileT<DataType>::type;

	template<typename Type> struct IsEmptyV { constexpr static Bool value = std::is_empty_v<Type>; };
	template<typename Type> constexpr inline Bool IsEmpty = IsEmptyV<Type>::value;
	template<typename Type> concept IsEmptyC = IsEmpty<Type>;

	template<typename Type> struct IsNotEmptyV { constexpr static Bool value = !IsEmpty<Type>; };
	template<typename Type> constexpr inline Bool IsNotEmpty = IsNotEmptyV<Type>::value;
	template<typename Type> concept IsNotEmptyC = IsNotEmpty<Type>;

	template<typename DataType> struct IsVoidV : IsSameT<void, RemoveConctVolatile<DataType>> {};
	template<typename DataType> constexpr inline Bool IsVoid = IsVoidV<DataType>::value;
	template<typename DataType> concept IsVoidC = IsVoid<DataType>;

	template<typename... AnyTypes> struct MakeVoidT { using type = void; };
	template<typename... AnyTypes> using MakeVoid = MakeVoidT<AnyTypes...>::type;


	template<typename DataType, template<typename...> typename Primary>
	struct IsSpecializationV : FalseType {};
	template<template<typename...> typename Primary, typename... Args>
	struct IsSpecializationV<Primary<Args...>, Primary> : TrueType {};
	template<typename DataType, template<typename...> typename Primary>
	inline constexpr Bool IsSpecialization = IsSpecializationV<DataType, Primary>::value;
	template<typename Test, template<typename...> typename SpecializationType>
	concept IsSpecializationC = IsSpecialization<Test, SpecializationType>;

	//reference 
	template<typename DataType> struct RemoveReferenceT { using type = DataType; };
	template<typename DataType> struct RemoveReferenceT<DataType&> { using type = DataType; };
	template<typename DataType> struct RemoveReferenceT<DataType&&> { using type = DataType; };
	template<typename DataType>
	using RemoveReference = typename RemoveReferenceT<DataType>::type;

	template <class DataType, class = void>
	struct UniversalAddReferenceT {
		using lvalue = DataType;
		using rvalue = DataType;
	};
	template <class DataType>
	struct UniversalAddReferenceT<DataType, MakeVoid<DataType&>> {
		using lvalue = DataType&;
		using rvalue = DataType&&;
	};
	template<typename DataType> struct AddLValueReferenceT {
		using type = UniversalAddReferenceT<DataType>::lvalue;
	};
	template<typename DataType> struct AddRValueReferenceT {
		using type = UniversalAddReferenceT<DataType>::rvalue;
	};

	template<typename DataType>
	using AddLValueReference = typename AddLValueReferenceT<DataType>::type;
	template<typename DataType>
	using AddRValueReference = typename AddRValueReferenceT<DataType>::type;
	
	template<typename DataType> struct IsLValueReferenceV : FalseType {};
	template<typename DataType> struct IsLValueReferenceV<DataType&> : TrueType {};
	template<typename DataType> constexpr inline Bool IsLValueReference = IsLValueReferenceV<DataType>::value;
	template<typename DataType> concept IsLValueReferenceC = IsLValueReference<DataType>;
	template<typename DataType> struct IsRValueReferenceV : FalseType {};
	template<typename DataType> struct IsRValueReferenceV<DataType&&> : TrueType {};
	template<typename DataType> constexpr inline Bool IsRValueReference = IsRValueReferenceV<DataType>::value;
	template<typename DataType> concept IsRValueReferenceC = IsRValueReference<DataType>;

	template<typename DataType> struct IsReferenceV : FalseType {};
	template<typename DataType> struct IsReferenceV<DataType&> : TrueType {};
	template<typename DataType> struct IsReferenceV<DataType&&> : TrueType {};
	template<typename DataType> constexpr inline Bool IsReference = IsReferenceV<TrueType>::value;
	template<typename DataType> concept IsReferenceC = IsReference<TrueType>;

	//pointers
	template<typename DataType> struct RemovePointerT { using type = DataType; };
	template<typename DataType> struct RemovePointerT<DataType*> { using type = DataType; };
	template<typename DataType> struct RemovePointerT<DataType* const> { using type = DataType; };
	template<typename DataType> struct RemovePointerT<DataType* volatile> { using type = DataType; };
	template<typename DataType> struct RemovePointerT<DataType* const volatile> { using type = DataType; };
	template<typename DataType> using RemovePointer = RemovePointerT<DataType>::type;

	namespace impl {
		template<typename DataType> auto tryAddPointer(int) -> TypeIdentityT<RemoveReference<DataType>*> {};
		template<typename DataType> auto tryAddPointer(...) -> TypeIdentityT<DataType> {};
	}
	template<typename DataType> struct AddPointerT : decltype(impl::tryAddPointer<DataType>(0)) {};
	template<typename DataType> using AddPointer = typename AddPointerT<DataType>::type;

	//misc op
	template<typename DataType> AddRValueReference<DataType> declval() noexcept {
		//static_assert(false, "natl: declval not allowed in an evaluated context");
	}
	template <typename DataType>
	constexpr RemoveReference<DataType>&& move(DataType&& arg) noexcept {
		return static_cast<RemoveReference<DataType>&&>(arg);
	}
	template <typename DataType>
	constexpr DataType&& forward(RemoveReference<DataType>& arg) noexcept {
		return static_cast<DataType&&>(arg);
	}
	template <typename DataType>
	constexpr DataType&& forward(RemoveReference<DataType>&& arg) noexcept {
		static_assert(!IsLValueReference<DataType>, "natl: template argument substituting DataType is an lvalue reference type");
		return static_cast<DataType&&>(arg);
	}
	template <typename DataType>
	constexpr void swap(DataType& a, DataType& b) noexcept {
		DataType temp = move(a);
		a = move(b);
		b = move(temp);
	}
	template<typename Type>
	constexpr const Type& toConst(const Type& value) noexcept {
		return value;
	}

	template<class DataType> struct RemoveExtentT { using type = DataType; };
	template<class DataType> struct RemoveExtentT<DataType[]> { using type = DataType; };
	template<class DataType, std::size_t Number> struct RemoveExtentT<DataType[Number]> { using type = DataType; };
	template<class DataType> using RemoveExtent = RemoveExtentT<DataType>::type;

	template<class Type> struct DecayT {
	private:
		using TypeNoReference = RemoveReference<Type>;
	public:
		using type = Conditional<
			IsArray<TypeNoReference>,
			AddPointer<RemoveExtent<TypeNoReference>>,
			Conditional<
				IsFunction<TypeNoReference>,
				AddPointer<TypeNoReference>,
				RemoveConctVolatile<TypeNoReference>
			>
		>;
	};
	template<class Type> using Decay = DecayT<Type>::type;

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
	struct IsConvertibleT : BoolConstant<
		(decltype(impl::testReturnable<To>(0))::value && decltype(impl::testImplicitlyConvertible<From, To>(0))::value) ||
		(IsVoid<From> && IsVoid<To>)
	> {};

	template<typename From, typename To> constexpr inline Bool IsConvertible = IsConvertibleT<From, To>::value;
	template<typename From, typename To> concept IsConvertibleC = IsConvertible<From, To>;

	template<typename From, typename To>
	struct IsNoThrowConvertibleT : ConjunctionV<IsVoidV<From>, IsVoidV<To>> {};
	template<typename From, typename To>
		requires (
			requires {
				static_cast<To(*)()>(nullptr);
				{ declval<void(&)(To) noexcept>()(declval<From>()) } noexcept;
		}
	)
	struct IsNoThrowConvertibleT<From, To> : TrueType {};

	template<typename From, typename To> constexpr inline Bool IsNothrowConvertible = IsNoThrowConvertibleT<From, To>::value;
	template<typename From, typename To> concept IsNothrowConvertibleC = IsNothrowConvertible<From, To>;

	template<typename From, typename To> 
	concept ConvertibleTo = IsConvertible<From, To> && requires { static_cast<To>(declval<From>()); };

	//common
	template<typename... Types> struct CommonReferenceT {};
	template<typename Type> struct CommonReferenceT<Type> { using type = Type; };

	template<typename Type, typename OtherType>
	struct CommonReferenceTwoT {
	private:
		template<typename LocalType, typename LocalOtherType>
		static auto commonReferenceTest(int) -> decltype(true ? declval<LocalType>() : declval<LocalOtherType>());
		template<typename, typename>
		static auto commonReferenceTest(...) -> void;
	public:
		using type = decltype(commonReferenceTest<Type, OtherType>(0));
	};

	template<typename FirstType, typename SecondType>
	struct CommonReferenceT<FirstType, SecondType> {
		using type = CommonReferenceTwoT<FirstType, SecondType>;
	};
	template<typename FirstType, typename SecondType, typename... Rest>
	struct CommonReferenceT<FirstType, SecondType, Rest...> {
		using type = CommonReferenceTwoT<typename CommonReferenceTwoT<FirstType, SecondType>::type, Rest...>;
	};

	template<typename... Types> using CommonReference = CommonReferenceT<Types...>::type;

	template<class Type, class OtherType>
	concept CommonReferenceWith =
		SameAs<CommonReference<Type, OtherType>, CommonReference<OtherType, Type>>&&
		ConvertibleTo<Type, CommonReference<Type, OtherType>>&&
		ConvertibleTo<OtherType, CommonReference<Type, OtherType>>;

	//assing
	template<typename LhsDataType, class RhsDataType>
	concept AssignableFromC =
		IsLValueReference<LhsDataType> &&
		CommonReferenceWith<
		const RemoveReference<LhsDataType>&,
		const RemoveReference<RhsDataType>&>&&
		requires(LhsDataType lhs, RhsDataType&& rhs) {
			{ lhs = natl::forward<RhsDataType>(rhs) } -> SameAs<RhsDataType>;
	};

	//redumentary 
	namespace impl {
		template<typename Type> struct IsBuiltInIntegerV : FalseType {};
		template<> struct IsBuiltInIntegerV<i8> : TrueType {};
		template<> struct IsBuiltInIntegerV<i16> : TrueType {};
		template<> struct IsBuiltInIntegerV<i32> : TrueType {};
		template<> struct IsBuiltInIntegerV<i64> : TrueType {};
		template<> struct IsBuiltInIntegerV<ui8> : TrueType {};
		template<> struct IsBuiltInIntegerV<ui16> : TrueType {};
		template<> struct IsBuiltInIntegerV<ui32> : TrueType {};
		template<> struct IsBuiltInIntegerV<ui64> : TrueType {};
	}
	template<typename Type> struct IsBuiltInIntegerV : impl::IsBuiltInIntegerV<Decay<Type>> {};
	template<typename Type> constexpr inline Bool IsBuiltInInteger = IsBuiltInIntegerV<Type>::value;
	template<typename Type> concept IsBuiltInIntegerC = IsBuiltInInteger<Type>;

	namespace impl {
		template<typename Type> struct IsBuiltInSignedIntegerV : FalseType {};
		template<> struct IsBuiltInSignedIntegerV<i8> : TrueType {};
		template<> struct IsBuiltInSignedIntegerV<i16> : TrueType {};
		template<> struct IsBuiltInSignedIntegerV<i32> : TrueType {};
		template<> struct IsBuiltInSignedIntegerV<i64> : TrueType {};
	}
	template<typename Type> struct IsBuiltInSignedIntegerV : impl::IsBuiltInSignedIntegerV<Decay<Type>> {};
	template<typename Type> constexpr inline Bool IsBuiltInSignedInteger = IsBuiltInSignedIntegerV<Type>::value;
	template<typename Type> concept IsBuiltInSignedIntegerC = IsBuiltInSignedInteger<Type>;

	namespace impl {
		template<typename Type> struct IsBuiltInUnsignedIntegerV : FalseType {};
		template<> struct IsBuiltInUnsignedIntegerV<ui8> : TrueType {};
		template<> struct IsBuiltInUnsignedIntegerV<ui16> : TrueType {};
		template<> struct IsBuiltInUnsignedIntegerV<ui32> : TrueType {};
		template<> struct IsBuiltInUnsignedIntegerV<ui64> : TrueType {};
	}
	template<typename Type> struct IsBuiltInUnsignedIntegerV : impl::IsBuiltInUnsignedIntegerV<Decay<Type>> {};
	template<typename Type> constexpr inline Bool IsBuiltInUnsignedInteger = IsBuiltInUnsignedIntegerV<Type>::value;
	template<typename Type> concept IsBuiltInUnsignedIntegerC = IsBuiltInUnsignedInteger<Type>;

	template<typename Type> struct IsBuiltInFloatingPointV : BoolConstant<SameAs<Decay<Type>, f32> || SameAs<Decay<Type>, f64>> {};
	template<typename Type> constexpr inline Bool IsBuiltInFloatingPoint = IsBuiltInFloatingPointV<Type>::value;
	template<typename Type> concept IsBuiltInFloatingPointC = IsBuiltInFloatingPoint<Type>;

	template<typename Type> struct IsBuiltInNumericV : BoolConstant<IsBuiltInInteger<Type> || IsBuiltInFloatingPoint<Type>> {};
	template<typename Type> constexpr inline Bool IsBuiltInNumeric = IsBuiltInNumericV<Type>::value;
	template<typename Type> concept IsBuiltInNumericC = IsBuiltInNumeric<Type>;

	namespace impl {
		template<typename Type> struct IsBuiltInCharacterT : FalseType {};
		template<> struct IsBuiltInCharacterT<Ascii> : TrueType {};
		template<> struct IsBuiltInCharacterT<Utf8> : TrueType {};
		template<> struct IsBuiltInCharacterT<Utf16> : TrueType {};
		template<> struct IsBuiltInCharacterT<Utf32> : TrueType {};
	}
	template<typename Type> struct IsBuiltInCharacterT : impl::IsBuiltInCharacterT<Decay<Type>> {};
	template<typename Type> constexpr inline Bool IsBuiltInCharacter = IsBuiltInCharacterT<Type>::value;
	template<typename Type> concept IsBuiltInCharacterC = IsBuiltInCharacter<Type>;

	template<typename Type> struct IsBuiltInTypeT : BoolConstant<IsBuiltInInteger<Type> || IsBuiltInFloatingPoint<Type> || IsBuiltInCharacter<Type> || IsPointer<Type>> {};
	template<typename Type> constexpr inline Bool IsBuiltInType = IsBuiltInTypeT<Type>::value;
	template<typename Type> concept IsBuiltInTypeC = IsBuiltInType<Type>;

	//type operations
	template<typename Type, typename... ConstructArgTypes>
	concept IsConstructibleC = requires {
		{ Type(natl::declval<ConstructArgTypes>()...) };
	};
	template<typename Type, typename... ConstructArgTypes>
	constexpr inline Bool IsConstructible = IsConstructibleC<Type, ConstructArgTypes...>;
	template<typename Type, typename... ConstructArgTypes>
	struct IsConstructibleV : BoolConstant<IsConstructible<Type, ConstructArgTypes...>> {};

	template<typename Type, typename OtherType>
	concept IsAssignableC = requires(Type value, OtherType other) {
		{ value = other } -> SameAs<Type&>;
	};
	template<typename Type, typename OtherType>
	constexpr inline Bool IsAssignable = IsAssignableC<Type, OtherType>;
	template<typename Type, typename OtherType>
	struct IsAssignableV : BoolConstant<IsAssignable<Type, OtherType>> {};

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
	concept IsTriviallyCompareable = IsBuiltInType<DataType> ||
		impl::customCheckIfIsTriviallyCompareable<DataType>();

	template <typename DataType>
	concept IsTriviallyRelocatable = (std::is_trivially_copyable_v<DataType> && std::is_trivially_destructible_v<DataType>) ||
		IsBuiltInType<DataType> || impl::customCheckIfIsTriviallyRelocatable<DataType>();

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

		NATL_OPERATOR_TESTABLE_CONCEPT(NotEqual, ==)
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

	//function 
	template <typename Functor, typename ReturnType, typename... ArgTypes>
	concept HasFunctionSignature = requires(Functor functor, ArgTypes... args) {
		{ functor(natl::forward<ArgTypes>(args)...) } -> std::same_as<ReturnType>;
	};

	template<typename Functor, typename ReturnType, typename... ArgTypes>
	concept IsConstCallable = HasFunctionSignature<Functor, ReturnType, ArgTypes...>&&
		requires(const Functor functor, ArgTypes... args) {
			{ functor(natl::forward<ArgTypes>(args)...) } -> std::same_as<ReturnType>;
	};

	namespace impl {
		template<typename Functor, typename... ArgTypes>
		auto tryInvoke(Functor&& functor, ArgTypes&&... args) -> decltype(natl::forward<Functor>(functor)(natl::forward<ArgTypes>(args)...)) {}
	}

	template<typename...> struct InvokeResultTypeT {
		using type = void;
	};
	template<typename Functor, typename... ArgTypes>
	struct InvokeResultTypeT<Functor(ArgTypes...)> {
		using type = decltype(impl::tryInvoke(declval<Functor>(), declval<ArgTypes>()...));
	};
	template<typename Functor> using InvokeResult = typename InvokeResultTypeT<Functor>::type;

	template<typename Functor, typename... ArgTypes>
	struct InvokeResultWithArgsT {
		using type = decltype(impl::tryInvoke(declval<Functor>(), declval<ArgTypes>()...));
	};
	template<typename Functor, typename... ArgTypes> using InvokeResultWithArgs = typename InvokeResultWithArgsT<Functor, ArgTypes...>::type;

	template<typename Functor> struct ResultOfT;

	template<typename Functor, typename... ArgTypes>
	struct ResultOfT<Functor(ArgTypes...)> {
		using type = decltype(impl::tryInvoke(declval<Functor>(), declval<ArgTypes>()...));
	};
	template<typename ReturnType, typename... ArgTypes>
	struct ResultOfT<ReturnType(*)(ArgTypes...)> {
		using type = ReturnType;
	};
	template<typename ReturnType, typename... ArgTypes>
	struct ResultOfT<ReturnType(*)(ArgTypes...) noexcept> {
		using type = ReturnType;
	};
	template<typename ReturnType, typename... ArgTypes>
	struct ResultOfT<ReturnType(&)(ArgTypes...)> {
		using type = ReturnType;
	};
	template<typename ReturnType, typename... ArgTypes>
	struct ResultOfT<ReturnType(&)(ArgTypes...) noexcept> {
		using type = ReturnType;
	};

	template<typename Functor>
	using ResultOf = ResultOfT<Functor>::type;

	template<typename Functor>
	struct FunctorBaseStorageT {};


	template<typename ReturnType, typename... ArgTypes>
	struct FunctorBaseStorageT<ReturnType(*)(ArgTypes...)> {
		using type = ReturnType(*)(ArgTypes...) noexcept;
	};
	template<typename ReturnType, typename... ArgTypes>
	struct FunctorBaseStorageT<ReturnType(*)(ArgTypes...) noexcept> {
		using type = ReturnType(*)(ArgTypes...) noexcept;
	};
	template<typename ReturnType, typename... ArgTypes>
	struct FunctorBaseStorageT<ReturnType(&)(ArgTypes...)> {
		using type = ReturnType(*)(ArgTypes...) noexcept;
	};
	template<typename ReturnType, typename... ArgTypes>
	struct FunctorBaseStorageT<ReturnType(&)(ArgTypes...) noexcept> {
		using type = ReturnType(*)(ArgTypes...) noexcept;
	};

	template<typename Functor, typename... ArgTypes>
	struct FunctorBaseStorageT<Functor(ArgTypes...)> {
		using ReturnType = ResultOf<Functor(ArgTypes...)>;
		using type = Conditional<ConvertibleTo<RemoveReference<Functor>, ReturnType(*)(ArgTypes...) noexcept>, ReturnType(*)(ArgTypes...) noexcept, Functor>;
	};

	template<typename Functor>
	using FunctorBaseStorage = FunctorBaseStorageT<Functor>::type;

	template<class Functor, typename... ArgTypes>
	constexpr InvokeResultWithArgs<Functor, ArgTypes...> invokeFunction(Functor&& functor, ArgTypes&&... args) noexcept {
		return natl::forward<Functor>(functor)(natl::forward<ArgTypes>(args)...);
	}

	template<typename Container>
	concept HasReserve = requires(Container container, const Size newCapacity) {
		{ container.reserve(newCapacity) };
	};
	template<typename Container>
	concept HasResize = requires(Container container, const Size newCapacity) {
		{ container.resize(newCapacity) };
	};

	//
	template <typename ArrayViewLike, class DataType>
	concept IsArrayViewLike = requires(ArrayViewLike arrayViewLike) {
		{ arrayViewLike.data() } -> std::convertible_to<const DataType*>;
		{ arrayViewLike.size() } -> std::convertible_to<Size>;
		{ arrayViewLike[std::declval<Size>()] } -> std::convertible_to<DataType>;
	};
	template <typename StringView, class CharType>
	concept IsStringViewLike = requires(StringView stringView) {
		{ stringView.data() } -> std::convertible_to<const CharType*>;
		{ stringView.size() } -> std::convertible_to<Size>;
		{ stringView[std::declval<Size>()] } -> std::convertible_to<CharType>;
	};
}