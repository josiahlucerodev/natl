#pragma once

//std
#include <iterator>
#include <cstring>

//own
#include "typeTraits.h"
#include "basicTypes.h"
#include "allocator.h"
#include "compare.h"

//interface 
namespace natl {
#ifdef NATL_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4180)
#endif // NATL_COMPILER_MSVC

	template<typename DataType>
	struct DefaultIteratorTraits {
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using difference_type = PtrDiff;
		using size_type = Size;
	};

#ifdef NATL_COMPILER_MSVC
#pragma warning(pop)
#endif // NATL_COMPILER_MSVC

	using RandomAccessIteratorTag = std::random_access_iterator_tag;
	using BidirectionalIteratorTag = std::bidirectional_iterator_tag;

	template<typename Iter>
	struct IteratorTraits {
		using value_type = Iter::value_type;
		using reference = Iter::reference;
		using difference_type = Iter::difference_type;
		using pointer = Iter::pointer;
		using iterator_category = Iter::iterator_category;
	};

	template<class Type> struct IteratorTraits<Type*> {
		using value_type = Type;
		using reference = Type&;
		using difference_type = PtrDiff;
		using pointer = Type*;
		using iterator_category = RandomAccessIteratorTag;
	};

	template<class Type> struct IteratorTraits<const Type*> {
		using value_type = const Type;
		using reference = const Type&;
		using difference_type = PtrDiff;
		using pointer = const Type*;
		using iterator_category = RandomAccessIteratorTag;
	};

	template<typename Type>
	concept HasIteratorType = requires {
		typename Type::iterator;
	};

	template <typename Iter>
	using IteratorCategory = typename IteratorTraits<Iter>::iterator_category;

	template<typename Iter>
	concept IsIterator = requires() { 
		typename Iter::iterator_category;
	};

	template <typename Iter>
	inline constexpr Bool IsIteratorV = BoolConstant<IsIterator<Iter>>;

	template <typename Iter>
	concept IsIterPtr = IsPointer<Iter> || IsIterator<Iter>;

	template <typename SrcIter, typename DstIter>
	concept MemcopyConstructibleIter =
		IsIterPtr<SrcIter> &&
		IsIterPtr<DstIter> &&
		IsSame<typename SrcIter::iterator_catagory, RandomAccessIteratorTag> &&
		IsSame<typename DstIter::iterator_catagory, RandomAccessIteratorTag> &&
		MemcopyConstructibleSrcDst<
		typename IteratorTraits<SrcIter>::value_type,
		typename IteratorTraits<DstIter>::value_type,
		typename IteratorTraits<SrcIter>::reference>;


	template <typename SrcIter, typename DstIter>
	concept MemcopyAssignableIter =
		IsIterPtr<SrcIter> &&
		IsIterPtr<DstIter> &&
		MemcopyAssignableSrcDst<
		typename IteratorTraits<SrcIter>::value_type,
		typename IteratorTraits<DstIter>::value_type,
		typename IteratorTraits<SrcIter>::reference,
		typename IteratorTraits<DstIter>::reference>;

	template <typename Iter, typename value_type = typename IteratorTraits<Iter>::value_type>
	constexpr value_type* iteratorToAddress(Iter iter) noexcept {
		if constexpr (IsPointerC<Decay<Iter>>) {
			return iter;
		} else {
			return &*iter;
		}
	}

	template <typename SrcIter, typename DstIter>
		requires(IsIterPtr<SrcIter>&& IsIterPtr<DstIter>)
	DstIter iterMemcopy(SrcIter first, SrcIter last, DstIter dst) noexcept {
		typename IteratorTraits<SrcIter>::value_type* firstPtr = iteratorToAddress<SrcIter>(first);
		typename IteratorTraits<SrcIter>::value_type* lastPtr = iteratorToAddress<SrcIter>(last);
		typename IteratorTraits<DstIter>::value_type* dstPtr = iteratorToAddress<DstIter>(dst);

		const char* const firstVoidPtr = const_cast<const char*>(reinterpret_cast<const volatile char*>(firstPtr));
		const char* const lastVoidPtr = const_cast<const char*>(reinterpret_cast<const volatile char*>(lastPtr));
		char* const dstVoidPtr = const_cast<char*>(reinterpret_cast<const volatile char*>(dstPtr));

		const Size count = static_cast<Size>(lastVoidPtr - firstVoidPtr);
		std::memcpy(dstVoidPtr, firstVoidPtr, static_cast<StdSize>(count));

		if constexpr (IsPointer<DstIter>) {
			return reinterpret_cast<DstIter>(dstPtr + count);
		} else {
			return dst + static_cast<Size>(lastPtr - firstPtr);
		}
	}

	template <typename SrcIter, typename DstIter, typename SizeType>
		requires(IsIterPtr<SrcIter>&& IsIterPtr<DstIter>&& std::equality_comparable_with<const SizeType, int>)
	DstIter iterMemcopyCount(SrcIter first, DstIter dst, SizeType count) noexcept {
		auto memcopyRst = iterMemcopy(first, first + count, dst);
		if constexpr (IsPointer<DstIter>) {
			return memcopyRst;
		} else {
			return dst + count;
		}
	}

	template <typename Iter, typename ValueType>
	concept IsIterPtrZeroMemsetAble =
		IsIterPtr<Iter> &&
		(std::is_trivially_constructible_v<typename IteratorTraits<Iter>::value_type, const ValueType&> ||
			(IsSameC<typename IteratorTraits<Iter>::value_type, ValueType> && IsTriviallyConstRefConstructible<typename IteratorTraits<Iter>::value_type>));

	template <typename Iter, typename SizeType>
	concept CanGetSizeFormIterPtrSub =
		IsIterPtr<Iter>
		&& std::is_integral_v<SizeType>
		&& requires(Iter first, Iter last) {
			{ last - first } -> ConvertibleTo<SizeType>;
	};

	template <typename Iter, typename SizeType>
		requires(IsIterPtr<Iter>&& CanGetSizeFormIterPtrSub<Iter, SizeType>)
	SizeType countFormIterators(const Iter& first, const Iter& last) {
		return static_cast<SizeType>(last - first);
	}

	template <typename DstIter>
		requires(IsIterPtr<DstIter>)
	void iterPtrMemset(DstIter dst, const int value, const Size count) {
		typename IteratorTraits<DstIter>::value_type* const dstPtr = iteratorToAddress<DstIter>(dst);
		void* const dstVoidPtr = reinterpret_cast<void* const>(dstPtr);
		std::memset(dstVoidPtr, value, static_cast<StdSize>(static_cast<Size>(count) * sizeof(typename IteratorTraits<DstIter>::value_type)));
	}

	template <typename SrcIter, typename DstIter>
	concept MemcpyCompareIter = IsIterPtr<SrcIter> && IsIterPtr<DstIter> &&
		MemcpyCompareableSrcDst<typename IteratorTraits<SrcIter>::value_type, typename IteratorTraits<DstIter>::value_type>;


	template <typename AIter, typename BIter>
		requires(MemcpyCompareIter<AIter, BIter>)
	Bool iterPtrMemcpy(AIter a, BIter b, const Size count) {
		const typename IteratorTraits<AIter>::value_type* const aPtr = iteratorToAddress<AIter>(a);
		const typename IteratorTraits<BIter>::value_type* const bPtr = iteratorToAddress<BIter>(b);
		const void* const aVoidPtr = reinterpret_cast<const void* const>(aPtr);
		const void* const bVoidPtr = reinterpret_cast<const void* const>(bPtr);
		return std::memcmp(aVoidPtr, bVoidPtr, static_cast<Size>(count) * sizeof(IteratorTraits<AIter>::value_type));
	}


	template <typename Iter>
		requires(IsIterPtr<Iter>)
	constexpr decltype(auto) unwrappedIterator(Iter&& iter) noexcept {
		if constexpr (IsPointer<Decay<Iter>>) {
			return iter + 0;
		} else {
			return iter;
		}
	}

	template<typename Iter, typename value_type = typename IteratorTraits<Iter>::value_type>
		requires(IsIterPtr<Iter>)
	constexpr value_type* getIterValuePtr(Iter iter) noexcept {
		if constexpr (IsPointer<Decay<Iter>>) {
			return iter;
		} else {
			return &*iter;
		}
	}

	template <typename Iter>
	concept IsRandomAccessIterator = IsIterPtr<Iter> || IsSame<IteratorCategory<Iter>, RandomAccessIteratorTag>;

	template <typename Iter>
	constexpr typename IteratorCategory<Iter>::difference_type
		iterDistance(Iter first, Iter last) noexcept {
		typename IteratorTraits<Iter>::difference_type distance = 0;
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
	constexpr IteratorTraits<Iter>::reference iterValue(Iter iter) noexcept {
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
			requires(IsIterator<OtherIter> && AssignableFromC<Iter&, const OtherIter&>)
		constexpr ReverseIterator& operator=(const ReverseIterator<OtherIter>& other) noexcept {
			iterStorage = other;
		}

		//access 
		constexpr iterator_type base() const noexcept { return iterStorage; }
		constexpr reference operator*() const { Iter temp = iterStorage; return *--temp; }
		constexpr pointer operator->() const requires (IsPointer<Iter> || requires (const Iter iter) { iter.operator->(); }) {
			Iter temp = base(); --temp;
			if constexpr (IsPointer<Iter>) {
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
		using difference_type = PtrDiff;
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
		constexpr iterator& self() noexcept { return *this; }
		constexpr const iterator& self() const noexcept { return *this; }
	public:
		constexpr reference operator*() noexcept requires(IsNotConst<DataType>) { return *dataPtr; }
		constexpr const_reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() noexcept requires(IsNotConst<DataType>) { return dataPtr; }
		constexpr const_pointer operator->() const noexcept { return dataPtr; }
		constexpr reference operator[](const size_type pos) noexcept requires(IsNotConst<DataType>) { return dataPtr[pos]; };
		constexpr const_reference operator[](const size_type pos) const noexcept { return dataPtr[pos]; };

		constexpr Bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr Bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr Bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
		constexpr Bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
		constexpr Bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
		constexpr Bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }

		constexpr iterator& operator++() noexcept { dataPtr++; return self(); }
		constexpr iterator operator++(int) noexcept { iterator tempIt = self(); ++self().dataPtr; return tempIt; }
		constexpr iterator& operator--() noexcept { dataPtr--; return self(); }
		constexpr iterator operator--(int) noexcept { iterator tempIt = self(); --self().dataPtr; return tempIt; }

		constexpr iterator& operator+=(const difference_type offset) noexcept { dataPtr += offset; return self(); }
		constexpr iterator operator+(const difference_type offset) const noexcept { return iterator(dataPtr + offset); }
		constexpr friend iterator operator+(const difference_type offset, const iterator& rhs) noexcept { return iterator(rhs.dataPtr + offset); }
		constexpr iterator& operator-=(const difference_type offset) noexcept { dataPtr -= offset; return self(); }
		constexpr iterator operator-(const difference_type offset) const noexcept { return iterator(dataPtr - offset); }
		constexpr difference_type operator-(const iterator rhs) const noexcept { return self().dataPtr - rhs.dataPtr; }
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

		using iterator_category = RandomAccessIteratorTag;
	private:
		pointer dataPtr;
	public:
		constexpr RandomAccessIteratorAlloc() : dataPtr(nullptr) {}
		constexpr RandomAccessIteratorAlloc(pointer ptr) noexcept requires(IsNotConst<value_type>) : dataPtr(ptr) {}
		constexpr RandomAccessIteratorAlloc(const_pointer ptr) noexcept requires(IsConst<value_type>) : dataPtr(ptr) {}
		constexpr ~RandomAccessIteratorAlloc() = default;
	private:
		constexpr iterator& self() noexcept { return *this; }
		constexpr const iterator& self() const noexcept { return *this; }
	public:
		constexpr reference operator*() noexcept requires(IsNotConst<value_type>) { return *dataPtr; }
		constexpr const_reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() noexcept requires(IsNotConst<value_type>) { return dataPtr; }
		constexpr const_pointer operator->() const noexcept { return dataPtr; }
		constexpr reference operator[](const size_type pos) noexcept requires(IsNotConst<value_type>) { return dataPtr[pos]; };
		constexpr const_reference operator[](const size_type pos) const noexcept { return dataPtr[pos]; };

		constexpr Bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr Bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr Bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
		constexpr Bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
		constexpr Bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
		constexpr Bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }

		constexpr iterator& operator++() noexcept { dataPtr++; return self(); }
		constexpr iterator operator++(int) noexcept { iterator tempIt = self(); ++self().dataPtr; return tempIt; }
		constexpr iterator& operator--() noexcept { dataPtr--; return self(); }
		constexpr iterator operator--(int) noexcept { iterator tempIt = self(); --self().dataPtr; return tempIt; }

		constexpr iterator& operator+=(const difference_type offset) noexcept { dataPtr += offset; return self(); }
		constexpr iterator operator+(const difference_type offset) const noexcept { return iterator(dataPtr + offset); }
		constexpr friend iterator operator+(const difference_type offset, const iterator& rhs) noexcept { return iterator(rhs.dataPtr + offset); }
		constexpr iterator& operator-=(const difference_type offset) noexcept { dataPtr -= offset; return self(); }
		constexpr iterator operator-(const difference_type offset) const noexcept { return iterator(dataPtr - offset); }
		constexpr difference_type operator-(const iterator rhs) const noexcept { return self().dataPtr - rhs.dataPtr; }
	};

	template<typename DataType, typename Alloc>
	using ConstRandomAccessIteratorAlloc = RandomAccessIteratorAlloc<const DataType, typename Alloc::template rebind_alloc<const DataType>>;

	template<typename DataType, typename Alloc>
	using ReverseRandomAccessIteratorAlloc = ReverseIterator<RandomAccessIteratorAlloc<DataType, Alloc>>;

	template<typename DataType, typename Alloc>
	using ReverseConstRandomAccessIteratorAlloc = ReverseIterator<ConstRandomAccessIteratorAlloc<DataType, typename Alloc::template rebind_alloc<const DataType>>>;


	template<typename OutputIter>
	constexpr void callReserveOnOutputIterator(const OutputIter& outputIter, const Size newCapacity) noexcept {
		if constexpr (requires(OutputIter testOutputIter) { { testOutputIter.reserve(Size()) }; }) {
			outputIter.reserve(newCapacity);
		}
	}
}
