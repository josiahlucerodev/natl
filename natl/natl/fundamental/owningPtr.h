#pragma once 

//own
#include "../util/basicTypes.h"

//interface 
namespace natl {
	template<typename DataType>
	struct OwningPtr {
	public:
		using element_type = DataType;
		using value_type = DataType;

	private:
		element_type* dataPtr;

	public:
		//construtor
		constexpr OwningPtr() noexcept = default;
		constexpr OwningPtr(const OwningPtr&) noexcept = default;
		constexpr OwningPtr(OwningPtr&&) noexcept = default;
		constexpr explicit OwningPtr(element_type* ptr) noexcept : dataPtr(ptr) {}

		//destructor
		constexpr ~OwningPtr() noexcept = default;

		//util 
		constexpr OwningPtr& self() noexcept { return *this; }
		constexpr const OwningPtr& self() const noexcept { return *this; }

		//assignment 
		constexpr OwningPtr& operator=(const OwningPtr&) noexcept = default;
		constexpr OwningPtr& operator=(OwningPtr&&) noexcept = default;

		//modifiers
		[[nodiscard]] constexpr element_type* release() noexcept {
			element_type* tempDataPtr = dataPtr;
			dataPtr = nullptr;
			return dataPtr;
		}
		constexpr void reset(element_type* p = nullptr) noexcept {
			dataPtr = p;
		}
		constexpr void swap(OwningPtr& other) noexcept {
			element_type* tempDataPtr = other.dataPtr;
			other.dataPtr = dataPtr;
			dataPtr = tempDataPtr;
		}

		//observers
		constexpr element_type* get() const noexcept { return dataPtr; }
		constexpr explicit operator Bool() const noexcept { return dataPtr; }
		constexpr element_type& operator*() const { return dataPtr; }
		constexpr element_type* operator->() const noexcept { return dataPtr; }
		constexpr Bool isNull() noexcept { return dataPtr == nullptr; }
		constexpr Bool isNotNull() noexcept { return dataPtr != nullptr; }

		//conversions
		constexpr explicit operator element_type* () const noexcept { return dataPtr; }

		//compare 
		constexpr Bool operator==(const OwningPtr<DataType>& other) const noexcept {
			return dataPtr == other.dataPtr;
		}
		constexpr Bool operator!=(const OwningPtr<DataType>& other) const noexcept {
			return dataPtr != other.dataPtr;
		}
		constexpr Bool operator<(const OwningPtr<DataType>& other) const noexcept {
			return dataPtr < other.dataPtr;
		}
		constexpr Bool operator>(const OwningPtr<DataType>& other) const noexcept {
			return dataPtr > other.dataPtr;
		}
		constexpr Bool operator<=(const OwningPtr<DataType>& other) const noexcept {
			return dataPtr <= other.dataPtr;
		}
		constexpr Bool operator>=(const OwningPtr<DataType>& other) const noexcept {
			return dataPtr >= other.dataPtr;
		}
		constexpr auto operator<=>(const OwningPtr<DataType>& other) const noexcept {
			return dataPtr <=> other.dataPtr;
		}

		friend constexpr Bool operator==(const OwningPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr == nullptr;
		}
		friend constexpr Bool operator!=(const OwningPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr != nullptr;
		}
		friend constexpr Bool operator<(const OwningPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr < nullptr;
		}
		friend constexpr Bool operator>(const OwningPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr > nullptr;
		}
		friend constexpr Bool operator<=(const OwningPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr <= nullptr;
		}
		friend constexpr Bool operator>=(const OwningPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr >= nullptr;
		}
		friend constexpr auto operator<=>(const OwningPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr <=> nullptr;
		}

		friend constexpr Bool operator==(NullptrType, const OwningPtr rhs) noexcept {
			return nullptr == rhs.dataPtr;
		}
		friend constexpr Bool operator!=(NullptrType, const OwningPtr rhs) noexcept {
			return nullptr != rhs.dataPtr;
		}
		friend constexpr Bool operator<(NullptrType, const OwningPtr rhs) noexcept {
			return nullptr < rhs.dataPtr;
		}
		friend constexpr Bool operator>(NullptrType, const OwningPtr rhs) noexcept {
			return nullptr > rhs.dataPtr;
		}
		friend constexpr Bool operator<=(NullptrType, const OwningPtr rhs) noexcept {
			return nullptr <= rhs.dataPtr;
		}
		friend constexpr Bool operator>=(NullptrType, const OwningPtr rhs) noexcept {
			return nullptr >= rhs.dataPtr;
		}
		friend constexpr auto operator<=>(NullptrType, const OwningPtr rhs) noexcept {
			return nullptr <=> rhs.dataPtr;
		}
	};

	template<typename DataType>
	constexpr OwningPtr<DataType> makeOwning(DataType* ptr) noexcept {
		return OwningPtr<DataType>(ptr);
	}
}