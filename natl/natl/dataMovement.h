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
    template<typename DataType>
    constexpr DataType* addressof(DataType& val) noexcept {
        return std::addressof<DataType>(val);
    }
    constexpr Bool isInBufferRegion(const Size bufferSize, const Size offset, const Size count) noexcept {
        const Size endOffset = offset + count;
        return endOffset < bufferSize;
    }

    template <class Iter, class NoThrowForwardIter>
        requires(IsIterPtr<Iter> && IsIterPtr<NoThrowForwardIter>)
    constexpr NoThrowForwardIter uninitializedCopyNoOverlap(Iter first, Iter last, NoThrowForwardIter Dst) 
        noexcept(std::is_nothrow_convertible_v<
            typename IterPtrTraits<NoThrowForwardIter>::value_type, 
            typename IterPtrTraits<NoThrowForwardIter>::value_type>) {

        if (!isConstantEvaluated()) {
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

        if (!isConstantEvaluated()) {
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
    Bool bitCompare(const Type& value, const TestType& testType) noexcept {
        const void* aptr = reinterpret_cast<const void*>(&value);
        const void* bPtr = reinterpret_cast<const void*>(&testType);
        return std::memcmp(aptr, bPtr, sizeof(TestType)) == 0;
    }

    template <class Type>
    Bool bitTestZero(const Type& value) noexcept {
        using ZeroArrayType = std::uint8_t[sizeof(Type)];
        ZeroArrayType zeroArray = {};
        return bitCompare<Type, ZeroArrayType>(value, zeroArray);
    }

    template <class Iter, class ValueType>
        requires(IsIterPtr<Iter>)
    constexpr void uninitializedFill(Iter first, Iter last, ValueType&& value)
        noexcept(std::is_nothrow_convertible_v<typename IterPtrTraits<Iter>::value_type, ValueType>) {
        if (!isConstantEvaluated()) {
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

        if (!isConstantEvaluated()) {
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
                if (isConstantEvaluated()) {
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
            if (isConstantEvaluated()) {
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

        if (!isConstantEvaluated()) {
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
            if (isConstantEvaluated()) {
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

        if (!isConstantEvaluated()) {
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

        if (!isConstantEvaluated()) {
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
        if (!isConstantEvaluated()) {
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
        if (!isConstantEvaluated()) {
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
    constexpr Bool compareCount(AIter aIter, BIter bIter, const Size count) noexcept {
        if (count == 0) {
            return false;
        }

        if (!isConstantEvaluated()) {
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

    template<typename DataType, typename... ConstructArgTypes>
    constexpr DataType* construct(DataType* location, ConstructArgTypes&&... constructArgs) noexcept {
        if (!isConstantEvaluated()) {
            return ::new (static_cast<void*>(location)) DataType(natl::forward<ConstructArgTypes>(constructArgs)...);
        } else {
            return std::construct_at<DataType, ConstructArgTypes...>(location, natl::forward<ConstructArgTypes>(constructArgs)...);
        }
    }

    template<typename DataType>
    constexpr void defaultConstructAll(DataType* dataPtr, const Size count) 
        noexcept(std::is_nothrow_constructible_v<DataType>) {

        if (!isConstantEvaluated()) {
            if constexpr (IsIterPtrZeroMemsetAble<DataType*, DataType> && CanGetSizeFormIterPtrSub<DataType*, Size>) {
                iterPtrMemset<DataType*>(dataPtr,0, count);
                return;
            }
        }

        for (Size index = 0; index < count; index++, dataPtr++) {
            std::construct_at<DataType>(dataPtr);
        }
    }

    template<typename DataType>
    constexpr void deconstructAll(DataType* dataPtr, const Size count) 
        noexcept(std::is_nothrow_destructible_v<DataType>) {
        if (!isConstantEvaluated()) {
            if constexpr (IsTriviallyDestructible<DataType>) {
                iterPtrMemset<DataType*>(dataPtr, 0, count);
                return;
            }
        }

        for (Size index = 0; index < count; index++, dataPtr++) {
            std::destroy_at<DataType>(dataPtr);
        }
    }

    template<typename DataType>
    constexpr void deconstruct(DataType* dataPtr)
        noexcept(std::is_nothrow_destructible_v<DataType>) {
        deconstructAll<DataType>(dataPtr, 1);
    }

    template<typename DataType>
    constexpr Bool typeHasToBeDestructed() noexcept {
        return isConstantEvaluated() || !IsTriviallyDestructible<DataType>;
    }


    template<typename DataType>
    constexpr static Size TypeBitSize = sizeof(DataType) * Size(8);
    template<typename DataType>
    constexpr static Size TypeByteSize = sizeof(DataType);

    enum class AllocationMoveAdapaterRequireCopy : Bool {
        False = false,
        True = true
    };
    enum class AllocationMoveAdapaterCanDealloc : Bool {
        False = false,
        True = true
    };


    template<typename DataType, typename Alloc = DefaultAllocator<DataType>>
        requires(IsAllocator<Alloc> && IsNotConst<DataType>)
    class AllocationMoveAdapater {
    public:
        using allocator_type = Alloc;

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
        constexpr static Bool triviallyRelocatable = true;
        constexpr static Bool triviallyDefaultConstructible = true;
        constexpr static Bool triviallyCompareable = true;
        constexpr static Bool triviallyDestructible = true;

        template<typename OtherDataType>
        using rebind_allocation_move_adapater = AllocationMoveAdapater<OtherDataType, typename Alloc::template rebind_alloc<OtherDataType>>;

    private:
        DataType* arrayDataPtr;
        Size arraySize;
        Size arrayCapacity;
        AllocationMoveAdapaterRequireCopy dataRequiresCopy;
        AllocationMoveAdapaterCanDealloc dataPtrCanBeDealloc;
    public:
        //constructor
        constexpr AllocationMoveAdapater(const AllocationMoveAdapater& other) noexcept = delete;
        constexpr AllocationMoveAdapater(AllocationMoveAdapater&& other) noexcept {
            arrayDataPtr = other.arrayDataPtr;
            arraySize = other.arraySize;
            arrayCapacity = other.arrayCapacity;
            dataRequiresCopy = other.dataRequiresCopy;
            dataPtrCanBeDealloc = other.dataPtrCanBeDealloc;
            other.release();
        }
        constexpr AllocationMoveAdapater() noexcept : arrayDataPtr(0), arraySize(0), arrayCapacity(0), 
            dataRequiresCopy(AllocationMoveAdapaterRequireCopy::False), dataPtrCanBeDealloc(AllocationMoveAdapaterCanDealloc::False) {}
        constexpr AllocationMoveAdapater(
            DataType* dataPtr, const Size arraySizeIn, const Size arrayCapacityIn,
            const AllocationMoveAdapaterRequireCopy requiresCopy, const AllocationMoveAdapaterCanDealloc ptrCanBeDealloc) noexcept :
            arrayDataPtr(dataPtr), arraySize(arraySizeIn), arrayCapacity(arrayCapacityIn), dataRequiresCopy(requiresCopy), dataPtrCanBeDealloc(ptrCanBeDealloc) {}

        //destructor
        constexpr ~AllocationMoveAdapater() noexcept {
            deallocate();
        };

        //util 
        constexpr AllocationMoveAdapater& self() noexcept { return *this; }
        constexpr const AllocationMoveAdapater& self() const noexcept { return *this; }

        //assignment 
        constexpr AllocationMoveAdapater& operator=(const AllocationMoveAdapater& other) noexcept = delete;
        constexpr AllocationMoveAdapater& operator=(AllocationMoveAdapater&& other) noexcept {
            arrayDataPtr = other.arrayDataPtr;
            arraySize = other.arraySize;
            arrayCapacity = other.arrayCapacity;
            dataRequiresCopy = other.dataRequiresCopy;
            dataPtrCanBeDealloc = other.dataPtrCanBeDealloc;
            other.release();
        }

        //capacity 
        constexpr Bool empty() const noexcept { return size() == 0; }
        constexpr Bool isEmpty() const noexcept { return empty(); }
        constexpr Bool isNotEmpty() const noexcept { return !empty(); }

        constexpr Size size() const noexcept { return arraySize; }
        constexpr Size capacity() const noexcept { return arrayCapacity; }

        //flags 
        constexpr Bool requiresCopy() const noexcept {
            return static_cast<Bool>(dataRequiresCopy);
        }
        constexpr Bool canDealloc() const noexcept {
            return static_cast<Bool>(dataPtrCanBeDealloc);
        }

        constexpr AllocationMoveAdapaterRequireCopy getRequiresCopyFlag() noexcept {
            return dataRequiresCopy;
        }
        constexpr AllocationMoveAdapaterCanDealloc getCanDeallocFlag() noexcept {
            return dataPtrCanBeDealloc;
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
        constexpr void deallocate() noexcept {
            if (canDealloc() && arrayDataPtr) {
                destructAll();
                Alloc::deallocate(arrayDataPtr, capacity());
                release();
            }
        }

        //modifier
        constexpr void destructAll() noexcept {
            if (!IsTriviallyDestructible<DataType>) {
                if (!isConstantEvaluated()) {
                    return;
                }
            }
            if (isNotEmpty()) {
                deconstructAll(arrayDataPtr, size());
            }
        }
        constexpr void release() noexcept {
            arrayDataPtr = nullptr;
            arraySize = 0;
            dataRequiresCopy = AllocationMoveAdapaterRequireCopy::False;
            dataPtrCanBeDealloc = AllocationMoveAdapaterCanDealloc::False;
        }
    };


    template<typename ToDataType, typename FromAllocationMoveAdapaterType>
        requires(
            IsSpecializationC<FromAllocationMoveAdapaterType, AllocationMoveAdapater> &&
            IsSameByteSize<ToDataType, typename FromAllocationMoveAdapaterType::value_type>)
        constexpr typename FromAllocationMoveAdapaterType::template rebind_allocation_move_adapater<ToDataType>
        equivalentCastAllocationMoveAdapter(FromAllocationMoveAdapaterType&& allocationMoveAdapter) noexcept {

        using ToAllocationMoveAdapterType = typename FromAllocationMoveAdapaterType::template rebind_allocation_move_adapater<ToDataType>;
        using ToAlloc = ToAllocationMoveAdapterType::allocator_type;
        //using FromAlloc = FromAllocationMoveAdapaterType::allocator_type;
        using FromDataType = FromAllocationMoveAdapaterType::value_type;

        if constexpr (IsSame<ToAllocationMoveAdapterType, FromAllocationMoveAdapaterType>) {
            return natl::move(allocationMoveAdapter);
        } else if constexpr (isConstantEvaluated()) {
            ToDataType* newDataPtr = ToAlloc::allocate(allocationMoveAdapter.capacity());
            ToAllocationMoveAdapterType newAllocationMoveAdapter(
                newDataPtr, allocationMoveAdapter.size(), allocationMoveAdapter.capacity(),
                AllocationMoveAdapaterRequireCopy::False, AllocationMoveAdapaterCanDealloc::True);

            FromDataType* copySrcPtr = allocationMoveAdapter.data();
            FromDataType* copySrcPtrEnd = copySrcPtr + allocationMoveAdapter.size();
            ToDataType* copyDstPtr = newDataPtr;

            for (; copySrcPtr < copySrcPtrEnd; copySrcPtr++, copyDstPtr++) {
                *copyDstPtr = std::bit_cast<ToDataType, FromDataType>(*copySrcPtr);
            }

            allocationMoveAdapter.deallocate();
            allocationMoveAdapter.release();
            return natl::move(newAllocationMoveAdapter);
        } else {
            ToAllocationMoveAdapterType newAllocationMoveAdapter(
                reinterpret_cast<ToDataType*>(allocationMoveAdapter.data(), allocationMoveAdapter.size(),
                    allocationMoveAdapter.capacity(), allocationMoveAdapter.getRequiresCopyFlag(),
                    allocationMoveAdapter.getCanDeallocFlag())
            );
            allocationMoveAdapter.release();
            return natl::move(newAllocationMoveAdapter);
        }
    }
}