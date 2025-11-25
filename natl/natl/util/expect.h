#pragma once

//@begin_non_modules
//own
#include "compilerDependent.h"
//@end_non_modules

//own
#include "../util/basicTypes.h"
#include "../util/dataMovement.h"
#include "../util/error.h"
#include "optionPtr.h"

//@export
namespace natl {
	template<typename ErrorType>
	struct Unexpected {
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

	enum struct ExpectState : i8 {
		dummy = 0,
		value,
		error,
	};

	template<typename DataType, typename ErrorType>
	struct Expect {
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
		constexpr Expect(const DataType& value) : state(ExpectState::value), data(value) {}
		constexpr Expect(DataType&& value) : state(ExpectState::value), data(natl::move(value)) {}
		constexpr Expect(const ErrorType& error) : state(ExpectState::error), errorData(error) {}
		constexpr Expect(ErrorType&& error) : state(ExpectState::error), errorData(natl::forward<ErrorType>(error)) {}
		constexpr Expect(const Unexpected<ErrorType>& unexpectd) : state(ExpectState::error), errorData(unexpectd.error()) {}
		constexpr Expect(Unexpected<ErrorType>&& unexpectd) : state(ExpectState::error), errorData(unexpectd.error()) {}

		constexpr Expect(const Expect& other) {
			switch (other.state) {
			break; case ExpectState::dummy:
				natl::deconstruct<Dummy>(&dummy);
			break; case ExpectState::value:
				natl::construct<DataType>(&data, other.data);
			break; case ExpectState::error:
				natl::construct<ErrorType>(&errorData, other.errorData);
			break; default:
				break;
			}
		}
		constexpr Expect(Expect&& other) {
			state = other.state;

			switch (other.state) {
			break; case ExpectState::dummy:
				natl::deconstruct<Dummy>(&dummy);
			break; case ExpectState::value:
				natl::construct<DataType>(&data, natl::forward<DataType>(other.data));
			break; case ExpectState::error:
				natl::construct<ErrorType>(&errorData, natl::forward<ErrorType>(other.errorData));
			break; default:
				break;
			}

			other.destruct();
			natl::construct<Dummy>(&other.dummy);
			other.state = ExpectState::dummy;
		}

		//destructor 
	private:
		constexpr void destruct() noexcept {
			switch (state) {
			break; case ExpectState::dummy:
				natl::deconstruct<Dummy>(&dummy);
			break; case ExpectState::value:
				natl::deconstruct<DataType>(&data);
			break; case ExpectState::error:
				natl::deconstruct<ErrorType>(&errorData);
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
			break; case ExpectState::dummy:
				natl::deconstruct<Dummy>(&dummy);
			break; case ExpectState::value:
				natl::construct<DataType>(&data, other.data);
			break; case ExpectState::error:
				natl::construct<ErrorType>(&errorData, other.errorData);
			break; default:
				break;
			}

			return self();
		}
		constexpr Expect& operator=(Expect&& other) noexcept {
			destruct();
			state = other.state;

			switch (other.state) {
			break; case ExpectState::dummy:
				natl::deconstruct<Dummy>(&dummy);
			break; case ExpectState::value:
				natl::construct<DataType>(&data, natl::forward<DataType>(other.data));
			break; case ExpectState::error:
				natl::construct<ErrorType>(&errorData, natl::forward<ErrorType>(other.errorData));
			break; default:
				break;
			}

			other.destruct();
			natl::construct<Dummy>(&other.dummy);
			other.state = ExpectState::dummy;

			return self();
		}
		constexpr Expect& operator=(const Unexpected<ErrorType>& other) noexcept {
			destruct();
			state = ExpectState::error;
			natl::construct<ErrorType>(&errorData, other.errorData);
			return self();
		}
		constexpr Expect& operator=(Unexpected<ErrorType>&& other) noexcept {
			destruct();
			state = ExpectState::error;
			natl::construct<ErrorType>(&errorData, move<ErrorType>(other.errorData));
			return self();
		}
		constexpr Expect& operator=(DataType&& value) noexcept {
			destruct();
			state = ExpectState::value;
			natl::construct<DataType>(&data, natl::forward<DataType>(value));
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
			return move(data); 
		};
		constexpr const DataType&& value() const&& noexcept { 
			testValue();
			return move(data); 
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
			return move(errorData);
		};
		constexpr const ErrorType&& error() const&& noexcept { 
			testError();
			return move(errorData);
		};

		constexpr OptionPtr<DataType> optionalValue()& noexcept {
			if (state == ExpectState::value) {
				return &data;
			} else {
				return natl::OptionEmpty{};
			}
		}
		constexpr OptionPtr<DataType> optionalValue() const& noexcept {
			if (state == ExpectState::value) {
				return &data;
			} else {
				return natl::OptionEmpty{};
			}
		};
		constexpr OptionPtr<DataType> optionalValue() && noexcept {
			if (state == ExpectState::value) {
				return &data;
			} else {
				return natl::OptionEmpty{};
			}
		};
		constexpr OptionPtr<DataType> optionalValue() const&& noexcept {
			if (state == ExpectState::value) {
				return &data;
			} else {
				return natl::OptionEmpty{};
			}
		};

		constexpr OptionPtr<ErrorType> optionalError() & noexcept {
			if (state == ExpectState::value) {
				return &errorData;
			} else {
				return natl::OptionEmpty{};
			}
		}
		constexpr OptionPtr<ErrorType> optionalError() const& noexcept {
			if (state == ExpectState::value) {
				return &errorData;
			} else {
				return natl::OptionEmpty{};
			}
		};
		constexpr OptionPtr<ErrorType> optionalError() && noexcept {
			if (state == ExpectState::value) {
				return &errorData;
			} else {
				return natl::OptionEmpty{};
			}
		};
		constexpr OptionPtr<ErrorType> optionalError() const&& noexcept {
			if (state == ExpectState::value) {
				return &errorData;
			} else {
				return natl::OptionEmpty{};
			}
		};

		constexpr const DataType* operator->() const noexcept { return value(); }
		constexpr DataType* operator->() noexcept { return value(); }
		constexpr const DataType& operator*() const& noexcept { return value(); }
		constexpr DataType& operator*() & noexcept { return value(); }
		constexpr const DataType&& operator*() const&& noexcept { return value(); }
		constexpr DataType&& operator*() && noexcept { return value(); }

		constexpr Bool hasValue() const noexcept { return state == ExpectState::value; }
		constexpr Bool doesNotHaveValue() const noexcept { return !hasValue(); }
		constexpr Bool hasError() const noexcept { return state == ExpectState::error; }
		constexpr Bool doesNotHaveError() const noexcept { return !hasError(); }
		constexpr explicit operator Bool() const noexcept { return hasValue(); }

		constexpr DataType valueOr(DataType&& defaultValue) const& noexcept {
			return Bool(*this) ? **this : forward<DataType>(defaultValue);
		}
		constexpr DataType valueOr(DataType&& defaultValue) && noexcept {
			return Bool(*this) ? move(**this) : forward<DataType>(defaultValue);
		}

		//modifiers
		template<typename... Args>
			requires(IsConstructibleC<DataType, Args...>)
		constexpr DataType& emplace(Args&&... args) noexcept {
			destruct();
			state = ExpectState::value;
			natl::construct<DataType>(data, forward(args)...);
			return data;
		}
		constexpr void swap(Expect& other) noexcept {
			Expect temp = move(other);
			other = move(self());
			self() = move(temp);
		}
	};

	template<typename ErrorType>
	constexpr Unexpected<Decay<ErrorType>> unexpected(const ErrorType& error) {
		return Unexpected<Decay<ErrorType>>(error);
	}
	template<typename ErrorType>
	constexpr Unexpected<Decay<ErrorType>> unexpected(ErrorType&& error) {
		return Unexpected<Decay<ErrorType>>(move(error));
	}

	template<typename Type> struct IsExpectV : FalseType {};
	template<typename Type> constexpr inline Bool IsExpect = IsExpectV<Type>::value;
	template<typename Type> concept IsExpectC = IsExpect<Type>;

	template<typename DataType, typename ErrorType>
	struct IsExpectV<Expect<DataType, ErrorType>> : TrueType {};
}