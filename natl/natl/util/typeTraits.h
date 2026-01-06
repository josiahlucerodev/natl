#pragma once

//@begin_non_modules
//std
#include <concepts>
#include <type_traits>

//own
#include "compilerDependent.h"
//@end_non_modules

//own
#include "basicTypes.h"

//@export
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
	template<typename Type> struct IsUnionV : BoolConstant<std::is_union_v<Type>> {};
	template<typename Type> inline constexpr Bool IsUnion = IsUnionV<Type>::value;
	template<typename Type> concept IsUnionC = IsUnion<Type>;

	template<typename Type> struct IsNotUnionV : NegationV<IsUnionV<Type>> {};
	template<typename Type> inline constexpr Bool IsNotUnion = IsNotUnionV<Type>::value;
	template<typename Type> concept IsNotUnionC = IsNotUnion<Type>;

	namespace impl {
		template<typename Type> IntegralConstant<Bool, IsNotUnion<Type>> isStructTest(int Type::*);
		template<typename> FalseType isStructTest(...);
	}
	template<typename Type> struct IsStructV : decltype(impl::isStructTest<Type>(0)) {};
	template<typename Type> constexpr inline Bool IsStruct = IsStructV<Type>::value;
	template<typename Type> concept IsStructC = IsStruct<Type>;

	template<typename Type> struct IsNotstructV : NegationV<IsStructV<Type>> {};
	template<typename Type> constexpr inline Bool IsNotstruct = IsNotstructV<Type>::value;
	template<typename Type> concept IsNotstructC = IsNotstruct<Type>;

	template<typename Type> struct IsEnumV : BoolConstant<std::is_enum_v<Type>> {};
	template<typename Type> inline constexpr Bool IsEnum = IsEnumV<Type>::value;
	template<typename Type> concept IsEnumC = IsEnum<Type>;

	template<typename Type> struct IsNotEnumV : NegationV<IsEnumV<Type>> {};
	template<typename Type> constexpr inline Bool IsNotEnum = IsNotEnumV<Type>::value;
	template<typename Type> concept IsNotEnumC = IsNotEnum<Type>;

	template<typename Type> struct IsPointerV : FalseType {};
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

#ifdef NATL_COMPILER_EMSCRIPTEN
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wambiguous-ellipsis" 
#endif // NATL_COMPILER_EMSCRIPTEN


	template<typename > struct IsFunctionV : FalseType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...)> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......)> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) volatile> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const volatile> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) volatile> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const volatile> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...)&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) volatile&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const volatile&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......)&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) volatile&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const volatile&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...)&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) volatile&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const volatile&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......)&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) volatile&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const volatile&&> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) volatile noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const volatile noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) volatile noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const volatile noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) & noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) volatile& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const volatile& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) & noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) volatile& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const volatile& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) && noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const&& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) volatile&& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes...) const volatile&& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) && noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const&& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) volatile&& noexcept> : TrueType {};
	template<typename ReturnType, typename... ArgTypes> struct IsFunctionV<ReturnType(ArgTypes......) const volatile&& noexcept> : TrueType {};
	template<typename FunctionSignature> constexpr inline Bool IsFunction = IsFunctionV<FunctionSignature>::value;
	template<typename FunctionSignature> concept IsFunctionC = IsFunction<FunctionSignature>;

#ifdef NATL_COMPILER_EMSCRIPTEN
#pragma GCC diagnostic pop
#endif // NATL_COMPILER_EMSCRIPTEN

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
	template<typename Type, typename OtherType> struct IsSameV : FalseType {};
	template<typename Type> struct IsSameV<Type, Type> : TrueType {};
	template<typename Type, typename OtherType> constexpr inline Bool IsSame = IsSameV<Type, OtherType>::value;
	template<typename Type, typename OtherType> concept IsSameC = IsSame<Type, OtherType>;

	template<typename Type, typename OtherType> struct IsNotSameV : TrueType {};
	template<typename Type> struct IsNotSameV<Type, Type> : FalseType {};
	template<typename Type, typename OtherType> constexpr inline Bool IsNotSame = IsNotSameV<Type, OtherType>::value;
	template<typename Type, typename OtherType> concept IsNotSameC = IsNotSame<Type, OtherType>;

	namespace impl {
		template<typename Type, typename OtherType> concept SameAsImpl = IsSame<Type, OtherType>;
	}
	template<typename Type, typename OtherType>
	concept SameAs = impl::SameAsImpl<Type, OtherType>&& impl::SameAsImpl<Type, OtherType>;

	template<typename... Types> struct IsSameByteSizeV : BoolConstant<(sizeof(Types) == ...)> {};
	template<typename... Types> constexpr inline Bool IsSameByteSize = IsSameByteSizeV<Types...>::value;
	template<typename... Types> concept IsSameByteSizeC = IsSameByteSize<Types...>;

	template<typename BaseType, typename DerivedType> struct IsBaseOfV : BoolConstant<std::is_base_of_v<BaseType, DerivedType>> {};
	template<typename BaseType, typename DerivedType> inline constexpr Bool IsBaseOf = IsBaseOfV<BaseType, DerivedType>::value;
	template<typename BaseType, typename DerivedType> concept IsBaseOfC = IsBaseOf<BaseType, DerivedType>;

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

	template<typename DataType> struct IsVoidV : IsSameV<void, RemoveConctVolatile<DataType>> {};
	template<typename DataType> constexpr inline Bool IsVoid = IsVoidV<DataType>::value;
	template<typename DataType> concept IsVoidC = IsVoid<DataType>;

	template<typename... AnyTypes> struct MakeVoidT { using type = void; };
	template<typename... AnyTypes> using MakeVoid = MakeVoidT<AnyTypes...>::type;
	template <typename... Types> using Void = void;

	struct NoReturn {};


	template<typename DataType, template<typename...> typename Primary>
	struct IsSpecializationV : FalseType {};
	template<template<typename...> typename Primary, typename... Args>
	struct IsSpecializationV<Primary<Args...>, Primary> : TrueType {};
	template<typename DataType, template<typename...> typename Primary>
	inline constexpr Bool IsSpecialization = IsSpecializationV<DataType, Primary>::value;
	template<typename Test, template<typename...> typename SpecializationType>
	concept IsSpecializationC = IsSpecialization<Test, SpecializationType>;

	//struct layout 
	template<typename Type> struct IsAggregateV : BoolConstant<std::is_aggregate_v<Type>> {};
	template<typename Type> inline constexpr Bool IsAggregate = IsAggregateV<Type>::value;
	template<typename Type> concept IsAggregateC = IsAggregateV<Type>::value;

	//reference 
	template<typename DataType> struct RemoveReferenceT { using type = DataType; };
	template<typename DataType> struct RemoveReferenceT<DataType&> { using type = DataType; };
	template<typename DataType> struct RemoveReferenceT<DataType&&> { using type = DataType; };
	template<typename DataType>
	using RemoveReference = typename RemoveReferenceT<DataType>::type;

	template <typename DataType, typename = void>
	struct UniversalAddReferenceT {
		using lvalue = DataType;
		using rvalue = DataType;
	};
	template <typename DataType>
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

	template<typename Type> concept IsPointerToConstantDataC = IsPointerC<Type> && IsConstC<RemovePointer<Type>>;
	template<typename Type> struct IsPointerToConstantDataV : BoolConstant<IsPointerToConstantDataC<Type>> {};
	template<typename Type> constexpr inline Bool IsPointerToConstantData = IsPointerToConstantDataC<Type>;

	namespace impl {
		template<typename DataType> auto tryAddPointer(int) -> TypeIdentityT<RemoveReference<DataType>*> {};
		template<typename DataType> auto tryAddPointer(...) -> TypeIdentityT<DataType> {};
	}
	template<typename DataType> struct AddPointerT : decltype(impl::tryAddPointer<DataType>(0)) {};
	template<typename DataType> using AddPointer = typename AddPointerT<DataType>::type;

	template <typename... Type> struct AlwaysFalseV : FalseType {};
	template <typename... Type> constexpr inline Bool AlwaysFalse = AlwaysFalseV<Type...>::value;
	template <typename... Type> concept AlwaysFalseC = AlwaysFalseV<Type...>::value;

	template <typename... Type> struct AlwaysTrueV : TrueType {};
	template <typename... Type> constexpr inline Bool AlwaysTrue = AlwaysTrueV<Type...>::value;
	template <typename... Type> concept AlwaysTrueC = AlwaysTrueV<Type...>::value;

	//misc op
	template<typename DataType> AddRValueReference<DataType> declval() noexcept {
		static_assert(AlwaysFalse<DataType>, "natl: calling declval is ill-formed");
	}
	template<typename DataType> AddLValueReference<DataType> declref() noexcept {
		static_assert(AlwaysFalse<DataType>, "natl: calling declref is ill-formed");
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

	template<typename DataType> struct RemoveExtentT { using type = DataType; };
	template<typename DataType> struct RemoveExtentT<DataType[]> { using type = DataType; };
	template<typename DataType, Size Number> struct RemoveExtentT<DataType[Number]> { using type = DataType; };
	template<typename DataType> using RemoveExtent = RemoveExtentT<DataType>::type;

	template<typename Type> struct DecayT {
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

	template<typename Type> using Decay = DecayT<Type>::type;

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

	template<typename Type, typename OtherType>
	concept CommonReferenceWith =
		SameAs<CommonReference<Type, OtherType>, CommonReference<OtherType, Type>>&&
		ConvertibleTo<Type, CommonReference<Type, OtherType>>&&
		ConvertibleTo<OtherType, CommonReference<Type, OtherType>>;

	//assing
	template<typename LhsDataType, typename RhsDataType>
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

	template<typename Type> concept IsCopyConstructibleC = std::is_copy_constructible_v<Decay<Type>>;
	template<typename Type> constexpr inline Bool IsCopyConstructible = IsCopyConstructibleC<Type>;
	template<typename Type> struct IsCopyConstructibleV : BoolConstant<IsCopyConstructibleC<Type>> {};

	template<typename Type> concept IsCopyAssignableC = std::is_copy_assignable_v<Decay<Type>>;
	template<typename Type> constexpr inline Bool IsCopyAssignable = IsCopyAssignableC<Type>;
	template<typename Type> struct IsCopyAssignableV : BoolConstant<IsCopyAssignableC<Type>> {};

	template<typename Type> concept IsMoveConstructibleC = std::is_move_constructible_v<Decay<Type>>;
	template<typename Type> constexpr inline Bool IsMoveConstructible = IsMoveConstructibleC<Type>;
	template<typename Type> struct IsMoveConstructibleV : BoolConstant<IsMoveConstructibleC<Type>> {};

	template<typename Type> concept IsMoveAssignableC = std::is_move_assignable_v<Decay<Type>>;
	template<typename Type> constexpr inline Bool IsMoveAssignable = IsMoveAssignableC<Type>;
	template<typename Type> struct IsMoveAssignableV : BoolConstant<IsMoveAssignableC<Type>> {};

	//comparable
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyCompareable = IsBuiltInType<Type>;
	template<typename Type> struct IsBuiltInTriviallyCompareableV : BoolConstant<IsBuiltInTriviallyCompareable<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyCompareableC = IsBuiltInTriviallyCompareable<Type>;

	template<typename Type> struct IsTriviallyCompareableV : BoolConstant<IsBuiltInTriviallyCompareable<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyCompareable = IsTriviallyCompareableV<Type>::value;
	template<typename Type> concept IsTriviallyCompareableC = IsTriviallyCompareable<Type>;

	//relocatable
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyRelocatable =
		std::is_trivially_copyable_v<Type> && std::is_trivially_destructible_v<Type>;
	template<typename Type> struct IsBuiltInTriviallyRelocatableV : BoolConstant<IsBuiltInTriviallyRelocatable<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyRelocatableC = IsBuiltInTriviallyRelocatable<Type>;

	template<typename Type> struct IsTriviallyRelocatableV : BoolConstant<IsBuiltInTriviallyRelocatable<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyRelocatable = IsTriviallyRelocatableV<Type>::value;
	template<typename Type> concept IsTriviallyRelocatableC = IsTriviallyRelocatable<Type>;

	//constructable
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyConstructible = std::is_trivially_default_constructible_v<Type>;
	template<typename Type> struct IsBuiltInTriviallyConstructibleV : BoolConstant<IsBuiltInTriviallyConstructible<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyConstructibleC = IsBuiltInTriviallyConstructible<Type>;

	template<typename Type> struct IsTriviallyConstructibleV : BoolConstant<IsBuiltInTriviallyConstructible<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyConstructible = IsTriviallyConstructibleV<Type>::value;
	template<typename Type> concept IsTriviallyConstructibleC = IsTriviallyConstructible<Type>;

	//destructable
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyDestructible = std::is_trivially_destructible_v<Type>;
	template<typename Type> struct IsBuiltInTriviallyDestructibleV : BoolConstant<IsBuiltInTriviallyDestructible<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyDestructibleC = IsBuiltInTriviallyDestructible<Type>;

	template<typename Type> struct IsTriviallyDestructibleV : BoolConstant<IsBuiltInTriviallyDestructible<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyDestructible = IsTriviallyDestructibleV<Type>::value;
	template<typename Type> concept IsTriviallyDestructibleC = IsTriviallyDestructible<Type>;

	//const ref constructible
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyConstRefConstructible = std::is_trivially_constructible_v<Type, const Type&>;
	template<typename Type> struct IsBuiltInTriviallyConstRefConstructibleV : BoolConstant<IsBuiltInTriviallyConstRefConstructible<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyConstRefConstructibleC = IsBuiltInTriviallyConstRefConstructible<Type>;

	template<typename Type> struct IsTriviallyConstRefConstructibleV : BoolConstant<IsBuiltInTriviallyConstRefConstructible<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyConstRefConstructible = IsTriviallyConstRefConstructibleV<Type>::value;
	template<typename Type> concept IsTriviallyConstRefConstructibleC = IsTriviallyConstRefConstructible<Type>;

	//move constructible
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyMoveConstructible = std::is_trivially_move_constructible_v<Type>;
	template<typename Type> struct IsBuiltInTriviallyMoveConstructibleV : BoolConstant<IsBuiltInTriviallyMoveConstructible<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyMoveConstructibleC = IsBuiltInTriviallyMoveConstructible<Type>;

	template<typename Type> struct IsTriviallyMoveConstructibleV : BoolConstant<IsBuiltInTriviallyMoveConstructible<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyMoveConstructible = IsTriviallyMoveConstructibleV<Type>::value;
	template<typename Type> concept IsTriviallyMoveConstructibleC = IsTriviallyMoveConstructible<Type>;

	//const ref assignable
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyConstRefAssignable = std::is_trivially_assignable_v<Type, const Type&>;
	template<typename Type> struct IsBuiltInTriviallyConstRefAssignableV : BoolConstant<IsBuiltInTriviallyConstRefAssignable<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyConstRefAssignableC = IsBuiltInTriviallyConstRefAssignable<Type>;

	template<typename Type> struct IsTriviallyConstRefAssignableV : BoolConstant<IsBuiltInTriviallyConstRefAssignable<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyConstRefAssignable = IsTriviallyConstRefAssignableV<Type>::value;
	template<typename Type> concept IsTriviallyConstRefAssignableC = IsTriviallyConstRefAssignable<Type>;

	//move assignable
	template<typename Type> constexpr inline Bool IsBuiltInTriviallyMoveAssignable = std::is_trivially_move_assignable_v<Type>;
	template<typename Type> struct IsBuiltInTriviallyMoveAssignableV : BoolConstant<IsBuiltInTriviallyMoveAssignable<Type>> {};
	template<typename Type> concept IsBuiltInTriviallyMoveAssignableC = IsBuiltInTriviallyMoveAssignable<Type>;

	template<typename Type> struct IsTriviallyMoveAssignableV : BoolConstant<IsBuiltInTriviallyMoveAssignable<Type>> {};
	template<typename Type> constexpr inline Bool IsTriviallyMoveAssignable = IsTriviallyMoveAssignableV<Type>::value;
	template<typename Type> concept IsTriviallyMoveAssignableC = IsTriviallyMoveAssignable<Type>;

	//memcopy
	template <typename Type>
	concept MemcopyConstructible = IsTriviallyConstructibleC<Type> && IsTriviallyRelocatableC<Type>;

	template <typename Src, typename Dst, typename SrcRef>
	concept MemcopyConstructibleSrcDst = !IsSameC<Src, void> 
		&& !IsSameC<Dst, void> 
		&& !IsSameC<SrcRef, void> 
		&& ((IsSameC<Decay<Src>, Decay<Dst>>&& MemcopyConstructible<Src>) 
			|| (sizeof(Src) == sizeof(Dst) && std::is_trivially_constructible_v<Dst, SrcRef>));

	template<typename Src, typename Dst, typename SrcRef, typename DstRef>
	concept MemcopyAssignableSrcDst =
		(IsSameC<Src, Dst> && MemcopyConstructible<Src>) ||
		(sizeof(Src) == sizeof(Dst) && std::is_trivially_assignable_v<DstRef, SrcRef>);

	template <typename Type>
	concept MemcpyCompareable = IsTriviallyCompareable<Type>;

	template<typename Src, typename Dst>
	concept MemcpyCompareableSrcDst = (IsSameC<Src, Dst> && MemcpyCompareable<Src>);
	
	//cast
	template <typename From, typename To>
	concept IsPolymorphicCastable = std::is_base_of_v<From, To> || IsConvertibleC<From*, To*>;


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
	namespace impl {
		template <typename Functor, typename ReturnType, typename... ArgTypes>
		constexpr auto testHasFunctionSignature() noexcept {
			static_assert(natl::SameAs<decltype(natl::forward<Functor>(declref<Functor>())(natl::forward<ArgTypes>(declref<ArgTypes>())...)), ReturnType>);
		}
		template <typename Functor, typename ReturnType, typename... ArgTypes>
		constexpr auto testIsConstCallable(const Functor functor, ArgTypes&&... args) noexcept {
			static_assert(natl::SameAs<decltype(declref<const Functor>()(natl::forward<ArgTypes>(declref<ArgTypes>())...)), ReturnType>);
		}
	}

#if defined(NATL_COMPILER_GCC)
	template <typename Functor, typename ReturnType, typename... ArgTypes>
	concept HasFunctionSignature = requires() {
		{ impl::testHasFunctionSignature<Functor, ReturnType, ArgTypes...>() };
	};
#else
	template <typename Functor, typename ReturnType, typename... ArgTypes>
	concept HasFunctionSignature = requires(Functor&& functor, ArgTypes&&... args) {
		{ functor(natl::forward<ArgTypes>(args)...) } -> natl::SameAs<ReturnType>;
	};
#endif

#if defined(NATL_COMPILER_GCC)
	template<typename Functor, typename ReturnType, typename... ArgTypes>
	concept IsConstCallable = HasFunctionSignature<Functor, ReturnType, ArgTypes...> && requires() {
			impl::testIsConstCallable<Functor, ReturnType, ArgTypes...>();
	};
#else
	template<typename Functor, typename ReturnType, typename... ArgTypes>
	concept IsConstCallable = HasFunctionSignature<Functor, ReturnType, ArgTypes...> 
		&& requires(const Functor& functor, ArgTypes&&... args) {
		{ functor(natl::forward<ArgTypes>(args)...) } -> natl::SameAs<ReturnType>;
	};
#endif

	namespace impl {
		template<typename Functor, typename... ArgTypes>
		auto tryInvoke(Functor&& functor, ArgTypes&&... args) -> decltype(natl::forward<Functor>(functor)(natl::forward<ArgTypes>(args)...));
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

	template<typename Functor, typename... ArgTypes>
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

	template <typename ArrayViewLike, typename DataType>
	concept IsArrayViewLike = requires(ArrayViewLike arrayViewLike) {
		{ arrayViewLike.data() } -> IsConvertibleC<const DataType*>;
		{ arrayViewLike.size() } -> IsConvertibleC<Size>;
		{ arrayViewLike[declval<Size>()] } -> IsConvertibleC<DataType>;
	};
	template <typename ArrayViewLike>
	concept IsBroadlyArrayViewLike = requires(ArrayViewLike arrayViewLike) {
		{ arrayViewLike.data() } ;
		{ arrayViewLike.size() } -> IsConvertibleC<Size>;
		{ arrayViewLike[declval<Size>()] };
	};

	template <typename StringView, typename CharType>
	concept IsStringViewLike = requires(StringView stringView) {
		{ stringView.data() } -> IsConvertibleC<const CharType*>;
		{ stringView.size() } -> IsConvertibleC<Size>;
		{ stringView[declval<Size>()] } -> IsConvertibleC<CharType>;
	};

	//apply
	template<template<typename> typename Predicate> struct NegatePredicateT {
		template<typename Type> using type = NegationV<Predicate<Type>>;
	};

	template<typename Type, template<typename> typename Predicate>
	concept ApplyRequirementC = Predicate<Type>::value;


	//byte like 
	template <typename Type> struct IsCopyableStorageDstT : FalseType {};
	template<typename Type> constexpr inline Bool IsCopyableStorageDst = IsCopyableStorageDstT<Type>::value;
	template<typename Type> concept IsCopyableStorageDstC = IsCopyableStorageDstT<Type>::value;

	template<typename Type> concept IsByteLikeC = sizeof(Type) == 1;
	template<typename Type> struct IsByteLikeT : BoolConstant<IsByteLikeC<Type>> {};
	template<typename Type> constexpr inline Bool IsByteLike = IsByteLikeC<Type>;

	template<typename Type> concept IsPtrToByteLikeC = IsPointerC<Type> && IsByteLikeC<RemovePointer<Type>>;
	template<typename Type> struct IsPtrToByteLikeT : BoolConstant<IsPtrToByteLikeC<Type>> {};
	template<typename Type> constexpr inline Bool IsPtrToByteLike = IsPtrToByteLikeC<Type>;

	template <typename ByteViewLike>
	concept IsByteViewLike = requires(ByteViewLike byteViewLike) {
		typename ByteViewLike::value_type;
		{ byteViewLike.data() } -> IsPtrToByteLikeC;
		{ byteViewLike.data() } -> ApplyRequirementC<NegatePredicateT<IsPointerToConstantDataV>::type>;
		{ byteViewLike.size() } -> IsConvertibleC<Size>;
		{ byteViewLike[declval<Size>()] } -> IsByteLikeC;
	};

	template <typename ByteViewLike>
	concept IsConstByteViewLike = requires(ByteViewLike byteViewLike) {
		{ byteViewLike.data() } -> IsPtrToByteLikeC;
		{ byteViewLike.size() } -> IsConvertibleC<Size>;
		{ byteViewLike[declval<Size>()] } -> IsByteLikeC;
	};

	template<typename DataType> constexpr inline Size TypeBitSize = sizeof(DataType) * Size(8);
	template<typename DataType> consteval Size getTypeBitSize() noexcept { return sizeof(DataType) * Size(8); }

	template<typename DataType> constexpr inline Size TypeByteSize = sizeof(DataType);
	template<typename DataType> consteval Size getTypeByteSize() noexcept { return sizeof(DataType); }


	struct UniversallyCastable {
		template<typename Type>
		constexpr operator Type && () const noexcept;
	};

	template<typename... RestrictedTypes>
	struct RestrictedUniversallyCastable {
		template<typename Type>
			requires(!natl::SameAs<Type, RestrictedTypes> && ...)
		constexpr operator Type && () const noexcept;
	};

	//@export
	namespace impl {
		template<typename SwitchElementType>
		concept IsSwitchDefaultC = requires() {
			typename SwitchElementType::natl_is_switch_default;
		};

		template<typename SwitchElementType>
		concept IsSwitchElementC = requires() {
			typename SwitchElementType::natl_is_switch_element;
		} || IsSwitchDefaultC<SwitchElementType>;

		template<typename Functor>
		concept IsSwitchFunctorC = requires(Functor && functor) {
			{ forward<Functor>(functor)() };
		};
	}

	template<auto vValue, typename Functor>
		requires(impl::IsSwitchFunctorC<Decay<Functor>>)
	struct SwitchElement {
	public:
		using natl_is_switch_element = int;
		constexpr static inline auto value = vValue;
		using functor_type = Functor;
		Functor mFunctor;
		constexpr SwitchElement(Functor&& functorIn) noexcept : mFunctor(forward<Functor>(functorIn)) {};
		constexpr Functor&& functor() noexcept { return move(mFunctor); }
	};

	template<typename Functor>
		requires(impl::IsSwitchFunctorC<Decay<Functor>>)
	struct SwitchDefault {
	public:
		using natl_is_switch_default = int;
		using functor_type = Functor;
		Functor mFunctor;
		constexpr SwitchDefault(Functor&& functorIn) noexcept : mFunctor(forward<Functor>(functorIn)) {};
		constexpr Functor&& functor() noexcept { return move(mFunctor); }
	};

	template<typename ConditionValueType, typename SwitchDefault>
		requires(impl::IsSwitchDefaultC<Decay<SwitchDefault>>)
	constexpr void cswitch(ConditionValueType value, SwitchDefault&& switchDefault) noexcept {
		switchDefault.functor()();
	}

	template<typename ConditionValueType, typename SwitchElement, typename... SwitchElements>
		requires(impl::IsSwitchElementC<SwitchElement> && (impl::IsSwitchElementC<SwitchElements> && ...))
	constexpr void cswitch(ConditionValueType value, SwitchElement&& element, SwitchElements&&... rest) noexcept {
		if (SwitchElement::value == value) {
			element.functor()();
		} else {
			cswitch(value, forward<SwitchElements>(rest)...);
		}
	}

	template<typename Type, Size vAlignment>
		requires(vAlignment != 0 && (vAlignment & (vAlignment - 1)) == 0)
	struct alignas(vAlignment) AlignedValue {
	public:
		using value_type = Type;
	
	public:
		Type mData;

	public:
		constexpr value_type& value() noexcept {
			return mData;
		}
		constexpr const value_type& value() const noexcept {
			return mData;
		}
	};
}
