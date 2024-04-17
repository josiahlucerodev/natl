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
		constexpr UninitializedValue() noexcept {}
		constexpr UninitializedValue(const DataType& dataIn) noexcept {
			construct(getValueAsPtr(), dataIn);
		}
		constexpr UninitializedValue(DataType&& dataIn) noexcept {
			construct(getValueAsPtr(), forward<DataType>(dataIn));
		}

		//destructor
		constexpr ~UninitializedValue() noexcept {};

		//util 
		constexpr UninitializedValue& self() noexcept { return *this; }
		constexpr const UninitializedValue& self() const noexcept { return *this; }

		//observers 
		constexpr DataType& getValueAsRef() noexcept { return data; }
		constexpr const DataType& getValueAsRef() const noexcept { return data; }
		constexpr DataType* getValueAsPtr() noexcept { return &data; }
		constexpr const DataType* getValueAsPtr() const noexcept { return &data; }
	};

}