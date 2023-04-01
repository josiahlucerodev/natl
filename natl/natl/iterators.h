//header
#pragma once
//own
#include "pch.h"
#include "typeTraits.h"
//endHeader

/*module
export module natl.iterators;
//std
import std;
endModule*/

//interface 
/*export*/ namespace natl {
	template<class T>
	class RandomAccessIterator {
	public:
		using iterator = RandomAccessIterator<T>;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using iterator_category = std::random_access_iterator_tag;
		using size_type = std::size_t;
	private:
		T* dataPtr;
	public:
		constexpr RandomAccessIterator(T* const dataPtr) noexcept : dataPtr(dataPtr) {}
	private:
		constexpr iterator& getSelf() noexcept { return *this; }
		constexpr const iterator& getSelf() const noexcept { return *this; }
	public:
		constexpr reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() const noexcept { return dataPtr; }
		constexpr reference operator[](const size_type pos) const noexcept { return dataPtr[pos]; };

		constexpr bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
		constexpr bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
		constexpr bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
		constexpr bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }

		constexpr iterator& operator++() noexcept { dataPtr++; return getSelf(); }
		constexpr iterator operator++(int) noexcept { iterator tempIt = getSelf(); ++getSelf().dataPtr; return tempIt; }
		constexpr iterator& operator--() noexcept { dataPtr--; return getSelf(); }
		constexpr iterator operator--(int) noexcept { iterator tempIt = getSelf(); --getSelf().dataPtr; return tempIt; }

		constexpr iterator& operator+=(const size_type offset) noexcept { dataPtr += offset; return getSelf(); }
		constexpr iterator operator+(const size_type offset) const noexcept { return iterator(dataPtr + offset); }
		constexpr friend iterator operator+(const size_type offset, iterator& rhs) noexcept { rhs += offset; return rhs; }
		constexpr iterator& operator-=(const size_type offset) noexcept { dataPtr -= offset; return getSelf(); }
		constexpr iterator operator-(const size_type offset) const noexcept { return iterator(dataPtr -= offset); }
		constexpr difference_type operator-(const iterator rhs) const noexcept { return getSelf().dataPtr - rhs.dataPtr; }
	};

	template<class T> 
	using ReverseRandomAccessIterator = std::reverse_iterator<RandomAccessIterator<T>>;

	template<class Type>
	concept HasIteratorType = requires {
		typename Type::iterator;
	};

	template <class Iter>
	using IteratorCategory = typename std::iterator_traits<Iter>::iterator_category;

	template <class Iter, class = void>
	inline constexpr bool IsIteratorV = false;

	template <class Iter>
	inline constexpr bool IsIteratorV<Iter, std::void_t<IteratorCategory<Iter>>> = true;

	template<class Iter>
	concept IsIterator = IsIteratorV<Iter>;

	template<class Ptr>
	concept IsPointer = std::is_pointer_v<std::decay_t<Ptr>>;

	template <class Iter>
	concept IsIterPtr = IsPointer<Iter> || IsIterator<Iter>;

	template <class Iter>
		requires (IsIterPtr<Iter>)
	struct IterPtrTraits {
		using value_type = std::conditional_t<std::is_pointer_v<std::decay_t<Iter>>, std::remove_pointer_t<std::decay_t<Iter>>, typename std::iterator_traits<Iter>::value_type>;
		using reference = std::conditional_t<std::is_pointer_v<std::decay_t<Iter>>, std::remove_pointer_t<std::decay_t<Iter>>&, typename std::iterator_traits<Iter>::reference>;
	};

	template <class SrcIter, class DstIter>
	concept MemcopyConstructibleIter =
		IsIterPtr<SrcIter> &&
		IsIterPtr<DstIter> &&
		MemcopyConstructibleSrcDst<
		typename IterPtrTraits<SrcIter>::value_type,
		typename IterPtrTraits<DstIter>::value_type,
		typename IterPtrTraits<SrcIter>::reference>;


	template <class SrcIter, class DstIter>
	concept MemcopyAssignableIter =
		IsIterPtr<SrcIter> &&
		IsIterPtr<DstIter> &&
		MemcopyAssignableSrcDst<
		typename IterPtrTraits<SrcIter>::value_type,
		typename IterPtrTraits<DstIter>::value_type,
		typename IterPtrTraits<SrcIter>::reference,
		typename IterPtrTraits<DstIter>::reference>;

	template <class Iter, typename value_type = typename IterPtrTraits<Iter>::value_type>
	constexpr value_type* iteratorToAddress(Iter iter) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<Iter>>) {
			return iter;
		}
		else {
			return &*iter;
		}
	}

	template <class SrcIter, class DstIter>
		requires(IsIterPtr<SrcIter>&& IsIterPtr<DstIter>)
	DstIter iterMemcopy(SrcIter first, SrcIter last, DstIter dst) noexcept {
		typename IterPtrTraits<SrcIter>::value_type* firstPtr = iteratorToAddress<SrcIter>(first);
		typename IterPtrTraits<SrcIter>::value_type* lastPtr = iteratorToAddress<SrcIter>(last);
		typename IterPtrTraits<DstIter>::value_type* dstPtr = iteratorToAddress<DstIter>(dst);

		const char* const firstVoidPtr = const_cast<const char*>(reinterpret_cast<const volatile char*>(firstPtr));
		const char* const lastVoidPtr = const_cast<const char*>(reinterpret_cast<const volatile char*>(lastPtr));
		char* const dstVoidPtr = const_cast<char*>(reinterpret_cast<const volatile char*>(dstPtr));

		const std::size_t count = static_cast<std::size_t>(lastVoidPtr - firstVoidPtr);
		std::memcpy(dstVoidPtr, firstVoidPtr, count);

		if constexpr (std::is_pointer_v<DstIter>) {
			return reinterpret_cast<DstIter>(DstIter + count);
		}
		else {
			return dst + (lastPtr - firstPtr);
		}
	}

	template <class SrcIter, class DstIter, class SizeType>
		requires(IsIterPtr<SrcIter>&& IsIterPtr<DstIter>&& std::equality_comparable_with<const SizeType, int>)
	DstIter iterMemcopyCount(SrcIter first, DstIter dst, SizeType count) noexcept {
		auto memcopyRst = iterMemcopy(first, first + count, dst);
		if constexpr (std::is_pointer_v<DstIter>) {
			return memcopyRst;
		} else { 
			return dst + count;
		}
	}

	template <class Iter, class ValueType>
	concept IsIterPtrZeroMemsetAble = IsIterPtr<Iter> && std::is_trivially_constructible_v<typename IterPtrTraits<Iter>::value_type, const ValueType&>;

	template <class Iter, class SizeType>
	concept CanGetSizeFormIterPtrSub = 
		IsIterPtr<Iter> 
		&& std::is_integral_v<SizeType> 
		&& requires(Iter first, Iter last) {
		{last - first} -> std::convertible_to<SizeType>;
	};

	template <class Iter, class SizeType>
		requires(IsIterPtr<Iter> && CanGetSizeFormIterPtrSub<Iter, SizeType>)
	void countFormIterators(const Iter& first, const Iter& last) {
		return static_cast<SizeType>(first - last);
	}

	template <class DstIter>
		requires(IsIterPtr<DstIter>)
	void iterPtrMemset(DstIter dst, const int value, const size_t count) {
		typename IterPtrTraits<DstIter>::value_type* const dstPtr = iteratorToAddress<DstIter>(dst);
		char* const dstVoidPtr = const_cast<char*>(reinterpret_cast<const volatile char*>(dstPtr));
		std::memset(dstPtr, static_cast<unsigned char>(value), count);
	}

	template <class Iter>
		requires(IsIterPtr<Iter>)
	constexpr decltype(auto) unwrappedIterator(Iter&& iter) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<Iter>>) {
			return iter + 0;
		}
		else {
			return iter;
		}
	}

	template<class Iter, typename value_type = typename IterPtrTraits<Iter>::value_type>
		requires(IsIterPtr<Iter>)
	constexpr value_type* getIterValuePtr(Iter iter) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<Iter>>) {
			return iter;
		}
		else {
			return &*iter;
		}
	}
}
