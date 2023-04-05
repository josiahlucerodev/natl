#pragma once 

//std
#include <type_traits>
#include <memory>

//interator
namespace natl {
    template<class T>
    class UninitilizedValue {
    public:
        union InternalValue {
        public:
            constexpr InternalValue() {}
            constexpr ~InternalValue() {};
            InternalValue() requires(std::is_trivially_constructible_v<T> && !std::is_constant_evaluated()) = default;
            ~InternalValue() requires(std::is_trivially_destructible_v<T> && !std::is_constant_evaluated()) = default;
            T value;
        };

        InternalValue internalValue;

        constexpr UninitilizedValue() {}
        constexpr ~UninitilizedValue() {}
        UninitilizedValue() requires(std::is_trivially_constructible_v<T>) = default;
        ~UninitilizedValue() requires(std::is_trivially_destructible_v<T>) = default;

        constexpr T& value() { return internalValue.value; }
        constexpr T* valuePtr() { return &internalValue.value; }
        constexpr void defaultConstruct() { std::construct_at<T>(&value()); }
        constexpr void defaultDestruct() { std::destroy_at<T>(&value()); }
    };
}