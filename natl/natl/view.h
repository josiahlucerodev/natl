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
		PtrView() : beginPtr(nullptr), endPtr(nullptr), viewSize(0) {}
		PtrView(pointer const ptr, const size_type size) : beginPtr(ptr), beginPtr(ptr), endPtr(ptr + size), viewSize(size) {}

		const size_type& getSize() const noexcept { return viewSize; }
		size_type size() const noexcept { return viewSize; }
		pointer data() const noexcept { return beginPtr; };

		pointer getBeginPtr() noexcept { return beginPtr; }
		const_pointer getBeginPtr() const noexcept { return beginPtr; }
		pointer getEndPtr() noexcept { return endPtr; }
		const_pointer getEndPtr() const noexcept { return endPtr; }

		iterator begin() noexcept { return iterator(getBeginPtr()); }
		const_iterator begin() const noexcept { return const_iterator(getBeginPtr()); }
		const_iterator cbegin() const noexcept { return const_iterator(getBeginPtr()); }
		iterator end() noexcept { return iterator(getEndPtr()); }
		const_iterator end() const noexcept { return const_iterator(getEndPtr()); }
		const_iterator cend() const noexcept { return const_iterator(getEndPtr()); }
		reverse_iterator rbegin() noexcept { return reverse_iterator(getEndPtr()); }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(getEndPtr()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(getEndPtr()); }
		reverse_iterator rend() noexcept { return reverse_iterator(getBeginPtr()); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(getBeginPtr()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(getBeginPtr()); }

		bool isEmpty() const noexcept { return !bool(viewSize); }
		bool isNotEmpty() const noexcept { return bool(viewSize); }
		operator bool() const noexcept { return isNotEmpty(); }

		reference getRefAt(size_type& index) { return beginPtr[index]; };
		const_reference getRefAt(size_type& index) const { return beginPtr[index]; };
		pointer getPtrAt(size_type& index) { return &beginPtr[index]; };
		const pointer getPtrAt(size_type& index) const { return &beginPtr[index]; };

		size_type clampIndex(const size_type index) { return std::clamp<size_type>(index, 0, viewSize); }

		reference getClampedRefAt(size_type& index) { return beginPtr[clampIndex(index)]; }
		const pointer getClampedRefAt(size_type& index) const { return beginPtr[clampIndex(index)]; }
		pointer getClampedPtrAt(size_type& index) { return &beginPtr[clampIndex(index)]; }
		const pointer getClampedPtrAt(size_type& index) const { return &beginPtr[clampIndex(index)]; }

		size_type getFrontIndex() { return 0; }
		size_type getBackIndex() { return viewSize ? 0 : viewSize - 1; }

		reference front() { return getRefAt(getFrontIndex()); }
		const_reference front() const { return getRefAt(getFrontIndex()); }
		reference back() { return getRefAt(getBackIndex()); }
		const_reference back() const { return getRefAt(getBackIndex()); }

		bool has(size_type& index) const { return index < viewSize; };
		bool notHave(size_type& index) const { return index >= viewSize; };

		optional_pointer optionalGetPtrAt(size_type& index) {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(getPtrAt(index));
		};
		optional_const_pointer optionalGetPtrAt(size_type& index) const {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(getPtrAt(index));
		};

		optional_pointer optionalFront() { return optionalGetRefAt(getFrontIndex()); }
		optional_const_pointer optionalFront() const { return optionalGetRefAt(getFrontIndex()); }
		optional_pointer optionalBack() { return optionalGetRefAt(getBackIndex()); }
		optional_const_pointer optionalBack() const { return optionalGetRefAt(getBackIndex()); }

		void copy(std::initializer_list<Type> initList) {
			const size_type count = std::max<size_type>(initList.size(), size());
			for (size_type i = 0; i < count; i++) {
				getRefAt(i) = initList[i];
			}
		}
		void sizeMatchCopy(std::initializer_list<Type> initList) {
			if (initList.size() != size()) { return; }
			copy(initList);
		}

		void assign(std::initializer_list<Type> initList) {
			beginPtr = initList.begin();
			endPtr = initList.end();
			viewSize = initList.size();
		}
		void operator=(std::initializer_list<Type> initList) { assign(initList); }
	};

	template<class Iter>
	class View {
	private:
		Iter beginIter;
		Iter endIter;
	public:
		View(const Iter beginIter, const Iter endIter) : beginIter(beginIter), endIter(endIter) {}
	public:
		Iter begin() noexcept { return beginIter; }
		Iter end() const noexcept { return endIter; }
	};

	template<class Type>
	PtrView<Type> createPtrView(Type* const ptr, const std::size_t size) {
		return PtrView<Type>(ptr, size);
	}

	template<class Type>
	concept PtrViewConstructable = HasBegin<Type> && std::random_access_iterator<Type>;

	template<PtrViewConstructable Container,
		class Iter = Container::iterator,
		typename ValueType = typename std::iterator_traits<Iter>::value_type>
	PtrView<ValueType> createPtrViewFromContainer(ValueType* const ptr, const std::size_t size) {
		return PtrView<ValueType>(ptr, size);
	}

	template<class Type>
	concept ViewConstructable = HasBegin<Type> && HasEnd<Type> && HasIteratorType<Type>;

	template<ViewConstructable Container, class Iter = Container::iterator>
	View<Iter> createView(Container& container) {
		return View<Iter>(container.begin(), container.end());
	}
}