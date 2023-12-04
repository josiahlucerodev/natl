#pragma once 

#include "basicTypes.h"

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

    template <Size, typename T> using AlwaysType = T;
    template <typename NumberType, NumberType, typename Type> using CustomAlwaysType = Type;
}