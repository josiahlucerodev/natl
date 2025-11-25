#pragma once

//@begin_non_modules
//std
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <concepts>
//@end_non_modules

//own
#include "../util/iterators.h"
#include "../util/typeTraits.h"

//@export
namespace natl {
	template<typename Contanier>
	concept HasBeginIterator = requires(Contanier container) {
		{ container.begin() };
	};

	template<typename Contanier>
	concept HasEndIterator = requires(Contanier container) {
		{ container.end() };
	};

	template<typename Contanier>
	concept HasConstBeginIterator = requires(Contanier container) {
		{ container.cbegin() };
	};

	template<typename Contanier>
	concept HasConstEndIterator = requires(Contanier container) {
		{ container.cend() };
	};

	template<typename Iter>
	concept IteratorConvertableToPtr = requires(Iter iter) {
		{&*iter} -> ConvertibleTo<typename Iter::value_type*>;
	};

	template<typename Contanier>
	concept HasBeginIteratorToPtr = requires(Contanier contanier) {
		{ contanier.begin() } -> IteratorConvertableToPtr;
	};

	template<typename Contanier>
	concept HasEndIteratorToPtr = requires(Contanier container) {
		{ container.end() } -> IteratorConvertableToPtr;
	};

	template<typename Contanier>
	concept HasConstBeginIteratorToPtr = requires(Contanier container) {
		{ container.cbegin() } -> IteratorConvertableToPtr;
	};

	template<typename Contanier>
	concept HasConstEndIteratorToPtr = requires(Contanier container) {
		{ container.cend() } -> IteratorConvertableToPtr;
	};

	template<typename Contanier>
	concept HasSizeMethod = requires(const Contanier container) {
		{ container.size() } -> ConvertibleTo<Size>;
	};

	template<typename Container, typename DataType>
	concept ContainerHasPushBackFunction = requires(Container container) {
		{ container.pushBack(natl::declval<DataType>()) };
	};

	template<typename Contanier>
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

	template<typename ForwardIter1, typename ForwardIter2>
	constexpr void iterSwap(ForwardIter1 a, ForwardIter2 b) noexcept {
		swap<typename IteratorTraits<ForwardIter1>::value_type>(*a, *b);
	}

	template<typename BidirectionalIter>
	constexpr void reverse(BidirectionalIter first, BidirectionalIter last) noexcept {
		using iterator_category = typename natl::IteratorTraits<BidirectionalIter>::iterator_category;
		if constexpr (IsSameC<RandomAccessIteratorTag, iterator_category>) {
			if (first == last) { return; }
			for (--last; first < last; (void)++first, --last) {
				iterSwap<BidirectionalIter, BidirectionalIter>(first, last);
			}
		} else {
			while (first != last && first != --last) {
				iterSwap<BidirectionalIter, BidirectionalIter>(first++, last);
			}
		}
	}
}