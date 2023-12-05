#pragma once

//std
#include <cstring>
#include <iterator>
#include <type_traits>

//own
#include "typeTraits.h"
#include "basicTypes.h"

//interface 
namespace natl {
	template<class T>
	class RandomAccessIterator {
	public:
		using iterator = RandomAccessIterator<T>;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using iterator_category = std::random_access_iterator_tag;
		using Sizeype = Size;
	private:
		T* dataPtr;
	public:
		constexpr RandomAccessIterator() : dataPtr(nullptr) {}
		constexpr RandomAccessIterator(T* const dataPtr) noexcept : dataPtr(dataPtr) {}
		constexpr ~RandomAccessIterator() = default;
	private:
		constexpr iterator& getSelf() noexcept { return *this; }
		constexpr const iterator& getSelf() const noexcept { return *this; }
	public:
		constexpr reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() const noexcept { return dataPtr; }
		constexpr reference operator[](const Sizeype pos) const noexcept { return dataPtr[pos]; };

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

		constexpr iterator& operator+=(const difference_type offset) noexcept { dataPtr += offset; return getSelf(); }
		constexpr iterator operator+(const difference_type offset) const noexcept { return iterator(dataPtr + offset); }
		constexpr friend iterator operator+(const difference_type offset, const iterator& rhs) noexcept { return iterator(rhs.dataPtr + offset); }
		constexpr iterator& operator-=(const difference_type offset) noexcept { dataPtr -= offset; return getSelf(); }
		constexpr iterator operator-(const difference_type offset) const noexcept { return iterator(dataPtr -= offset); }
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

		const Size count = static_cast<Size>(lastVoidPtr - firstVoidPtr);
		std::memcpy(dstVoidPtr, firstVoidPtr, static_cast<std::size_t>(count));

		if constexpr (std::is_pointer_v<DstIter>) {

			return reinterpret_cast<DstIter>(dstPtr + count);
		}else {
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
	SizeType countFormIterators(const Iter& first, const Iter& last) {
		return static_cast<SizeType>(first - last);
	}

	template <class DstIter>
		requires(IsIterPtr<DstIter>)
	void iterPtrMemset(DstIter dst, const int value, const Size count) {
		typename IterPtrTraits<DstIter>::value_type* const dstPtr = iteratorToAddress<DstIter>(dst);
		void* const dstVoidPtr = reinterpret_cast<void* const>(dstPtr);
		std::memset(dstVoidPtr, value, static_cast<std::size_t>(count) * sizeof(typename IterPtrTraits<DstIter>::value_type));
	}

	template <class SrcIter, class DstIter>
	concept MemcpyCompareIter = IsIterPtr<SrcIter> && IsIterPtr<DstIter> &&
		MemcpyCompareableSrcDst<typename IterPtrTraits<SrcIter>::value_type, typename IterPtrTraits<DstIter>::value_type>;


	template <class AIter, class BIter>
		requires(MemcpyCompareIter<AIter, BIter>)
	bool iterPtrMemcpy(AIter a, BIter b, const Size count) {
		const typename IterPtrTraits<AIter>::value_type* const aPtr = iteratorToAddress<AIter>(a);
		const typename IterPtrTraits<BIter>::value_type* const bPtr = iteratorToAddress<BIter>(b);
		const void* const aVoidPtr = reinterpret_cast<const void* const>(aPtr);
		const void* const bVoidPtr = reinterpret_cast<const void* const>(bPtr);
		return std::memcmp(aVoidPtr, bVoidPtr, static_cast<Size>(count) * sizeof(IterPtrTraits<AIter>::value_type));
	}


	template <class Iter>
		requires(IsIterPtr<Iter>)
	constexpr decltype(auto) unwrappedIterator(Iter&& iter) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<Iter>>) {
			return iter + 0;
		} else {
			return iter;
		}
	}

	template<class Iter, typename value_type = typename IterPtrTraits<Iter>::value_type>
		requires(IsIterPtr<Iter>)
	constexpr value_type* getIterValuePtr(Iter iter) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<Iter>>) {
			return iter;
		} else {
			return &*iter;
		}
	}

	template <typename Iter>
	concept IsRandomAccessIterator = IsIterPtr<Iter> || std::is_same_v<IteratorCategory<Iter>, std::random_access_iterator_tag>;

	template <typename Iter>
	typename IteratorCategory<Iter>::difference_type
		iterDistance(Iter first, Iter last) {
		typename std::iterator_traits<Iter>::difference_type distance = 0;
		while (first != last) {
			++distance;
			++first;
		}
		return distance;
	}

	template <typename Iter>
		requires(IsRandomAccessIterator<Iter>)
	Size iterDistance(Iter first, Iter last) {
		if constexpr (IsIterPtr<Iter>) {
			return std::bit_cast<Size, decltype(first)>(last - first);
		} else {
			return std::bit_cast<Size, decltype(&*first)>(&*last - &*first);
		}
	}



}
