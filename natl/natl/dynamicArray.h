#pragma once 

//std
#include <vector>

//own
#include "allocator.h"

//interface 
namespace natl {
	template<class DataType, class Alloc = DefaultAllocator<DataType>> 
	class DynamicArray : public std::vector<DataType, Alloc> {
	public:
		using std::vector<DataType, Alloc>::vector;
	};
}