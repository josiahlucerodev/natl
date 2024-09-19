#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/hash.h"
#include "../util/dataMovement.h"

//interface
namespace natl {
	struct OptionEmpty {};

	template<typename DataType>
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
		template<typename OtherDataType>
		friend class Option;

		//constructor
		constexpr Option() noexcept : isValid(false) {
			constructAsDummy();
		}
		constexpr Option(const Option& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				construct(other.data);
			} else {
				constructAsDummy();
			}
		}
		constexpr Option(Option&& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				construct(natl::forward<DataType>(other.data));
			} else {
				constructAsDummy();
			}
		}

		template<typename OtherDataType>
			requires(IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(const Option<OtherDataType>& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				construct(other.data);
			} else {
				constructAsDummy();
			}
		}
		template<typename OtherDataType>
			requires(IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(Option<OtherDataType>&& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				construct(natl::forward<OtherDataType>(other.data));
			} else {
				constructAsDummy();
			}
		}

		constexpr Option(const DataType& valueIn) noexcept : isValid(false) {
			construct(valueIn);
		}
		constexpr Option(DataType&& valueIn) noexcept : isValid(false) {
			construct(natl::forward<DataType>(valueIn));
		}

		template<typename OtherDataType>
			requires(IsNotSame<Decay<OtherDataType>, OptionEmpty> && IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(const OtherDataType& valueIn) noexcept : isValid(false) {
			construct(valueIn);
		}
		template<typename OtherDataType>
			requires(IsNotSame<Decay<OtherDataType>, OptionEmpty> && IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(OtherDataType&& valueIn) noexcept : isValid(false) {
			construct(natl::forward<OtherDataType>(valueIn));
		}

		constexpr Option(OptionEmpty) noexcept : isValid(false) { 
			constructAsDummy();
		}

	private:
		constexpr void constructAsDummy() noexcept {
			if (isConstantEvaluated()) {
				natl::construct<Dummy>(&dummy);
			}
		}
	public:
		//destructor
		constexpr ~Option() noexcept {
			deconstruct();
		}

		//util
		constexpr Option& self() noexcept { return *this; }
		constexpr const Option& self() const noexcept { return *this; }

		//assignment 
		constexpr Option& operator=(const Option& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					if constexpr (IsAssignableC<DataType, DataType>) {
						data = other.data;
					} else {
						natl::deconstruct<DataType>(&data);
						natl::construct(&data, other.data);
					}
				} else {
					isValid = true;
					construct(other.data);
				}
			} else {
				if (isValid == true) {
					deconstruct();
				}
			}
			return self();
		}
		constexpr Option& operator=(Option&& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					if constexpr (IsAssignableC<DataType, DataType>) {
						data = natl::forward<DataType>(other.data);
					} else {
						natl::deconstruct<DataType>(&data);
						natl::construct(&data, natl::forward<DataType>(other.data));
					}
				} else {
					isValid = true;
					construct(natl::forward<DataType>(other.data));
				}
			} else {
				if (isValid == true) {
					deconstruct();
				}
			}
			return self();
		}

		template<typename OtherDataType>
			requires(IsAssignableC<DataType, OtherDataType>)
		constexpr Option& operator=(const Option<OtherDataType>& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					data = other.data;
				} else {
					isValid = true;
					construct(other.data);
				}
			} else {
				if (isValid == true) {
					deconstruct();
				}
			}
			return self();
		}
		template<typename OtherDataType>
			requires(IsAssignableC<DataType, OtherDataType>)
		constexpr Option& operator=(Option<OtherDataType>&& other) noexcept {
			if (other.isValid == true) {
				if (isValid == true) {
					data = natl::forward<OtherDataType>(other.data);
				} else {
					isValid = true;
					construct(natl::forward<OtherDataType>(other.data));
				}
			} else {
				if (isValid == true) {
					deconstruct();
				}
			}
			return self();
		}

		constexpr Option& operator=(const DataType& valueIn) noexcept {
			if (isValid == true) {
				if constexpr (IsAssignableC<DataType, DataType>) {
					data = valueIn;
				} else {
					natl::deconstruct<DataType>(&data);
					natl::construct(&data, valueIn);
				}
			} else {
				isValid = true;
				construct(valueIn);
			}
			return self();
		}
		constexpr Option& operator=(DataType&& valueIn) noexcept {
			if (isValid == true) {
				if constexpr (IsAssignableC<DataType, DataType>) {
					data = natl::forward<DataType>(valueIn);
				} else {
					natl::deconstruct<DataType>(&data);
					natl::construct(&data, natl::forward<DataType>(valueIn));
				}
			} else {
				isValid = true;
				construct(natl::forward<DataType>(valueIn));
			}
			return self();
		}

		template<typename OtherDataType>
			requires(IsNotSame<Decay<OtherDataType>, OptionEmpty> && IsAssignableC<DataType, OtherDataType>)
		constexpr Option& operator=(const OtherDataType& valueIn) noexcept {
			if (isValid == true) {
				data = valueIn;
			} else {
				isValid = true;
				construct(valueIn);
			}
			return self();
		}
		template<typename OtherDataType>
			requires(IsNotSame<Decay<OtherDataType>, OptionEmpty> && IsAssignableC<DataType, OtherDataType>)
		constexpr Option& operator=(OtherDataType&& valueIn) noexcept {
			if (isValid == true) {
				data = natl::forward<OtherDataType>(valueIn);
			} else {
				isValid = true;
				construct(natl::forward<OtherDataType>(valueIn));
			}
			return self();
		}

		constexpr Option& operator=(OptionEmpty) noexcept {
			if (isValid == true) {
				deconstruct();
			}
			return self();
		}

		//access
	private:
		constexpr void checkIfValid() const noexcept {
#ifdef NATL_IN_DEBUG
			if (!hasValue()) {
				natlTerminate();
			}
#endif
		}
	public:
		constexpr DataType& value() & noexcept { 
			checkIfValid();
			return data; 
		}
		constexpr const DataType& value() const& noexcept { 
			checkIfValid();
			return data; 
		};
		constexpr DataType& valueAsRef() & noexcept {
			return value();
		}
		constexpr const DataType& valueAsRef() const& noexcept {
			return value();
		};
		constexpr DataType* valueAsPtr() noexcept { 
			checkIfValid();
			return &data; 
		}
		constexpr const DataType* valueAsPtr() const noexcept { 
			checkIfValid();
			return &data; 
		}
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
		template<typename... ConstructArgTypes>
			requires(IsConstructibleC<DataType, ConstructArgTypes...>)
		constexpr DataType* unsafeConstruct(ConstructArgTypes&&... constructArgs) noexcept {
			isValid = true;
			return natl::construct<DataType, ConstructArgTypes...>(valueAsPtr(), natl::forward<ConstructArgTypes>(constructArgs)...);
		}
		template<typename... ConstructArgTypes>
			requires(IsConstructibleC<DataType, ConstructArgTypes...>)
		constexpr DataType* construct(ConstructArgTypes&&... constructArgs) noexcept {
			if (hasValue()) {
				if constexpr (IsAssignableC<DataType, DataType>) {
					data = DataType(natl::forward<ConstructArgTypes>(constructArgs)...);
				} else {
					natl::deconstruct<DataType>(&data);
					natl::construct(&data, DataType(natl::forward<ConstructArgTypes>(constructArgs)...));
				}
				return &data;
			} else {
				return unsafeConstruct<ConstructArgTypes...>(natl::forward<ConstructArgTypes>(constructArgs)...);
			}
		}
		template<typename... ConstructArgTypes>
			requires(IsConstructibleC<DataType, ConstructArgTypes...>)
		constexpr DataType& emplace(ConstructArgTypes&&... constructArgs) noexcept {
			return &construct<ConstructArgTypes...>(natl::forward<ConstructArgTypes>(constructArgs)...);
		}
		constexpr void unsafeDeconstruct() noexcept {
			if (typeHasToBeDestructed<DataType>()) {
				natl::deconstruct<DataType>(valueAsPtr());
				constructAsDummy();
			}
			isValid = false;
		}
		constexpr void deconstruct() noexcept {
			if (hasValue()) {
				unsafeDeconstruct();
			}
		}

		//special
		constexpr Size staticHash(const Option& option) requires(Hashable<DataType>) {
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