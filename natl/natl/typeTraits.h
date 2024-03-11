#pragma once

//std
#include <concepts>
#include <type_traits>

//own
#include "basicTypes.h"

//interface
namespace natl {
    template<class Type>
    concept isNotConst = !std::is_const_v<Type>; 
    template<class Type>
    concept isConst = std::is_const_v<Type>;

    template<class Type>
    concept IsEmpty = std::is_empty_v<Type>;

    template<class Type>
    concept IsNotEmpty = !std::is_empty_v<Type>;

    template<class Type>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

    template<class LhsType, class RhsType>
    concept IsTheSame = std::is_same_v<LhsType, RhsType>;

    template<class LhsType, class RhsType>
    concept IsNotTheSame = !IsTheSame<LhsType, RhsType>;

    template<class DataType>
    concept IsItergerType = std::is_integral_v<DataType>;

    template<class DataType>
    concept IsSignedItergerType = IsItergerType<DataType> && std::is_signed_v<DataType>;

    template<class DataType>
    concept IsUnsignedItergerType = IsItergerType<DataType> && std::is_unsigned_v<DataType>;

    template<class DataType>
    concept IsNotItergerType = !IsItergerType<DataType>;

    template<class DataType>
    concept IsFloatingPointType = std::is_floating_point_v<DataType>;

    template<class DataType>
    concept IsNotFloatingPointType = !std::is_floating_point_v<DataType>;

    template<class DataType>
    concept IsNumericType = IsItergerType<DataType> || IsFloatingPointType<DataType>;

    template <typename T>
    concept IsCharacterType =
        std::is_same_v<std::remove_cv_t<T>, AssciCode> ||
        std::is_same_v<std::remove_cv_t<T>, char8_t> ||
        std::is_same_v<std::remove_cv_t<T>, char16_t> ||
        std::is_same_v<std::remove_cv_t<T>, Utf32>;

    template <class DataType>
    concept IsBasicType = std::is_integral_v<DataType> || std::is_floating_point_v<DataType> || std::is_pointer_v<DataType> || IsCharacterType<DataType> || std::same_as<DataType, Byte>;

    template <typename DataType>
    concept IsCopyConstructible = std::is_copy_constructible_v<std::decay_t<DataType>>;

    template <typename DataType>
    concept IsCopyAssignable = std::is_copy_assignable_v<std::decay_t<DataType>>;

    template <typename DataType>
    concept IsMoveConstructible = std::is_move_constructible_v<std::decay_t<DataType>>;

    namespace impl {
#define NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(conceptName, memberName) \
        template<class DataType> \
        consteval bool customCheckIf##conceptName() noexcept { \
            if constexpr (requires() { { DataType::##memberName } -> std::convertible_to<bool>; }) { \
                return DataType::##memberName; \
            } else { \
                return false; \
            } \
        }

        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyCompareable, triviallyCompareable);
        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyRelocatable, triviallyRelocatable);
        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyDefaultConstructible, triviallyDefaultConstructible);
        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyDestructible, triviallyDestructible);
        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyMoveConstructible, triviallyMoveConstructedable);
        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyConstRefConstructible, triviallyConstRefConstructedable);
        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyMoveAssignable, triviallyMoveAssignable);
        NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN(IsTriviallyConstRefAssignable, triviallyConstRefAssignable);

    }
#undef NATL_TYPE_TRIAT_CUSTOM_TRIVIALLY_FUN

    template <class DataType>
    concept IsTriviallyCompareable = IsBasicType<DataType> || 
        impl::customCheckIfIsTriviallyCompareable<DataType>();

    template <class DataType>
    concept IsTriviallyRelocatable = (std::is_trivially_copyable_v<DataType> && std::is_trivially_destructible_v<DataType>) || 
        IsBasicType<DataType> || impl::customCheckIfIsTriviallyRelocatable<DataType>();

    template <class DataType>
    concept IsTriviallyDefaultConstructible = std::is_trivially_default_constructible_v<DataType> || 
        impl::customCheckIfIsTriviallyDefaultConstructible<DataType>();

    template <class DataType>
    concept IsTriviallyDestructible = std::is_trivially_destructible_v<DataType> || 
        impl::customCheckIfIsTriviallyDestructible<DataType>();

    template <class DataType>
    concept IsTriviallyMoveConstructible = std::is_trivially_move_constructible_v<DataType> || 
        impl::customCheckIfIsTriviallyMoveConstructible<DataType>();

    template <class DataType>
    concept IsTriviallyConstRefConstructible = std::is_trivially_constructible_v<DataType, const DataType&> ||
        impl::customCheckIfIsTriviallyConstRefConstructible<DataType>();

    template <class DataType>
    concept IsTriviallyMoveAssignable = std::is_trivially_move_assignable_v<DataType> ||
        impl::customCheckIfIsTriviallyMoveAssignable<DataType>();

    template <class DataType>
    concept IsTriviallyConstRefAssignable = std::is_trivially_assignable_v<DataType, const DataType&> ||
        impl::customCheckIfIsTriviallyConstRefAssignable<DataType>();

    template <class Type>
    concept MemcopyConstructible = IsTriviallyDefaultConstructible<Type> && IsTriviallyRelocatable<Type>;

    template <class Src, class Dst, class SrcRef>
    concept MemcopyConstructibleSrcDst =
        !std::is_same_v<Src, void> &&
        !std::is_same_v<Dst, void> &&
        !std::is_same_v<SrcRef, void> &&
            (
                (std::is_same_v<std::decay_t<Src>, std::decay_t<Dst>> && MemcopyConstructible<Src>) ||
                (sizeof(Src) == sizeof(Dst) && std::is_trivially_constructible_v<Dst, SrcRef>)
            );

    template<class Src, class Dst, class SrcRef, class DstRef>
    concept MemcopyAssignableSrcDst =
        (std::is_same_v<Src, Dst> && MemcopyConstructible<Src>) ||
        (sizeof(Src) == sizeof(Dst) && std::is_trivially_assignable_v<DstRef, SrcRef>);

    template <class Type>
    concept MemcpyCompareable = IsTriviallyCompareable<Type>;

    template<class Src, class Dst>
    concept MemcpyCompareableSrcDst = (std::is_same_v<Src, Dst> && MemcpyCompareable<Src>);

    struct SynthesizeThreeWay {
        template <class T1, class T2>
        constexpr auto operator()(const T1& lhs, const T2& rhs) const
            requires(requires {
                { lhs < rhs } -> std::convertible_to<bool>;
                { rhs < lhs } -> std::convertible_to<bool>; }) {
            if constexpr (std::three_way_comparable_with<T1, T2>) {
                return lhs <=> rhs;
            } else {
                if (lhs < rhs) {
                    return std::weak_ordering::less;
                } else if (rhs < lhs) {
                    return std::weak_ordering::greater;
                } else {
                    return std::weak_ordering::equivalent;
                }
            }
        }
    };

    template <class T1, class T2 = T1>
    using SynthesizeThreeWayResult = decltype(SynthesizeThreeWay{}(std::declval<T1&>(), std::declval<T2&>()));

    template <typename From, typename To>
    concept IsPolymorphicCastable = std::is_base_of_v<From, To> || std::is_convertible_v<From*, To*>;

    namespace impl {
        template<typename T, template<typename...> typename Primary>
        struct IsSpecialization : std::false_type {};

        template<template<typename...> typename Primary, typename... Args>
        struct IsSpecialization<Primary<Args...>, Primary> : std::true_type {};

        template<typename T, template<typename...> typename Primary>
        inline constexpr bool IsSpecializationV = IsSpecialization<T, Primary>::value;
    }

    template<typename Test, template<typename...> class SpecializationType>
    concept IsSpecialization = impl::IsSpecializationV<Test, SpecializationType>;

    template<typename... ArgTypes>
    concept IsSameByteSize = (sizeof(ArgTypes) == ...);
}