/*header
#pragma once
//own
#include "pch.h"
#include "typeTraits.h"
endHeader*/

//module
export module natl.iterators;
//std
import std;
//endModule

//interface 
export namespace natl {
	template<class T>
	class RandomAccessIterator {
	public:
		using iterator = RandomAccessIterator<T>;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using iterator_category = std::random_access_iterator_tag;
		using size_type = std::size_t;
	private:
		T* dataPtr;
	public:
		constexpr RandomAccessIterator(T* const dataPtr) noexcept : dataPtr(dataPtr) {}
	private:
		constexpr iterator& getSelf() noexcept { return *this; }
		constexpr const iterator& getSelf() const noexcept { return *this; }
	public:
		constexpr reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() const noexcept { return dataPtr; }
		constexpr reference operator[](const size_type pos) const noexcept { return dataPtr[pos]; };

		constexpr bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr bool operator<(const iterator rhs) const noexcept { dataPtr < rhs.dataPtr; }
		constexpr bool operator>(const iterator rhs) const noexcept { dataPtr > rhs.dataPtr; }
		constexpr bool operator<=(const iterator rhs) const noexcept { dataPtr <= rhs.dataPtr; }
		constexpr bool operator>=(const iterator rhs) const noexcept { dataPtr >= rhs.dataPtr; }

		constexpr iterator& operator++() noexcept { dataPtr++; return getSelf(); }
		constexpr iterator operator++(int) noexcept { iterator tempIt = getSelf(); ++getSelf().dataPtr; return tempIt; }
		constexpr iterator& operator--() noexcept { dataPtr--; return getSelf(); }
		constexpr iterator operator--(int) noexcept { iterator tempIt = getSelf(); --getSelf().dataPtr; return tempIt; }

		constexpr iterator& operator+=(const size_type offset) noexcept { dataPtr += offset; return getSelf(); }
		constexpr iterator operator+(const size_type offset) const noexcept { return iterator(dataPtr + offset); }
		constexpr friend iterator operator+(const size_type offset, iterator& rhs) noexcept { rhs += offset; return rhs; }
		constexpr iterator& operator-=(const size_type offset) noexcept { dataPtr -= offset; return getSelf(); }
		constexpr iterator operator-(const size_type offset) const noexcept { return iterator(dataPtr -= offset); }
		constexpr difference_type operator-(const iterator rhs) const noexcept { return getSelf().dataPtr - rhs.dataPtr; }
	};

	template<class T> 
	using ReverseRandomAccessIterator = std::reverse_iterator<RandomAccessIterator<T>>;

	template<class Type>
	concept HasIteratorType = requires {
		typename Type::iterator;
	};
}
