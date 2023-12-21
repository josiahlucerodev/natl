#pragma once

//std
#include <cstdint>

//own
#include "container.h"
#include "iterators.h"
#include "option.h"

//interface
namespace natl {
	template<class Iter>
	class View {
	private:
		Iter beginIter;
		Iter endIter;
	public:
		constexpr View() : beginIter(), endIter() {}
		constexpr View(const Iter beginIter, const Iter endIter) : beginIter(beginIter), endIter(endIter) {}
	public:
		constexpr Iter begin() noexcept { return beginIter; }
		constexpr Iter end() const noexcept { return endIter; }
	};

	template<class Container>
	concept ViewConstructable = HasIteratorType<Container> && std::contiguous_iterator<typename Container::iterator> && 
		requires(Container container) {
			{ container.end() } -> std::convertible_to<typename Container::iterator>;
			{ container.begin() } -> std::convertible_to<typename Container::iterator>;
	};

	template<class Container, class Iter = Container::iterator>
		requires(ViewConstructable<Container>)
	constexpr View<Iter> createView(Container& container) noexcept {
		return View<Iter>(container.begin(), container.end());
	}
}