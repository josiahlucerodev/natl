#pragma once

//std
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <concepts>

//own
#include "iterators.h"

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

	template <typename... Contaniers>
	constexpr std::size_t sumContaniersSizes(const Contaniers&... contaniers) noexcept {
		return (contaniers.size() + ...);
	}

	template <typename T>
	constexpr typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
		return static_cast<typename std::remove_reference<T>::type&&>(arg);
	}

	template <typename T>
	constexpr T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
		return static_cast<T&&>(arg);
	}

	template <typename T>
	constexpr T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
		static_assert(!std::is_lvalue_reference<T>::value, "template argument substituting T is an lvalue reference type");
		return static_cast<T&&>(arg);
	}

	template <typename DataType>
	constexpr void swap(DataType& a, DataType& b) noexcept {
		DataType temp = move(a);
		a = move(b);
		b = move(temp);
	}

	template<class ForwardIter1, class ForwardIter2>
	constexpr void iterSwap(ForwardIter1 a, ForwardIter2 b) noexcept {
		swap<typename ForwardIter1::value_type>(*a, *b);
	}

	template<class BidirectionalIter>
	constexpr void reverse(BidirectionalIter first, BidirectionalIter last) noexcept {
		using iterator_category = typename std::iterator_traits<BidirectionalIter>::iterator_category;
		if constexpr (std::is_base_of_v<std::random_access_iterator_tag, iterator_category>) {
			if (first == last) { return; }
			for (--last; first < last; (void)++first, --last) {
				iterSwap<BidirectionalIter, BidirectionalIter>(first, last);
			}
		}
		else {
			while (first != last && first != --last) {
				iterSwap<BidirectionalIter, BidirectionalIter>(first++, last);
			}
		}
	}
}