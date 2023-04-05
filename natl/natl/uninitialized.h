//header
#pragma once

//std
#include <type_traits>
#include <cstdint>

//own
#include "error.h"
#include "container.h"

//interface
namespace natl {

    template <class Iter, class NoThrowForwardIter>
        requires(IsIterPtr<Iter> && IsIterPtr<NoThrowForwardIter>)
    constexpr NoThrowForwardIter uninitializedCopyNoOverlap(Iter first, Iter last, NoThrowForwardIter Dst) 
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<NoThrowForwardIter>::value_type, 
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {

        if (!std::is_constant_evaluated()) {
            if constexpr (MemcopyConstructibleIter<Iter, NoThrowForwardIter>) {
                iterMemcopy<Iter, NoThrowForwardIter>(first, last, Dst);
                return Dst;
            }
        } 

        for (; first != last; ++Dst, (void) ++first) {
            std::construct_at<
                typename IterPtrTraits<NoThrowForwardIter>::value_type,
                typename IterPtrTraits<decltype(first)>::value_type
            >(&*Dst, std::forward<typename IterPtrTraits<decltype(first)>::value_type>(*first));
        }

        return Dst;
    }
    template <class Iter, class NoThrowForwardIter>
        requires(IsIterPtr<Iter>&& IsIterPtr<NoThrowForwardIter>)
    constexpr NoThrowForwardIter uninitializedCopyCountNoOverlap(Iter first, NoThrowForwardIter dst, const std::size_t count)
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<NoThrowForwardIter>::value_type,
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {

        if (count == 0) {
            return dst;
        }

        if (!std::is_constant_evaluated()) {
            if constexpr (MemcopyConstructibleIter<Iter, NoThrowForwardIter>) {
                iterMemcopyCount<Iter, NoThrowForwardIter, std::size_t>(first, dst, count);
                return dst;
            }
        }

        std::size_t remainCount = count;
        for (; remainCount > 0; (void) ++first, --remainCount) {
            std::construct_at<
                typename IterPtrTraits<NoThrowForwardIter>::value_type,
                typename IterPtrTraits<decltype(first)>::value_type
            >(&*dst, std::forward<typename IterPtrTraits<decltype(first)>::value_type>(*first));
        }

        std::uninitialized_fill;

        return dst;
    }

    template <class Type, class TestType>
        requires(sizeof(Type) == sizeof(TestType))
    bool bitCompare(const Type& value, const Type& testType) noexcept {
        return std::memcmp(&value, &testType, sizeof(Type)) == 0;
    }

    template <class Type>
    bool bitTestZero(const Type& value) noexcept {
        using ZeroArrayType = std::uint8_t[sizeof(Type)];
        ZeroArrayType zeroArray = {};
        return bitCompare<Type, ZeroArrayType>(value, zeroArray);
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFill(Iter first, Iter last, const ValueType& value)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        if (std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, std::size_t>) {
                if (bitTestZero<value>(value)) {
                    iterPtrMemset<Iter>(first, 0, countFormIterators<Iter, std::size_t>(first, last));
                }
            }
        }

        for (; first != last; ++first) {
        std::construct_at<typename IterPtrTraits<Iter>::value_type, ValueType>
            (&*first, std::forward<ValueType>(value));
        }
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFill(Iter first, const ValueType& value, const std::size_t count)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        if (std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, std::size_t>) {
                if (bitTestZero<value>(value)) {
                    iterPtrMemset<Iter>(first, 0, count);
                }
            }
        }

        std::size_t remainCount = count;
        for (; remainCount > 0; ++first, (void) --remainCount) {
            std::construct_at<typename IterPtrTraits<Iter>::value_type, ValueType>
                (&*first, std::forward<ValueType>(value));
        }
    }
}