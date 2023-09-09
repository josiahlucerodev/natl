#pragma once 

//std
#include <cstdint>
#include <iterator>
#include <type_traits>

//own
#include "error.h"
#include "dataMovement.h"

//interator
namespace natl {
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
    public:
        constexpr reference operator*() noexcept //requires(!std::is_const<Type>)
        {
            return getValue();
        }
        constexpr const_reference operator*() const noexcept { return getValue(); }
        constexpr pointer operator->() const noexcept { return &getValue(); }
        constexpr reference operator[](const size_type index) const noexcept { return arrayBeingPtr + index; };

        constexpr bool operator== (const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex == rhs.arrayIndex; }
        constexpr bool operator!= (const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex != rhs.arrayIndex; }
        constexpr bool operator<(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex < rhs.arrayIndex; }
        constexpr bool operator>(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex > rhs.arrayIndex; }
        constexpr bool operator<=(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex <= rhs.arrayIndex; }
        constexpr bool operator>=(const ArraySizeTypeIterator rhs) const noexcept { return arrayIndex >= rhs.arrayIndex; }

        constexpr ArraySizeTypeIterator& operator++() noexcept { arrayIndex++; return getSelf(); }

        constexpr ArraySizeTypeIterator operator++(int) noexcept {
            ArraySizeTypeIterator tempIt = getSelf();
            ++getSelf();
            return tempIt;
        }
        constexpr ArraySizeTypeIterator& operator--() noexcept {
            arrayIndex--;
            return getSelf();
        }

        constexpr ArraySizeTypeIterator operator--(int) noexcept {
            ArraySizeTypeIterator tempIt = getSelf();
            --getSelf();
            return tempIt;
        }
        constexpr ArraySizeTypeIterator& operator+=(const size_type offset) noexcept {
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
            InternalArray() requires(std::is_trivially_constructible_v<Type>) = default;
            ~InternalArray() requires(std::is_trivially_destructible_v<Type>) = default;
            constexpr InternalArray() {}
            constexpr ~InternalArray() {}

            Type values[arraySize];
        };

        InternalArray internalArray;
    public:
        UninitilizedArraySizeType() requires(std::is_trivially_constructible_v<Type>) = default;
        ~UninitilizedArraySizeType() requires(std::is_trivially_destructible_v<Type>) = default;
        constexpr UninitilizedArraySizeType() {}
        constexpr ~UninitilizedArraySizeType() {}
        constexpr UninitilizedArraySizeType(std::initializer_list<Type> initList) {
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

        constexpr reference at(const size_type index) noexcept requires(!std::is_const_v<Type>) { return *(data() + index); };
        constexpr const_reference at(const size_type index) const noexcept { return *(data() + index); };

        constexpr size_type clamp(const size_type value, const size_type min, const size_type max) const noexcept {
            const size_type t = value < min ? min : value;
            return t > max ? max : t;
        }

        constexpr size_type frontIndex() const noexcept { return 0; }
        constexpr size_type backIndex() const noexcept { return 0; }

        constexpr reference atClamped(const size_type index) noexcept requires(!std::is_const_v<Type>) { return *data(); }
        constexpr const_pointer atClamped(const size_type index) const noexcept { return *data(); }

        constexpr reference front() noexcept requires(!std::is_const_v<Type>) { return at(frontIndex()); }
        constexpr const_reference front() const noexcept { return at(frontIndex()); }

        constexpr reference back() noexcept requires(!std::is_const_v<Type>) { return at(backIndex()); }
        constexpr const_reference back() const noexcept { return at(backIndex()); }

        constexpr bool empty() const noexcept { return true; }
        constexpr bool isEmpty() const noexcept { return true; }
        constexpr bool isNotEmpty() const noexcept { return false; }
    };

    template<class Type, std::size_t arraySize = 0>
    using UninitilizedArray = UninitilizedArraySizeType<Type, std::size_t, arraySize>;
}