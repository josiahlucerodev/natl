#pragma once

//own
#include "../util/basicTypes.h"

//interface 
namespace natl {
	template<typename DataType>
	struct ObserverPtr {
	public:
		using element_type = DataType;
		using value_type = DataType;

	private:
		element_type* dataPtr;

	public:
		//construtor
		constexpr ObserverPtr() noexcept = default;
		constexpr ObserverPtr(const ObserverPtr&) noexcept = default;
		constexpr ObserverPtr(ObserverPtr&&) noexcept = default;
		constexpr explicit ObserverPtr(element_type* ptr) noexcept : dataPtr(ptr) {}

		//destructor
		constexpr ~ObserverPtr() noexcept = default;

		//util 
		constexpr ObserverPtr& self() noexcept { return *this; }
		constexpr const ObserverPtr& self() const noexcept { return *this; }

		//assignment 
		constexpr ObserverPtr& operator=(const ObserverPtr&) noexcept = default;
		constexpr ObserverPtr& operator=(ObserverPtr&&) noexcept = default;

		//modifiers
		[[nodiscard]] constexpr element_type* release() noexcept {
			element_type* tempDataPtr = dataPtr;
			dataPtr = nullptr;
			return dataPtr;
		}
		constexpr void reset(element_type* p = nullptr) noexcept {
			dataPtr = p;
		}
		constexpr void swap(ObserverPtr& other) noexcept {
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
		constexpr Bool operator==(const ObserverPtr<DataType>& other) const noexcept {
			return dataPtr == other.dataPtr;
		}
		constexpr Bool operator!=(const ObserverPtr<DataType>& other) const noexcept {
			return dataPtr != other.dataPtr;
		}
		constexpr Bool operator<(const ObserverPtr<DataType>& other) const noexcept {
			return dataPtr < other.dataPtr;
		}
		constexpr Bool operator>(const ObserverPtr<DataType>& other) const noexcept {
			return dataPtr > other.dataPtr;
		}
		constexpr Bool operator<=(const ObserverPtr<DataType>& other) const noexcept {
			return dataPtr <= other.dataPtr;
		}
		constexpr Bool operator>=(const ObserverPtr<DataType>& other) const noexcept {
			return dataPtr >= other.dataPtr;
		}
		constexpr auto operator<=>(const ObserverPtr<DataType>& other) const noexcept {
			return dataPtr <=> other.dataPtr;
		}

		friend constexpr Bool operator==(const ObserverPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr == nullptr;
		}
		friend constexpr Bool operator!=(const ObserverPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr != nullptr;
		}
		friend constexpr Bool operator<(const ObserverPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr < nullptr;
		}
		friend constexpr Bool operator>(const ObserverPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr > nullptr;
		}
		friend constexpr Bool operator<=(const ObserverPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr <= nullptr;
		}
		friend constexpr Bool operator>=(const ObserverPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr >= nullptr;
		}
		friend constexpr auto operator<=>(const ObserverPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr <=> nullptr;
		}

		friend constexpr Bool operator==(NullptrType, const ObserverPtr rhs) noexcept {
			return nullptr == rhs.dataPtr;
		}
		friend constexpr Bool operator!=(NullptrType, const ObserverPtr rhs) noexcept {
			return nullptr != rhs.dataPtr;
		}
		friend constexpr Bool operator<(NullptrType, const ObserverPtr rhs) noexcept {
			return nullptr < rhs.dataPtr;
		}
		friend constexpr Bool operator>(NullptrType, const ObserverPtr rhs) noexcept {
			return nullptr > rhs.dataPtr;
		}
		friend constexpr Bool operator<=(NullptrType, const ObserverPtr rhs) noexcept {
			return nullptr <= rhs.dataPtr;
		}
		friend constexpr Bool operator>=(NullptrType, const ObserverPtr rhs) noexcept {
			return nullptr >= rhs.dataPtr;
		}
		friend constexpr auto operator<=>(NullptrType, const ObserverPtr rhs) noexcept {
			return nullptr <=> rhs.dataPtr;
		}
	};

	template<typename DataType>
	constexpr ObserverPtr<DataType> makeObserver(DataType* ptr) noexcept {
		return ObserverPtr<DataType>(ptr);
	}
}