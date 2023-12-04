//header
#pragma once

//std
#include <type_traits>
#include <cstdint>

//own
#include "container.h"
#include "basicTypes.h"

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
    constexpr NoThrowForwardIter uninitializedCopy(Iter first, Iter last, NoThrowForwardIter Dst)
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<NoThrowForwardIter>::value_type,
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {
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
    constexpr NoThrowForwardIter uninitializedCopyConvert(Iter first, Iter last, NoThrowForwardIter Dst)
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<NoThrowForwardIter>::value_type,
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {
        for (; first != last; ++Dst, (void) ++first) {
            std::construct_at<
                typename IterPtrTraits<NoThrowForwardIter>::value_type,
                typename IterPtrTraits<NoThrowForwardIter>::value_type
            >(&*Dst, std::forward<typename IterPtrTraits<NoThrowForwardIter>::value_type>(static_cast<typename IterPtrTraits<NoThrowForwardIter>::value_type>(*first)));
        }
        return Dst;
    }

    

    template <class Iter, class NoThrowForwardIter>
        requires(IsIterPtr<Iter>&& IsIterPtr<NoThrowForwardIter>)
    constexpr NoThrowForwardIter uninitializedCopyCountNoOverlap(Iter first, NoThrowForwardIter dst, const Size count)
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<Iter>::value_type,
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {

        if (count == 0) {
            return dst;
        }

        if (!std::is_constant_evaluated()) {
            if constexpr (MemcopyConstructibleIter<Iter, NoThrowForwardIter>) {
                iterMemcopyCount<Iter, NoThrowForwardIter, Size>(first, dst, count);
                return dst;
            }
        }

        for (Size index = 0; index < count; (void) ++first, index++) {
            std::construct_at<
                typename IterPtrTraits<NoThrowForwardIter>::value_type,
                typename IterPtrTraits<decltype(first)>::value_type
            >(((& *dst) + index), std::forward<typename IterPtrTraits<decltype(first)>::value_type>(*first));
        }

        return dst;
    }

    template <class Type, class TestType>
        requires(sizeof(Type) == sizeof(TestType))
    bool bitCompare(const Type& value, const TestType& testType) noexcept {
        return std::memcmp(&value, &testType, sizeof(TestType)) == 0;
    }

    template <class Type>
    bool bitTestZero(const Type& value) noexcept {
        using ZeroArrayType = std::uint8_t[sizeof(Type)];
        ZeroArrayType zeroArray = {};
        return bitCompare<Type, ZeroArrayType>(value, zeroArray);
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFill(Iter first, Iter last, ValueType&& value)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        if (std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, Size>) {
                if (bitTestZero<ValueType>(value)) {
                    iterPtrMemset<Iter>(first, 0, countFormIterators<Iter, Size>(first, last));
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
    constexpr void uninitializedFillConvert(Iter first, Iter last, ValueType&& value)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        for (; first != last; ++first) {
            std::construct_at<typename IterPtrTraits<Iter>::value_type, typename IterPtrTraits<Iter>::value_type>
                (&*first, std::forward<typename IterPtrTraits<Iter>::value_type>(static_cast<typename IterPtrTraits<Iter>::value_type>(value)));
        }
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFillCount(Iter first, ValueType&& value, const Size count)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        if (std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, Size>) {
                if (bitTestZero<ValueType>(value)) {
                    iterPtrMemset<Iter>(first, 0, count);
                    return;
                }
            }
        }

        Size remainCount = count;
        for (; remainCount > 0; ++first, (void) --remainCount) {
            std::construct_at<typename IterPtrTraits<Iter>::value_type, ValueType>(&*first, std::forward<ValueType>(value));
        }
    }

    template <class Iter, class NoThrowForwardIter>
        requires(IsIterPtr<Iter>&& IsIterPtr<NoThrowForwardIter>)
    constexpr NoThrowForwardIter copyNoOverlap(Iter first, Iter last, NoThrowForwardIter Dst)
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
            *(&*Dst) = *first;
        }

        return Dst;
    }
    template <class Iter, class NoThrowForwardIter>
        requires(IsIterPtr<Iter>&& IsIterPtr<NoThrowForwardIter>)
    constexpr NoThrowForwardIter copyCountNoOverlap(Iter first, NoThrowForwardIter dst, const Size count)
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<NoThrowForwardIter>::value_type,
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {

        if (count == 0) {
            return dst;
        }

        if (!std::is_constant_evaluated()) {
            if constexpr (MemcopyConstructibleIter<Iter, NoThrowForwardIter>) {
                iterMemcopyCount<Iter, NoThrowForwardIter, Size>(first, dst, count);
                return dst;
            }
        }

        for (Size index = 0; index < count; (void) ++first, index++) {
            *(dst + index) = *first;
        }

        return dst;
    }

    template <class Iter, class NoThrowForwardIter>
        requires(IsIterPtr<Iter>&& IsIterPtr<NoThrowForwardIter>)
    constexpr NoThrowForwardIter copy(Iter first, Iter last, NoThrowForwardIter Dst)
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<NoThrowForwardIter>::value_type,
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {

        for (; first != last; ++Dst, (void) ++first) {
            *(&*Dst) = *first;
        }

        return Dst;
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void fill(Iter first, Iter last, const ValueType& value)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        if (!std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, Size>) {
                if (bitTestZero<ValueType>(value)) {
                    iterPtrMemset<Iter>(first, 0, countFormIterators<Iter, Size>(first, last));
                    return;
                }
            }
        }

        for (; first != last; ++first) {
            *first = value;
        }
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void fillCount(Iter first, const ValueType& value, const Size count)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        if (!std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, Size>) {
                if (bitTestZero<ValueType>(value)) {
                    iterPtrMemset<Iter>(first, 0, count);
                    return;
                }
            }
        }

        Size remainCount = count;
        for (; remainCount > 0; ++first, (void) --remainCount) {
            *first = value;
        }
    }

    template <class AIter, class BIter>
        requires(IsIterPtr<AIter> && IsIterPtr<BIter>)
    constexpr bool compareCount(AIter aIter, BIter bIter, const Size count) noexcept {
        if (count == 0) {
            return false;
        }

        if (!std::is_constant_evaluated()) {
            if constexpr (MemcpyCompareIter<AIter, BIter>) {
                return iterPtrMemcpy<AIter, BIter>(aIter, bIter, count);
            }
        }

        for (Size index = 0; index < count; index++, ++aIter, ++bIter) {
            if (*aIter != bIter) {
                return false;
            }
        }

        return true;
    }


    template<class DataType>
    constexpr void defualtConstructAll(DataType* dataPtr, const Size count) 
        noexcept(std::is_nothrow_constructible_v<DataType>) {

        if (!std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<DataType*, DataType> && CanGetSizeFormIterPtrSub<DataType*, Size>) {
                iterPtrMemset<DataType*>(dataPtr,0, count);
                return;
            }
        }

        for (Size index = 0; index < count; index++) {
            std::construct_at<DataType>(&dataPtr[index]);
        }
    }

    template<class DataType>
    constexpr void defualtDeconstructAll(DataType* dataPtr, const Size count) 
        noexcept(std::is_nothrow_destructible_v<DataType>) {
        if (!std::is_constant_evaluated()) {
            if constexpr (std::is_trivially_destructible_v<DataType>) {
                iterPtrMemset<DataType*>(dataPtr, 0, count);
            }
        }

        for (Size index = 0; index < count; index++) {
            std::destroy_at<DataType>(dataPtr);
        }
    }

    template<class DataType> 
    constexpr DataType* addressof(DataType& val) noexcept {
        return std::addressof<DataType>(val);
    }
}