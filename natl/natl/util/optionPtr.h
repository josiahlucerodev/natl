#pragma once 

//own
#include "basicTypes.h"

//@export
namespace natl {
	template<typename DataType>
	struct OptionPtr {
	public:
		using value_type = DataType;
	private:
		DataType* dataPtr;
	public:
		//constructor 
		constexpr OptionPtr() noexcept : dataPtr(nullptr) {}
		constexpr OptionPtr(const OptionPtr& other) noexcept : dataPtr(other.dataPtr) {}
		constexpr OptionPtr(OptionPtr&& other) noexcept : dataPtr(other.dataPtr) {
			other.dataPtr = nullptr;
		}
		constexpr OptionPtr(DataType* dataPtrIn) noexcept : dataPtr(dataPtrIn) {}
		constexpr OptionPtr(NullptrType) noexcept : dataPtr(nullptr) {}
		constexpr OptionPtr(OptionEmpty) noexcept : dataPtr(nullptr) {}

		//destructor 
		constexpr ~OptionPtr() noexcept = default;

		//util 
		constexpr OptionPtr& self() noexcept { return *this; }
		constexpr const OptionPtr& self() const noexcept { return *this; }

		//assignment 
		constexpr OptionPtr& operator=(const OptionPtr& other) noexcept {
			dataPtr = other.dataPtr;
			return self();
		}
		constexpr OptionPtr& operator=(OptionPtr&& other) noexcept {
			dataPtr = other.dataPtr;
			other.dataPtr = nullptr;
			return self();
		}
		constexpr OptionPtr& operator=(DataType* dataPtrIn) noexcept {
			dataPtr = dataPtrIn;
			return self();
		}
		constexpr OptionPtr& operator=(NullptrType) noexcept {
			dataPtr = nullptr;
			return self();
		}
		constexpr OptionPtr& operator=(OptionEmpty) noexcept {
			dataPtr = nullptr;
			return self();
		}

		//observers
		constexpr Bool hasValue() const noexcept { return dataPtr != nullptr; }
		constexpr Bool doesNotHaveValue() const noexcept { return dataPtr == nullptr; }
		constexpr explicit operator Bool() const noexcept { return hasValue(); }

		//access
		constexpr DataType& value() noexcept { return *dataPtr; }
		constexpr const DataType& value() const noexcept { return *dataPtr; };
		constexpr DataType* get() noexcept { return dataPtr; }
		constexpr const DataType* get() const noexcept { return dataPtr; }
		constexpr DataType* getPtr() noexcept { return dataPtr; }
		constexpr const DataType* getPtr() const noexcept { return dataPtr; }
		constexpr const DataType* operator->() const noexcept { return dataPtr; }
		constexpr DataType* operator->() noexcept { return dataPtr; }
		constexpr const DataType& operator*() const& noexcept { return *dataPtr; }
		constexpr DataType& operator*() & noexcept { return *dataPtr; }
		constexpr const DataType&& operator*() const&& noexcept { return *dataPtr; }
		constexpr DataType&& operator*() && noexcept { return *dataPtr; }


		//modifiers 
		constexpr void reset() noexcept {
			dataPtr = nullptr;
		}

		//compare 
		constexpr Bool operator==(const OptionPtr other) const noexcept { return dataPtr == other.dataPtr; }
		constexpr Bool operator!=(const OptionPtr other) const noexcept { return dataPtr != other.dataPtr; }
		constexpr Bool operator<(const OptionPtr other) const noexcept { return dataPtr < other.dataPtr; }
		constexpr Bool operator<=(const OptionPtr other) const noexcept { return dataPtr <= other.dataPtr; }
		constexpr Bool operator>(const OptionPtr other) const noexcept { return dataPtr > other.dataPtr; }
		constexpr Bool operator>=(const OptionPtr other) const noexcept { return dataPtr >= other.dataPtr; }

		constexpr Bool operator==(const DataType* otherPtr) const noexcept { return dataPtr == otherPtr; }
		constexpr Bool operator!=(const DataType* otherPtr) const noexcept { return dataPtr != otherPtr; }
		constexpr Bool operator<(const DataType* otherPtr) const noexcept { return dataPtr < otherPtr; }
		constexpr Bool operator<=(const DataType* otherPtr) const noexcept { return dataPtr <= otherPtr; }
		constexpr Bool operator>(const DataType* otherPtr) const noexcept { return dataPtr > otherPtr; }
		constexpr Bool operator>=(const DataType* otherPtr) const noexcept { return dataPtr >= otherPtr; }

		friend constexpr Bool operator==(const OptionPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr == nullptr;
		}
		friend constexpr Bool operator!=(const OptionPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr != nullptr;
		}
		friend constexpr Bool operator<(const OptionPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr < nullptr;
		}
		friend constexpr Bool operator>(const OptionPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr > nullptr;
		}
		friend constexpr Bool operator<=(const OptionPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr <= nullptr;
		}
		friend constexpr Bool operator>=(const OptionPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr >= nullptr;
		}
		friend constexpr auto operator<=>(const OptionPtr lhs, NullptrType) noexcept {
			return lhs.dataPtr <=> nullptr;
		}

		friend constexpr Bool operator==(NullptrType, const OptionPtr rhs) noexcept {
			return nullptr == rhs.dataPtr;
		}
		friend constexpr Bool operator!=(NullptrType, const OptionPtr rhs) noexcept {
			return nullptr != rhs.dataPtr;
		}
		friend constexpr Bool operator<(NullptrType, const OptionPtr rhs) noexcept {
			return nullptr < rhs.dataPtr;
		}
		friend constexpr Bool operator>(NullptrType, const OptionPtr rhs) noexcept {
			return nullptr > rhs.dataPtr;
		}
		friend constexpr Bool operator<=(NullptrType, const OptionPtr rhs) noexcept {
			return nullptr <= rhs.dataPtr;
		}
		friend constexpr Bool operator>=(NullptrType, const OptionPtr rhs) noexcept {
			return nullptr >= rhs.dataPtr;
		}
		friend constexpr auto operator<=>(NullptrType, const OptionPtr rhs) noexcept {
			return nullptr <=> rhs.dataPtr;
		}
	};
}