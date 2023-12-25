//header
#pragma once

//std
#include <type_traits>
#include <cstdint>

//own
#include "container.h"
#include "allocator.h"
#include "basicTypes.h"

//interface
namespace natl {
    template<class DataType>
    constexpr DataType* addressof(DataType& val) noexcept {
        return std::addressof<DataType>(val);
    }

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
        const void* aptr = reinterpret_cast<const void*>(&value);
        const void* bPtr = reinterpret_cast<const void*>(&testType);
        return std::memcmp(aptr, bPtr, sizeof(TestType)) == 0;
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
        if (!std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, Size>) {
                if (bitTestZero<ValueType>(value)) {
                    iterPtrMemset<Iter>(first, 0, countFormIterators<Iter, Size>(first, last));
                    return;
                }
            }
        }

        for (; first != last; ++first) {
            std::construct_at<typename IterPtrTraits<Iter>::value_type, ValueType>(&*first, std::forward<ValueType>(value));
        }
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFill(Iter first, Iter last, const ValueType& value)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        using IterValueType = typename std::iterator_traits<Iter>::value_type;

        if (!std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, Size>) {
                if (bitTestZero<ValueType>(value)) {
                    iterPtrMemset<Iter>(first, 0, countFormIterators<Iter, Size>(first, last));
                    return;
                }
            }
        }

        if constexpr (IsTriviallyMoveConstructible<typename IterPtrTraits<Iter>::value_type>) {
            ValueType tempValue = value;
            for (; first != last; ++first) {
                std::construct_at<IterValueType, ValueType>(&*first, std::forward<ValueType>(tempValue));
            }
            return;
        } else {
            for (; first != last; ++first) {
                if (std::is_constant_evaluated()) {
                    ValueType tempValue = value;
                    std::construct_at<IterValueType, ValueType>(&*first, std::forward<ValueType>(tempValue));
                } else {
                    ::new (static_cast<void*>(addressof<IterValueType>(*first))) IterValueType(value);
                }
            }
        }
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFillConvert(Iter first, Iter last, const ValueType& value)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        using IterValueType = typename std::iterator_traits<Iter>::value_type;
        IterValueType convertedValue = static_cast<IterValueType>(value);

        if constexpr (IsTriviallyMoveConstructible<typename IterPtrTraits<Iter>::value_type>) {
            IterValueType tempValue = convertedValue;
            for (; first != last; ++first) {
                std::construct_at<IterValueType, IterValueType>(&*first, std::forward<IterValueType>(tempValue));
            }
            return;
        }

        for (; first != last; ++first) {
            if (std::is_constant_evaluated()) {
                IterValueType tempValue = convertedValue;
                std::construct_at<IterValueType, IterValueType>(&*first, std::forward<IterValueType>(tempValue));
            } else {
                ::new (static_cast<void*>(addressof(*first))) IterValueType(static_cast<const IterValueType&>(convertedValue));
            }
        }
    }


    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFillCount(Iter first, const ValueType& value, const Size count)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        using IterValueType = typename std::iterator_traits<Iter>::value_type;

        if (!std::is_constant_evaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<Iter, ValueType> && CanGetSizeFormIterPtrSub<Iter, Size>) {
                if (bitTestZero<ValueType>(value)) {
                    iterPtrMemset<Iter>(first, 0, count);
                    return;
                }
            }
        }

        Size remainCount = count;
        if constexpr (IsTriviallyMoveConstructible<typename IterPtrTraits<Iter>::value_type>) {
            ValueType tempValue = value;
            for (; remainCount > 0; ++first, (void) --remainCount) {
                std::construct_at<IterValueType, ValueType>(&*first, std::forward<ValueType>(tempValue));
            }
            return;
        }

        for (; remainCount > 0; ++first, (void) --remainCount) {
            if (std::is_constant_evaluated()) {
                ValueType tempValue = value;
                std::construct_at<IterValueType, ValueType>(&*first, std::forward<ValueType>(tempValue));
            } else {
                ::new (static_cast<void*>(addressof(*first))) IterValueType(value);
            }
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

        for (Size index = 0; index < count; index++, dataPtr++) {
            std::construct_at<DataType>(dataPtr);
        }
    }

    template<class DataType>
    constexpr void defualtDeconstructAll(DataType* dataPtr, const Size count) 
        noexcept(std::is_nothrow_destructible_v<DataType>) {
        if (!std::is_constant_evaluated()) {
            if constexpr (IsTriviallyDestructible<DataType>) {
                iterPtrMemset<DataType*>(dataPtr, 0, count);
                return;
            }
        }

        for (Size index = 0; index < count; index++, dataPtr++) {
            std::destroy_at<DataType>(dataPtr);
        }
    }

    template<class DataType>
    constexpr void defualtDeconstruct(DataType* dataPtr)
        noexcept(std::is_nothrow_destructible_v<DataType>) {
        defualtDeconstructAll<DataType>(dataPtr, 1);
    }

    enum class AllocationMoveAdapaterRequireCopy : bool {
        v_false = false,
        v_true = true
    };
    enum class AllocationMoveAdapaterCanDealloc : bool {
        v_false = false,
        v_true = true
    };


    template<class DataType, class Alloc = DefaultAllocator<DataType>>
        requires(IsAllocator<Alloc> && isNotConst<DataType>)
    class AllocationMoveAdapater {
    public:
        using value_type = DataType;
        using reference = DataType&;
        using const_reference = const DataType&;
        using pointer = DataType*;
        using const_pointer = const DataType*;
        using difference_type = std::ptrdiff_t;
        using size_type = Size;

        using iterator = RandomAccessIterator<DataType>;
        using const_iterator = RandomAccessIterator<const DataType>;
        using reverse_iterator = ReverseRandomAccessIterator<DataType>;
        using const_reverse_iterator = ReverseRandomAccessIterator<const DataType>;

        //movement info  
        constexpr static bool triviallyRelocatable = true;
        constexpr static bool triviallyDefaultConstructible = true;
        constexpr static bool triviallyCompareable = true;
        constexpr static bool triviallyDestructible = true;
    private:
        mutable DataType* arrayDataPtr;
        Size arraySize;
        Size arrayCapacity;
        AllocationMoveAdapaterRequireCopy dataRequiresCopy;
        AllocationMoveAdapaterCanDealloc dataPtrCanBeDealloc;
    public:
        //constructor
        constexpr AllocationMoveAdapater() noexcept : arrayDataPtr(0), arraySize(0), arrayCapacity(0), 
            dataRequiresCopy(AllocationMoveAdapaterRequireCopy::v_false), dataPtrCanBeDealloc(AllocationMoveAdapaterCanDealloc::v_false) {}
        constexpr AllocationMoveAdapater(
            DataType* dataPtr, const Size arraySize, const Size arrayCapacity,
            const AllocationMoveAdapaterRequireCopy dataRequiresCopy, const AllocationMoveAdapaterCanDealloc dataPtrCanBeDealloc) noexcept :
            arrayDataPtr(dataPtr), arraySize(arraySize), arrayCapacity(arrayCapacity), dataRequiresCopy(dataRequiresCopy), dataPtrCanBeDealloc(dataPtrCanBeDealloc) {}

        //destructor
        ~AllocationMoveAdapater() noexcept = default;

        //util 
        constexpr AllocationMoveAdapater& self() noexcept { return *this; }
        constexpr const AllocationMoveAdapater& self() const noexcept { return *this; }

        //capacity 
        constexpr bool empty() const noexcept { return size() == 0; }
        constexpr bool isEmpty() const noexcept { return empty(); }
        constexpr bool isNotEmpty() const noexcept { return !empty(); }

        constexpr Size size() const noexcept { return arraySize; }
        constexpr Size capacity() const noexcept { return arrayCapacity; }

        //flags 
        constexpr bool requiresCopy() const noexcept {
            return static_cast<bool>(dataRequiresCopy);
        }
        constexpr bool canDealloc() const noexcept {
            return static_cast<bool>(dataPtrCanBeDealloc);
        }

        //element access 
        constexpr pointer data() const noexcept { return arrayDataPtr; }

        //iterators 
        constexpr pointer beginPtr() const noexcept { return data(); }
        constexpr pointer endPtr() const noexcept { return data() + size(); }

        constexpr iterator begin() const noexcept { return iterator(beginPtr()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
        constexpr iterator end() const noexcept { return iterator(endPtr()); }
        constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
        constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator(endPtr()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
        constexpr reverse_iterator rend() const noexcept { return reverse_iterator(beginPtr()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

        //allocation 
        constexpr void deallocate() const noexcept {
            if (arrayDataPtr) {
                destructAll();
                Alloc::deallocate(arrayDataPtr, capacity());
            }
        }

        //modifier
        constexpr void destructAll() const noexcept {
            if (!IsTriviallyDestructible<DataType>) {
                if (!std::is_constant_evaluated()) {
                    return;
                }
            }
            if (isNotEmpty()) {
                defualtDeconstructAll(arrayDataPtr, size());
            }
        }
    };
}