#pragma once 

//own
#include "typeTraits.h"
#include "compare.h"

//interface 
namespace natl {
    template<typename DataType, typename TypeTag>
    class StrongType {
    public:
        using value_type = DataType;
    private:
        value_type valueStorage;
    public:
        //constructor 
        constexpr StrongType() noexcept = default;
        explicit constexpr StrongType(const value_type& valueIn) noexcept : valueStorage(valueIn) {};

        //destructor 
        constexpr ~StrongType() noexcept = default;

        //element access 
        constexpr value_type& value() & noexcept { return valueStorage; }
        constexpr const value_type& value() const& noexcept { return valueStorage; }
        constexpr value_type&& value() && noexcept { return natl::move(valueStorage); }
        constexpr const value_type&& value() const&& noexcept { return natl::move(valueStorage); }

        //modifiers 
        constexpr void setValue(const value_type& valueIn) noexcept { valueStorage = valueIn; }
        constexpr void setValue(value_type&& valueIn) noexcept { valueStorage = natl::move(valueIn); }

        //compare
		constexpr Bool operator==(const StrongType& rhs) const noexcept requires(IsOneWayEqualTestable<DataType>) {
			return value() == rhs.value();
        }
        constexpr Bool operator!=(const StrongType& rhs) const noexcept requires(IsOneWayNotEqualTestable<DataType>) {
            return value() != rhs.value();
        }
        constexpr Bool operator<(const StrongType& rhs) const noexcept requires(IsOneWayLessThanTestable<DataType>) {
            return value() < rhs.value();
        }
        constexpr Bool operator<=(const StrongType& rhs) const noexcept requires(IsOneWayLessThanEqualTestable<DataType>) {
            return value() <= rhs.value();
        }
        constexpr Bool operator>(const StrongType& rhs) const noexcept requires(IsOneWayGreaterThanTestable<DataType>) {
            return value() > rhs.value();
        }
        constexpr Bool operator>=(const StrongType& rhs) const noexcept requires(IsOneWayGreaterThanEqualTestable<DataType>) {
            return value() >= rhs.value();
		}
        constexpr CompareThreeWayResult<value_type> operator<=>(const StrongType& rhs) const noexcept requires(IsOneWayThreeWayTestable<DataType>) {
           return value() <=> rhs.value();
        }

    };
}