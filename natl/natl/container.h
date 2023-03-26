//header
#pragma once
//own
#include "pch.h"
//endHeader

/*module
export module natl.container;
//std
import std;
endModule*/

//interface
namespace natl {
	template<class Contanier>
	concept HasBeginIterator = requires(Contanier container) {
		{ container.begin() };
	};

	template<class Contanier>
	concept HasEndIterator = requires(Contanier container) {
		{ container.end() };
	};

	template<class Contanier>
	concept HasConstBeginIterator = requires(Contanier container) {
		{ container.cbegin() };
	};

	template<class Contanier>
	concept HasConstEndIterator = requires(Contanier container) {
		{ container.cend() };
	};

	template<class Iter>
	concept IteratorConvertableToPtr = requires(Iter iter) {
		{&*iter} -> std::convertible_to<typename std::iterator_traits<Iter>::value_type*>;
	};

	template<class Contanier>
	concept HasBeginIteratorToPtr = requires(Contanier contanier) {
		{ contanier.begin() } -> IteratorConvertableToPtr;
	};

	template<class Contanier>
	concept HasEndIteratorToPtr = requires(Contanier container) {
		{ container.end() } -> IteratorConvertableToPtr;
	};

	template<class Contanier>
	concept HasConstBeginIteratorToPtr = requires(Contanier container) {
		{ container.cbegin() } -> IteratorConvertableToPtr;
	};

	template<class Contanier>
	concept HasConstEndIteratorToPtr = requires(Contanier container) {
		{ container.cend() } -> IteratorConvertableToPtr;
	};


	template<class Contanier>
	concept HasSizeMethod = requires(const Contanier container) {
		{ container.size() } -> std::convertible_to<std::size_t>;
	};

	template<class Contanier>
	requires(HasIteratorType<Contanier>)
	struct ContainerIteratorTraits {
	public:
		using difference_type = std::iterator_traits<typename Contanier::iterator>::difference_type;
		using value_type = std::iterator_traits<typename Contanier::iterator>::value_type;
		using pointer = std::iterator_traits<typename Contanier::iterator>::pointer;
		using reference = std::iterator_traits<typename Contanier::iterator>::reference;
		using iterator_category = std::iterator_traits<typename Contanier::iterator>::iterator_category;
	};

	template <class Iter>
	constexpr void* voidPtrAddressFromIter(Iter iter) noexcept {
		if constexpr (std::is_pointer_v<Iter>) {
			return static_cast<void*>(iter);
		} else {
			return static_cast<void*>(std::addressof<decltype(*iter)>(*iter));
		}
	}

	template <class Type, class... Args>
	constexpr Type* constructAt(Type* const location, Args&&... args) noexcept(
		noexcept(::new(voidPtrAddressFromIter<Type>(location)) Type(std::forward<Args>(args)...))) {
		return ::new (voidPtrAddressFromIter<Type>(location)) Type(std::forward<Args>(args)...);
	}

	template <class Type, class... Args>
	constexpr void constructInPlace(Type& iter, Args&&... args) noexcept(
		std::is_nothrow_constructible_v<Type, Args...>) {
		if (std::is_constant_evaluated()) {
			std::construct_at(std::addressof<Type>(iter), std::forward<Type>(args)...);
		} else {
			::new (voidPtrAddressFromIter<Type>(std::addressof<Type>(iter))) Type(std::forward<Args>(args)...);
		}
	}

	template <typename Optional_T> struct GetOptionalContainedType;
	template<typename T>
	struct GetOptionalContainedType<std::optional<T>> {
		using type = T;
	};
}