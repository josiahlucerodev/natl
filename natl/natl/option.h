#pragma once 

//own
#include "hash.h"

//interface
namespace natl {
	struct OptionEmpty {};

	template<class DataType>
	class Option {
	public:
		using value_type = DataType;
	private:
		Bool isValid;
		DataType data;
	public:
		constexpr Option() : isValid(false), data() {}
		constexpr Option(const DataType& value) : isValid(true), data(value) { }
		constexpr Option(DataType&& value) : isValid(true), data(value) { }
		constexpr Option([[maybe_unused]] OptionEmpty empty) : isValid(false), data() { }

		constexpr DataType& value()& { return data; }
		constexpr const DataType& value() const& { return data; };
		//constexpr DataType&& value()&& { return data; };
		//constexpr const DataType&& value() const&& { return data; };

		constexpr const DataType* operator->() const noexcept { return data; }
		constexpr DataType* operator->() noexcept { return data; }
		constexpr const DataType& operator*() const& noexcept { return data; }
		constexpr DataType& operator*() & noexcept { return data; }
		constexpr const DataType&& operator*() const&& noexcept { return data; }
		constexpr DataType&& operator*() && noexcept { return data; }

		constexpr Bool hasValue() const noexcept { return isValid; }
		constexpr Bool doesNotHaveValue() const noexcept { return !isValid; }
		constexpr explicit operator Bool() const noexcept { return isValid; }

		constexpr std::size_t staticHash(const Option& option) requires(Hashable<DataType>) {
			if (option.hasValue()) {
				return Hash<DataType>::hash(value());
			} else {
				return 0;
			}
		}
	};
}