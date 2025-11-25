#pragma once 

//own
#include "basicTypes.h"
#include "hash.h"
#include "dataMovement.h"

//@export
namespace natl {
	template<typename DataType>
	struct Option {
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
		friend struct Option;

		//constructor
		constexpr Option() noexcept : isValid(false) {
			constructAsDummy();
		}
		constexpr Option(const Option& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				unsafeConstruct(other.data);
			} else {
				constructAsDummy();
			}
		}
		constexpr Option(Option&& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				unsafeConstruct(natl::forward<DataType>(other.data));
			} else {
				constructAsDummy();
			}
		}

		template<typename OtherDataType>
			requires(IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(const Option<OtherDataType>& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				unsafeConstruct(other.data);
			} else {
				constructAsDummy();
			}
		}
		template<typename OtherDataType>
			requires(IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(Option<OtherDataType>&& other) noexcept : isValid(false) {
			if (other.isValid == true) {
				unsafeConstruct(natl::forward<OtherDataType>(other.data));
			} else {
				constructAsDummy();
			}
		}

		constexpr Option(const DataType& valueIn) noexcept : isValid(false) {
			unsafeConstruct(valueIn);
		}
		constexpr Option(DataType&& valueIn) noexcept : isValid(false) {
			unsafeConstruct(natl::forward<DataType>(valueIn));
		}

		template<typename OtherDataType>
			requires(IsNotSameC<Decay<OtherDataType>, OptionEmpty> && IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(const OtherDataType& valueIn) noexcept : isValid(false) {
			unsafeConstruct(valueIn);
		}
		template<typename OtherDataType>
			requires(IsNotSameC<Decay<OtherDataType>, OptionEmpty> && IsConstructibleC<DataType, OtherDataType>)
		constexpr Option(OtherDataType&& valueIn) noexcept : isValid(false) {
			unsafeConstruct(natl::forward<OtherDataType>(valueIn));
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
		constexpr Option& operator=(const Option& other) noexcept 
			requires(IsAssignableC<DataType, DataType>) {
			if (other.isValid == true) {
				if (isValid == true) {
					data = other.data;
				} else {
					construct(other.data);
					isValid = true;
				}
			} else {
				if (isValid == true) {
					deconstruct();
				}
			}
			return self();
		}
		constexpr Option& operator=(Option&& other) noexcept 
			requires(IsAssignableC<DataType, DataType>) {
			if (other.isValid == true) {
				if (isValid == true) {
					data = natl::forward<DataType>(other.data);
				} else {
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
					construct(other.data);
					isValid = true;
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
					construct(natl::forward<OtherDataType>(other.data));
					isValid = true;
				}
			} else {
				if (isValid == true) {
					deconstruct();
				}
			}
			return self();
		}

		constexpr Option& operator=(const DataType& valueIn) noexcept 
			requires(IsAssignableC<DataType, const DataType&>) {
			if (isValid == true) {
				if constexpr (IsAssignableC<DataType, DataType>) {
					data = valueIn;
				} else {
					natl::deconstruct<DataType>(&data);
					natl::construct(&data, valueIn);
				}
			} else {
				construct(valueIn);
				isValid = true;
			}
			return self();
		}
		constexpr Option& operator=(DataType&& valueIn) noexcept 
			requires(IsAssignableC<DataType, DataType&&>) {
			if (isValid == true) {
				data = natl::forward<DataType>(valueIn);
			} else {
				construct(natl::forward<DataType>(valueIn));
				isValid = true;
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
				construct(natl::forward<OtherDataType>(valueIn));
				isValid = true;
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
				natl::terminate();
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
		constexpr const DataType* operator->() const noexcept { return &data; }
		constexpr DataType* operator->() noexcept { return &data; }
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
		constexpr DataType* construct(ConstructArgTypes&&... constructArgs) noexcept 
			requires(requires() { { DataType(natl::forward<ConstructArgTypes>(constructArgs)...) }; }) {
			if (hasValue()) {
				data = DataType(natl::forward<ConstructArgTypes>(constructArgs)...);
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
		constexpr Size staticHash(const Option& option) requires(IsHashableC<DataType>) {
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
}