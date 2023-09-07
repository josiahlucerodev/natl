#pragma once 

//std
#include <type_traits>

//interface 
namespace natl {
	template<class Alloc>
	struct AllocatorTraits {
	public:
		using size_type = typename Alloc::size_type;
		using value_type = typename Alloc::value_type;
	};

	template<class DataType, class Alloc>
	class AllocatorStorage : public Alloc {
	public:
		using size_type = std::size_t;
	public:
		constexpr AllocatorStorage() = default;
		
		
	};

	class Allocator {
	public:
		using size_type = std::size_t;
	public:
		constexpr Allocator() = default;
		constexpr ~Allocator() = default;
	public:
		template<class DataType>
		constexpr DataType* alloc(const size_type count) {
			return new DataType[count];
		}
		template<class DataType>
		constexpr void free(DataType* data) {
			delete[] data;
		}

		//constexpr DataType* alloc(const size_type count) { return new DataType[count]; }
		//constexpr void free(DataType* data) { return delete[] data; }
	};


	class StaticAllocatorWithCounters {
	public:
		std::size_t allocations;
		std::size_t deallocations;
		using size_type = std::size_t;

	public:
		using size_type = std::size_t;
	public:
		constexpr StaticAllocatorWithCounters() : allocations(0), deallocations(0) {}
		constexpr StaticAllocatorWithCounters(StaticAllocatorWithCounters& sa) 
			: allocations(sa.allocations), deallocations(sa.deallocations) {}

		template<class DataType>
		constexpr DataType* alloc(const size_type count) { 
			allocations++;
			DataType* data = new DataType[count];
			std::cout << "allocated: " << data << "\n";
			return data;
		}
		template<class DataType>
		constexpr void free(DataType* data) {  
			deallocations++;
			std::cout << "deallocated: " << data << "\n";
			delete[] data;
		}
	};

#define NATL_GET_UNQUIE_ID __COUNTER__


	template<class StaticAllocator, std::size_t id>
	class StaticAllocatorWrapper {
	public:
		static StaticAllocator allocator;

		template<class DataType>
		constexpr DataType* alloc(const std::size_t count) {
			return allocator.template alloc<DataType>(count);
		}
		template<class DataType>
		constexpr void free(DataType* data) {
			allocator.template free<DataType>(data);
		}
	};


#define NATL_STATIC_ALLOCATOR_INSTANCE(StaticAllocatorType, StaticAllocatorName) \
	constexpr inline std::size_t StaticAllocatorName##Id = NATL_GET_UNQUIE_ID; \
	using StaticAllocatorName = natl::StaticAllocatorWrapper<StaticAllocatorType, StaticAllocatorName##Id>; \
	StaticAllocatorType StaticAllocatorName::allocator = StaticAllocatorType();
} 