#pragma once 

//own
#include "basicTypes.h"
#include "hash.h"
#include "dataMovement.h"

//interface
namespace natl {
	struct OptionEmpty {};

	template<class DataType>
	class Option {
	public:
		using value_type = DataType;
	private:
		Bool isValid;
		union {
			Dummy dummy;
			DataType data;
		};
	public:
		//constructor
		constexpr Option() noexcept : isValid(false) {
			constructAsDummy();
		}
		constexpr Option(const Option& other) noexcept : isValid(other.isValid) {
			if (other.isValid == true) {
				construct(&data, other.data);
			} else {
				constructAsDummy();
			}
		}
		constexpr Option(Option&& other) noexcept : isValid(other.isValid) {
			if (other.isValid == true) {
				construct(&data, natl::forward<DataType>(other.data));
			} else {
				constructAsDummy();
			}
		}

		template<typename OtherDataType>
		constexpr Option(const Option<OtherDataType>& other) noexcept : isValid(other.isValid) {
			if (other.isValid == true) {
				construct(&data, other.data);
			} else {
				constructAsDummy();
			}
		}
		template<typename OtherDataType>
		constexpr Option(Option<OtherDataType>&& other) noexcept : isValid(other.isValid) {
			if (other.isValid == true) {
				construct(&data, natl::forward<OtherDataType>(other.data));
			} else {
				constructAsDummy();
			}
		}

		constexpr Option(const DataType& valueIn) noexcept : isValid(true) {
			construct(&data, valueIn);
		}
		constexpr Option(DataType&& valueIn) noexcept : isValid(true) { 
			construct(&data, natl::forward<DataType>(valueIn));
		}

		template<typename OtherDataType>
		constexpr Option(const OtherDataType& valueIn) noexcept : isValid(true) {
			construct(&data, valueIn);
		}
		template<typename OtherDataType>
		constexpr Option(OtherDataType&& valueIn) noexcept : isValid(true) {
			construct(&data, natl::forward<OtherDataType>(valueIn));
		}


		constexpr Option([[maybe_unused]] OptionEmpty empty) noexcept : isValid(false) { 
			constructAsDummy();
		}

	private:
		constexpr void constructAsDummy() noexcept {
			if (isConstantEvaluated()) {
				construct<Dummy>(&dummy);
			}
		}
		constexpr void internalDeconstruct() noexcept {
			if (typeHasToBeDestructed<DataType>()) {
				if (isValid) {
					deconstruct(&data);
				}
			}
		}
	public:
		//destructor
		constexpr ~Option() noexcept {
			internalDeconstruct();
		}

		//util
		constexpr Option& self() noexcept { return *this; }
		constexpr const Option& self() const noexcept { return *this; }

		//assignment 
		constexpr Option& operator=(const Option& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					data = other.data;
				} else {
					isValid = true;
					construct(&data, other.data);
				}
			} else {
				if (isValid == true) {
					internalDeconstruct();
					constructAsDummy();
					isValid = false;
				}
			}
			return self();
		}
		constexpr Option& operator=(Option&& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					data = natl::forward<DataType>(other.data);
				} else {
					isValid = true;
					construct(&data, natl::forward<DataType>(other.data));
				}
			} else {
				if (isValid == true) {
					internalDeconstruct();
					constructAsDummy();
					isValid = false;
				}
			}
			return self();
		}

		template<typename OtherDataType>
		constexpr Option& operator=(const Option<OtherDataType>& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					data = other.data;
				} else {
					isValid = true;
					construct(&data, other.data);
				}
			} else {
				if (isValid == true) {
					internalDeconstruct();
					constructAsDummy();
					isValid = false;
				}
			}
			return self();
		}
		template<typename OtherDataType>
		constexpr Option& operator=(Option<OtherDataType>&& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					data = natl::forward<OtherDataType>(other.data);
				} else {
					isValid = true;
					construct(&data, natl::forward<OtherDataType>(other.data));
				}
			} else {
				if (isValid == true) {
					internalDeconstruct();
					constructAsDummy();
					isValid = false;
				}
			}
			return self();
		}

		constexpr Option& operator=(const DataType& valueIn) noexcept {
			if (isValid == true) {
				data = valueIn;
			} else {
				isValid = true;
				construct(&data, valueIn);
			}
			return self();
		}
		constexpr Option& operator=(DataType&& valueIn) noexcept {
			if (isValid == true) {
				data = natl::forward<DataType>(valueIn);
			} else {
				isValid = true;
				construct(&data, natl::forward<DataType>(valueIn));
			}
			return self();
		}

		template<typename OtherDataType>
		constexpr Option& operator=(const OtherDataType& valueIn) noexcept {
			if (isValid == true) {
				data = valueIn;
			} else {
				isValid = true;
				construct(&data, valueIn);
			}
			return self();
		}
		template<typename OtherDataType>
		constexpr Option& operator=(OtherDataType&& valueIn) noexcept {
			if (isValid == true) {
				data = natl::forward<OtherDataType>(valueIn);
			} else {
				isValid = true;
				construct(&data, natl::forward<OtherDataType>(valueIn));
			}
			return self();
		}

		constexpr Option& operator=(OptionEmpty) noexcept {
			if (isValid == true) {
				internalDeconstruct();
				constructAsDummy();
				isValid = false;
			}
			return self();
		}


		//access
		constexpr DataType& value()& noexcept { return data; }
		constexpr const DataType& value() const& noexcept { return data; };
		constexpr const DataType* operator->() const noexcept { return data; }
		constexpr DataType* operator->() noexcept { return data; }
		constexpr const DataType& operator*() const& noexcept { return data; }
		constexpr DataType& operator*() & noexcept { return data; }
		constexpr const DataType&& operator*() const&& noexcept { return data; }
		constexpr DataType&& operator*() && noexcept { return data; }

		//observers
		constexpr Bool hasValue() const noexcept { return isValid; }
		constexpr Bool doesNotHaveValue() const noexcept { return !isValid; }
		constexpr explicit operator Bool() const noexcept { return isValid; }

		//modifiers 
		constexpr void reset() noexcept {
			self() = OptionEmpty{};
		}

		//special
		constexpr std::size_t staticHash(const Option& option) requires(Hashable<DataType>) {
			if (option.hasValue()) {
				return Hash<DataType>::hash(value());
			} else {
				return 0;
			}
		}

		//compare 
		constexpr Bool operator==(NullptrType) const noexcept { return doesNotHaveValue(); }
		constexpr Bool operator==(OptionEmpty) const noexcept { return doesNotHaveValue(); }
		constexpr Bool operator!=(NullptrType) const noexcept { return hasValue(); }
		constexpr Bool operator!=(OptionEmpty) const noexcept { return hasValue(); }
	};

	template<typename DataType>
	class OptionPtr {
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
		constexpr DataType& value() & noexcept { return *dataPtr; }
		constexpr const DataType& value() const& noexcept { return *dataPtr; };
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

		constexpr Bool operator==(NullptrType) const noexcept { return dataPtr == nullptr; }
		constexpr Bool operator==(OptionEmpty) const noexcept { return dataPtr == nullptr; }
		constexpr Bool operator!=(NullptrType) const noexcept { return dataPtr != nullptr; }
		constexpr Bool operator!=(OptionEmpty) const noexcept { return dataPtr != nullptr; }
	};
}