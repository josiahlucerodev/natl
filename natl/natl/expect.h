#pragma once

//std
#include <type_traits>

//interface
namespace natl {
	template<class ErrorType>
	class Unexpected {
	public:
		using error_type = ErrorType;
	private:
		ErrorType errorData;
	public:
		constexpr Unexpected(const ErrorType& error) : errorData(error) { }
		constexpr Unexpected(ErrorType&& error) : errorData(error) { }

		constexpr ErrorType& error() & { return errorData; }
		constexpr const ErrorType& error() const& { return errorData; };
		constexpr ErrorType&& error()&& { return errorData; };
		constexpr const ErrorType&& error() const&& { return errorData; };
	};

	template<class DataType, class ErrorType>
	class Expect {
	public:
		using ValueType = DataType;
		using value_type = DataType;
		using error_type = ErrorType;
	private:
		bool isValid;
		DataType data;
		ErrorType errorData;
	public:
		constexpr Expect() = default;
		constexpr Expect(const DataType& data) : isValid(true), data(data), errorData() { }
		constexpr Expect(DataType&& data) : isValid(true), data(data), errorData() { }
		constexpr Expect(const ErrorType& error) : isValid(false), data(), errorData(error) { }
		constexpr Expect(ErrorType&& error) : isValid(false), data(), errorData(error) { }
		constexpr Expect(const ErrorType& error, const bool isValid) : isValid(isValid), data(), errorData(error) { }
		constexpr Expect(ErrorType&& error, const bool isValid) : isValid(isValid), data(), errorData(error) { }

		constexpr Expect(const Unexpected<ErrorType>& unexpectd) : isValid(false), data(), errorData(unexpectd.error()) { }
		constexpr Expect(Unexpected<ErrorType>&& unexpectd) : isValid(false), data(), errorData(unexpectd.error()) { }

	public:
		constexpr DataType& value() & { return data; }
		constexpr const DataType& value() const& { return data; };
		constexpr DataType&& value() && { return data; };
		constexpr const DataType&& value() const&& { return data; };

		constexpr ErrorType& error() & { return errorData; }
		constexpr const ErrorType& error() const& { return errorData; };
		constexpr ErrorType&& error() && { return errorData; };
		constexpr const ErrorType&& error() const&& { return errorData; };

		constexpr const DataType* operator->() const noexcept { return data; }
		constexpr DataType* operator->() noexcept { return data; }
		constexpr const DataType& operator*() const& noexcept { return data; }
		constexpr DataType& operator*() & noexcept { return data; }
		constexpr const DataType&& operator*() const&& noexcept { return data; }
		constexpr DataType&& operator*() && noexcept { return data; }

		constexpr bool hasValue() const { return isValid; }
		constexpr explicit operator bool() const noexcept { return isValid; }
	};

	template<class ErrorType>
	constexpr Unexpected<ErrorType> unexpected(const ErrorType& error) {
		return Unexpected<ErrorType>(error);
	}
	template<class ErrorType>
	constexpr Unexpected<ErrorType> unexpected(ErrorType&& error) {
		return Unexpected<ErrorType>(error);
	}
}