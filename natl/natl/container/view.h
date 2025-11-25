#pragma once

//own
#include "container.h"
#include "../util/iterators.h"
#include "../util/option.h"

//@export
namespace natl {
	template<typename Iter>
	struct View {
	private:
		Iter beginIter;
		Iter endIter;
	public:
		constexpr View() : beginIter(), endIter() {}
		constexpr View(const Iter beginIterIn, const Iter endIterIn) : beginIter(beginIterIn), endIter(endIterIn) {}
	public:
		constexpr Iter begin() noexcept { return beginIter; }
		constexpr Iter end() const noexcept { return endIter; }
	};

	template<typename Container>
	concept ViewConstructable = HasIteratorType<Container> && IsRandomAccessIteratorC<typename Container::iterator> && 
		requires(Container container) {
			{ container.end() } -> ConvertibleTo<typename Container::iterator>;
			{ container.begin() } -> ConvertibleTo<typename Container::iterator>;
	};

	template<typename Container, typename Iter = Container::iterator>
		requires(ViewConstructable<Container>)
	constexpr View<Iter> createView(Container& container) noexcept {
		return View<Iter>(container.begin(), container.end());
	}
}