//header
#pragma once
//own
#include "pch.h"
#include "error.h"
#include "container.h"
//endHeader

/*module
export module natl.uninitialized;
//std
import std;
endModule*/

//interface
/*export*/ namespace natl {

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

        constexpr UninitilizedValue() requires(std::is_trivially_constructible_v<T>) = default;
        constexpr ~UninitilizedValue() requires(std::is_trivially_destructible_v<T>) = default;
        constexpr UninitilizedValue() {}
        constexpr ~UninitilizedValue() {}

        constexpr T& value() { return internalValue.value; }
        constexpr T* valuePtr() { return &internalValue.value; }
        constexpr void defaultConstruct() { std::construct_at<T>(&value()); }
        constexpr void defaultDestruct() { std::destroy_at<T>(&value()); }
    };

    template<class Type, class SizeType, SizeType sizeArray>
    class ArraySizeTypeIterator {
    public:
        using iterator_concept = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Type;
        using difference_type = ptrdiff_t;
        using pointer = Type*;
        using reference = Type&;
        using const_reference = const Type&;
        using size_type = SizeType;
    private:
        pointer arrayBeingPtr;
        size_type arrayIndex;
    public:
        constexpr ArraySizeTypeIterator() = default;
        constexpr ~ArraySizeTypeIterator() = default;

        constexpr ArraySizeTypeIterator(pointer const arrayBeingPtr, const size_type index) noexcept
            : arrayBeingPtr(arrayBeingPtr), arrayIndex(index) {}
    private:
        constexpr ArraySizeTypeIterator& getSelf() noexcept { return *this; }
        constexpr const ArraySizeTypeIterator& getSelf() const noexcept { return *this; }
        constexpr reference getValue() noexcept { return *(arrayBeingPtr + arrayIndex); };

        constexpr void verifyOffset(const std::size_t offset) const noexcept {
            fatalErrorVerify(offset != 0 && arrayBeingPtr, "array iterator array ptr uninitilized");
            fatalErrorVerify(offset < 0 && (arrayIndex >= (std::size_t{ 0 } - static_cast<std::size_t>(offset))), "array iterator offset before begin");
            fatalErrorVerify(offset > 0 && ((arrayIndex + offset) > sizeArray), "array iterator offset after end");
        }
    public:
        constexpr reference operator*() noexcept //requires(!std::is_const<Type>)
        {
            fatalErrorVerify(arrayBeingPtr, "array iterator dereference array ptr uninitilized");
            fatalErrorVerify(arrayIndex < sizeArray, "array iterator dereference out of bounds");
            return getValue();
        }
        constexpr const_reference operator*() const noexcept {
            fatalErrorVerify(arrayBeingPtr, "array iterator dereference array ptr uninitilized");
            fatalErrorVerify(arrayIndex < sizeArray, "array iterator dereference out of bounds");
            return getValue();
        }
        constexpr pointer operator->() const noexcept {
            fatalErrorVerify(arrayBeingPtr, "array iterator dereference array ptr uninitilized");
            fatalErrorVerify(arrayIndex < sizeArray, "array iterator dereference out of bounds");
            return &getValue();
        }
        constexpr reference operator[](const size_type index) const noexcept {
            fatalErrorVerify(arrayBeingPtr, "array iterator array ptr uninitilized");
            fatalErrorVerify(index < 0, "array iterator index points before begin");
            fatalErrorVerify(index > 0, "array iterator index points after end");
            return arrayBeingPtr + index;
        };

        constexpr bool operator== (const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex == rhs.arrayIndex; }
        constexpr bool operator!= (const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex != rhs.arrayIndex; }
        constexpr bool operator<(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex < rhs.arrayIndex; }
        constexpr bool operator>(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex > rhs.arrayIndex; }
        constexpr bool operator<=(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex <= rhs.arrayIndex; }
        constexpr bool operator>=(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex >= rhs.arrayIndex; }

        constexpr ArraySizeTypeIterator& operator++() noexcept {
            fatalError(arrayBeingPtr, "array iterator increment array ptr uninitilized");
            fatalError(arrayIndex < sizeArray, "array iterator increment out of bounds");
            arrayIndex++;
            return getSelf();
        }

        constexpr ArraySizeTypeIterator operator++(int) noexcept {
            ArraySizeTypeIterator tempIt = getSelf();
            ++getSelf();
            return tempIt;
        }
        constexpr ArraySizeTypeIterator& operator--() noexcept {
            fatalError(arrayBeingPtr, "array iterator increment array ptr uninitilized");
            fatalError(arrayIndex != sizeArray, "array iterator increment out of bounds");
            arrayIndex--;
            return getSelf();
        }

        constexpr ArraySizeTypeIterator operator--(int) noexcept {
            ArraySizeTypeIterator tempIt = getSelf();
            --getSelf();
            return tempIt;
        }
        constexpr ArraySizeTypeIterator& operator+=(const size_type offset) noexcept {
            verifyOffset(offset);
            arrayBeingPtr += offset;
            return getSelf();
        }
        constexpr ArraySizeTypeIterator operator+(const size_type offset) const noexcept {
            ArraySizeTypeIterator tempIt = getSelf();
            tempIt += offset;
            return tempIt;
        }
        constexpr friend ArraySizeTypeIterator operator+(const size_type offset, ArraySizeTypeIterator& rhs) noexcept {
            rhs += offset;
            return rhs;
        }
        constexpr ArraySizeTypeIterator& operator-=(const size_type offset) noexcept {
            return getSelf() += -offset;
        }
        constexpr ArraySizeTypeIterator operator-(const size_type offset) const noexcept {
            ArraySizeTypeIterator tempIt = getSelf();
            tempIt -= offset;
            return tempIt;
        }
    };

    template<class Type, class SizeType, std::size_t arraySize>
    class UninitilizedArraySizeType {
        using value_type = Type;
        using reference = Type&;
        using const_reference = const Type&;
        using pointer = Type*;
        using const_pointer = const Type*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        using iterator = ArraySizeTypeIterator<Type, SizeType, arraySize>;
        using const_iterator = ArraySizeTypeIterator<const Type, SizeType, arraySize>;
        using reverse_iterator = std::reverse_iterator<ArraySizeTypeIterator<Type, SizeType, arraySize>>;
        using const_reverse_iterator = std::reverse_iterator<ArraySizeTypeIterator<const Type, SizeType, arraySize>>;
    public:
        union InternalArray {
            constexpr InternalArray() {}
            constexpr ~InternalArray() {};
            constexpr InternalArray() requires(std::is_trivially_constructible_v<Type>) = default;
            constexpr ~InternalArray() requires(std::is_trivially_destructible_v<Type>) = default;

            Type values[arraySize];
        };

        InternalArray internalArray;
    public:
        constexpr UninitilizedArraySizeType() = default;
        constexpr ~UninitilizedArraySizeType() = default;
        constexpr UninitilizedArraySizeType(std::initializer_list<Type> initList) {
            fatalErrorVerify(size() == initList.size(), "UninitilizedArraySizeType<Type, SizeType, size> initializer_list constructor must be the same size as array");
            uninitializedCopyNoOverlap(initList.begin(), initList.end(), begin());
        };

        constexpr size_type size() const noexcept { return arraySize; }
        constexpr pointer data() noexcept requires(!std::is_const_v<Type>) { return internalArray.values; }
        constexpr const_pointer data() const noexcept { return internalArray.values; };

        constexpr size_type beginIndex() const noexcept { return 0; };
        constexpr size_type endIndex() const noexcept { return arraySize; };

        constexpr iterator begin() noexcept requires(!std::is_const_v<Type>) { return iterator(internalArray.values, beginIndex()); }
        constexpr const_iterator begin() const noexcept { return const_iterator(internalArray.values, beginIndex()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(internalArray.values, beginIndex()); }
        constexpr iterator end() noexcept requires(!std::is_const_v<Type>) { return iterator(internalArray.values, endIndex()); }
        constexpr const_iterator end() const noexcept { return const_iterator(internalArray.values, endIndex()); }
        constexpr const_iterator cend() const noexcept { return const_iterator(internalArray.values, endIndex()); }
        constexpr reverse_iterator rbegin() noexcept requires(!std::is_const_v<Type>) { return reverse_iterator(end()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr reverse_iterator rend() noexcept requires(!std::is_const_v<Type>) { return reverse_iterator(begin()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

        constexpr reference at(const size_type index) noexcept requires(!std::is_const_v<Type>) { return internalArray.values[index]; };
        constexpr const_reference at(const size_type index) const noexcept { return internalArray.values[index]; };

        constexpr size_type clamp(const size_type value, const size_type min, const size_type max) const noexcept {
            const size_type t = value < min ? min : value;
            return t > max ? max : t;
        }

        constexpr size_type frontIndex() const noexcept { return 0; }
        constexpr size_type backIndex() const noexcept { return size() - 1; }

        constexpr size_type clampIndex(const size_type index) const noexcept { return clamp(index, 0, backIndex()); }
        constexpr reference atClamped(const size_type index) noexcept requires(!std::is_const_v<Type>) { return internalArray.values[clampIndex(index)]; }
        constexpr const_pointer atClamped(const size_type index) const noexcept { return internalArray.values[clampIndex(index)]; }

        constexpr reference front() noexcept requires(!std::is_const_v<Type>) { return at(frontIndex()); }
        constexpr const_reference front() const noexcept { return at(frontIndex()); }

        constexpr reference back() noexcept requires(!std::is_const_v<Type>) { return at(backIndex()); }
        constexpr const_reference back() const noexcept { return at(backIndex()); }

        constexpr bool empty() const noexcept { return false; }
        constexpr bool isEmpty() const noexcept { return false; }
        constexpr bool isNotEmpty() const noexcept { return true; }

        constexpr void defualtConstructAt(const size_type index) noexcept(std::is_nothrow_constructible_v<Type>) {
            std::construct_at<Type>(&at(index));
        }
        constexpr void deconstructAt(const size_type index) noexcept(std::is_nothrow_constructible_v<Type>) {
            std::destroy_at<Type>(&at(index));
        }

        constexpr void defualtConstructAll() noexcept(std::is_nothrow_destructible_v<Type>) {
            for (size_type index = 0; index < size(); index++) {
                defualtConstructAt(index);
            }
        }
        constexpr void deconstructAll() noexcept(std::is_nothrow_destructible_v<Type>) {
            for (size_type index = 0; index < size(); index++) {
                deconstructAt(index);
            }
        }
    };


    template<class Type, class SizeType>
    class UninitilizedArraySizeType<Type, SizeType, 0> {
    public:
        using value_type = Type;
        using reference = Type&;
        using const_reference = const Type&;
        using pointer = Type*;
        using const_pointer = const Type*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        using iterator = UninitilizedArraySizeType<Type, SizeType, 0>;
        using const_iterator = UninitilizedArraySizeType<const Type, SizeType, 0>;
        using reverse_iterator = std::reverse_iterator<UninitilizedArraySizeType<Type, SizeType, 0>>;
        using const_reverse_iterator = std::reverse_iterator<UninitilizedArraySizeType<const Type, SizeType, 0>>;
    public:
        constexpr size_type size() const noexcept { return nullptr; }
        constexpr pointer data() noexcept requires(!std::is_const_v<Type>) { return nullptr; }
        constexpr const_pointer data() const noexcept { return nullptr; };

        constexpr iterator begin() noexcept requires(!std::is_const_v<Type>) { return iterator{}; }
        constexpr const_iterator begin() const noexcept { return const_iterator{}; }
        constexpr const_iterator cbegin() const noexcept { return const_iterator{}; }
        constexpr iterator end() noexcept requires(!std::is_const_v<Type>) { return iterator{}; }
        constexpr const_iterator end() const noexcept { return const_iterator{}; }
        constexpr const_iterator cend() const noexcept { return const_iterator{}; }

        constexpr reverse_iterator rbegin() noexcept requires(!std::is_const_v<Type>) { return reverse_iterator(end()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr reverse_iterator rend() noexcept requires(!std::is_const_v<Type>) { return reverse_iterator(begin()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

        constexpr reference at(const size_type index) noexcept requires(!std::is_const_v<Type>) {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> at undefined");
            return *data();
        };
        constexpr const_reference at(const size_type index) const noexcept {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> at undefined");
            return *data();
        };

        constexpr size_type clamp(const size_type value, const size_type min, const size_type max) const noexcept {
            const size_type t = value < min ? min : value;
            return t > max ? max : t;
        }

        constexpr size_type frontIndex() const noexcept { return 0; }
        constexpr size_type backIndex() const noexcept { return 0; }

        constexpr reference atClamped(const size_type index) noexcept requires(!std::is_const_v<Type>) {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> atClamped undefined");
            return *data();
        }
        constexpr const_pointer atClamped(const size_type index) const noexcept {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> atClamped undefined");
            return *data();
        }

        constexpr reference front() noexcept requires(!std::is_const_v<Type>) {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> front undefined");
            return at(frontIndex());
        }
        constexpr const_reference front() const noexcept {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> front undefined");
            return at(frontIndex());
        }

        constexpr reference back() noexcept requires(!std::is_const_v<Type>) {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> back undefined");
            return at(backIndex());
        }
        constexpr const_reference back() const noexcept {
            fatalError("UninitilizedArraySizeType<Type, SizeType, 0> back undefined");
            return at(backIndex());
        }

        constexpr bool empty() const noexcept { return true; }
        constexpr bool isEmpty() const noexcept { return true; }
        constexpr bool isNotEmpty() const noexcept { return false; }
    };

    template<class Type, std::size_t arraySize = 0>
    using UninitilizedArray = UninitilizedArraySizeType<Type, std::size_t, arraySize>;
}