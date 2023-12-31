#pragma once

//own
#include "typeTraits.h"
#include "allocator.h"
#include "dataMovement.h"

//interface
namespace natl {
	template<class DataType>
	class Ptr {
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
	public:
		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = true;
		constexpr static bool triviallyDestructible = true;
		constexpr static bool triviallyConstRefConstructedable = true;
		constexpr static bool triviallyMoveConstructedable = true;
	private:
		DataType* dataPtr;
	public:
		//constructor 
		constexpr Ptr() noexcept : dataPtr(nullptr) {}
		constexpr Ptr(DataType* data) noexcept requires(isNotConst<DataType>) : dataPtr(&data) {};
		constexpr Ptr(const DataType* data) noexcept requires(isConst<DataType>) : dataPtr(data) {};

		//destructor
		constexpr ~Ptr() noexcept = default;

		//access
		constexpr reference unwrap() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference unwrap() const noexcept { return *dataPtr; };
		constexpr reference get() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference get() const noexcept { return *dataPtr; };
		constexpr pointer getPtr() noexcept requires(isNotConst<DataType>) { return dataPtr; };
		constexpr const_pointer getPtr() const noexcept { return dataPtr; };
		constexpr reference operator*() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference operator*() const noexcept { return *dataPtr; };
		constexpr reference operator->() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference operator->() const noexcept { return *dataPtr; };
	};

	template<class DataType, class Alloc = DefaultAllocator<DataType>>
	class DefaultDeleter {
		//constructor
		constexpr DefaultDeleter() noexcept = default;

		//delete operations
		constexpr void operator()(typename Alloc::pointer* ptr) const {
			std::destroy_at<DataType>(ptr);
			Alloc::deallocate(ptr, 1); 
		}
	};

	//

	template<class DataType, class Alloc = DefaultAllocator<DataType>, class Deleter = DefaultDeleter<DataType, Alloc>>
		requires(IsAllocator<Alloc>)
	class UniquePtr {
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;

		using element_type = DataType;
		using deleter_type = Deleter;
	public:
		//movement info 
		constexpr static bool triviallyRelocatable = true && (IsEmpty<Deleter> || IsTriviallyRelocatable<Deleter>);
		constexpr static bool triviallyDefaultConstructible = true && (IsEmpty<Deleter> || IsTriviallyDefaultConstructible<Deleter>);
		constexpr static bool triviallyCompareable = true && (IsEmpty<Deleter> || IsTriviallyCompareable<Deleter>);
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;
	private:
		pointer dataPtr;
		[[no_unique_address]] deleter_type deleter;
	public:
		//constructor 
		constexpr UniquePtr() noexcept : dataPtr(nullptr), deleter() {};
		constexpr UniquePtr(const UniquePtr&) noexcept = delete;

		
		constexpr UniquePtr(UniquePtr&& other) noexcept requires(IsEmpty<Deleter>) = default;
		constexpr UniquePtr(UniquePtr&& other) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter() {
			dataPtr = other.dataPtr;
			deleter = move(deleter);
		}

		constexpr UniquePtr(std::nullptr_t) noexcept : dataPtr(nullptr), deleter() {}

		constexpr UniquePtr(pointer ptr) noexcept requires(IsNotEmpty<Deleter>) { dataPtr = ptr; }
		constexpr UniquePtr(const value_type& value) noexcept requires(IsNotEmpty<Deleter>) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(value_type&& value) noexcept requires(IsNotEmpty<Deleter>) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, forward<DataType>(value));
		}

		constexpr UniquePtr(pointer ptr, const deleter_type& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(ptr), deleter(deleter) {}
		constexpr UniquePtr(pointer ptr, deleter_type&& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(ptr), deleter(move(deleter)) {}
		constexpr UniquePtr(const value_type& value, const deleter_type& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(deleter) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(const value_type& value, deleter_type&& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(move(deleter)) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(value_type&& value, const deleter_type& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(deleter) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, forward<DataType>(value));
		}
		constexpr UniquePtr(value_type&& value, deleter_type&& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(move(deleter)) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, forward<DataType>(value));
		}

		//destructor
	private:
		constexpr void destruct() noexcept {
			if (dataPtr) {
				if constexpr (IsEmpty<Deleter>) {
					Deleter{}(dataPtr);
				} else {
					deleter(dataPtr);
				}
			}
		}
	public:
		constexpr ~UniquePtr() noexcept {
			destruct();
		}

		//util 
		constexpr UniquePtr& self() noexcept { return *this; }
		constexpr const UniquePtr& self() const noexcept { return *this; }

		//assignment 
		constexpr UniquePtr& operator=(const UniquePtr&) = delete;
		constexpr UniquePtr& operator=(UniquePtr&& other) noexcept {
			reset(other.release());
			if constexpr (IsNotEmpty<Deleter>) {
				deleter = move(deleter);
			}
		}
		constexpr UniquePtr& operator=(std::nullptr_t) noexcept {
			reset();
		}

		//modifiers
		constexpr pointer release() noexcept {
			pointer releasePtr = dataPtr;
			dataPtr = nullptr;
			return releasePtr;
		}
		constexpr void reset(pointer ptr = pointer()) noexcept {
			destruct();
			dataPtr = ptr;
		}
		constexpr void swap(UniquePtr& other) noexcept {
			exchange<UniquePtr>(dataPtr, other.dataPtr);
			if constexpr (IsNotEmpty<Deleter>) {
				exchange<Deleter>(deleter, deleter);
			}
		}

		//observers
		constexpr pointer get() noexcept { return dataPtr; }
		constexpr const_pointer get() const noexcept { return dataPtr; }

		constexpr Deleter& get_deleter() noexcept requires(IsNotEmpty<Deleter>) {
			return deleter;
		}
		constexpr const Deleter& get_deleter() const noexcept requires(IsNotEmpty<Deleter>) {
			return deleter;
		}

		constexpr bool empty() const noexcept { return dataPtr == nullptr; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }
		explicit constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr pointer operator*() noexcept { return dataPtr; }
		constexpr const_pointer operator*() const noexcept { return dataPtr; }
		constexpr pointer operator->() noexcept { return dataPtr; }
		constexpr const_pointer operator->() const noexcept { return dataPtr; }

		//compare
		constexpr bool operator==(const UniquePtr& other) const noexcept { return get() == other.get(); }
		constexpr bool operator!=(const UniquePtr& other) const noexcept { return get() != other.get(); }
		constexpr bool operator<(const UniquePtr& other) const noexcept { return get() < other.get(); }
		constexpr bool operator<=(const UniquePtr& other) const noexcept { return get() <= other.get(); }
		constexpr bool operator>(const UniquePtr& other) const noexcept { return get() > other.get(); }
		constexpr bool operator>=(const UniquePtr& other) const noexcept { return get() >= other.get(); }
		constexpr std::compare_three_way_result_t<pointer> operator<=>(const UniquePtr& other) { return get() <=> other.get(); };

		constexpr bool operator==(std::nullptr_t) const noexcept { return !self(); }
		constexpr bool operator!=(std::nullptr_t) const noexcept { return static_cast<bool>(self()); }

		//special
		constexpr Size hash() const noexcept {
			return static_cast<Size>(std::bit_cast<UIntOfByteSize_t<sizeof(pointer)>, pointer>(dataPtr));
		}
	};

	template<class CharT, class Traits, class DataType, class Alloc, class Deleter> 
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const UniquePtr<DataType, Alloc, Deleter>& ptr) {
		os << ptr.get();
		return os;
	}
}