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
    struct ParameterPackMaxSizeOfTypes;

    template <class T>
    struct ParameterPackMaxSizeOfTypes<T> {
        constexpr static Size value = sizeof(T);
    };

    template <class T, class... Ts>
    struct ParameterPackMaxSizeOfTypes<T, Ts...> {
        constexpr static Size value = sizeof(T) > ParameterPackMaxSizeOfTypes<Ts...>::value ? sizeof(T) : ParameterPackMaxSizeOfTypes<Ts...>::value;
    };


    template <Size Index, class... Types>
    struct ParameterPackNthElement {
        using type = void;
    };

    template <class NthType, class... Types>
    struct ParameterPackNthElement<0, NthType, Types...> {
        using type = NthType;
    };

    template <Size Index, class NotType, class... Types>
        requires(Index < (sizeof...(Types) + 1))
    struct ParameterPackNthElement<Index, NotType, Types...> {
        using type = ParameterPackNthElement<Index - 1, Types...>::type;
    };

    template <Size Index, class FindType, class... Types>
    struct ParameterPackFindIndexOfTypeImpl {
        constexpr static Size value = limits<Size>::max();
    };

    template <Size Index, class FindType, class TestType, class... Types>
    struct ParameterPackFindIndexOfTypeImpl<Index, FindType, TestType, Types...> {
        constexpr static Size value = std::is_same_v<FindType, TestType> ? Index : ParameterPackFindIndexOfTypeImpl<Index + 1, FindType, Types...>::value;
    };

    template <class FindType, class... Types>
    struct ParameterPackFindIndexOfType {
        constexpr static Size value = ParameterPackFindIndexOfTypeImpl<0, FindType, Types...>::value;
    };
    struct IndexNotFound {
        constexpr static Size value = limits<Size>::max();
    };

    constexpr bool findImplCompareStrings(const char a[], const char b[]) {
        const char* aPtr = a;
        const char *bPtr = b;
        while (*aPtr != '\0' && *bPtr != '\0' && *aPtr == *bPtr) {
            ++aPtr;
            ++bPtr;
        }
        return *aPtr == *bPtr;

    }

    template <Size Index, class FindType, class... Types>
    struct ParameterPackStringLiteralFindImpl {
        constexpr static Size value = limits<Size>::max();
    };

    template <Size Index, class FindType, class TestType, class... Types>
    struct ParameterPackStringLiteralFindImpl<Index, FindType, TestType, Types...> {
        constexpr static Size value = (std::is_same_v<FindType, TestType> && findImplCompareStrings(FindType::name, TestType::name))? Index : ParameterPackStringLiteralFindImpl<Index + 1, FindType, Types...>::value;
    };

    template <class FindType, class... Types>
    struct ParameterPackStringLiteralFindIndex {
        constexpr static Size value = ParameterPackStringLiteralFindImpl<0, FindType, Types...>::value;
    };
}