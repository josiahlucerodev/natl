#pragma once

//std
#include <cstdint>

//own
#include "container.h"
#include "iterators.h"
#include "option.h"

//interface
namespace natl {
	template<class Type>
	class PtrView {
	public:
		using value_type = Type;
		using reference = Type&;
		using const_reference = const Type&;
		using pointer = Type*;
		using const_pointer = Type*;
		using optional_pointer = Option<Type*>;
		using optional_const_pointer = Option<const Type*>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;

		using iterator = RandomAccessIterator<Type>;
		using const_iterator = ConstRandomAccessIterator<Type>;
		using reverse_iterator = ReverseRandomAccessIterator<Type>;
		using const_reverse_iterator = ReverseConstRandomAccessIterator<Type>;
	private:
		pointer beginPtr;
		pointer endPtr;
		size_type viewSize;
	public:
		constexpr PtrView() : beginPtr(nullptr), endPtr(nullptr), viewSize(0) {}
		constexpr PtrView(pointer const ptr, const size_type size) : beginPtr(ptr), beginPtr(ptr), endPtr(ptr + size), viewSize(size) {}

		constexpr const size_type& getSize() const noexcept { return viewSize; }
		constexpr size_type size() const noexcept { return viewSize; }
		constexpr pointer data() const noexcept { return beginPtr; };

		constexpr pointer getBeginPtr() noexcept { return beginPtr; }
		constexpr const_pointer getBeginPtr() const noexcept { return beginPtr; }
		constexpr pointer getEndPtr() noexcept { return endPtr; }
		constexpr const_pointer getEndPtr() const noexcept { return endPtr; }

		constexpr iterator begin() noexcept { return iterator(getBeginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(getBeginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(getBeginPtr()); }
		constexpr iterator end() noexcept { return iterator(getEndPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(getEndPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(getEndPtr()); }
		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(getEndPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(getEndPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(getEndPtr()); }
		constexpr reverse_iterator rend() noexcept { return reverse_iterator(getBeginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(getBeginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(getBeginPtr()); }

		constexpr bool isEmpty() const noexcept { return !bool(viewSize); }
		constexpr bool isNotEmpty() const noexcept { return bool(viewSize); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr reference getRefAt(size_type& index) noexcept { return beginPtr[index]; };
		constexpr const_reference getRefAt(size_type& index) const noexcept { return beginPtr[index]; };
		constexpr pointer getPtrAt(size_type& index) noexcept { return &beginPtr[index]; };
		constexpr const pointer getPtrAt(size_type& index) const noexcept { return &beginPtr[index]; };

		constexpr size_type clampIndex(const size_type index) noexcept { return std::clamp<size_type>(index, 0, viewSize); }

		constexpr reference getClampedRefAt(size_type& index) noexcept { return beginPtr[clampIndex(index)]; }
		constexpr const pointer getClampedRefAt(size_type& index) const noexcept { return beginPtr[clampIndex(index)]; }
		constexpr pointer getClampedPtrAt(size_type& index) noexcept { return &beginPtr[clampIndex(index)]; }
		constexpr const pointer getClampedPtrAt(size_type& index) const noexcept { return &beginPtr[clampIndex(index)]; }

		constexpr size_type getFrontIndex() noexcept { return 0; }
		constexpr size_type getBackIndex() noexcept { return viewSize ? 0 : viewSize - 1; }

		constexpr reference front() noexcept { return getRefAt(getFrontIndex()); }
		constexpr const_reference front() const noexcept { return getRefAt(getFrontIndex()); }
		constexpr reference back() { return getRefAt(getBackIndex()); }
		constexpr const_reference back() const noexcept { return getRefAt(getBackIndex()); }

		constexpr bool has(size_type& index) const noexcept { return index < viewSize; };
		constexpr bool notHave(size_type& index) const noexcept { return index >= viewSize; };

		constexpr optional_pointer optionalGetPtrAt(size_type& index) noexcept {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(getPtrAt(index));
		};
		constexpr optional_const_pointer optionalGetPtrAt(size_type& index) const noexcept {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(getPtrAt(index));
		};

		constexpr optional_pointer optionalFront() noexcept { return optionalGetRefAt(getFrontIndex()); }
		constexpr optional_const_pointer optionalFront() const noexcept { return optionalGetRefAt(getFrontIndex()); }
		constexpr optional_pointer optionalBack() noexcept { return optionalGetRefAt(getBackIndex()); }
		constexpr optional_const_pointer optionalBack() const noexcept { return optionalGetRefAt(getBackIndex()); }

		constexpr void copy(std::initializer_list<Type> initList) noexcept {
			const size_type count = std::max<size_type>(initList.size(), size());
			for (size_type i = 0; i < count; i++) {
				getRefAt(i) = initList[i];
			}
		}
		constexpr void sizeMatchCopy(std::initializer_list<Type> initList) noexcept {
			if (initList.size() != size()) { return; }
			copy(initList);
		}

		constexpr void assign(std::initializer_list<Type> initList) noexcept {
			beginPtr = initList.begin();
			endPtr = initList.end();
			viewSize = initList.size();
		}
		constexpr void operator=(std::initializer_list<Type> initList) noexcept { assign(initList); }
	};

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

	template<class Type>
	constexpr PtrView<Type> createPtrView(Type* const ptr, const std::size_t size) noexcept {
		return PtrView<Type>(ptr, size);
	}

	template<class Type>
	concept PtrViewConstructable = HasBegin<Type> && std::random_access_iterator<Type>;

	template<PtrViewConstructable Container,
		class Iter = Container::iterator,
		typename ValueType = typename std::iterator_traits<Iter>::value_type>
	constexpr PtrView<ValueType> createPtrViewFromContainer(ValueType* const ptr, const std::size_t size) noexcept {
		return PtrView<ValueType>(ptr, size);
	}

	template<class Type>
	concept ViewConstructable = HasBegin<Type> && HasEnd<Type> && HasIteratorType<Type>;

	template<ViewConstructable Container, class Iter = Container::iterator>
	constexpr View<Iter> createView(Container& container) noexcept {
		return View<Iter>(container.begin(), container.end());
	}
}