//header
#pragma once
//own
#include "pch.h"
#include "typeTraits.h"
//endHeader

/*module
export module natl.Optional;
//std
import std;
//own
import natl.error;

endModule*/

//interface 
/*export*/ namespace natl {
	template<class Type>
	class Optional {
	public:
		constexpr static bool natl_trivially_relocatable = IsTriviallyRelocatable<Type> ? true : false;
	private:
		bool initialized;
		Type internalValue;
	public:
		constexpr bool isInitialized() const noexcept { return initialized; }
		constexpr Optional<Type>& getSelf() { return *this; }
		constexpr const Optional<Type>& getSelf() const { return *this; }

		constexpr Optional() 
			noexcept(std::is_nothrow_constructible_v<Type>) 
			requires(std::is_trivial_v<Type>) = default;
		constexpr Optional() 
			noexcept(std::is_nothrow_constructible_v<Type>) 
			requires(!std::is_trivial_v<Type>) : 
			initialized(false), 
			internalValue() {};

		constexpr ~Optional() noexcept requires(!std::is_trivially_destructible_v<Type>) { if (isInitialized()) { internalValue.Type::~Type(); }; }
		constexpr ~Optional() noexcept = default;

		explicit constexpr Optional(const Type& inputValue) : initialized(true), internalValue(inputValue) {}
		explicit constexpr Optional(Type&& inputValue) : initialized(true), internalValue(std::move<Type>(inputValue)) {}

		template <class... Args> explicit Optional(Args&&... args) : 
			initialized(true), internalValue(std::forward<Args>(args)...) {}

	private:
		constexpr void checkAccessError() {
#ifdef _DEBUG || NDEBUG
			fatelError(isInitialized(), "cannot access uninizialized natl::Optional");
#endif
		};
	public:

		constexpr Type* operator->() noexcept {
			checkAccessError();
			return &internalValue;
		}
		constexpr const Type* operator->() const noexcept {
			checkAccessError();
			return &internalValue;
		}
		constexpr const Type& operator*() const& noexcept {
			checkAccessError();
			return internalValue;
		}
		constexpr Type& operator*() & noexcept {
			checkAccessError();
			return internalValue;
		}
		constexpr const Type&& operator*() const&& noexcept {
			checkAccessError();
			return std::move<Type>(internalValue);
		}
		constexpr Type&& operator*() && noexcept {
			checkAccessError();
			return std::move<Type>(internalValue);
		}

		constexpr Type& value()& {
			checkAccessError();
			return internalValue;
		}
		constexpr const Type& value() const& {
			checkAccessError();
			return internalValue;
		}
		constexpr Type&& value()&& {
			checkAccessError();
			return std::move<Type>(internalValue);
		}
		constexpr const Type&& value() const&& {
			checkAccessError();
			return std::move<Type>(internalValue);
		}

		constexpr explicit operator bool() const noexcept {
			return isInitialized();
		}
		constexpr bool has_value() const noexcept {
			return isInitialized();
		}

		constexpr void reset() noexcept {
			if (isInitialized()) {
				initialized = false;
				value().Type::~Type();
			} 
		}

		constexpr void clear() noexcept {
			reset();
		}

		template<class DefaultValueType = Type>
		constexpr Type valueOr(DefaultValueType&& defaultValue) const& {
			static_assert(std::is_convertible_v<const Type&, std::remove_cv_t<Type>>,
				"The const overload of natl::Optional<T>::valueOr requires const Type& to be convertible to remove_cv_t<Type>");
			static_assert(std::is_convertible_v<DefaultValueType, Type>,
				"natl::Optional<Type>::valueOr(DefaultValueType) requires DefaultValueType to be convertible to Type");

			if (isInitialized()) {
				return internalValue;
			}

			return static_cast<std::remove_cv_t<Type>>(std::forward<DefaultValueType>(defaultValue));
		}

		template<class DefaultValueType = Type>
		constexpr Type valueOr(DefaultValueType&& defaultValue)&& {
			static_assert(std::is_convertible_v<const Type&, std::remove_cv_t<Type>>,
				"The const overload of natl::Optional<T>::valueOr requires const Type& to be convertible to remove_cv_t<Type>");
			static_assert(std::is_convertible_v<DefaultValueType, Type>,
				"natl::Optional<Type>::valueOr(DefaultValueType) requires DefaultValueType to be convertible to Type");

			if (isInitialized()) {
				return std::move<Type>(internalValue);
			}

			return static_cast<std::remove_cv_t<Type>>(std::forward<DefaultValueType>(defaultValue));
		}

		template<class DefaultValueType = Type>
		constexpr Type value_or(DefaultValueType&& defaultValue) const& {
			static_assert(std::is_convertible_v<const Type&, std::remove_cv_t<Type>>,
				"The const overload of natl::Optional<T>::valueOr requires const Type& to be convertible to remove_cv_t<Type>");
			static_assert(std::is_convertible_v<DefaultValueType, Type>,
				"natl::Optional<Type>::valueOr(DefaultValueType) requires DefaultValueType to be convertible to Type");

			if (isInitialized()) {
				return internalValue;
			}

			return static_cast<std::remove_cv_t<Type>>(std::forward<DefaultValueType>(defaultValue));
		}

		template<class DefaultValueType = Type>
		constexpr Type value_or(DefaultValueType&& defaultValue)&& {
			static_assert(std::is_convertible_v<const Type&, std::remove_cv_t<Type>>,
				"The const overload of natl::Optional<T>::valueOr requires const Type& to be convertible to remove_cv_t<Type>");
			static_assert(std::is_convertible_v<DefaultValueType, Type>,
				"natl::Optional<Type>::valueOr(DefaultValueType) requires DefaultValueType to be convertible to Type");

			if (isInitialized()) {
				return std::move<Type>(internalValue);
			}

			return static_cast<std::remove_cv_t<Type>>(std::forward<DefaultValueType>(defaultValue));
		}

		constexpr void swap(Optional other)
			noexcept(std::is_nothrow_move_constructible_v<Type> && std::is_nothrow_swappable_v<Type>) 
			requires(std::is_move_constructible_v<Type> && std::is_move_constructible_v<Type> || std::is_swappable_v<Type>) {
			if constexpr (IsTriviallyRelocatable<Type>) {
				std::swap<Type>(getSelf(), other);
			} else {
				const bool hasValue = has_value();
				const bool otherHasValue = other.has_value();

				if (!hasValue && !otherHasValue) {
					if (hasValue == otherHasValue) {
						using std::swap;
						swap(**this, *other);
					} else {
						Optional& src = hasValue ? *this : other;
						Optional& dst = hasValue ? other : *this;
						dst = std::move<Type>(*src);
						src.reset();
					}
				} 
			}
		}

		constexpr auto and_then(auto&& f)& {
			if (*this) {
				return f(this->value());
			} else {
				return std::remove_cvref_t<std::invoke_result_t<decltype(f), Type&>>();
			}
		}
		constexpr auto and_then(auto&& f) const& {
			if (*this) {
				return f(this->value());
			} else {
				return std::remove_cvref_t<std::invoke_result_t<decltype(f), const Type&>>();
			}
		}
		constexpr auto and_then(auto&& f)&& {
			if (*this) {
				return f(std::move<Type>(this->value()));
			} else {
				return std::remove_cvref_t<std::invoke_result_t<decltype(f), Type&>>();
			}
		}

		constexpr auto and_then(auto&& f) const&& {
			if (*this) {
				return  f(std::move<Type>(this->value()));
			} else {
				return std::remove_cvref_t<std::invoke_result_t<decltype(f), Type&>>();
			}
		}

		constexpr Optional or_else(auto&& f) const& {
			return *this ? *this : std::forward<decltype(f)>(f)();
		}
		constexpr Optional or_else(auto&& f) && {
			return *this ? std::move(*this) : std::forward<decltype(f)>(f)();
		}
	};

	template< class T, class U >
	constexpr bool operator==(const Optional<T>& lhs, const Optional<U>& rhs) 
		noexcept(noexceptExpression<bool>(*lhs > * rhs))
		requires(requires{ {*lhs == *rhs} -> std::convertible_to<bool>;}) {
		const bool lhsHasValue = lhs.has_value();
		const bool rhsHasValue = rhs.has_value();
		if (lhsHasValue && rhsHasValue) {
			return *lhs == rhs;
		} else {
			return lhsHasValue && rhsHasValue;
		}
	}
	template< class T, class U >
	constexpr bool operator!=(const Optional<T>& lhs, const Optional<U>& rhs)
		noexcept(noexceptExpression<bool>(*lhs > * rhs))
		requires(requires{ {*lhs != *rhs} -> std::convertible_to<bool>;}) {
		const bool lhsHasValue = lhs.has_value();
		const bool rhsHasValue = rhs.has_value();
		if (lhsHasValue && rhsHasValue) {
			return *lhs != rhs;
		} else {
			return lhsHasValue && rhsHasValue;
		}
	}
	template< class T, class U >
	constexpr bool operator<(const Optional<T>& lhs, const Optional<U>& rhs)
		noexcept(noexceptExpression<bool>(*lhs > * rhs))
		requires(requires{ {*lhs < *rhs} -> std::convertible_to<bool>;}) {
		const bool lhsHasValue = lhs.has_value();
		const bool rhsHasValue = rhs.has_value();
		if (lhsHasValue && rhsHasValue) {
			return *lhs < rhs;
		} else {
			return lhsHasValue && rhsHasValue;
		}
	}
	template< class T, class U >
	constexpr bool operator<=(const Optional<T>& lhs, const Optional<U>& rhs)
		noexcept(noexceptExpression<bool>(*lhs > * rhs))
		requires(requires{ {*lhs <= *rhs} -> std::convertible_to<bool>;}) {
		const bool lhsHasValue = lhs.has_value();
		const bool rhsHasValue = rhs.has_value();
		if (lhsHasValue && rhsHasValue) {
			return *lhs <= rhs;
		} else {
			return lhsHasValue && rhsHasValue;
		}
	}
	template< class T, class U >
	constexpr bool operator>(const Optional<T>& lhs, const Optional<U>& rhs)
		noexcept(noexceptExpression<bool>(*lhs > * rhs))
		requires(requires{ {*lhs > * rhs} -> std::convertible_to<bool>;}) {
		const bool lhsHasValue = lhs.has_value();
		const bool rhsHasValue = rhs.has_value();
		if (lhsHasValue && rhsHasValue) {
			return *lhs > rhs;
		} else {
			return lhsHasValue && rhsHasValue;
		}
	}
	template< class T, class U >
	constexpr bool operator>=(const Optional<T>& lhs, const Optional<U>& rhs)
		noexcept(noexceptExpression<bool>(*lhs > * rhs))
		requires(requires{ {*lhs >= *rhs} -> std::convertible_to<bool>; }) {
		const bool lhsHasValue = lhs.has_value();
		const bool rhsHasValue = rhs.has_value();
		if (lhsHasValue && rhsHasValue) {
			return *lhs >= rhs;
		}
		else {
			return lhsHasValue && rhsHasValue;
		}
	}

	template< class T>
	constexpr bool operator==(const Optional<T>& opt, std::nullopt_t) noexcept {
		return !opt;
	}
	template< class T >
	constexpr std::strong_ordering operator<=>(const Optional<T>& opt, std::nullopt_t) noexcept {
		return bool(opt) <=> false;
	}
	template< class T, class U >
	constexpr bool operator==(const Optional<T>& opt, const U& value) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType == rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? *opt == value : false;
	}
	template< class T, class U >
	constexpr bool operator==(const T& value, const Optional<U>& opt) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType == rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? value == *opt : false;
	}
	template< class T, class U >
	constexpr bool operator!=(const Optional<T>& opt, const U& value) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType != rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? *opt != value : true;
	}
	template< class T, class U >
	constexpr bool operator!=(const T& value, const Optional<U>& opt) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType != rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? value != *opt : true;
	}
	template< class T, class U >
	constexpr bool operator<(const Optional<T>& opt, const U& value) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType < rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? *opt < value : true;
	}
	template< class T, class U >
	constexpr bool operator<(const T& value, const Optional<U>& opt) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType < rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? value < *opt : false;
	}
	template< class T, class U >
	constexpr bool operator<=(const Optional<T>& opt, const U& value) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType <= rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? *opt <= value : true;
	}
	template< class T, class U >
	constexpr bool operator<=(const T& value, const Optional<U>& opt) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType <= rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? value <= *opt : false;
	}
	template< class T, class U >
	constexpr bool operator>(const Optional<T>& opt, const U& value) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType > rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? value > *opt  : true;
	}
	template< class T, class U >
	constexpr bool operator>(const T& value, const Optional<U>& opt) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType > rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? value > *opt  : true;
	}
	template< class T, class U >
	constexpr bool operator>=(const Optional<T>& opt, const U& value) noexcept
		requires(requires (T lhsType, U rhsType) { {lhsType >= rhsType } -> std::same_as<bool>; } ) {
		return bool(opt) ? *opt >= value : false;
	}
	template< class T, class U >
	constexpr bool operator>=(const T& value, const Optional<U>& opt) noexcept
		requires(requires (T lhsType, U rhsType) {{lhsType >= rhsType } -> std::same_as<bool>;}) {
		return bool(opt) ? value >= *opt : true;
	}
	template< class T, std::three_way_comparable_with<T> U >
	constexpr std::compare_three_way_result_t<T, U> operator<=>(const Optional<T>& opt, const U& value) noexcept {
		return bool(opt) ? *opt <=> value : std::strong_ordering::less;
	}
}
