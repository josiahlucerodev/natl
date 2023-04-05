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
}