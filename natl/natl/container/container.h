#pragma once

//std
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <concepts>

//own
#include "../util/iterators.h"
#include "../util/typeTraits.h"

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
		{&*iter} -> ConvertibleTo<typename Iter::value_type*>;
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
		{ container.size() } -> ConvertibleTo<Size>;
	};

	template<class Container, class DataType>
	concept ContainerHasPushBackFunction = requires(Container container) {
		{ container.push_back(natl::declval<DataType>()) };
	};

	template<class Contanier>
		requires(HasIteratorType<Contanier>)
	struct ContainerIteratorTraits {
	public:
		using difference_type = IteratorTraits<typename Contanier::iterator>::difference_type;
		using value_type = IteratorTraits<typename Contanier::iterator>::value_type;
		using pointer = IteratorTraits<typename Contanier::iterator>::pointer;
		using reference = IteratorTraits<typename Contanier::iterator>::reference;
		using iterator_category = IteratorTraits<typename Contanier::iterator>::iterator_category;
	};

	template <typename... Contaniers>
	constexpr Size sumContaniersSizes(const Contaniers&... contaniers) noexcept {
		return (contaniers.size() + ...);
	}

	template <typename DataType>
	constexpr void exchange(DataType& a, DataType& b) noexcept {
		DataType temp = move(a);
		a = move(b);
		b = move(temp);
	}

	template<class ForwardIter1, class ForwardIter2>
	constexpr void iterSwap(ForwardIter1 a, ForwardIter2 b) noexcept {
		swap<typename IteratorTraits<ForwardIter1>::value_type>(*a, *b);
	}

	template<class BidirectionalIter>
	constexpr void reverse(BidirectionalIter first, BidirectionalIter last) noexcept {
		using iterator_category = typename natl::IteratorTraits<BidirectionalIter>::iterator_category;
		if constexpr (IsSameC<RandomAccessIteratorTag, iterator_category>) {
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