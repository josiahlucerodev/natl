/*header
#pragma once
//own
#include "pch.h"
endHeader*/

//module
export module natl.repeat;
//std
import std;
//endModule

//interface
export namespace natl {
    class RepeatIterator {
    public:
        using iterator_concept = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::size_t;
        using difference_type = ptrdiff_t;
        using pointer = std::size_t*;
        using reference = std::size_t&;
        using const_reference = const std::size_t&;
        using size_type = std::size_t;
    private:
        constexpr RepeatIterator& getSelf() noexcept { return *this; };
        constexpr const RepeatIterator& getSelf() const noexcept { return *this; };
    public:

        std::size_t index;
        constexpr reference operator*() noexcept { return index; }
        constexpr const_reference operator*() const noexcept { return index; }
        constexpr const_reference operator->() const noexcept { return index; }
        constexpr reference operator[](const size_type index) = delete;

        constexpr bool operator== (const RepeatIterator rhs) const noexcept { return index == rhs.index; }
        constexpr bool operator!= (const RepeatIterator rhs) const noexcept { return index != rhs.index; }
        constexpr bool operator<(const RepeatIterator rhs) const noexcept { return index < rhs.index; }
        constexpr bool operator>(const RepeatIterator rhs) const noexcept { return index > rhs.index; }
        constexpr bool operator<=(const RepeatIterator rhs) const noexcept { return index <= rhs.index; }
        constexpr bool operator>=(const RepeatIterator rhs) const noexcept { return index >= rhs.index; }

        constexpr RepeatIterator& operator++() noexcept {
            index++;
            return getSelf();
        }

        constexpr RepeatIterator operator++(int) noexcept {
            RepeatIterator tempIt = getSelf();
            ++getSelf();
            return tempIt;
        }
        constexpr RepeatIterator& operator--() noexcept {
            index--;
            return getSelf();
        }

        constexpr RepeatIterator operator--(int) noexcept {
            RepeatIterator tempIt = getSelf();
            --getSelf();
            return tempIt;
        }
        constexpr RepeatIterator& operator+=(const size_type offset) noexcept {
            index += offset;
            return getSelf();
        }
        constexpr RepeatIterator operator+(const size_type offset) const noexcept {
            RepeatIterator tempIt = getSelf();
            tempIt += offset;
            return tempIt;
        }
        constexpr friend RepeatIterator operator+(const size_type offset, RepeatIterator& rhs) noexcept {
            rhs += offset;
            return rhs;
        }
        constexpr RepeatIterator& operator-=(const size_type offset) noexcept {
            index -= offset;
            return getSelf();
        }
        constexpr RepeatIterator operator-(const size_type offset) const noexcept {
            RepeatIterator tempIt = getSelf();
            tempIt -= offset;
            return tempIt;
        }
    };

    class Repeat {
    public:
        using value_type = std::size_t;
        using reference = std::size_t&;
        using const_reference = const std::size_t&;
        using pointer = std::size_t*;
        using const_pointer = const std::size_t*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        using const_iterator = RepeatIterator;
        using const_reverse_iterator = RepeatIterator;
    private:
        std::size_t count;
    public:
        constexpr Repeat() = default;
        constexpr ~Repeat() = default;
        constexpr Repeat(const std::size_t count) : count(count) {}

        constexpr std::size_t size() const noexcept { return count; }
        constexpr size_type beginIndex() const noexcept { return 0; };
        constexpr size_type endIndex() const noexcept { return count; };

        constexpr const_iterator begin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator end() const noexcept { return const_iterator(endIndex()); }
        constexpr const_iterator cend() const noexcept { return const_iterator(endIndex()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
    };

    class RepeatFromTo {
    public:
        using value_type = std::size_t;
        using reference = std::size_t&;
        using const_reference = const std::size_t&;
        using pointer = std::size_t*;
        using const_pointer = const std::size_t*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        using const_iterator = RepeatIterator;
        using const_reverse_iterator = RepeatIterator;
    private:
        std::size_t repeatFrom;
        std::size_t repeatTo;
    public:
        constexpr RepeatFromTo() = default;
        constexpr ~RepeatFromTo() = default;
        constexpr RepeatFromTo(const std::size_t repeatFrom, const std::size_t repeatTo) : repeatFrom(repeatFrom), repeatTo(repeatTo) {}
        constexpr RepeatFromTo(const std::size_t count) : repeatFrom(0), repeatTo(count) {}

        constexpr std::size_t size() const noexcept { return repeatTo - repeatFrom; }
        constexpr size_type beginIndex() const noexcept { return repeatFrom; };
        constexpr size_type endIndex() const noexcept { return repeatTo + 1; };

        constexpr const_iterator begin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator end() const noexcept { return const_iterator(endIndex()); }
        constexpr const_iterator cend() const noexcept { return const_iterator(endIndex()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
    };
}