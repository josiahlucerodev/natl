#pragma once 

//own
#include "../util/typeTraits.h"
#include "../util/compare.h"

//interface 
namespace natl {
    template<typename DataType, typename TypeTag>
    class StrongType {
    public:
        using value_type = DataType;
        value_type internalValueStorage;
    public:
        //constructor 
        constexpr StrongType() noexcept = default;
        explicit constexpr StrongType(const value_type& valueIn) noexcept : internalValueStorage(valueIn) {};

        //destructor 
        constexpr ~StrongType() noexcept = default;

        //element access 
        constexpr value_type& value() & noexcept { return internalValueStorage; }
        constexpr const value_type& value() const& noexcept { return internalValueStorage; }
        constexpr value_type&& value() && noexcept { return natl::move(internalValueStorage); }
        constexpr const value_type&& value() const&& noexcept { return natl::move(internalValueStorage); }

        //convert 
        explicit constexpr operator value_type&() & noexcept { return internalValueStorage; }
        explicit constexpr operator const value_type&() const& noexcept { return internalValueStorage; }
        explicit constexpr operator value_type&&() && noexcept { return natl::move(internalValueStorage); }
        explicit constexpr operator const value_type&&() const&& noexcept { return natl::move(internalValueStorage); }

        //modifiers 
        constexpr void setValue(const value_type& valueIn) noexcept { internalValueStorage = valueIn; }
        constexpr void setValue(value_type&& valueIn) noexcept { internalValueStorage = natl::move(valueIn); }

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