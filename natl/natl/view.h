//header
#pragma once
//own
#include "pch.h"
#include "container.h"
#include "iterators.h".h"
//endHeader

/*module
export module natl.view;
//std
import std;
//own
import natl.iterators
import natl.container
endModule*/

//interface
namespace natl {
	template<class Type>
	class PtrView {
	public:
		using value_type = Type;
		using reference = Type&;
		using const_reference = const Type&;
		using optional_reference = std::optional<std::reference_wrapper<Type>>;
		using optional_const_reference = std::optional<std::reference_wrapper<const Type>>;
		using pointer = Type*;
		using const_pointer = Type*;
		using optional_pointer = std::optional<Type*>;
		using optional_const_pointer = std::optional<const Type*>;
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
		const_pointer getConstBeginPtr() const noexcept { return beginPtr; }
		pointer getEndPtr() noexcept { return endPtr; }
		const_pointer getConstEndPtr() const noexcept { return endPtr; }

		iterator begin() noexcept { return iterator(getBeginPtr()); }
		const_iterator begin() const noexcept { return const_iterator(getConstBeginPtr()); }
		const_iterator cbegin() const noexcept { return const_iterator(getConstBeginPtr()); }
		iterator end() noexcept { return iterator(getEndPtr()); }
		const_iterator end() const noexcept { return const_iterator(getConstEndPtr()); }
		const_iterator cend() const noexcept { return const_iterator(getConstEndPtr()); }
		reverse_iterator rbegin() noexcept { return reverse_iterator(getEndPtr()); }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(getConstEndPtr()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(getConstEndPtr()); }
		reverse_iterator rend() noexcept { return reverse_iterator(getBeginPtr()); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(getConstBeginPtr()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(getConstBeginPtr()); }

		bool isEmpty() const noexcept { return !bool(viewSize); }
		bool isNotEmpty() const noexcept { return bool(viewSize); }
		operator bool() const noexcept { return isNotEmpty(); }

		const_reference getConstRefAt(size_type& index) const { return beginPtr[index]; };
		reference getRefAt(size_type& index) { return beginPtr[index]; };
		const pointer getConstPtrAt(size_type& index) const { return &beginPtr[index]; };
		pointer getPtrAt(size_type& index) { return &beginPtr[index]; };

		size_type clampIndex(const size_type index) { return std::clamp<size_type>(index, 0, viewSize); }

		const pointer getClampedConstRefAt(size_type& index) const { return beginPtr[clampIndex(index)]; }
		reference getClampedRefAt(size_type& index) { return beginPtr[clampIndex(index)]; }
		const pointer getClampedConstPtrAt(size_type& index) const { return &beginPtr[clampIndex(index)]; }
		pointer getClampedPtrAt(size_type& index) { return &beginPtr[clampIndex(index)]; }

		size_type getFrontIndex() { return 0; }
		size_type getBackIndex() { return viewSize ? 0 : viewSize - 1; }

		reference front() { return getRefAt(getFrontIndex()); }
		const_reference front() const { return getConstRefAt(getFrontIndex()); }
		reference back() { return getRefAt(getBackIndex()); }
		const_reference back() const { return getConstRefAt(getBackIndex()); }

		bool has(size_type& index) const { return index < viewSize; };
		bool notHave(size_type& index) const { return index >= viewSize; };

		optional_const_reference optionalGetConstRefAt(size_type& index) const {
			if (notHave(index)) { return optional_const_reference(); }
			return optional_const_reference(getConstRefAt(index));
		};
		optional_reference optionalGetRefAt(size_type& index) {
			if (notHave(index)) { return optional_reference(); }
			return optional_reference(getRefAt(index));
		};

		optional_const_pointer optionalGetConstPtrAt(size_type& index) {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(getConstPtrAt(index));
		};
		optional_pointer optionalGetPtrAt(size_type& index) {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(getPtrAt(index));
		};

		optional_reference optionalFront() { return optionalGetRefAt(getFrontIndex()); }
		optional_const_reference optionalFront() const { return optionalGetConstRefAt(getFrontIndex()); }
		optional_reference optionalBack() { return optionalGetRefAt(getBackIndex()); }
		optional_const_reference optionalBack() const { return optionalGetConstRefAt(getBackIndex()); }

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