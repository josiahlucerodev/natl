#pragma once 

//std
#include <vector>

//own
#include "allocator.h"
#include "container.h"

//interface 
namespace natl {
	template<class DataType, class Alloc = DefaultAllocator<DataType>> 
	class DynamicArray : public std::vector<DataType, Alloc> {
	public:
		using std::vector<DataType, Alloc>::vector;
	public:
		constexpr bool isEmpty() const noexcept { return this->empty(); }
		constexpr bool isNotEmpty() const noexcept { return !this->empty(); }
		constexpr DynamicArray& reverse() noexcept {  reverse<DynamicArray::iterator>(begin(), end()); return *this; }
		constexpr void eraseAtIndex(const std::size_t index) noexcept { erase(begin() + index); }
	};
}