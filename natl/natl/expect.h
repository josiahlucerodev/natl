#pragma once

//own
#include "compilerDependent.h"
#include "basicTypes.h"
#include "datamovement.h"
#include "error.h"

//interface
namespace natl {
	template<class ErrorType>
	class Unexpected {
	public:
		using error_type = ErrorType;
	private:
		ErrorType errorData;
	public:
		constexpr Unexpected(const ErrorType& error) noexcept : errorData(error) {}
		constexpr Unexpected(ErrorType&& error) noexcept : errorData(error) {}
		constexpr ~Unexpected() noexcept = default;

		constexpr Unexpected& self() noexcept { return *this; }
		constexpr const Unexpected& self() const noexcept { return *this; }

		constexpr ErrorType& error() & noexcept { 
			return errorData;
		}
		constexpr const ErrorType& error() const& noexcept { 
			return errorData;
		};
		constexpr ErrorType&& error()&& noexcept { 
			return errorData;
		};
		constexpr const ErrorType&& error() const&& noexcept { 
			return errorData;
		};

		constexpr void swap(Unexpected& other) noexcept {
			Unexpected temp = move(other);
			other = move(self());
			self() = move(temp);
		}
	};

	enum class ExpectState : i8 {
		dummy = 0,
		value,
		error,
	};

	template<class DataType, class ErrorType>
	class Expect {
	public:
		using ValueType = DataType;
		using value_type = DataType;
		using error_type = ErrorType;
	private:
		ExpectState state;
		union {
			Dummy dummy;
			DataType data;
			ErrorType errorData;
		};
	public:
		//constructor
		constexpr Expect() : state(ExpectState::dummy), dummy() {}
		constexpr Expect(const DataType& data) : state(ExpectState::value), data(data) {}
		constexpr Expect(DataType&& data) : state(ExpectState::value), data(move(data)) {}
		constexpr Expect(const ErrorType& error) : state(ExpectState::error), errorData(error) {}
		constexpr Expect(ErrorType&& error) : state(ExpectState::error), errorData(forward(error)) {}
		constexpr Expect(const Unexpected<ErrorType>& unexpectd) : state(ExpectState::error), errorData(unexpectd.error()) {}
		constexpr Expect(Unexpected<ErrorType>&& unexpectd) : state(ExpectState::error), errorData(unexpectd.error()) {}

		//destructor 
	private:
		constexpr void destruct() noexcept {
			switch (state) {
			case ExpectState::dummy:
				std::destroy_at<Dummy>(&dummy);
				break;
			case ExpectState::value:
				std::destroy_at<DataType>(&data);
				break;
			case ExpectState::error:
				std::destroy_at<ErrorType>(&errorData);
				break;
			}
		}
	public:

		constexpr ~Expect() noexcept {
			destruct();
		}

		//util 
		constexpr Expect& self() noexcept { return *this; }
		constexpr const Expect& self() const noexcept { return *this; }

		//assignment 
		constexpr Expect& operator=(const Expect& other) noexcept {
			destruct();
			state = other.state;

			switch (other.state) {
			case ExpectState::value:
				std::construct_at<DataType>(&data, other.data);
				break;
			case ExpectState::error:
				std::construct_at<ErrorType>(&errorData, other.errorData);
				break;
			default:
				break;
			}

			return self();
		}
		constexpr Expect& operator=(Expect&& other) noexcept {
			destruct();
			state = other.state;

			switch (other.state) {
			case ExpectState::value:
				std::construct_at<DataType>(&data, move<DataType>(other.data));
				break;
			case ExpectState::error:
				std::construct_at<ErrorType>(&errorData, move<ErrorType>(other.errorData));
				break;
			default:
				break;
			}

			other.destruct();
			std::construct_at<Dummy>(&other.dummy);
			other.state = ExpectState::dummy;

			return self();
		}
		constexpr Expect& operator=(const Unexpected<ErrorType>& other) noexcept {
			destruct();
			state = ExpectState::error;
			std::construct_at<ErrorType>(&errorData, other.errorData);
			return self();
		}
		constexpr Expect& operator=(Unexpected<ErrorType>&& other) noexcept {
			destruct();
			state = ExpectState::error;
			std::construct_at<ErrorType>(&errorData, move<ErrorType>(other.errorData));
			return self();
		}
		constexpr Expect& operator=(DataType&& value) noexcept {
			destruct();
			state = ExpectState::value;
			std::construct_at<ErrorType>(&data, move<DataType>(value));
			return self();
		}

	private:
		constexpr void testValue() const noexcept {
#ifdef NATL_IN_DEBUG
			if (doesNotHaveValue()) [[unlikely]] {
				fatalError("natl: bad expected access - value_data");
			}
#endif
		}
		constexpr void testError() const noexcept {
#ifdef NATL_IN_DEBUG
			if (doesNotHaveError()) [[unlikely]] {
				fatalError("natl: bad expected access - error_data");
			}
#endif
		}

	public:
		//observers
		constexpr DataType& value() & noexcept {
			testValue();
			return data; 
		}
		constexpr const DataType& value() const& noexcept {
			testValue();
			return data; 
		};
		constexpr DataType&& value() && noexcept {
			testValue();
			return data; 
		};
		constexpr const DataType&& value() const&& noexcept { 
			testValue();
			return data; 
		};

		constexpr ErrorType& error() & noexcept { 
			testError();
			return errorData;
		}
		constexpr const ErrorType& error() const& noexcept { 
			testError();
			return errorData;
		};
		constexpr ErrorType&& error() && noexcept { 
			testError();
			return errorData;
		};
		constexpr const ErrorType&& error() const&& noexcept { 
			testError();
			return errorData;
		};

		constexpr const DataType* operator->() const noexcept { return value(); }
		constexpr DataType* operator->() noexcept { return value(); }
		constexpr const DataType& operator*() const& noexcept { return value(); }
		constexpr DataType& operator*() & noexcept { return value(); }
		constexpr const DataType&& operator*() const&& noexcept { return value(); }
		constexpr DataType&& operator*() && noexcept { return value(); }

		constexpr bool hasValue() const noexcept { return state == ExpectState::value; }
		constexpr bool doesNotHaveValue() const noexcept { return !hasValue(); }
		constexpr bool hasError() const noexcept { return state == ExpectState::error; }
		constexpr bool doesNotHaveError() const noexcept { return !hasError(); }
		constexpr explicit operator bool() const noexcept { return hasValue(); }

		constexpr DataType valueOr(DataType&& defaultValue) const& noexcept {
			return bool(*this) ? **this : forward<DataType>(defaultValue);
		}
		constexpr DataType valueOr(DataType&& defaultValue) && noexcept {
			return bool(*this) ? move(**this) : forward<DataType>(defaultValue);
		}

		//modifiers
		template<class... Args>
			requires(std::is_constructible_v<DataType, Args...>)
		constexpr DataType& emplace(Args&&... args) noexcept {
			destruct();
			state = ExpectState::value;
			std::construct_at<DataType>(data, forward(args)...);
			return data;
		}
		constexpr void swap(Expect& other) noexcept {
			Expect temp = move(other);
			other = move(self());
			self() = move(temp);
		}
	};

	template<class ErrorType>
	constexpr Unexpected<ErrorType> unexpected(const ErrorType& error) {
		return Unexpected<ErrorType>(error);
	}
	template<class ErrorType>
	constexpr Unexpected<ErrorType> unexpected(ErrorType&& error) {
		return Unexpected<ErrorType>(move(error));
	}
}