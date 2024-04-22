#pragma once

//std
#include <iterator>

//own
#include "typeTraits.h"
#include "basicTypes.h"
#include "allocator.h"
#include "compare.h"

//interface 
namespace natl {
	template<typename Type>
	concept HasIteratorType = requires {
		typename Type::iterator;
	};

	template <typename Iter>
	using IteratorCategory = typename std::iterator_traits<Iter>::iterator_category;

	template <typename Iter, typename = void>
	inline constexpr Bool IsIteratorV = false;

	template <typename Iter>
	inline constexpr Bool IsIteratorV<Iter, std::void_t<IteratorCategory<Iter>>> = true;

	template<typename Iter>
	concept IsIterator = IsIteratorV<Iter>;

	template <typename Iter>
	concept IsIterPtr = IsPointerV<Iter> || IsIterator<Iter>;

	template <typename Iter>
		requires (IsIterPtr<Iter>)
	struct IterPtrTraits {
		using value_type = std::conditional_t<std::is_pointer_v<std::decay_t<Iter>>, std::remove_pointer_t<std::decay_t<Iter>>, typename std::iterator_traits<Iter>::value_type>;
		using reference = std::conditional_t<std::is_pointer_v<std::decay_t<Iter>>, std::remove_pointer_t<std::decay_t<Iter>>&, typename std::iterator_traits<Iter>::reference>;
		using difference_type = std::conditional_t<std::is_pointer_v<std::decay_t<Iter>>, PtrDiff, typename std::iterator_traits<Iter>::difference_type>;
	};

	template <typename SrcIter, typename DstIter>
	concept MemcopyConstructibleIter =
		IsIterPtr<SrcIter> &&
		IsIterPtr<DstIter> &&
		IsSameV<typename SrcIter::iterator_catagory, std::random_access_iterator_tag> &&
		IsSameV<typename DstIter::iterator_catagory, std::random_access_iterator_tag> &&
		MemcopyConstructibleSrcDst<
		typename IterPtrTraits<SrcIter>::value_type,
		typename IterPtrTraits<DstIter>::value_type,
		typename IterPtrTraits<SrcIter>::reference>;


	template <typename SrcIter, typename DstIter>
	concept MemcopyAssignableIter =
		IsIterPtr<SrcIter> &&
		IsIterPtr<DstIter> &&
		MemcopyAssignableSrcDst<
		typename IterPtrTraits<SrcIter>::value_type,
		typename IterPtrTraits<DstIter>::value_type,
		typename IterPtrTraits<SrcIter>::reference,
		typename IterPtrTraits<DstIter>::reference>;

	template <typename Iter, typename value_type = typename IterPtrTraits<Iter>::value_type>
	constexpr value_type* iteratorToAddress(Iter iter) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<Iter>>) {
			return iter;
		} else {
			return &*iter;
		}
	}

	template <typename SrcIter, typename DstIter>
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
		} else {
			return dst + static_cast<Size>(lastPtr - firstPtr);
		}
	}

	template <typename SrcIter, typename DstIter, typename SizeType>
		requires(IsIterPtr<SrcIter>&& IsIterPtr<DstIter>&& std::equality_comparable_with<const SizeType, int>)
	DstIter iterMemcopyCount(SrcIter first, DstIter dst, SizeType count) noexcept {
		auto memcopyRst = iterMemcopy(first, first + count, dst);
		if constexpr (std::is_pointer_v<DstIter>) {
			return memcopyRst;
		} else {
			return dst + count;
		}
	}

	template <typename Iter, typename ValueType>
	concept IsIterPtrZeroMemsetAble =
		IsIterPtr<Iter> &&
		(std::is_trivially_constructible_v<typename IterPtrTraits<Iter>::value_type, const ValueType&> ||
			(std::is_same_v<typename IterPtrTraits<Iter>::value_type, ValueType> && IsTriviallyConstRefConstructible<typename IterPtrTraits<Iter>::value_type>));

	template <typename Iter, typename SizeType>
	concept CanGetSizeFormIterPtrSub =
		IsIterPtr<Iter>
		&& std::is_integral_v<SizeType>
		&& requires(Iter first, Iter last) {
			{ last - first } -> std::convertible_to<SizeType>;
	};

	template <typename Iter, typename SizeType>
		requires(IsIterPtr<Iter>&& CanGetSizeFormIterPtrSub<Iter, SizeType>)
	SizeType countFormIterators(const Iter& first, const Iter& last) {
		return static_cast<SizeType>(last - first);
	}

	template <typename DstIter>
		requires(IsIterPtr<DstIter>)
	void iterPtrMemset(DstIter dst, const int value, const Size count) {
		typename IterPtrTraits<DstIter>::value_type* const dstPtr = iteratorToAddress<DstIter>(dst);
		void* const dstVoidPtr = reinterpret_cast<void* const>(dstPtr);
		std::memset(dstVoidPtr, value, static_cast<std::size_t>(count) * sizeof(typename IterPtrTraits<DstIter>::value_type));
	}

	template <typename SrcIter, typename DstIter>
	concept MemcpyCompareIter = IsIterPtr<SrcIter> && IsIterPtr<DstIter> &&
		MemcpyCompareableSrcDst<typename IterPtrTraits<SrcIter>::value_type, typename IterPtrTraits<DstIter>::value_type>;


	template <typename AIter, typename BIter>
		requires(MemcpyCompareIter<AIter, BIter>)
	Bool iterPtrMemcpy(AIter a, BIter b, const Size count) {
		const typename IterPtrTraits<AIter>::value_type* const aPtr = iteratorToAddress<AIter>(a);
		const typename IterPtrTraits<BIter>::value_type* const bPtr = iteratorToAddress<BIter>(b);
		const void* const aVoidPtr = reinterpret_cast<const void* const>(aPtr);
		const void* const bVoidPtr = reinterpret_cast<const void* const>(bPtr);
		return std::memcmp(aVoidPtr, bVoidPtr, static_cast<Size>(count) * sizeof(IterPtrTraits<AIter>::value_type));
	}


	template <typename Iter>
		requires(IsIterPtr<Iter>)
	constexpr decltype(auto) unwrappedIterator(Iter&& iter) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<Iter>>) {
			return iter + 0;
		} else {
			return iter;
		}
	}

	template<typename Iter, typename value_type = typename IterPtrTraits<Iter>::value_type>
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
	constexpr typename IteratorCategory<Iter>::difference_type
		iterDistance(Iter first, Iter last) noexcept {
		typename std::iterator_traits<Iter>::difference_type distance = 0;
		while (first != last) {
			++distance;
			++first;
		}
		return distance;
	}

	template <typename Iter>
		requires(IsRandomAccessIterator<Iter>)
	constexpr Size iterDistance(Iter first, Iter last) noexcept {
		if constexpr (IsIterPtr<Iter>) {
			return static_cast<Size>(last - first);
		} else {
			return static_cast<Size>(&*last - &*first);
		}
	}

	template<typename Iter>
		requires(IsIterator<Iter>)
	constexpr IterPtrTraits<Iter>::reference iterValue(Iter iter) noexcept {
		return *iter;
	}

	template<typename Iter> 
		requires(IsIterator<Iter>)
	class ReverseIterator {
	public:
		using iterator_type = Iter;
		using iterator_category = Iter::iterator_category;
		using value_type = Iter::value_type;
		using difference_type = Iter::difference_type;
		using pointer = Iter::pointer;
		using reference = Iter::reference;

		using const_reference = const reference&;
		using const_pointer = const pointer;
	protected:
		iterator_type iterStorage;
	public:
		//constructor 
		constexpr ReverseIterator() noexcept = default;
		constexpr explicit ReverseIterator(const iterator_type& iter) noexcept : iterStorage(iter) {}
		template<class OtherIter>
			requires(IsIterator<Iter> && ConvertibleTo<const OtherIter&, Iter>)
		constexpr explicit ReverseIterator(const ReverseIterator<OtherIter>& other) : 
			iterStorage(static_cast<iterator_type>(other)) {}

		//destructor
		constexpr ~ReverseIterator() noexcept = default;

		//util 
		constexpr ReverseIterator& self() noexcept { return *this; }
		constexpr const ReverseIterator& self() const noexcept { return *this; }

		//assignment 
		template<class OtherIter>
			requires(IsIterator<OtherIter> && AssignableFrom<Iter&, const OtherIter&>)
		constexpr ReverseIterator& operator=(const ReverseIterator<OtherIter>& other) noexcept {
			iterStorage = other;
		}

		//access 
		constexpr iterator_type base() const noexcept { return iterStorage; }
		constexpr reference operator*() const { Iter temp = iterStorage; return *--temp; }
		constexpr pointer operator->() const requires (IsPointerV<Iter> || requires (const Iter iter) { iter.operator->(); }) {
			Iter temp = base(); --temp;
			if constexpr (IsPointerV<Iter>) {
				return temp;
			} else {
				return temp.operator->();
			}
		}
		constexpr auto operator[](const difference_type index) const noexcept { return base()[-index - 1]; }

		//iteration op
		constexpr ReverseIterator& operator++() noexcept {
			--iterStorage;
			return self();
		}
		constexpr ReverseIterator operator++(int) noexcept {
			ReverseIterator temp = self();
			--iterStorage;
			return self();
		}
		constexpr ReverseIterator& operator--() noexcept {
			++iterStorage;
			return self();
		}
		constexpr ReverseIterator operator--(int) noexcept {
			ReverseIterator temp = self();
			++iterStorage;
			return temp;
		}

		constexpr ReverseIterator operator+(const difference_type offset) const noexcept {
			return ReverseIterator(base() - offset);
		}
		constexpr ReverseIterator& operator+=(const difference_type offset) const noexcept {
			iterStorage -= offset;
			return self();
		}
		constexpr ReverseIterator operator-(const difference_type offset) const noexcept {
			return ReverseIterator(base() + offset);
		}
		constexpr ReverseIterator& operator-=(const difference_type offset) const noexcept {
			iterStorage += offset;
			return self();
		}
	};

	template<typename LhsIter, typename RhsIter>
		requires(IsIterator<LhsIter> && IsIterator<RhsIter> && IsOneWayEqualTestable<LhsIter, RhsIter>)
	constexpr Bool operator==(const ReverseIterator<LhsIter>& lhs, const ReverseIterator<RhsIter> rhs) noexcept {
		return lhs.base() == rhs.base();
	}
	template<typename LhsIter, typename RhsIter>
		requires(IsIterator<LhsIter> && IsIterator<RhsIter> && IsOneWayNotEqualTestable<LhsIter, RhsIter>)
	constexpr Bool operator!=(const ReverseIterator<LhsIter>& lhs, const ReverseIterator<RhsIter> rhs) noexcept {
		return lhs.base() != rhs.base();
	}
	template<typename LhsIter, typename RhsIter>
		requires(IsIterator<LhsIter> && IsIterator<RhsIter> && IsOneWayLessThanTestable<LhsIter, RhsIter>)
	constexpr Bool operator<(const ReverseIterator<LhsIter>& lhs, const ReverseIterator<RhsIter> rhs) noexcept {
		return lhs.base() > rhs.base();
	}
	template<typename LhsIter, typename RhsIter>
		requires(IsIterator<LhsIter> && IsIterator<RhsIter> && IsOneWayLessThanEqualTestable<LhsIter, RhsIter>)
	constexpr Bool operator<=(const ReverseIterator<LhsIter>& lhs, const ReverseIterator<RhsIter> rhs) noexcept {
		return lhs.base() >= rhs.base();
	}
	template<typename LhsIter, typename RhsIter>
		requires(IsIterator<LhsIter> && IsIterator<RhsIter> && IsOneWayGreaterThanTestable<LhsIter, RhsIter>)
	constexpr Bool operator>(const ReverseIterator<LhsIter>& lhs, const ReverseIterator<RhsIter> rhs) noexcept {
		return lhs.base() < rhs.base();
	}
	template<typename LhsIter, typename RhsIter>
		requires(IsIterator<LhsIter> && IsIterator<RhsIter> && IsOneWayGreaterThanEqualTestable<LhsIter, RhsIter>)
	constexpr Bool operator>=(const ReverseIterator<LhsIter>& lhs, const ReverseIterator<RhsIter> rhs) noexcept {
		return lhs.base() <= rhs.base();
	}

	template<typename LhsIter, typename RhsIter>
		requires(IsIterator<LhsIter>&& IsIterator<RhsIter> && IsOneWayThreeWayTestable<LhsIter, RhsIter>)
	constexpr CompareThreeWayResult<LhsIter, RhsIter> operator<=>(const ReverseIterator<LhsIter>& lhs, const ReverseIterator<RhsIter> rhs) noexcept {
		return rhs.base() <=> lhs.base();
	}


	template<typename DataType>
	class RandomAccessIterator {
	public:
		using iterator = RandomAccessIterator<DataType>;
		using difference_type = std::ptrdiff_t;
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using iterator_category = std::random_access_iterator_tag;
		using size_type = Size;
	private:
		DataType* dataPtr;
	public:
		constexpr RandomAccessIterator() : dataPtr(nullptr) {}
		constexpr RandomAccessIterator(DataType* const ptr) noexcept : dataPtr(ptr) {}
		constexpr ~RandomAccessIterator() = default;
	private:
		constexpr iterator& getSelf() noexcept { return *this; }
		constexpr const iterator& getSelf() const noexcept { return *this; }
	public:
		constexpr reference operator*() noexcept requires(IsNotConstV<DataType>) { return *dataPtr; }
		constexpr const_reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() noexcept requires(IsNotConstV<DataType>) { return dataPtr; }
		constexpr const_pointer operator->() const noexcept { return dataPtr; }
		constexpr reference operator[](const size_type pos) noexcept requires(IsNotConstV<DataType>) { return dataPtr[pos]; };
		constexpr const_reference operator[](const size_type pos) const noexcept { return dataPtr[pos]; };

		constexpr Bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr Bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr Bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
		constexpr Bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
		constexpr Bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
		constexpr Bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }

		constexpr iterator& operator++() noexcept { dataPtr++; return getSelf(); }
		constexpr iterator operator++(int) noexcept { iterator tempIt = getSelf(); ++getSelf().dataPtr; return tempIt; }
		constexpr iterator& operator--() noexcept { dataPtr--; return getSelf(); }
		constexpr iterator operator--(int) noexcept { iterator tempIt = getSelf(); --getSelf().dataPtr; return tempIt; }

		constexpr iterator& operator+=(const difference_type offset) noexcept { dataPtr += offset; return getSelf(); }
		constexpr iterator operator+(const difference_type offset) const noexcept { return iterator(dataPtr + offset); }
		constexpr friend iterator operator+(const difference_type offset, const iterator& rhs) noexcept { return iterator(rhs.dataPtr + offset); }
		constexpr iterator& operator-=(const difference_type offset) noexcept { dataPtr -= offset; return getSelf(); }
		constexpr iterator operator-(const difference_type offset) const noexcept { return iterator(dataPtr - offset); }
		constexpr difference_type operator-(const iterator rhs) const noexcept { return getSelf().dataPtr - rhs.dataPtr; }
	};

	template<typename DataType>
	using ConstRandomAccessIterator = ReverseIterator<RandomAccessIterator<const DataType>>;

	template<typename DataType>
	using ReverseRandomAccessIterator = ReverseIterator<RandomAccessIterator<DataType>>;

	template<typename DataType>
	using ReverseConstRandomAccessIterator = ReverseIterator<ConstRandomAccessIterator<DataType>>;

	template<typename DataType, typename Alloc>
	class RandomAccessIteratorAlloc {
	public:
		using iterator = RandomAccessIteratorAlloc<DataType, Alloc>;
		using allocator_type = Alloc;

		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;

		using iterator_category = std::random_access_iterator_tag;
	private:
		pointer dataPtr;
	public:
		constexpr RandomAccessIteratorAlloc() : dataPtr(nullptr) {}
		constexpr RandomAccessIteratorAlloc(pointer ptr) noexcept requires(IsNotConstV<value_type>) : dataPtr(ptr) {}
		constexpr RandomAccessIteratorAlloc(const_pointer ptr) noexcept requires(IsConstV<value_type>) : dataPtr(ptr) {}
		constexpr ~RandomAccessIteratorAlloc() = default;
	private:
		constexpr iterator& getSelf() noexcept { return *this; }
		constexpr const iterator& getSelf() const noexcept { return *this; }
	public:
		constexpr reference operator*() noexcept requires(IsNotConstV<value_type>) { return *dataPtr; }
		constexpr const_reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() noexcept requires(IsNotConstV<value_type>) { return dataPtr; }
		constexpr const_pointer operator->() const noexcept { return dataPtr; }
		constexpr reference operator[](const size_type pos) noexcept requires(IsNotConstV<value_type>) { return dataPtr[pos]; };
		constexpr const_reference operator[](const size_type pos) const noexcept { return dataPtr[pos]; };

		constexpr Bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr Bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr Bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
		constexpr Bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
		constexpr Bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
		constexpr Bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }

		constexpr iterator& operator++() noexcept { dataPtr++; return getSelf(); }
		constexpr iterator operator++(int) noexcept { iterator tempIt = getSelf(); ++getSelf().dataPtr; return tempIt; }
		constexpr iterator& operator--() noexcept { dataPtr--; return getSelf(); }
		constexpr iterator operator--(int) noexcept { iterator tempIt = getSelf(); --getSelf().dataPtr; return tempIt; }

		constexpr iterator& operator+=(const difference_type offset) noexcept { dataPtr += offset; return getSelf(); }
		constexpr iterator operator+(const difference_type offset) const noexcept { return iterator(dataPtr + offset); }
		constexpr friend iterator operator+(const difference_type offset, const iterator& rhs) noexcept { return iterator(rhs.dataPtr + offset); }
		constexpr iterator& operator-=(const difference_type offset) noexcept { dataPtr -= offset; return getSelf(); }
		constexpr iterator operator-(const difference_type offset) const noexcept { return iterator(dataPtr - offset); }
		constexpr difference_type operator-(const iterator rhs) const noexcept { return getSelf().dataPtr - rhs.dataPtr; }
	};

	template<typename DataType, typename Alloc>
	using ConstRandomAccessIteratorAlloc = RandomAccessIteratorAlloc<const DataType, typename Alloc::template rebind_alloc<const DataType>>;

	template<typename DataType, typename Alloc>
	using ReverseRandomAccessIteratorAlloc = ReverseIterator<RandomAccessIteratorAlloc<DataType, Alloc>>;

	template<typename DataType, typename Alloc>
	using ReverseConstRandomAccessIteratorAlloc = ReverseIterator<ConstRandomAccessIteratorAlloc<DataType, typename Alloc::template rebind_alloc<const DataType>>>;


	template<typename OutputIter>
	constexpr void callReserveOnOutputIterator(const OutputIter& outputIter, const Size newCapacity) noexcept {
		if constexpr (requires(OutputIter outputIter) { {outputIter.reserve(Size()) }; }) {
			outputIter.reserve(newCapacity);
		}
	}
}
