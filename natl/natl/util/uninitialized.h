#pragma once 

//own
#include "typeTraits.h"
#include "dataMovement.h"

//interface 
namespace natl {
	template<typename DataType>
	class UninitializedValue {
	public:
		using value_type = DataType;
	private:
		union { DataType data; };
	public:
		//constructor 
		constexpr UninitializedValue() noexcept {};
		constexpr UninitializedValue(const UninitializedValue&) noexcept = delete;
		constexpr UninitializedValue(UninitializedValue&&) noexcept = delete;
		constexpr UninitializedValue(const DataType& dataIn) noexcept {
			construct(dataIn);
		}
		constexpr UninitializedValue(DataType&& dataIn) noexcept {
			construct(natl::forward<DataType>(dataIn));
		}
		template<typename... ConstructArgTypes>
		constexpr UninitializedValue(ConstructArgTypes&&... constructArgs) noexcept {
			construct(natl::forward<ConstructArgTypes>(constructArgs)...);
		}

		//destructor
		constexpr ~UninitializedValue() noexcept {};

		//util 
		constexpr UninitializedValue& self() noexcept { return *this; }
		constexpr const UninitializedValue& self() const noexcept { return *this; }

		//assignment 
		constexpr UninitializedValue& operator=(const UninitializedValue&) noexcept = delete;
		constexpr UninitializedValue& operator=(UninitializedValue&&) noexcept = delete;

		//observers 
		constexpr DataType& value() noexcept { return data; }
		constexpr const DataType& value() const noexcept { return data; }
		constexpr DataType& valueAsRef() & noexcept { return value(); }
		constexpr const DataType& valueAsRef() const& noexcept { return value(); };
		constexpr DataType* valueAsPtr() noexcept { return &data; }
		constexpr const DataType* valueAsPtr() const noexcept { return &data; }

		//modifiers
		constexpr void copyInto(const DataType& valueIn) noexcept {
			data = valueIn;
		}
		constexpr void copyInto(const UninitializedValue& valueIn) noexcept {
			data = valueIn.data;
		}
		constexpr void moveInto(DataType&& valueIn) noexcept {
			data = natl::move(valueIn);
		}
		constexpr void moveInto(UninitializedValue&& valueIn) noexcept {
			data = natl::move(valueIn.data);
		}

		template<typename... ConstructArgTypes>
		constexpr DataType* construct(ConstructArgTypes&&... constructArgs) noexcept {
			return natl::construct<DataType, ConstructArgTypes...>(valueAsPtr(), natl::forward<ConstructArgTypes>(constructArgs)...);
		}
		constexpr void deconstruct() noexcept {
			natl::deconstruct<DataType>(valueAsPtr());
		}
	};
}