#pragma once 

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "limits.h"

//interface 
namespace natl {
    template <typename T>
    constexpr T multiplyAll(T value) noexcept {
        return value;
    }

    template <typename T, typename... Args>
    constexpr T multiplyPackAll(T first, Args... args) noexcept {
        return first * multiplyAll(args...);
    }

    template <typename SizeType>
    constexpr SizeType multiplySizes(SizeType value) noexcept {
        return value;
    }

    template <typename SizeType, typename... Sizes>
    constexpr SizeType multiplySizes(SizeType first, Sizes... rest) noexcept {
        return first * multiplySizes(rest...);
    }

    template <Size, class T> using AlwaysType = T;
    template <class NumberType, NumberType, class Type> using CustomAlwaysType = Type;

    template <class... Ts>
    struct TemplatePackMaxSizeOfTypes;

    template <class T>
    struct TemplatePackMaxSizeOfTypes<T> {
        constexpr static Size value = sizeof(T);
    };

    template <class T, class... Ts>
    struct TemplatePackMaxSizeOfTypes<T, Ts...> {
        constexpr static Size value = sizeof(T) > TemplatePackMaxSizeOfTypes<Ts...>::value ? sizeof(T) : TemplatePackMaxSizeOfTypes<Ts...>::value;
    };


    template <Size Index, class... Types>
    struct TemplatePackNthElement {};
    template<Size Index>
    struct TemplatePackNthElement<Index> {
        // static_assert(false, "natl: TemplatePackNthElement could not find element because the index is out of range");
    };

    template <class NthType, class... Types>
    struct TemplatePackNthElement<0, NthType, Types...> {
        using type = NthType;
    };

    template <Size Index, class NotType, class... Types>
    struct TemplatePackNthElement<Index, NotType, Types...> {
        using type = TemplatePackNthElement<Index - 1, Types...>::type;
    };

    template <Size Index, class FindType, class... Types>
    struct TemplatePackFindIndexOfTypeImpl {
        constexpr static Size value = Limits<Size>::max();
    };

    template <Size Index, class FindType, class TestType, class... Types>
    struct TemplatePackFindIndexOfTypeImpl<Index, FindType, TestType, Types...> {
        constexpr static Size value = std::is_same_v<FindType, TestType> ? Index : TemplatePackFindIndexOfTypeImpl<Index + 1, FindType, Types...>::value;
    };

    template <class FindType, class... Types>
    struct TemplatePackFindIndexOfType {
        constexpr static Size value = TemplatePackFindIndexOfTypeImpl<0, FindType, Types...>::value;
    };
    struct IndexNotFound {
        constexpr static Size value = Limits<Size>::max();
    };

    template <template<typename, typename> typename TypeCompare, Size Index, class FindType, class... Types>
    struct  TemplatePackFindIndexOfTypeCompareImpl {
        constexpr static Size value = IndexNotFound::value;
    };
    template <template<typename, typename> typename TypeCompare, Size Index, class FindType, class TestType, class... Types>
    struct  TemplatePackFindIndexOfTypeCompareImpl<TypeCompare, Index, FindType, TestType, Types...> {
        constexpr static Size value = TypeCompare<FindType, TestType>::value ? Index : TemplatePackFindIndexOfTypeCompareImpl<TypeCompare, Index + 1, FindType, Types...>::value;
    };

    template <template<typename, typename> typename TypeCompare, class FindType, class... Types>
    struct  TemplatePackFindIndexOfTypeCompare {
        constexpr static Size value = TemplatePackFindIndexOfTypeCompareImpl<TypeCompare, 0, FindType, Types...>::value;
    };

    namespace impl {
        //TemplatePackOpFoldWithIndexImpl
        template <typename DataType, Size Index,
            template<typename, Size> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename... ArgTypes>
        struct TemplatePackOpFoldWithIndexImpl {
            //constexpr static Size value = Limits<Size>::max();
        };

        template <typename DataType, Size Index,
            template<typename, Size> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename ArgType,
            typename... RemainingArgTypes>
        struct TemplatePackOpFoldWithIndexImpl<DataType, Index, ValuePredicate, OpPredicate, ArgType, RemainingArgTypes...> {
            constexpr static DataType value =
                OpPredicate<
                ValuePredicate<ArgType, Index>::value,
                TemplatePackOpFoldWithIndexImpl<DataType, Index + 1, ValuePredicate, OpPredicate, RemainingArgTypes...>::value
                >::value;
        };

        template <typename DataType, Size Index,
            template<typename, Size> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename ArgType,
            typename LastArgType>
        struct TemplatePackOpFoldWithIndexImpl<DataType, Index, ValuePredicate, OpPredicate, ArgType, LastArgType> {
            constexpr static DataType value =
                OpPredicate<
                ValuePredicate<ArgType, Index>::value,
                ValuePredicate<LastArgType, Index + 1>::value
                >::value;
        };

        template <typename DataType, Size Index,
            template<typename, Size> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename LastArgType>
        struct TemplatePackOpFoldWithIndexImpl<DataType, Index, ValuePredicate, OpPredicate, LastArgType> {
            constexpr static DataType value = ValuePredicate<LastArgType, Index>::value;
        };

        template <typename DataType, Size Index,
            template<typename, Size> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate>
        struct TemplatePackOpFoldWithIndexImpl<DataType, Index, ValuePredicate, OpPredicate> {
            // static_assert(false, "natl: TemplatePackOpFoldWithIndex - no ArgTypes passed");
        };

        //TemplatePackOpFoldWithIndexAndArgImpl
        template <typename DataType, Size Index, typename ValuePredicateArg,
            template<typename, Size, typename> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename... ArgTypes>
        struct TemplatePackOpFoldWithIndexAndArgImpl {
            constexpr static DataType value = Limits<Size>::max();
        };

        template <typename DataType, Size Index, typename ValuePredicateArg,
            template<typename, Size, typename> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename ArgType,
            typename... RemainingArgTypes>
        struct TemplatePackOpFoldWithIndexAndArgImpl<DataType, Index, ValuePredicateArg, ValuePredicate, OpPredicate, ArgType, RemainingArgTypes...> {
            constexpr static DataType value =
                OpPredicate<
                ValuePredicate<ArgType, Index, ValuePredicateArg>::value,
                TemplatePackOpFoldWithIndexAndArgImpl<DataType, Index + 1, ValuePredicateArg, ValuePredicate, OpPredicate, RemainingArgTypes...>::value
                >::value;
        };

        template <typename DataType, Size Index, typename ValuePredicateArg,
            template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename ArgType,
            typename LastArgType>
        struct TemplatePackOpFoldWithIndexAndArgImpl<DataType, Index, ValuePredicateArg, ValuePredicate, OpPredicate, ArgType, LastArgType> {
            constexpr static DataType value =
                OpPredicate<
                ValuePredicate<ArgType, Index, ValuePredicateArg>::value,
                ValuePredicate<LastArgType, Index + 1, ValuePredicateArg>::value
                >::value;
        };

        template <typename DataType, Size Index, typename ValuePredicateArg,
            template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename LastArgType>
        struct TemplatePackOpFoldWithIndexAndArgImpl<DataType, Index, ValuePredicateArg, ValuePredicate, OpPredicate, LastArgType> {
            constexpr static DataType value = ValuePredicate<LastArgType, Index, ValuePredicateArg>::value;
        };

        template <typename DataType, Size Index, typename ValuePredicateArg,
            template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate>
        struct TemplatePackOpFoldWithIndexAndArgImpl<DataType, Index, ValuePredicateArg, ValuePredicate, OpPredicate> {
            // static_assert(false, "natl: TemplatePackOpFoldWithIndexAndArg - no ArgTypes passed");
        };
    }

    template<typename DataType,
        template<typename, Size> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename... ArgTypes>
    constexpr DataType TemplatePackOpFoldWithIndexValue = impl::TemplatePackOpFoldWithIndexImpl<DataType, 0, ValuePredicate, OpPredicate, ArgTypes...>::value;

    template<typename DataType, typename ValuePredicateArg,
        template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename... ArgTypes>
    constexpr DataType TemplatePackOpFoldWithIndexAndArgValue = impl::TemplatePackOpFoldWithIndexAndArgImpl<DataType, 0, ValuePredicateArg, ValuePredicate, OpPredicate, ArgTypes...>::value;


    template<typename... Elements>
    struct TypePack {
        template<typename... NewElements>
        using add_new_elements_back = TypePack<Elements..., NewElements...>;
        template<typename... NewElements>
        using add_new_elements_front = TypePack<NewElements..., Elements...>;
        constexpr static Size size = sizeof...(Elements);
    };

    template<auto Value>
    struct TypeValue {
        using value_type = decltype(Value);
        constexpr static value_type value = Value;
    };

    namespace impl {

        //CreateTypeWithTypePackImpl
        template<template<typename... Elements> typename CreateType, typename... Types>
        struct CreateTypeWithTypePackImpl {
            // static_assert(false, "natl: CreateTypeWithTypePackImpl second arg was not a natl::TypePack");
        };

        template<template<typename... Elements> typename CreateType, typename... TypePackElements>
        struct CreateTypeWithTypePackImpl<CreateType, TypePack<TypePackElements...>> {
            using type = CreateType<TypePackElements...>;
        };

        //TypePackRemoveElementsIfImpl
        template<template<typename> typename PredicateType, typename... Elements>
        struct TypePackRemoveElementsIfImpl {};

        template<template<typename> typename PredicateType, typename TestElement, typename... RemainingElements>
        struct TypePackRemoveElementsIfImpl<PredicateType, TestElement, RemainingElements...> {
            using type = std::conditional_t<
                !PredicateType<TestElement>::value,
                typename TypePackRemoveElementsIfImpl<PredicateType, RemainingElements...>::type::template add_new_elements_front<TestElement>,
                TypePack<RemainingElements...>
            >;
        };

        template <template<typename> typename PredicateType>
        struct TypePackRemoveElementsIfImpl<PredicateType> {
            using type = TypePack<>;
        };

        template<template<typename> typename PredicateType, typename... Types>
        struct TypePackRemoveElementsIfTypePackArgImpl {
            // static_assert(false, "natl: CreateTypeWithTypePackImpl second arg was not a natl::TypePack");
        };

        template<template<typename> typename PredicateType, typename... TypePackElements>
        struct TypePackRemoveElementsIfTypePackArgImpl<PredicateType, TypePack<TypePackElements...>> {
            using type = typename TypePackRemoveElementsIfImpl<PredicateType, TypePackElements...>::type;
        };



        //MergeTwoTypePack
        template<typename... Types>
        struct MergeTwoTypePackImpl {};
        template<typename LhsTypePack, typename... RhsTypePackElements>
        struct MergeTwoTypePackImpl<LhsTypePack, TypePack<RhsTypePackElements...>> {
            using type = typename LhsTypePack::template add_new_elements_back<RhsTypePackElements...>;
        };


        //TypePackHasElementImpl
        template<template<typename, typename> typename TypeCompare, typename TestElement, typename... TypePackElements>
        struct TypePackHasElementImpl {};

        template<template<typename, typename> typename TypeCompare, typename TestElement, typename... TypePackElements>
        struct TypePackHasElementImpl<TypeCompare, TestElement, TypePack<TypePackElements...>> {
            constexpr static Bool value = (TypeCompare<TestElement, TypePackElements>::value || ...);
        };

        //TypePackAddUniqueImpl
        template <template<typename, typename> typename TypeCompare, typename ExistingTypePack, typename... TestElements>
        struct TypePackAddUniqueImpl;
        template <template<typename, typename> typename TypeCompare, typename ExistingTypePack, typename TestElement, typename... RemainingElement>
        struct TypePackAddUniqueImpl<TypeCompare, ExistingTypePack, TestElement, RemainingElement...> {
            using remaining_test_type = typename TypePackAddUniqueImpl<TypeCompare, ExistingTypePack, RemainingElement...>::type;
            using type = std::conditional_t<
                TypePackHasElementImpl<TypeCompare, TestElement, ExistingTypePack>::value,
                remaining_test_type,
                typename remaining_test_type::template add_new_elements_back<TestElement>
            >;
        };

        template <template<typename, typename> typename TypeCompare, typename ExistingTypePack>
        struct TypePackAddUniqueImpl<TypeCompare, ExistingTypePack> {
            using type = ExistingTypePack;
        };

        //TypePackTransformWithTypePackArgsImpl
        template<template<typename ChangeType, typename Args> typename TransformType, typename TypePackTransformArgs, typename... TransformTypePackElements>
        struct TypePackTransformWithTypePackArgsImpl {};

        template<template<typename ChangeType, typename Args> typename TransformType, typename TypePackTransformArgs, typename... TransformTypePackElements>
        struct TypePackTransformWithTypePackArgsImpl<TransformType, TypePackTransformArgs, TypePack<TransformTypePackElements...>> {
            using type = TypePack<typename TransformType<TransformTypePackElements, TypePackTransformArgs>::type...>;
        };

        //TypePackFindIndexOfTypeCompareImpl
        template<template<typename, typename> typename TypeCompare, typename FindType, typename... TypePackElements>
        struct TypePackFindIndexOfTypeCompareImpl {};

        template<template<typename, typename> typename TypeCompare, typename FindType, typename... TypePackElements>
        struct TypePackFindIndexOfTypeCompareImpl<TypeCompare, FindType, TypePack<TypePackElements...>> {
            constexpr static Size value = TemplatePackFindIndexOfTypeCompare<TypeCompare, FindType, TypePackElements...>::value;
        };

        //TypePackFindIndexOfTypeCompareImpl
        template<Size Index, class... TypePackElements>
        struct TypePackNthElementImpl {};

        template<Size Index, class... TypePackElements>
        struct TypePackNthElementImpl<Index, TypePack<TypePackElements...>> {
            using type = TemplatePackNthElement<Index, TypePackElements...>::type;
        };

        //TypePackOpFoldWithIndexImpl
        template<typename DataType,
            template<typename, Size> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename... TypePackElements>
        struct TypePackOpFoldWithIndexImpl {};

        template<typename DataType,
            template<typename, Size> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename... TypePackElements>
        struct TypePackOpFoldWithIndexImpl<DataType, ValuePredicate, OpPredicate, TypePack<TypePackElements...>> {
            constexpr static DataType value = TemplatePackOpFoldWithIndexImpl<DataType, 0, ValuePredicate, OpPredicate, TypePackElements...>::type;
        };

        //TypePackOpFoldWithIndexAndArgImpl
        template<typename DataType, typename ValuePredicateArg,
            template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename... TypePackElements>
        struct TypePackOpFoldWithIndexAndArgImpl {};

        template<typename DataType, typename ValuePredicateArg,
            template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
            template<DataType, DataType> typename OpPredicate,
            typename... TypePackElements>
        struct TypePackOpFoldWithIndexAndArgImpl<DataType, ValuePredicateArg, ValuePredicate, OpPredicate, TypePack<TypePackElements...>> {
            constexpr static DataType value = TemplatePackOpFoldWithIndexAndArgImpl<DataType, 0, ValuePredicateArg, ValuePredicate, OpPredicate, TypePackElements...>::value;
        };
    }

    template<template<typename... Elements> typename CreateType, typename TypePackArgs>
    using CreateTypeWithTypePack = typename impl::CreateTypeWithTypePackImpl<CreateType, TypePackArgs>::type;

    template<template<typename> typename PredicateType, typename InputTypePack>
    using TypePackRemoveElementsIf = typename impl::TypePackRemoveElementsIfTypePackArgImpl<PredicateType, InputTypePack>::type;

    template<typename LhsTypePack, typename RhsTypePack>
    using MergeTwoTypePack = typename impl::MergeTwoTypePackImpl<LhsTypePack, RhsTypePack>::type;
    
    template<template<typename, typename> typename TypeCompare, typename TestElement, typename InputTypePack>
    constexpr Bool TypePackHasElement = impl::TypePackHasElementImpl<TypeCompare, TestElement, InputTypePack>::value;

    template<template<typename, typename> typename TypeCompare, typename ExistingTypePack, typename... TestElements>
    using TypePackAddUnique = typename impl::TypePackAddUniqueImpl<TypeCompare, ExistingTypePack, TestElements...>::type;

    template<template<typename Args, typename ChangeType> typename TransformType, typename TypePackTransformArgs, typename TransformTypePack>
    using TypePackTransformWithTypePackArgs = typename impl::TypePackTransformWithTypePackArgsImpl<TransformType, TypePackTransformArgs, TransformTypePack>::type;

    template<template<typename, typename> typename TypeCompare, typename FindType, typename TypePack>
    constexpr static Size TypePackFindIndexOfTypeCompareValue = impl::TypePackFindIndexOfTypeCompareImpl<TypeCompare, FindType, TypePack>::value;

    template<Size Index, typename TypePack>
    using TypePackNthElement = typename impl::TypePackNthElementImpl<Index, TypePack>::type;

    template<typename DataType,
        template<typename, Size> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePack>
    using TypePackOpFoldWithIndex = impl::TypePackOpFoldWithIndexImpl<DataType, ValuePredicate, OpPredicate, TypePack >;

    template<typename DataType,
        template<typename, Size> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePack>
    constexpr static DataType TypePackOpFoldWithIndexValue = impl::TypePackOpFoldWithIndexImpl<DataType, ValuePredicate, OpPredicate, TypePack >::value;

    template<typename DataType, typename ValuePredicateArg,
        template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePack>
    using TypePackOpFoldWithIndexAndArg = impl::TypePackOpFoldWithIndexAndArgImpl<DataType, ValuePredicateArg, ValuePredicate, OpPredicate, TypePack>;

    template<typename DataType, typename ValuePredicateArg,
        template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePack>
    constexpr static DataType TypePackOpFoldWithIndexAndArgValue = impl::TypePackOpFoldWithIndexAndArgImpl<DataType, ValuePredicateArg, ValuePredicate, OpPredicate, TypePack >::value;


    template <typename DataType, DataType... Ints>
    struct IntegerSequence {
        using type = IntegerSequence;
        using value_type = DataType;
        static constexpr Size size() noexcept { return sizeof...(Ints); }
    };

    namespace impl {
        template <typename DataType, Size Number, DataType... Seq>
        struct MakeIntegerSequenceType;

        template <typename DataType, DataType... Seq>
        struct MakeIntegerSequenceType<DataType, 0, Seq...> {
            using type = IntegerSequence<DataType, Seq...>;
        };

        template <typename DataType, Size Number, DataType... Seq>
        struct MakeIntegerSequenceType : MakeIntegerSequenceType<DataType, Number - 1, Number - 1, Seq...> {};
    }

    template <typename DataType, DataType Number>
    using MakeIntegerSequence = typename impl::MakeIntegerSequenceType<DataType, Number>::type;
    template<Size Number>
    using MakeIndexSequence = MakeIntegerSequence<Size, Number>;
    template<class... DataTypeTypes>
    using MakeIndexSequenceFor = MakeIndexSequence<sizeof...(DataTypeTypes)>;
    template<Size... Ints>
    using IndexSequence = IntegerSequence<Size, Ints...>;

}