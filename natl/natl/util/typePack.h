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

    template <Size, typename Type> using AlwaysType = Type;
    template<typename NumberType, NumberType, typename Type> using CustomAlwaysType = Type;

    template<typename... Ts>
    struct TemplatePackMaxSizeOfTypes;

    template<typename T>
    struct TemplatePackMaxSizeOfTypes<T> {
        constexpr static Size value = sizeof(T);
    };

    template<typename T, typename... Ts>
    struct TemplatePackMaxSizeOfTypes<T, Ts...> {
        constexpr static Size value = sizeof(T) > TemplatePackMaxSizeOfTypes<Ts...>::value ? sizeof(T) : TemplatePackMaxSizeOfTypes<Ts...>::value;
    };


    template <Size Index, typename... Types>
        requires(Index < sizeof...(Types))
    struct TemplatePackNthElement {};

    template<Size Index>
    struct TemplatePackNthElement<Index> {
        // static_assert(false, "natl: TemplatePackNthElement could not find element because the index is out of range");
    };

    template<typename NthType, typename... Types>
    struct TemplatePackNthElement<0, NthType, Types...> {
        using type = NthType;
    };

    template <Size Index, typename NotType, typename... Types>
    struct TemplatePackNthElement<Index, NotType, Types...> {
        using type = TemplatePackNthElement<Index - 1, Types...>::type;
    };

    template <Size Index, typename FindType, typename... Types>
    struct TemplatePackFindIndexOfTypeImpl {
        constexpr static Size value = Limits<Size>::max();
    };

    template <Size Index, typename FindType, typename TestType, typename... Types>
    struct TemplatePackFindIndexOfTypeImpl<Index, FindType, TestType, Types...> {
        constexpr static Size value = IsSameC<FindType, TestType> ? Index : TemplatePackFindIndexOfTypeImpl<Index + 1, FindType, Types...>::value;
    };

    template<typename FindType, typename... Types>
    struct TemplatePackFindIndexOfType {
        constexpr static Size value = TemplatePackFindIndexOfTypeImpl<0, FindType, Types...>::value;
    };
    struct IndexNotFound {
        constexpr static Size value = Limits<Size>::max();
    };

    template <template<typename, typename> typename TypeCompare, Size Index, typename FindType, typename... Types>
    struct  TemplatePackFindIndexOfTypeCompareImpl {
        constexpr static Size value = IndexNotFound::value;
    };
    template <template<typename, typename> typename TypeCompare, Size Index, typename FindType, typename TestType, typename... Types>
    struct  TemplatePackFindIndexOfTypeCompareImpl<TypeCompare, Index, FindType, TestType, Types...> {
        constexpr static Size value = TypeCompare<FindType, TestType>::value ? Index : TemplatePackFindIndexOfTypeCompareImpl<TypeCompare, Index + 1, FindType, Types...>::value;
    };

    template <template<typename, typename> typename TypeCompare, typename FindType, typename... Types>
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


    template<template<typename, typename> typename TypeCompare, typename TestElement, typename... Elements>
    struct TemplatePackHasElementV {
        constexpr static Bool value = (TypeCompare<TestElement, Elements>::value || ...);
    };
    template<template<typename, typename> typename TypeCompare, typename TestElement, typename... Elements>
    constexpr natl::Bool TemplatePackHasElement = TemplatePackHasElementV<TypeCompare, TestElement, Elements...>::value;
    template<template<typename, typename> typename TypeCompare, typename TestElement, typename... Elements>
    concept TemplatePackHasElementC = TemplatePackHasElement<TypeCompare, TestElement, Elements...>;


    template<typename... Elements>
    struct TypePack {
        template<typename... NewElements>
        using add_new_elements_back = TypePack<Elements..., NewElements...>;
        template<typename... NewElements>
        using add_new_elements_front = TypePack<NewElements..., Elements...>;
        constexpr static Size size = sizeof...(Elements);
        template<Size Index>
        using at = TemplatePackNthElement<Index, Elements...>::type;
    };

    namespace impl {
        template<typename...>
        struct IsTypePackV : FalseType {};

        template<>
        struct IsTypePackV<TypePack<>> : TrueType {};

        template<typename... TypePackElements>
        struct IsTypePackV<TypePack<TypePackElements...>> : TrueType {};
    }

    template<typename Type>
    constexpr inline Bool IsTypePack = impl::IsTypePackV<Type>::value;

    template<typename Type>
    concept IsTypePackC = IsTypePack<Type>;

    template<auto Value>
    struct TypeValue {
        using value_type = decltype(Value);
        constexpr static value_type value = Value;
    };

    namespace impl {
        template<template<typename... Elements> typename CreateType, typename... Types>
        struct InstantiateWithTypePackImpl {
            // static_assert(false, "natl: InstantiateWithTypePackImpl second arg was not a natl::TypePack");
        };

        template<template<typename... Elements> typename CreateType, typename... TypePackElements>
        struct InstantiateWithTypePackImpl<CreateType, TypePack<TypePackElements...>> {
            using type = CreateType<TypePackElements...>;
        };
    }

    template<template<typename... Elements> typename CreateType, typename TypePackArgs>
        requires(IsTypePackC<TypePackArgs>)
    using InstantiateWithTypePack = typename impl::InstantiateWithTypePackImpl<CreateType, TypePackArgs>::type;

    namespace impl {
        template<template<typename> typename PredicateType, typename... Elements>
        struct TypePackRemoveElementsIfImpl {};

        template<template<typename> typename PredicateType, typename TestElement, typename... RemainingElements>
        struct TypePackRemoveElementsIfImpl<PredicateType, TestElement, RemainingElements...> {
            using type = Conditional<
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
            // static_assert(false, "natl: InstantiateWithTypePackImpl second arg was not a natl::TypePack");
        };

        template<template<typename> typename PredicateType, typename... TypePackElements>
        struct TypePackRemoveElementsIfTypePackArgImpl<PredicateType, TypePack<TypePackElements...>> {
            using type = typename TypePackRemoveElementsIfImpl<PredicateType, TypePackElements...>::type;
        };
    }

    template<template<typename> typename PredicateType, typename InputTypePack>
        requires(IsTypePackC<InputTypePack>)
    using TypePackRemoveElementsIf = typename impl::TypePackRemoveElementsIfTypePackArgImpl<PredicateType, InputTypePack>::type;

    namespace impl {
        template<template<typename> typename PredicateType, typename... Elements>
        struct TemplatePackFilterT;

        template<template<typename> typename PredicateType, typename FilteredElements, typename TestElement, typename... RemainingElements>
        struct TemplatePackFilterT<PredicateType, FilteredElements, TestElement, RemainingElements...> {

            using new_filtered_elements = Conditional<
                PredicateType<TestElement>::value,
                typename FilteredElements::template add_new_elements_back<TestElement>,
                FilteredElements
            >;

            using type = TemplatePackFilterT<PredicateType, new_filtered_elements, RemainingElements...>::type;
        };

        template <template<typename> typename PredicateType, typename FilteredElements>
        struct TemplatePackFilterT<PredicateType, FilteredElements> {
            using type = FilteredElements;
        };

        template<template<typename> typename PredicateType, typename... Types>
        struct TypePackFilterT;

        template<template<typename> typename PredicateType, typename... TypePackElements>
        struct TypePackFilterT<PredicateType, TypePack<TypePackElements...>> {
            using type = typename TemplatePackFilterT<PredicateType, TypePack<>, TypePackElements...>::type;
        };
    }

    template<template<typename> typename PredicateType, typename... ElementTypes>
    using TemplatePackFilter = typename impl::TemplatePackFilterT<PredicateType, TypePack<>, ElementTypes...>::type;

    template<template<typename> typename PredicateType, typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    using TypePackFilter = typename impl::TypePackFilterT<PredicateType, TypePackArg>::type;

    namespace impl {
        template<typename... Types>
        struct TypePackMergeTwoImpl {};
        template<typename LhsTypePack, typename... RhsTypePackElements>
        struct TypePackMergeTwoImpl<LhsTypePack, TypePack<RhsTypePackElements...>> {
            using type = typename LhsTypePack::template add_new_elements_back<RhsTypePackElements...>;
        };
        template<typename LhsTypePack>
        struct TypePackMergeTwoImpl<LhsTypePack, TypePack<>> {
            using type = LhsTypePack;
        };
    }

    template<typename LhsTypePack, typename RhsTypePack>
        requires(IsTypePackC<LhsTypePack>&& IsTypePackC<RhsTypePack>)
    using TypePackMergeTwo = typename impl::TypePackMergeTwoImpl<LhsTypePack, RhsTypePack>::type;

    namespace impl {
        template<template<typename, typename> typename TypeCompare, typename TestElement, typename... TypePackElements>
        struct TypePackHasElementImpl {};

        template<template<typename, typename> typename TypeCompare, typename TestElement, typename... TypePackElements>
        struct TypePackHasElementImpl<TypeCompare, TestElement, TypePack<TypePackElements...>> {
            constexpr static Bool value = (TypeCompare<TestElement, TypePackElements>::value || ...);
        };
    }

    template<template<typename, typename> typename TypeCompare, typename TestElement, typename InputTypePack>
        requires(IsTypePackC<InputTypePack>)
    constexpr Bool TypePackHasElement = impl::TypePackHasElementImpl<TypeCompare, TestElement, InputTypePack>::value;
    template<template<typename, typename> typename TypeCompare, typename TestElement, typename InputTypePack>
    concept TypePackHasElementC = IsTypePackC<InputTypePack> && TypePackHasElement<TypeCompare, TestElement, InputTypePack>;

    namespace impl {
        template <template<typename, typename> typename TypeCompare, typename ExistingTypePack, typename... TestElements>
        struct TypePackAddUniqueImpl;
        template <template<typename, typename> typename TypeCompare, typename ExistingTypePack, typename TestElement, typename... RemainingElement>
        struct TypePackAddUniqueImpl<TypeCompare, ExistingTypePack, TestElement, RemainingElement...> {
            using remaining_test_type = typename TypePackAddUniqueImpl<TypeCompare, ExistingTypePack, RemainingElement...>::type;
            using type = Conditional<
                TypePackHasElementImpl<TypeCompare, TestElement, ExistingTypePack>::value,
                remaining_test_type,
                typename remaining_test_type::template add_new_elements_back<TestElement>
            >;
        };

        template <template<typename, typename> typename TypeCompare, typename ExistingTypePack>
        struct TypePackAddUniqueImpl<TypeCompare, ExistingTypePack> {
            using type = ExistingTypePack;
        };
    }

    template<template<typename, typename> typename TypeCompare, typename ExistingTypePack, typename... TestElements>
        requires(IsTypePackC<ExistingTypePack>)
    using TypePackAddUnique = typename impl::TypePackAddUniqueImpl<TypeCompare, ExistingTypePack, TestElements...>::type;

    namespace impl {
        template<template<typename TansformType> typename TransformPredicate, typename... Types>
        struct TemplatePackTransformT {
            using type = TypePack<typename TransformPredicate<Types>::type...>;
        };

        template<template<typename TansformType> typename TransformPredicate, typename TypePack>
        struct TypePackTransformT;

        template<template<typename TansformType> typename TransformPredicate, typename... TypePackElements>
        struct TypePackTransformT<TransformPredicate, TypePack<TypePackElements...>> {
            using type = TemplatePackTransformT<TransformPredicate, TypePackElements...>::type;
        };
    }

    template<template<typename TansformType> typename TransformPredicate, typename... Types>
    using TemplatePackTransform = impl::TemplatePackTransformT<TransformPredicate, Types...>::type;

    template<template<typename TansformType> typename TransformPredicate, typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    using TypePackTransform = impl::TypePackTransformT<TransformPredicate, TypePackArg>::type;

    namespace impl {
        template<template<typename ChangeType, typename Args> typename TransformType, typename TypePackTransformArgs, typename... TransformTypePackElements>
        struct TypePackTransformWithTypePackArgsImpl {};

        template<template<typename ChangeType, typename Args> typename TransformType, typename TypePackTransformArgs, typename... TransformTypePackElements>
        struct TypePackTransformWithTypePackArgsImpl<TransformType, TypePackTransformArgs, TypePack<TransformTypePackElements...>> {
            using type = TypePack<typename TransformType<TransformTypePackElements, TypePackTransformArgs>::type...>;
        };

    }

    template<template<typename Args, typename ChangeType> typename TransformType, typename TypePackTransformArgs, typename TransformTypePack>
        requires(IsTypePackC<TypePackTransformArgs>&& IsTypePackC<TransformTypePack>)
    using TypePackTransformWithTypePackArgs = typename impl::TypePackTransformWithTypePackArgsImpl<TransformType, TypePackTransformArgs, TransformTypePack>::type;

    namespace impl {
        template<template<typename, typename> typename TypeCompare, typename FindType, typename... TypePackElements>
        struct TypePackFindIndexOfTypeCompareImpl;

        template<template<typename, typename> typename TypeCompare, typename FindType, typename... TypePackElements>
        struct TypePackFindIndexOfTypeCompareImpl<TypeCompare, FindType, TypePack<TypePackElements...>> {
            constexpr static Size value = TemplatePackFindIndexOfTypeCompare<TypeCompare, FindType, TypePackElements...>::value;
        };
    }

    template<template<typename, typename> typename TypeCompare, typename FindType, typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    constexpr static Size TypePackFindIndexOfTypeCompareValue = impl::TypePackFindIndexOfTypeCompareImpl<TypeCompare, FindType, TypePackArg>::value;

    namespace impl {
        template<Size Index, typename... TypePackElements>
        struct TypePackNthElementImpl {};

        template<Size Index, typename... TypePackElements>
        struct TypePackNthElementImpl<Index, TypePack<TypePackElements...>> {
            using type = TemplatePackNthElement<Index, TypePackElements...>::type;
        };
    }

    template<Size Index, typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    using TypePackNthElement = typename impl::TypePackNthElementImpl<Index, TypePackArg>::type;

    namespace impl {
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
    }

    template<typename DataType,
        template<typename, Size> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    using TypePackOpFoldWithIndex = impl::TypePackOpFoldWithIndexImpl<DataType, ValuePredicate, OpPredicate, TypePackArg>;

    template<typename DataType,
        template<typename, Size> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    constexpr static DataType TypePackOpFoldWithIndexValue = impl::TypePackOpFoldWithIndexImpl<DataType, ValuePredicate, OpPredicate, TypePackArg>::value;

    namespace impl {
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


    template<typename DataType, typename ValuePredicateArg,
        template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    using TypePackOpFoldWithIndexAndArg = impl::TypePackOpFoldWithIndexAndArgImpl<DataType, ValuePredicateArg, ValuePredicate, OpPredicate, TypePackArg>;

    template<typename DataType, typename ValuePredicateArg,
        template<typename, Size, typename  ValuePredicateArgType> typename ValuePredicate,
        template<DataType, DataType> typename OpPredicate,
        typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    constexpr static DataType TypePackOpFoldWithIndexAndArgValue = impl::TypePackOpFoldWithIndexAndArgImpl<DataType, ValuePredicateArg, ValuePredicate, OpPredicate, TypePackArg>::value;

    namespace impl {
        template<typename... TypePacks>
        struct TypePackMergeImplT {
            using type = TypePack<>;
        };
        template<typename CurrentTypePack, typename... RestTypePacks>
        struct TypePackMergeImplT<CurrentTypePack, RestTypePacks...> {
            using type = TypePackMergeTwo<CurrentTypePack, typename TypePackMergeImplT<RestTypePacks...>::type>;
        };
    }

    template<typename... TypePacks>
        requires(IsTypePackC<TypePacks> && ...)
    using TypePackMerge = impl::TypePackMergeImplT<TypePacks...>::type;

    namespace impl {
        template<typename BlendType>
        struct AsTypePackImplT {
            using type = TypePack<BlendType>;
        };
        template<typename... TypePackElementTypes>
        struct AsTypePackImplT<TypePack<TypePackElementTypes...>> {
            using type = TypePack<TypePackElementTypes...>;
        };
    }

    template<typename BlendType>
    using AsTypePack = impl::AsTypePackImplT<BlendType>::type;

    namespace impl {
        template<typename... BlendTypes>
        struct TypePackMergeBlendImplT {
            using type = TypePackMerge<AsTypePack<BlendTypes>...>;
        };
    }

    template<typename... BlendTypes>
    using TypePackMergeBlend = impl::TypePackMergeBlendImplT<BlendTypes...>::type;

    namespace impl {
        template <Size Count, typename... Types>
        struct TypePackTakeT;

        template <typename... Types>
        struct TypePackTakeT<0, TypePack<Types...>> {
            using type = TypePack<>;
            using rest = TypePack<Types...>;
        };

        template <typename FirstType, typename... RestTypes>
        struct TypePackTakeT<1, TypePack<FirstType, RestTypes...>> {
            using type = TypePack<FirstType>;
            using rest = TypePack<RestTypes...>;
        };

        template <Size Count, typename FirstType, typename... RestTypes>
        struct TypePackTakeT<Count, TypePack<FirstType, RestTypes...>> {
            using take_rest = TypePackTakeT<Count - 1, TypePack<RestTypes...>>;
            using type = TypePackMergeTwo<TypePack<FirstType>, typename take_rest::type>;
            using rest = typename take_rest::rest;
        };
    }

    template <Size Count, typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    using TypePackTake = typename impl::TypePackTakeT<Count, TypePackArg>::type;

    template <Size Count, typename... Types>
    using TemplatePackTake = typename impl::TypePackTakeT<Count, Types...>::type;

    namespace impl {
        template<template<typename, typename> typename TypeCompare, typename... Types>
        struct TypePackSortCompareT;

        template <template<typename, typename> typename TypeCompare, typename OneType>
        struct TypePackSortCompareT<TypeCompare, TypePack<OneType>> {
            using type = TypePack<OneType>;
        };

        template <template<typename, typename> typename TypeCompare, typename LhsType, typename RhsType>
        struct TypePackSortCompareT<TypeCompare, TypePack<LhsType, RhsType>> {
            using type = Conditional<
                TypeCompare<LhsType, RhsType>::value, 
                TypePack<LhsType, RhsType>, 
                TypePack<RhsType, LhsType>
            >;
        };

        template <template<typename, typename> typename TypeCompare, typename... Types>
        struct TypePackSortCompareT<TypeCompare, TypePack<Types...>> {
            static constexpr auto firstSize = sizeof...(Types) / 2;
            using split = TypePackTakeT<firstSize, TypePack<Types...>>;
            using type = TypePackMergeTwo<
                typename TypePackSortCompareT<TypeCompare, typename split::type>::type,
                typename TypePackSortCompareT<TypeCompare, typename split::rest>::type
            >;
        };
    }

    template <template<typename, typename> typename TypeCompare, typename TypePackArg>
        requires(IsTypePackC<TypePackArg>)
    using TypePackSortCompare = impl::TypePackSortCompareT<TypeCompare, TypePackArg>::type;

    template <template<typename, typename> typename TypeCompare, typename... Types>
    using TemplatePackSortCompare = impl::TypePackSortCompareT<TypeCompare, TypePack<Types...>>::type;

    template <typename DataType, DataType... Ints>
    struct IntegerSequence {
        using type = IntegerSequence;
        using value_type = DataType;
        using values_type_pack = TypePack<TypeValue<Ints>...>;
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
    template<typename... DataTypeTypes>
    using MakeIndexSequenceFor = MakeIndexSequence<sizeof...(DataTypeTypes)>;
    template<Size... Ints>
    using IndexSequence = IntegerSequence<Size, Ints...>;


    namespace impl {
        template<typename Type, Size Index>
        struct TypePackTypeAndIndexIdentityT {
            using type = Type;
            constexpr static Size index = Index;
        };

        template<typename, typename>
        struct TypePackUniformT;

        template<typename Type, Size... Indices>
        struct TypePackUniformT<Type, IndexSequence<Indices...>> {
            using type = TypePack<typename TypePackTypeAndIndexIdentityT<Type, Indices>::type...>;
        };
    }

    template<Size Number, typename Type>
    using TypePackUniform = impl::TypePackUniformT<Type, MakeIndexSequence<Number>>::type;

    namespace impl {
        template<Size Index>
        struct PackAtAnyType {
            template<typename Type>
            constexpr PackAtAnyType(Type) noexcept {}
        };
    }

    template<Size Index, typename... ArgTypes>
    constexpr decltype(auto) packAt(ArgTypes&&... args) noexcept {
        return[&]<Size... Indices>(IndexSequence<Indices...>) noexcept {
            return[]<typename AtArgType>(impl::PackAtAnyType<Indices>&&..., AtArgType&& atArg, auto&&...) noexcept {
                return atArg;
            }(natl::forward<ArgTypes>(args)...);
        }(MakeIndexSequence<Index>{});
    }

}