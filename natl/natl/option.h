#pragma once 

//interface
namespace natl {
	struct OptionEmpty {};

	template<class DataType>
	class Option {
	public:
		using value_type = DataType;
	private:
		bool isValid;
		DataType data;
	public:
		constexpr Option() = default;
		constexpr Option(const DataType& data) : isValid(true), data(data) { }
		constexpr Option(DataType&& data) : isValid(true), data(data) { }
		constexpr Option(OptionEmpty) : isValid(false), data() { }

		constexpr DataType& value()& { return data; }
		constexpr const DataType& value() const& { return data; };
		constexpr DataType&& value()&& { return data; };
		constexpr const DataType&& value() const&& { return data; };

		constexpr const DataType* operator->() const noexcept { return data; }
		constexpr DataType* operator->() noexcept { return data; }
		constexpr const DataType& operator*() const& noexcept { return data; }
		constexpr DataType& operator*() & noexcept { return data; }
		constexpr const DataType&& operator*() const&& noexcept { return data; }
		constexpr DataType&& operator*() && noexcept { return data; }

		constexpr bool hasValue() const { return isValid; }
		constexpr explicit operator bool() const noexcept { return isValid; }
	};
}