#pragma once 

//std
#include <string>
#include <string_view>

//own
#include "../util/bits.h"
#include "../util/basicTypes.h"
#include "../util/allocator.h"
#include "../util/iterators.h"
#include "../util/dataMovement.h"
#include "../util/algorithm.h"
#include "container.h"
#include "stringView.h"
#include "arrayView.h"
#include "limits.h"

//interface 
namespace natl {
	template<typename CharType>
	struct BaseStringBaseMembersRef {
	public:
		Size stringSizeAndSmallStringFlag;
		CharType* stringPtr;
		Size stringCapacity;
	};

	template<typename CharType,
		Size bufferSize, 
		typename Alloc = DefaultAllocator<CharType>,
		Bool EnableDynAllocation = true, 
		Bool EnableIncreasedSmallBufferSize = true>
		requires(IsAllocator<Alloc>)
	struct BaseString {
	public:
		using allocator_type = Alloc;

		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;

		using optional_pointer = Option<pointer>;
		using optional_const_pointer = Option<const_pointer>;

		using iterator = RandomAccessIteratorAlloc<value_type, Alloc>;
		using const_iterator = ConstRandomAccessIteratorAlloc<value_type, Alloc>;
		using reverse_iterator = ReverseRandomAccessIteratorAlloc<value_type, Alloc>;
		using const_reverse_iterator = ReverseConstRandomAccessIteratorAlloc<value_type, Alloc>;

		using allocation_move_adapater = AllocationMoveAdapater<value_type, Alloc>;

		constexpr static Bool enableSmallString = true;
	private:
		constexpr static Bool enableIncreasedSmallBufferSize = 
			sizeof(BaseStringBaseMembersRef<CharType>) + bufferSize <= 64
			&& TypeByteSize<CharType> == 1 &&
			EnableIncreasedSmallBufferSize;
		constexpr static Size increasedSmallBufferForCharSize = sizeof(size_type) - sizeof(ui8);
		constexpr static Size standardSmallBufferSize = sizeof(size_type) - sizeof(ui8);
		constexpr static Size increasedSizeToSmallBuffer = (enableIncreasedSmallBufferSize) ? increasedSmallBufferForCharSize : 0;
	public:
		constexpr static Size smallBufferSize = ((sizeof(CharType*) + sizeof(size_type) + bufferSize) / sizeof(CharType)) + increasedSizeToSmallBuffer;

		static const size_type npos = size_type(-1);

		using ConstBaseStringView = BaseStringView<const value_type>;


		/*
		in increased small buffer state
			if is samllString()
				stringSizeAndSmallStringFlag format is 
				1 bit small string flag | 7 bits size storage | (sizeof(size_type) * 8) - 8 bits additional small buffer storage 
			else  
				stringSizeAndSmallStringFlag format is
				1 bit small string flag | (sizeof(size_type) * 8) - 1 bits is size storage 
				
		not in increased small buffer state 
			stringSizeAndSmallStringFlag format is
			1 bit small string flag | (sizeof(size_type) * 8) - 1 bits is size storage 
		*/
	private:
		size_type stringSizeAndSmallStringFlag; 
		pointer stringPtr;
		size_type stringCapacity;
		value_type smallStringStorage[bufferSize];

		//small string 
	private:
		constexpr Bool isConstexprSmallString() const noexcept {
			if (!isConstantEvaluated()) {
				if constexpr (enableIncreasedSmallBufferSize) {
					constexpr size_type increasedBufferMask = ~static_cast<size_type>(0xff);
					if (isSmallString()
						&& (stringSizeAndSmallStringFlag & increasedBufferMask) != 0
						&& *(reinterpret_cast<const char*>(reinterpret_cast<const ui8*>(this) + sizeof(CharType))) == '\0') [[unlikely]] {
						return true;
					}
				} else {
					if (stringSizeAndSmallStringFlag != 0 
						&& *(reinterpret_cast<const char*>(reinterpret_cast<const ui8*>(this) + sizeof(stringSizeAndSmallStringFlag))) == '\0') [[unlikely]] {
						return true;
					}
				}
			} 
			return false;
		}
		constexpr void setAsSmallString() noexcept { 
			if constexpr (enableSmallString) {
				stringSizeAndSmallStringFlag = setNthBitToZero<size_type>(stringSizeAndSmallStringFlag, TypeBitSize<size_type> - 1);
			}
		}
		constexpr void setAsNotSmallString() noexcept { 
			if constexpr(enableSmallString) {
				stringSizeAndSmallStringFlag = setNthBitToOne<size_type>(stringSizeAndSmallStringFlag, TypeBitSize<size_type> - 1);
			}
		}
		constexpr static size_type getMask() noexcept {
			return setNthBitToZero<size_type>(~size_type(0), TypeBitSize<size_type> - 1);
		}
		constexpr void setSize(const size_type newSize) noexcept {
			if constexpr (enableIncreasedSmallBufferSize) {
				constexpr size_type increasedBufferMask = ~static_cast<size_type>(0xff);
				constexpr size_type standardMaskInverted = ~getMask();
				const size_type mask = isSmallString() ? increasedBufferMask : standardMaskInverted;
				stringSizeAndSmallStringFlag = (stringSizeAndSmallStringFlag & mask) | newSize;
			} else {
				stringSizeAndSmallStringFlag = (stringSizeAndSmallStringFlag & ~getMask()) | newSize;
			}
		}
	public:
		constexpr Bool isSmallString() const noexcept { 
			if constexpr (!enableSmallString) {
				return false;
			} else {
				return !(stringSizeAndSmallStringFlag & ~getMask());
			}
		}
		constexpr Bool isNotSmallString() const noexcept { 
			return !isSmallString();
		}
		constexpr size_type smallStringCapacity() const noexcept {
			if (isConstantEvaluated() || !enableSmallString) {
				return bufferSize;
			} else {
				if constexpr (enableIncreasedSmallBufferSize) {
					if (isConstexprSmallString()) [[unlikely]] {
						return bufferSize;
					} else {
						return bufferSize + sizeof(stringPtr) + sizeof(stringCapacity) + (sizeof(Size) - sizeof(ui8));
					}
				} else {
					if (isConstexprSmallString()) [[unlikely]] {
						return bufferSize;
					} else {
						return bufferSize + ((sizeof(stringPtr) + sizeof(stringCapacity)) / sizeof(value_type));
					}
				}
			}
		}
		constexpr pointer smallStringLocation() noexcept {
			if (isConstantEvaluated()) {
				return smallStringStorage;
			} else {
				if constexpr (enableIncreasedSmallBufferSize) {
					if (isConstexprSmallString()) [[unlikely]] {
						return smallStringStorage;
					} else {
						return reinterpret_cast<pointer>(reinterpret_cast<ui8*>(this) + sizeof(CharType));
					}
				} else {
					if (isConstexprSmallString()) [[unlikely]] {
						return smallStringStorage;
					} else {
						return reinterpret_cast<pointer>(reinterpret_cast<ui8*>(this) + sizeof(stringSizeAndSmallStringFlag));
					}
				}
			}
		}
		constexpr const_pointer smallStringLocation() const noexcept {
			if (isConstantEvaluated()) {
				return smallStringStorage;
			} else {
				if constexpr (enableIncreasedSmallBufferSize) {
					if (isConstexprSmallString()) [[unlikely]] {
						return smallStringStorage;
					} else {
						return reinterpret_cast<const_pointer>(reinterpret_cast<const ui8*>(this) + sizeof(CharType));
					}
				} else {
					if (isConstexprSmallString()) [[unlikely]] {
						return smallStringStorage;
					} else {
						return reinterpret_cast<const_pointer>(reinterpret_cast<const ui8*>(this) + sizeof(stringSizeAndSmallStringFlag));
					}
				}
			}
		}

		constexpr size_type capacity() const noexcept { return isSmallString() ? smallStringCapacity() : stringCapacity; };

		constexpr size_type size() const noexcept {
			if constexpr (enableIncreasedSmallBufferSize) {
				constexpr size_type increasedBufferMask = static_cast<size_type>(0xff);
				constexpr size_type standardMask = getMask();
				const size_type mask = isSmallString() ? increasedBufferMask : standardMask;
				return stringSizeAndSmallStringFlag & mask;
			} else {
				return stringSizeAndSmallStringFlag & getMask(); 
			}
		}
		constexpr size_type length() const noexcept { return size(); };
		constexpr size_type max_size() const noexcept { return getMask(); };

		constexpr const_pointer data() const noexcept {
			return isSmallString() ? smallStringLocation() : stringPtr;
		}
		constexpr pointer data() noexcept {
			return isSmallString() ? smallStringLocation() : stringPtr;
		}
	private:
		constexpr void baseConstructorInit() noexcept {
			stringSizeAndSmallStringFlag = 0;
			stringPtr = 0;
			stringCapacity = 0;
			if (isConstantEvaluated()) {
				uninitializedFill<pointer, value_type>(smallStringStorage, smallStringStorage + bufferSize, value_type(0));
			}
		}
		constexpr BaseString& self() noexcept { return *this; }
		constexpr const BaseString& self() const noexcept { return *this; }
	public:
		//constructors 
		constexpr BaseString() noexcept : stringSizeAndSmallStringFlag(0), stringPtr(0), stringCapacity(0), smallStringStorage{} {}

		constexpr BaseString(const BaseString& other) noexcept {
			baseConstructorInit();
			construct(other.data(), other.size());
		}
		constexpr BaseString(BaseString&& other) noexcept {
			baseConstructorInit();
			construct(forward<allocation_move_adapater>(other.getAlloctionMoveAdapater()));
		}

		template<Size OtherBufferSize, Bool OtherEnableDynAllocation, Bool OtherEnableIncreasedSmallBufferSize>
		constexpr BaseString(const BaseString<CharType, OtherBufferSize, Alloc, OtherEnableDynAllocation, OtherEnableIncreasedSmallBufferSize>& other) noexcept {
			baseConstructorInit();
			construct(other.data(), other.size());
		}
		template<Size OtherBufferSize, Bool OtherEnableDynAllocation, Bool OtherEnableIncreasedSmallBufferSize>
		constexpr BaseString(BaseString<CharType, OtherBufferSize, Alloc, OtherEnableDynAllocation, OtherEnableIncreasedSmallBufferSize>&& other) noexcept {
			baseConstructorInit();
			construct(forward<allocation_move_adapater>(other.getAlloctionMoveAdapater()));
		}
		constexpr BaseString(const_pointer str) noexcept {
			baseConstructorInit();
			construct(str, cstringLength(str));
		}
		constexpr BaseString(const_pointer str, const size_type count) noexcept {
			baseConstructorInit();
			construct(str, count);
		}

		constexpr BaseString(const size_type count, const value_type character) noexcept {
			baseConstructorInit();
			construct(count, character);
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString(const StringViewLike& str) noexcept {
			baseConstructorInit();
			construct(str.data(), str.size());
		}
		constexpr BaseString(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			baseConstructorInit();
			construct(natl::move(allocationMoveAdapater));
		}

		constexpr BaseString(const Ascii* str, const size_type count) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			baseConstructorInit();
			construct(str, count);
		}

		constexpr BaseString(const Ascii* str) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			baseConstructorInit();
			construct(str);
		}

		constexpr BaseString(const size_type count, const Ascii character) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			baseConstructorInit();
			construct(character, count);
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, Ascii>)
		constexpr BaseString(const StringViewLike& str) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			baseConstructorInit();
			construct<StringViewLike>(str);
		}

		//destructor
		constexpr ~BaseString() {
			if constexpr (EnableDynAllocation) {
				if (isNotSmallString() && stringPtr) {
					Alloc::deallocate(stringPtr, capacity());
				}
			}
		}

		//construct
	private:
		constexpr BaseString& construct(const_pointer otherPtr, const size_type& count) noexcept {
			if (count == 0) {
				stringSizeAndSmallStringFlag = 0;
				stringCapacity = 0;
				stringPtr = nullptr;
			} else if (count + 1 <= smallStringCapacity()) {
				const_pointer srcDataPtrFirst = otherPtr;
				const_pointer srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, smallStringLocation());

				setAsSmallString();

				const size_type newSize = count;
				setSize(newSize);
				addNullTerminater();
			} else {
				const size_type newSize = count;
				factorReserve(newSize);

				const_pointer srcDataPtrFirst = otherPtr;
				const_pointer srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, data());

				setSize(newSize);
				addNullTerminater();
			}

			return self();
		}
		constexpr BaseString& construct(const size_type count, const value_type& value = value_type()) noexcept {
			if (count == 0) {
				stringSizeAndSmallStringFlag = 0;
				stringCapacity = 0;
				stringPtr = nullptr;
				return self();
			}

			factorReserve(count);

			pointer fillDstPtr = data();
			pointer fillDstPtrLast = fillDstPtr + count;
			uninitializedFill<pointer, value_type>(fillDstPtr, fillDstPtrLast, value);

			setSize(count);
			addNullTerminater();
			return self();
		}

		template<typename Iter>
			requires(IsIterPtr<Iter>&& IsSameC<typename IteratorTraits<Iter>::value_type, value_type>)
		constexpr BaseString& construct(Iter first, Iter last) noexcept {
			if constexpr (std::contiguous_iterator<Iter>) {
				const size_type count = iterDistance<Iter>(first, last);
				const_pointer firstPtr = iteratorToAddress<Iter>(first);
				return construct(firstPtr, count);
			}

			factorReserve(10);
			for (; first != last; first++) {
				pushBack(*first);
			}
			return self();
		}		
		constexpr BaseString& construct(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			if (allocationMoveAdapater.isEmpty()) {
				stringSizeAndSmallStringFlag = 0;
			} else if (allocationMoveAdapater.requiresCopy() || allocationMoveAdapater.size() <= smallStringCapacity()) {
				construct(allocationMoveAdapater.data(), allocationMoveAdapater.size());

				if (allocationMoveAdapater.canDealloc()) {
					allocationMoveAdapater.deallocate();
				}
			} else {
				setAsNotSmallString();
				setSize(allocationMoveAdapater.size());
				stringCapacity = allocationMoveAdapater.capacity();
				stringPtr = allocationMoveAdapater.data();
			}
			allocationMoveAdapater.release();
			return self();
		}

		constexpr BaseString& construct(const Ascii* str, const size_type count) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			const size_type stringSize = count;
			if (stringSize == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const size_type srcStringSize = stringSize;
			const Ascii* srcStringPtrFirst = str;
			const Ascii* srcStringPtrLast = srcStringPtrFirst + srcStringSize;

			pointer stringDst = nullptr;
			if (srcStringSize + 1 <= smallStringCapacity()) {
				stringDst = smallStringLocation();
			}
			else {
				factorReserve(srcStringSize);
				stringDst = stringPtr;
			}

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, stringDst++) {
				*stringDst = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& construct(const Ascii* str) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			if (str == nullptr) {
				stringSizeAndSmallStringFlag = 0;
				return self();
			}
			return assign(str, cstringLength(str));
		}

		constexpr BaseString& construct(const Ascii character, const size_type count = 1) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			if (count == 0) {
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			pointer stringDst = nullptr;
			if (count + 1 <= smallStringCapacity()) {
				stringDst = smallStringLocation();
			} else {
				factorReserve(count);
				stringDst = stringPtr;
			}

			for (size_type i = 0; i < count; i++, stringDst++) {
				*stringDst = static_cast<Utf32>(character);
			}

			setSize(count);
			addNullTerminater();
			return self();
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, Ascii>)
		constexpr BaseString& construct(const StringViewLike& stringView) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			if (stringView.size() == 0) {
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const size_type srcStringSize = stringView.size();
			const Ascii* srcStringPtrFirst = stringView.data();
			const Ascii* srcStringPtrLast = srcStringPtrLast + srcStringSize;

			pointer stringDst = nullptr;
			if (srcStringSize + 1 <= smallStringCapacity()) {
				stringDst = smallStringLocation();
			} else {
				factorReserve(srcStringSize);
				stringDst = stringPtr;
			}

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, stringDst++) {
				*stringDst = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}
	public:

		//assignment 
		constexpr BaseString& operator=(const BaseString& other) noexcept {
			return assign(other.data(), other.size());
		}
		constexpr BaseString& operator=(BaseString&& other) noexcept {
			return assign(forward<allocation_move_adapater>(other.getAlloctionMoveAdapater()));
		}

		template<Size OtherBufferSize, Bool OtherEnableDynAllocation, Bool OtherEnableIncreasedSmallBufferSize>
		constexpr BaseString& operator=(const BaseString<CharType, OtherBufferSize, Alloc, OtherEnableDynAllocation, OtherEnableIncreasedSmallBufferSize>& other) noexcept {
			return assign(other.data(), other.size());
		}
		template<Size OtherBufferSize, Bool OtherEnableDynAllocation, Bool OtherEnableIncreasedSmallBufferSize>
		constexpr BaseString& operator=(BaseString<CharType, OtherBufferSize, Alloc, OtherEnableDynAllocation, OtherEnableIncreasedSmallBufferSize>&& other) noexcept {
			return assign(forward<allocation_move_adapater>(other.getAlloctionMoveAdapater()));
		}
		constexpr BaseString& operator=(const_pointer str) noexcept {
			return assign(str);
		}
		constexpr BaseString& operator=(const value_type character) noexcept {
			return assign(character);
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& operator=(const StringViewLike& stringView) noexcept {
			return assign<StringViewLike>(stringView);
		}
		constexpr BaseString& operator=(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			return assign(natl::move(allocationMoveAdapater));
		}

		constexpr BaseString& operator=(const Ascii* str) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return assign(str);
		}
		constexpr BaseString& operator=(const Ascii character) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return assign(character);
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, Ascii>)
		constexpr BaseString& operator=(const StringViewLike& stringView) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return assign<StringViewLike>(stringView);
		}

		constexpr BaseString& assign(const_pointer str, const size_type count) noexcept {
			if (str == nullptr) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const size_type srcStringSize = count;
			const_pointer srcStringPtrFirst = str;
			const_pointer srcStringPtrLast = srcStringPtrFirst + srcStringSize;

			if (srcStringSize + 1 <= smallStringCapacity()) {
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, smallStringLocation());
			} else {
				factorReserve(srcStringSize);
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, stringPtr);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& assign(const_pointer str) noexcept {
			if (str == nullptr) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			return assign(str, cstringLength(str));
		}

		constexpr BaseString& assign(const size_type count, value_type character) noexcept {
			if (count == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			if (count + 1 <= smallStringCapacity()) {
				pointer srcStringPtrFirst = smallStringLocation();
				pointer srcStringPtrLast = srcStringPtrFirst + count;
				uninitializedFill<pointer, value_type>(srcStringPtrFirst, srcStringPtrLast, forward<value_type>(character));
			} else {
				factorReserve(count);
				pointer srcStringPtrFirst = stringPtr;
				pointer srcStringPtrLast = srcStringPtrFirst + count;
				uninitializedFill<pointer, value_type>(srcStringPtrFirst, srcStringPtrLast, forward<value_type>(character));
			}
			
			setSize(count);
			addNullTerminater();
			return self();
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& assign(const StringViewLike& stringView) noexcept {
			if (stringView.size() == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const size_type srcStringSize = stringView.size();
			const_pointer srcStringPtrFirst = stringView.data();
			const_pointer srcStringPtrLast = srcStringPtrFirst + srcStringSize;

			if (srcStringSize + 1 <= smallStringCapacity()) {
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, smallStringLocation());
			} else {
				factorReserve(srcStringSize);
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, stringPtr);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}
		constexpr BaseString& assign(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			if (allocationMoveAdapater.isEmpty()) {
				release();
				stringSizeAndSmallStringFlag = 0;
				stringCapacity = 0;
				stringPtr = nullptr;
			} else if (allocationMoveAdapater.requiresCopy() || allocationMoveAdapater.size() <= smallStringCapacity()) {
				assign(allocationMoveAdapater.data(), allocationMoveAdapater.size());
				if (allocationMoveAdapater.canDealloc()) {
					allocationMoveAdapater.deallocate();
				}
			} else {
				if (isNotSmallString()) {
					release();
				}
				setAsNotSmallString();
				setSize(allocationMoveAdapater.size());
				stringCapacity = allocationMoveAdapater.capacity();
				stringPtr = allocationMoveAdapater.data();
			}
			allocationMoveAdapater.release();
			return self();
		}

		constexpr BaseString& assign(const Ascii* str, const size_type count) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			const size_type stringSize = count;
			if (stringSize == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const size_type srcStringSize = stringSize;
			const Ascii* srcStringPtrFirst = str;
			const Ascii* srcStringPtrLast = srcStringPtrFirst + srcStringSize;

			pointer stringDst = nullptr;
			if (srcStringSize + 1 <= smallStringCapacity()) {
				stringDst = smallStringLocation();
			}
			else {
				factorReserve(srcStringSize);
				stringDst = stringPtr;
			}

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, stringDst++) {
				*stringDst = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& assign(const Ascii* str) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			if (str == nullptr) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}
			return assign(str, cstringLength(str));
		}

		constexpr BaseString& assign(const Ascii character, const size_type count = 1) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			if (count == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			pointer stringDst = nullptr;
			if (count + 1 <= smallStringCapacity()) {
				stringDst = smallStringLocation();
			} else {
				factorReserve(count);
				stringDst = stringPtr;
			}

			for (size_type i = 0; i < count; i++, stringDst++) {
				*stringDst = static_cast<Utf32>(character);
			}

			setSize(count);
			addNullTerminater();
			return self();
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, Ascii>)
		constexpr BaseString& assign(const StringViewLike& stringView) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			if (stringView.size() == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const size_type srcStringSize = stringView.size();
			const Ascii* srcStringPtrFirst = stringView.data();
			const Ascii* srcStringPtrLast = srcStringPtrLast + srcStringSize;

			pointer stringDst = nullptr;
			if (srcStringSize + 1 <= smallStringCapacity()) {
				stringDst = smallStringLocation();
			} else {
				factorReserve(srcStringSize);
				stringDst = stringPtr;
			}

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, stringDst++) {
				*stringDst = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}

		[[nodiscard]] constexpr allocation_move_adapater getAlloctionMoveAdapater() noexcept {
			const Bool stringIsSmallBuffer = isSmallString();
			AllocationMoveAdapaterRequireCopy requireCopy = stringIsSmallBuffer ? AllocationMoveAdapaterRequireCopy::True : AllocationMoveAdapaterRequireCopy::False;
			AllocationMoveAdapaterCanDealloc canBeDealloc = stringIsSmallBuffer ? AllocationMoveAdapaterCanDealloc::False : AllocationMoveAdapaterCanDealloc::True;
			allocation_move_adapater allocationMoveAdapater(data(), size(), capacity(), requireCopy, canBeDealloc);
			if (stringIsSmallBuffer) {
				if constexpr (enableIncreasedSmallBufferSize) {
					constexpr size_type increasedBufferMask = static_cast<size_type>(0xff);
					stringSizeAndSmallStringFlag ^= (stringSizeAndSmallStringFlag & increasedBufferMask);
				} else {
					stringSizeAndSmallStringFlag = 0;
				}
			} else {
				stringPtr = nullptr;
				stringSizeAndSmallStringFlag = 0;
				stringCapacity = 0;
			}
			return natl::move(allocationMoveAdapater);
		}

		//element access
		constexpr reference at(const size_type index) noexcept { return data()[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return data()[index]; };

		constexpr reference operator[] (const size_type index) { return at(index); }
		constexpr const_reference operator[] (const size_type index) const { return at(index); }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() - 1; }

		constexpr reference front() noexcept { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr const_pointer c_str() const noexcept { return data(); }

		constexpr operator BaseStringView<value_type>() noexcept {
			return BaseStringView<value_type>(data(), size());
		}
		constexpr operator ConstBaseStringView() const noexcept {
			return ConstBaseStringView(data(), size());
		}

		constexpr BaseStringView<value_type> toStringView() noexcept {
			return BaseStringView<value_type>(data(), size());
		}
		constexpr ConstBaseStringView toStringView() const noexcept {
			return ConstBaseStringView(data(), size());
		}

		constexpr ArrayView<value_type> toArrayView() noexcept {
			return ArrayView<value_type>(data(), size());
		}
		constexpr ArrayView<const value_type> toArrayView() const noexcept {
			return ArrayView<const value_type>(data(), size());
		}

		//iterators 
		constexpr pointer beginPtr() noexcept { return data(); }
		constexpr const_pointer beginPtr() const noexcept { return data(); }
		constexpr pointer endPtr() noexcept { return data() + size(); }
		constexpr const_pointer endPtr() const noexcept { return data() + size(); }

		constexpr iterator begin() noexcept { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
		constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }


		//capactity 
	private:
		constexpr void release() noexcept {
			if (!isSmallString() && stringPtr) {
				Alloc::deallocate(stringPtr, capacity());
				stringPtr = nullptr;
			}
		}
	public:
		constexpr Bool empty() const noexcept { return size() == 0; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }

	private:
		constexpr Bool reserveTest(size_type newCapacity) noexcept {
			newCapacity += 1;
			if (newCapacity <= capacity()) {
				//Error if by default small string 
				return true;
			} else if (newCapacity <= smallStringCapacity()) {
				setAsSmallString();
				return true;
			}
			return false;
		}
	public:
		constexpr void factorReserve(const size_type newCapacity) noexcept {
			if (reserveTest(newCapacity)) { return; }
			reserveExact(newCapacity * 2);
		}

		constexpr void reserve(const size_type newCapacity) noexcept {
			if (reserveTest(newCapacity)) { return; }
			reserveExact(newCapacity);
		}

		constexpr void reserveExact(size_type newCapacity) noexcept {
			if constexpr (EnableDynAllocation) {
				if (reserveTest(newCapacity)) { return; }
				newCapacity += 1;

				pointer newStringPtr = Alloc::allocate(newCapacity);

				const_pointer srcStringPtrFirst = data();
				const_pointer srcStringPtrLast = srcStringPtrFirst + size();
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, newStringPtr);

				if (isNotSmallString() && stringPtr) {
					Alloc::deallocate(stringPtr, capacity());
				} 

				stringPtr = newStringPtr;
				stringCapacity = newCapacity;

				if constexpr (enableIncreasedSmallBufferSize) {
					/*
					for increased small buffer size, setAsNotSmallString() no longer maintains size 
					in stringSizeAndSmallStringFlag as there is a difference in format to increase size 
				 
					in increased small buffer state
						if is samllString()
							stringSizeAndSmallStringFlag format is 
							1 bit small string flag | 7 bits size storage | (sizeof(size_type) * 8) - 8 bits additional small buffer storage 
						else  
							stringSizeAndSmallStringFlag format is
							1 bit small string flag | (sizeof(size_type) * 8) - 1 bits is size storage 
				
					not in increased small buffer state 
						stringSizeAndSmallStringFlag format is
							1 bit small string flag | (sizeof(size_type) * 8) - 1 bits is size storage 
					*/

					const size_type oldSize = size();
					setAsNotSmallString();
					setSize(oldSize);
				} else {
					setAsNotSmallString();
				}
			} else {
				setAsSmallString();
			}
		}

		constexpr void shrink_to_fit() {
			const size_type newCapacity = size() + 1;
			const Bool sameSize = newCapacity == capacity();
			const Bool canBeSmallString = newCapacity <= smallStringCapacity();
			if (isSmallString()) {
				return;
			} else if (isNotSmallString() && canBeSmallString) {
				if (!stringPtr) {
					stringSizeAndSmallStringFlag = 0;
					return;
				}

				//small string can copy data into the memory location of the stringPtr and stringCapacity member variable
				//need to make copy because they will be written to by the subsuquent uninitializedCopyNoOverlap
				//volatile is used so the compiler does not optimize the temperary variables away if possable 
				pointer volatile oldStringPtr = stringPtr;
				size_type volatile oldCapacity = capacity();

				pointer dstStringPtr = smallStringLocation();
				const_pointer srcStringPtrFirst = stringPtr;
				const_pointer srcStringPtrLast = stringPtr + newCapacity;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

				Alloc::deallocate(oldStringPtr, oldCapacity);

				setAsSmallString();
				return;
			} else if (sameSize) {
				return;
			}

			if (newCapacity <= smallStringCapacity() || newCapacity == capacity()) {
				return;
			}

			if (size() == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
			} else {
				pointer newStringPtr = Alloc::allocate(newCapacity);
				const_pointer srcStringPtrFirst = stringPtr;
				const_pointer srcStringPtrLast = stringPtr + newCapacity;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, newStringPtr);
				Alloc::deallocate(stringPtr, capacity());

				stringPtr = newStringPtr;
				stringCapacity = newCapacity;
			}
		}

		//modifiers
	private:
		constexpr void addNullTerminater() noexcept { setEnd(0); }
	public:
		constexpr void clear() noexcept { setSize(0); }

		constexpr BaseString& insert(const size_type index, const ConstBaseStringView& sv) noexcept {
			if (sv.empty()) { return self(); }
			const size_type newSize = size() + sv.size();
			factorReserve(newSize);

			const size_type relocateCount = size() - index;
			pointer relocateDstPtr = data() + index + sv.size();
			const_pointer relocateSrcPtrFirst = data() + index;
			const_pointer relocateSrcPtrLast = relocateSrcPtrFirst + relocateCount;
			uninitializedCopy<const_pointer, pointer>(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			pointer insertStrDstPtr = data() + index;
			const_pointer insertStrSrcPtrFirst = sv.data();
			const_pointer insertStrSrcPtrLast = insertStrSrcPtrFirst + sv.size();
			uninitializedCopy<const_pointer, pointer>(insertStrSrcPtrFirst, insertStrSrcPtrLast, insertStrDstPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& insert(const size_type index, const size_type count, const value_type character) noexcept {
			const size_type newSize = size() + count;
			factorReserve(newSize);

			const size_type relocateCount = size() - index;
			pointer relocateDstPtr = data() + index + count;
			const_pointer relocateSrcPtrFirst = data() + index;
			const_pointer relocateSrcPtrLast = relocateSrcPtrFirst + relocateCount;
			uninitializedCopy<const_pointer, pointer>(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			const_pointer insertDstPtrFirst = data() + index;
			const_pointer insertDstPtrLast = insertDstPtrFirst + count;
			uninitializedFill<pointer, value_type>(insertDstPtrFirst, insertDstPtrLast, character);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& insert(const size_type index, const_pointer str) noexcept {
			return insert(index, ConstBaseStringView(str));
		}
		constexpr BaseString& insert(const size_type index, const_pointer str, const size_type count) noexcept {
			return insert(index, ConstBaseStringView(str, min<size_type>(cstringLength(str), count)));
		}
		constexpr BaseString& insert(const size_type index, const BaseString& str) noexcept {
			return insert(index, str.toStringView());
		}
		constexpr BaseString& insert(size_type index, const BaseString& str, const size_type s_index, const size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			return insert(index, str.substrView(s_index, count));
		}

		constexpr iterator insert(const_iterator pos, const value_type character) noexcept {
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			insert(index, character, 1);
			return iterator(data() + index);
		}
		constexpr iterator insert(const_iterator pos, size_type count, const value_type character) noexcept {
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			insert(index, character, count);
			return iterator(data() + index);
		}
		
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type> && !IsSameC<StringViewLike, ConstBaseStringView>)
		constexpr BaseString& insert(const size_type index, const StringViewLike& stringView) noexcept {
			return insert(index, ConstBaseStringView(stringView.data(), stringView.size()));
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& insert(const size_type index, const StringViewLike& stringView, size_type t_index, size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			return insert(index, ConstBaseStringView(stringView.data() + t_index, min<size_type>(stringView.size(), count)));
		}

		constexpr BaseString& erase(const size_type index, const size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			const size_type newCount = min<size_type>(count, size() - index);
			pointer dstStringPtr = data() + index;
			const_pointer srcStringPtrFirst = dstStringPtr + newCount;
			const_pointer srcStringPtrLast = endPtr();
			uninitializedCopy<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			setSize(size() - newCount);
			addNullTerminater();
			return self();
		};
		constexpr BaseString& erase(const_iterator position) noexcept {
			pointer dstStringPtr = data() + (&*position-data());
			const_pointer srcStringPtrFirst = dstStringPtr + 1;
			const_pointer srcStringPtrLast = endPtr();
			uninitializedCopy<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			setSize(size() - 1);
			addNullTerminater();
			return self();
		}
		constexpr iterator erase(const_iterator first, const_iterator last) noexcept {
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			pointer dstStringPtr = data() + (&*first-data());
			const_pointer srcStringPtrFirst = dstStringPtr + count;
			const_pointer srcStringPtrLast = endPtr();
			uninitializedCopy<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			setSize(size() - count);
			addNullTerminater();
			return iterator(dstStringPtr);
		}

		constexpr void pushBack(const value_type character) noexcept {
			const size_type newSize = size() + 1;
			factorReserve(newSize);
			setEnd(character);
			setSize(newSize);
			addNullTerminater();
		}
		constexpr void pop_back() noexcept {
			setSize(size() - 1);
			addNullTerminater();
		}

		constexpr BaseString& append(size_type count, const value_type character) noexcept {
			const size_type newSize = size() + count;
			factorReserve(newSize);

			pointer srcStringPtrFirst = endPtr();
			pointer srcStringPtrLast = srcStringPtrFirst + count;
			uninitializedFill<pointer, value_type>(srcStringPtrFirst, srcStringPtrLast, character);

			setSize(newSize);
			addNullTerminater();
			return self();
		}
		constexpr BaseString& append(const BaseString& str) noexcept {
			const size_type newSize = size() + str.size();
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			const_pointer srcStringPtrFirst = str.beginPtr();
			const_pointer srcStringPtrLast = str.endPtr();
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}
		constexpr BaseString& append(const_pointer str, const size_type count) noexcept {
			return append(BaseStringView<const value_type>(str, count));
		}
		constexpr BaseString& append(const BaseString& str, const size_type pos, const size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			const size_type newCount = min<size_type>(str.size() - pos, count);
			const size_type newSize = size() + newCount;
			resize(newSize);

			pointer dstStringPtr = endPtr();
			const_pointer srcStringPtrFirst = str.beginPtr() + pos;
			const_pointer srcStringPtrLast = str + newCount;
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const_pointer str) noexcept {
			const size_type count = cstringLength(str);
			const size_type newSize = size() + count;
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			const_pointer srcStringPtrFirst = str;
			const_pointer srcStringPtrLast = str + count;
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const value_type character) noexcept {
			pushBack(character);
			return self();
		}

		template<typename InputIt>
		constexpr BaseString& append(InputIt first, InputIt last) noexcept {
			const size_type newSize = size() + iterDistance<InputIt>(first, last);
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			uninitializedCopyNoOverlap<InputIt, pointer>(first, last, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& append(const StringViewLike& stringView) noexcept {
			const size_type count = stringView.size();
			const size_type newSize = size() + count;
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			const_pointer srcStringPtrFirst = stringView.data();
			const_pointer srcStringPtrLast = srcStringPtrFirst + count;
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& append(const StringViewLike& stringView, size_type pos, size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			const size_type newCount = min<size_type>(stringView.size() - pos, count);
			const size_type newSize = size() + newCount;
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			const_pointer srcStringPtrFirst = stringView.data() + pos;
			const_pointer srcStringPtrLast = srcStringPtrFirst + newCount;
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const Ascii* str) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			const size_type count = cstringLength(str);
			const size_type newSize = size() + count;
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			const Ascii* srcStringPtrFirst = str;
			const Ascii* srcStringPtrLast = srcStringPtrFirst + count;

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, dstStringPtr++) {
				*dstStringPtr = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(newSize);
			addNullTerminater();
			return self();
		}
		constexpr BaseString& append(const Ascii character) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			pushBack(character);
			return self();
		}

		constexpr BaseString& append(const size_type count, const Ascii character) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			const size_type newSize = size() + count;
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			const_pointer dstStringPtrLast = dstStringPtr + count;
			for (; dstStringPtr < dstStringPtrLast; dstStringPtr++) {
				*dstStringPtr = static_cast<Utf32>(character);
			}

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const Ascii* str, const size_type count) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return append(BaseStringView<const Ascii>(str, count));
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, Ascii>)
		constexpr BaseString& append(const StringViewLike& stringView) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			const size_type count = stringView.size();
			const size_type newSize = size() + count;
			factorReserve(newSize);

			pointer dstStringPtr = endPtr();
			const Ascii* srcStringPtrFirst = stringView.data();
			const Ascii* srcStringPtrLast = srcStringPtrLast + count;

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, dstStringPtr++) {
				*dstStringPtr = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& operator+=(const BaseString& rhs) noexcept {
			return append(rhs);
		}
		constexpr BaseString& operator+=(const_pointer rhs) noexcept {
			return append(rhs);
		}
		constexpr BaseString& operator+=(const value_type rhs) noexcept {
			return append(rhs);
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		constexpr BaseString& operator+=(const StringLike& rhs) noexcept {
			return append(rhs);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, value_type>)
		constexpr BaseString& operator+=(const StringLike& rhs) noexcept {
			return append(rhs);
		}
		constexpr BaseString& operator+=(const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return append(rhs);
		}
		constexpr BaseString& operator+=(const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return append(rhs);
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		constexpr BaseString& operator+=(const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return append(rhs);
		}

		template<typename SrcCharType>
		constexpr BaseString& replace(const size_type pos, const size_type count, const BaseStringView<const SrcCharType>& sv, const size_type pos2, const size_type count2 = npos) {
			const size_type index = min<size_type>(size(), pos);
			const size_type srcIndex = min<size_type>(sv.size(), pos2);
			const size_type srcCount = min<size_type>(sv.size() - srcIndex, count2);
			const size_type replaceSize = min<size_type>(size() - index, count);
			const size_type endingSize = size() - index - replaceSize;

			if (endingSize > 0) {
				const size_type newSize = index + srcCount + endingSize;
				factorReserve(newSize);

				pointer dstShiftStringPtr = data() + index + srcCount;
				const_pointer srcShiftStringPtrFirst = data() + index + replaceSize;
				const_pointer srcShiftStringPtrLast = endPtr();
				uninitializedCopy<const_pointer, pointer>(srcShiftStringPtrFirst, srcShiftStringPtrLast, dstShiftStringPtr);

				setSize(newSize);
				addNullTerminater();
			} 

			if constexpr (IsSameC<Decay<value_type>, Utf32> && IsSameC<SrcCharType, Ascii>) {
				pointer dstStringPtr = data() + index;
				const Ascii* srcStringPtrFirst = sv.data() + srcIndex;
				const Ascii* srcStringPtrLast = srcStringPtrFirst + srcCount;
				uninitializedCopyConvert<const Ascii*, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			} else {
				pointer dstStringPtr = data() + index;
				const_pointer srcStringPtrFirst = sv.data() + srcIndex;
				const_pointer srcStringPtrLast = srcStringPtrFirst + srcCount;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			}

			return self();
		}

		template<typename SrcCharType>
		constexpr BaseString& replace(const size_type pos, const size_type count, const size_type count2, SrcCharType ch) {
			const size_type index = min<size_type>(size(), pos);
			const size_type srcCount = count2;
			const size_type replaceSize = min<size_type>(size() - index, count);
			const size_type endingSize = size() - index - replaceSize;

			if (endingSize > 0) {
				const size_type newSize = index + srcCount + endingSize;
				factorReserve(newSize);

				pointer dstShiftStringPtr = data() + index + srcCount;
				const_pointer srcShiftStringPtrFirst = data() + index + replaceSize;
				const_pointer srcShiftStringPtrLast = endPtr();
				uninitializedCopy<const_pointer, pointer>(srcShiftStringPtrFirst, srcShiftStringPtrLast, dstShiftStringPtr);

				setSize(newSize);
				addNullTerminater();
			}

			if (IsSameC<Decay<value_type>, Utf32> && IsSameC<SrcCharType, Ascii>) {
				const Ascii* dstCovertPtrFirst = data() + pos;
				const Ascii* dstConvertPtrLast = dstCovertPtrFirst + srcCount;
				uninitializedFillConvert<pointer, SrcCharType>(dstCovertPtrFirst, dstConvertPtrLast, natl::forward<SrcCharType>(ch));
			} else {
				const_pointer dstCovertPtrFirst = data() + pos;
				const_pointer dstConvertPtrLast = dstCovertPtrFirst + srcCount;
				uninitializedFill<pointer, value_type>(dstCovertPtrFirst, dstConvertPtrLast, natl::forward<value_type>(ch));
			}

			return self();
		}

		constexpr BaseString& replace(const size_type pos, const size_type count, const BaseString& str) noexcept {
			return replace<value_type>(pos, count, str.toStringView(), 0, npos);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, const BaseString& str) noexcept {
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const size_type pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<value_type>(pos, count, str.toStringView(), 0, npos);
		}

		constexpr BaseString& replace(const size_type pos, size_type count, const BaseString& str, const size_type pos2, size_type count2 = npos) noexcept {
			return replace<value_type>(pos, count, str.toStringView(), pos2, count2);
		}

		constexpr BaseString& replace(size_type pos, size_type count, const_pointer cstr, size_type count2) noexcept {
			return replace<value_type>(pos, count, ConstBaseStringView(cstr), 0, count2);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, const_pointer cstr, size_type count2) noexcept {
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const size_type pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<value_type>(pos, count, ConstBaseStringView(cstr), 0, count2);
		}

		constexpr BaseString& replace(size_type pos, size_type count, const_pointer cstr) noexcept {
			return replace<value_type>(pos, count, ConstBaseStringView(cstr), 0, npos);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, const_pointer cstr) noexcept {
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const size_type pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<value_type>(pos, count, ConstBaseStringView(cstr), 0, npos);
		}

		constexpr BaseString& replace(size_type pos, size_type count, size_type count2, const value_type character) noexcept {
			return replace<value_type>(pos, count, count2, character);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, size_type count2, const value_type character) noexcept {
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const size_type pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<value_type>(pos, count, count2, character);
		}

		template<typename InputIt>
		constexpr BaseString& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2) noexcept {
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const size_type pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			const size_type count2 = iterDistance<typename const_iterator::pointer>(&*first2, &*last2);
			return replace<value_type>(pos, count, ConstBaseStringView(&*last, count2), 0, npos);
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& replace(size_type pos, size_type count, const StringViewLike& stringView) noexcept {
			return replace<value_type>(pos, count, ConstBaseStringView(stringView.data(), stringView.size()), 0, npos);
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& replace(const_iterator first, const_iterator last, const StringViewLike& stringView) noexcept {
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const size_type pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<value_type>(pos, count, ConstBaseStringView(stringView.data(), stringView.size()), 0, npos);
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		constexpr BaseString& replace(size_type pos, size_type count, const StringViewLike& stringView, size_type pos2, size_type count2 = npos) noexcept {
			return replace<value_type>(pos, count, ConstBaseStringView(stringView.data(), stringView.size()), pos2, count2);
		}

		constexpr void resize(const size_type count) noexcept {
			resize(count, value_type{});
		}

		constexpr void resize(const size_type count, const value_type character) noexcept {
			if (count < size()) {
				setSize(count);
				addNullTerminater();
			} else if (count > size()) {
				factorReserve(count);

				pointer srcStringPtrFirst = endPtr();
				pointer srcStringPtrLast = srcStringPtrFirst + count;
				uninitializedFill<pointer, value_type>(srcStringPtrFirst, srcStringPtrLast, character);

				setSize(count);
				addNullTerminater();
			}
		}

		constexpr void swap(BaseString& other) noexcept {
			natl::swap<BaseString>(self(), other);
		}

		//Search
		constexpr size_type find(const BaseString& v, size_type pos = 0) const noexcept {
			return toStringView().find(v.toStringView(), pos);
		}
		constexpr size_type find(const value_type ch, size_type pos = 0) const noexcept {
			return toStringView().find(ConstBaseStringView(addressof<value_type>(ch), 1), pos);
		}
		constexpr size_type find(const_pointer s, const size_type pos, const size_type count) const noexcept {
			return toStringView().find(ConstBaseStringView(s, count), pos);
		}
		constexpr size_type find(const_pointer s, const size_type pos = 0) const noexcept {
			return toStringView().find(ConstBaseStringView(s), pos);
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type> && !IsSameC<StringViewLike, ConstBaseStringView>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) noexcept {
			return toStringView().find(ConstBaseStringView(sv.data(), sv.size()), pos);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept {
			return toStringView().find(static_cast<ConstBaseStringView>(sv), pos);
		}

		constexpr size_type find(Ascii ch, const size_type pos = 0) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().find(BaseStringView<const Ascii>(addressof<Ascii>(ch), 1), pos);
		}
		constexpr size_type find(const Ascii* s, const size_type pos, const size_type count) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().find(BaseStringView<const Ascii>(s, count), pos);
		}
		constexpr size_type find(const Ascii* s, const size_type pos = 0) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().find(BaseStringView<const Ascii>(s), pos);
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, Ascii>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().find(BaseStringView<const Ascii>(sv.data(), sv.size()), pos);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, Ascii>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().find(static_cast<ConstBaseStringView>(sv), pos);
		}

		constexpr size_type rfind(const BaseString& v, size_type pos = npos) const noexcept {
			return toStringView().rfind(v.toStringView(), pos);
		};
		constexpr size_type rfind(value_type ch, size_type pos = npos) const noexcept {
			return toStringView().rfind(ConstBaseStringView(addressof<value_type>(ch), 1), pos);
		}
		constexpr size_type rfind(const_pointer s, size_type pos, size_type count) const {
			return toStringView().rfind(ConstBaseStringView(s, count), pos);
		}
		constexpr size_type rfind(const_pointer s, const size_type pos = npos) const {
			return toStringView().rfind(ConstBaseStringView(s), pos);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, value_type>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept {
			return toStringView().rfind(static_cast<ConstBaseStringView>(sv), pos);
		}

		constexpr size_type rfind(Ascii ch, const size_type pos = npos) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().rfind(BaseStringView<const Ascii>(addressof<Ascii>(ch), 1), pos);
		}
		constexpr size_type rfind(const Ascii* s, const size_type pos, const size_type count) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().rfind(BaseStringView<const Ascii>(s, count), pos);
		}
		constexpr size_type rfind(const Ascii* s, const size_type pos = npos) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().rfind(BaseStringView<const Ascii>(s), pos);
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) const noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().rfind(BaseStringView<const Ascii>(sv.data(), sv.size()), pos);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, Ascii>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return toStringView().rfind(static_cast<ConstBaseStringView>(sv), pos);
		}

		//operations
		constexpr Bool starts_with(const BaseStringView<value_type>& sv) const noexcept {
			return toStringView().starts_with(sv);
		}
		constexpr Bool starts_with(const value_type ch) const noexcept {
			return toStringView().starts_with(ch);
		}
		constexpr Bool starts_with(const_pointer s) const noexcept {
			return toStringView().starts_with(s);
		}

		constexpr Bool ends_with(const BaseStringView<value_type>& sv) const noexcept {
			return toStringView().ends_with(sv);
		}
		constexpr Bool ends_with(const value_type ch) const noexcept {
			return toStringView().ends_with(ch);
		}
		constexpr Bool ends_with(const_pointer s) const noexcept {
			return toStringView().ends_with(s);
		}

		constexpr Bool contains(const BaseStringView<value_type>& sv) const noexcept {
			return find(sv) != npos;
		}
		constexpr Bool contains(const value_type ch) const noexcept {
			return find(ch) != npos;
		}
		constexpr Bool contains(const_pointer s) const {
			return find(s) != npos;
		}

		constexpr ConstBaseStringView substrView(size_type pos = 0, size_type count = npos) const& noexcept {
			if (pos > size()) {
				return ConstBaseStringView(nullptr, 0);
			} else {
				return ConstBaseStringView(data() + pos, min<size_type>(size() - pos, count));
			}
		}
		constexpr BaseString substrView(size_type pos = 0, size_type count = npos)&& {
			if (pos > size()) {
				return BaseStringView<value_type>(nullptr, 0);
			} else {
				return BaseStringView<value_type>(data() + pos, min<size_type>(size() - pos, count));
			}
		}

		constexpr BaseString substr(size_type pos = 0, size_type count = npos) const& noexcept {
			return BaseString(substrView(pos, count));
		}
		constexpr BaseString substr(size_type pos = 0, size_type count = npos)&& {
			return BaseString(substrView(pos, count));
		}

		//cat operators
		friend constexpr BaseString operator+(const BaseString& lhs, const BaseString& rhs) noexcept {
			BaseString temp;
			temp.factorReserve(lhs.size() + rhs.size());
			temp.append(lhs.toStringView());
			temp.append(rhs);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const_pointer rhs) noexcept {
			BaseStringView<const value_type> rhsSringView = BaseStringView<const value_type>(rhs);

			BaseString temp;
			temp.factorReserve(lhs.size() + rhsSringView.size());
			temp.append(lhs.toStringView());
			temp.append(rhsSringView);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const value_type rhs) noexcept {
			BaseString temp;
			temp.factorReserve(lhs.size() + 1);
			temp.append(lhs.toStringView());
			temp.append(rhs);
			return temp;
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const value_type>)
		friend constexpr BaseString operator+(const BaseString& lhs, const StringViewLike& rhs) noexcept {
			BaseString temp;
			temp.factorReserve(lhs.size() + rhs.size());
			temp.append(lhs.toStringView());
			temp.append<StringViewLike>(rhs);
			return temp;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr BaseString operator+(const BaseString& lhs, const StringLike& rhs) noexcept {
			BaseStringView<value_type> rhsSringView = static_cast<BaseStringView<value_type>>(rhs);

			BaseString temp;
			temp.factorReserve(lhs.size() + rhsSringView.size());
			temp.append(lhs.toStringView());
			temp.append(rhsSringView);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			BaseStringView<const Ascii> rhsSringView = BaseStringView<const Ascii>(rhs);

			BaseString temp;
			temp.factorReserve(lhs.size() + rhsSringView.size());
			temp.append(lhs.toStringView());
			temp.append(rhsSringView);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			BaseString temp;
			temp.factorReserve(lhs.size() + 1);
			temp.append(lhs.toStringView());
			temp.append(rhs);
			return temp;
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, Ascii>)
		friend constexpr BaseString operator+(const BaseString& lhs, const StringViewLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			BaseString temp;
			temp.factorReserve(lhs.size() + rhs.size());
			temp.append(lhs.toStringView());
			temp.append<StringViewLike>(rhs);
			return temp;
		}


		//compare operators 
		friend constexpr Bool operator==(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() == rhs.toStringView();
		}
		friend constexpr Bool operator==(const BaseString& lhs, const_pointer rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		friend constexpr Bool operator==(const BaseString& lhs, const value_type rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator==(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator==(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		friend constexpr Bool operator==(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() == rhs;
		}
		friend constexpr Bool operator==(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() == rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		friend constexpr Bool operator==(const BaseString& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() == rhs;
		}

		friend constexpr Bool operator!=(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() != rhs.toStringView();
		}
		friend constexpr Bool operator!=(const BaseString& lhs, const_pointer rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		friend constexpr Bool operator!=(const BaseString& lhs, const value_type rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator!=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator!=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		friend constexpr Bool operator!=(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() != rhs;
		}
		friend constexpr Bool operator!=(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() != rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		friend constexpr Bool operator!=(const BaseString& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() != rhs;
		}

		friend constexpr Bool operator<(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() < rhs.toStringView();
		}
		friend constexpr Bool operator<(const BaseString& lhs, const_pointer rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		friend constexpr Bool operator<(const BaseString& lhs, const value_type rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator<(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator<(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		friend constexpr Bool operator<(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() < rhs;
		}
		friend constexpr Bool operator<(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() < rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		friend constexpr Bool operator<(const BaseString& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() < rhs;
		}

		friend constexpr Bool operator<=(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() <= rhs.toStringView();
		}
		friend constexpr Bool operator<=(const BaseString& lhs, const_pointer rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		friend constexpr Bool operator<=(const BaseString& lhs, const value_type rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator<=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator<=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		friend constexpr Bool operator<=(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() <= rhs;
		}
		friend constexpr Bool operator<=(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() <= rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		friend constexpr Bool operator<=(const BaseString& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() <= rhs;
		}

		friend constexpr Bool operator>(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() > rhs.toStringView();
		}
		friend constexpr Bool operator>(const BaseString& lhs, const_pointer rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		friend constexpr Bool operator>(const BaseString& lhs, const value_type rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		friend constexpr Bool operator>(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() > rhs;
		}
		friend constexpr Bool operator>(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() > rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		friend constexpr Bool operator>(const BaseString& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() > rhs;
		}

		friend constexpr Bool operator>=(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() >= rhs.toStringView();
		}
		friend constexpr Bool operator>=(const BaseString& lhs, const_pointer rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		friend constexpr Bool operator>=(const BaseString& lhs, const value_type rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator>=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr Bool operator>=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		friend constexpr Bool operator>=(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() >= rhs;
		}
		friend constexpr Bool operator>=(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() >= rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		friend constexpr Bool operator>=(const BaseString& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() >= rhs;
		}

		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() <=> rhs.toStringView();
		}
		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const_pointer rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const value_type rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, value_type>)
		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<value_type>> && !IsStringViewLike<StringLike, value_type>)
		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const Ascii* rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() <=> rhs;
		}
		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const Ascii rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() <=> rhs;
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, Ascii>)
		friend constexpr StrongOrdering operator<=>(const BaseString& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<value_type>, Utf32>) {
			return lhs.toStringView() <=> rhs;
		}

		//operations 
		constexpr BaseString& reverse() noexcept {
			natl::reverse<iterator>(begin(), end());
			return self();
		}

		//hash
		constexpr size_type hash() const noexcept {
			return fnv1aHash(data(), size());
		}
		constexpr static size_type staticHash(const BaseString& stringView) noexcept {
			return fnv1aHash(stringView.data(), stringView.size());
		}

		//set
		constexpr void setEnd(value_type value) noexcept {
			if (isConstantEvaluated()) {
				natl::construct<value_type, value_type>(endPtr(), forward<value_type>(value));
			} else {
				*end() = value;
			}
		}
	};

	template<typename DataType, Size ByteSize, typename Alloc = DefaultAllocator<DataType>>
		requires(ByteSize >= 32 && IsAllocator<Alloc>)
	using BaseStringByteSize = BaseString<DataType, (ByteSize - sizeof(BaseStringBaseMembersRef<DataType>)) / sizeof(DataType), Alloc>;

	template<Size ByteSize, typename Alloc = DefaultAllocator<Ascii>>
		requires(ByteSize >= 32 && IsAllocator<Alloc>)
	using StringByteSize = BaseString<Ascii, (ByteSize - sizeof(BaseStringBaseMembersRef<Ascii>)) / sizeof(Ascii), Alloc>;

	template<Size ByteSize, typename Alloc = DefaultAllocator<Ascii>>
		requires(ByteSize >= 32 && IsAllocator<Alloc>)
	using AsciiStringByteSize = StringByteSize<ByteSize, Alloc>;


	using String32 = BaseStringByteSize<Ascii, 32>;
	static_assert(sizeof(String32) == 32, "natl: String32 should be 32 bytes");

	using String64 = BaseStringByteSize<Ascii, 64>;
	static_assert(sizeof(String64) == 64, "natl: String64 should be 64 bytes");

	using String96 = BaseStringByteSize<Ascii, 96>;
	static_assert(sizeof(String96) == 96, "natl: String96 should be 96 bytes");

	using String128 = BaseStringByteSize<Ascii, 128>;
	static_assert(sizeof(String128) == 128, "natl: String128 should be 128 bytes");

	using String256 = BaseStringByteSize<Ascii, 256>;
	static_assert(sizeof(String256) == 256, "natl: String256 should be 256 bytes");

	using AsciiString32 = String32; 
	using AsciiString64 = String64; 
	using AsciiString96 = String96; 
	using AsciiString128 = String128; 
	using AsciiString256 = String256; 

	using Utf32String32 = BaseStringByteSize<Utf32, 32>;
	static_assert(sizeof(Utf32String32) == 32, "natl: Utf32String32 should be 32 bytes");

	using Utf32String64 = BaseStringByteSize<Utf32, 64>;;
	static_assert(sizeof(Utf32String64) == 64, "natl: Utf32String64 should be 64 bytes");

	using Utf32String96 = BaseStringByteSize<Utf32, 96>;
	static_assert(sizeof(Utf32String96) == 96, "natl: Utf32String96 should be 96 bytes");

	using Utf32String128 = BaseStringByteSize<Utf32, 128>;
	static_assert(sizeof(Utf32String128) == 128, "natl: Utf32String128 should be 128 bytes");

	using Utf32String256 = BaseStringByteSize<Utf32, 256>;
	static_assert(sizeof(Utf32String256) == 256, "natl: Utf32String256 should be 256 bytes");

	using Utf32String512 = BaseStringByteSize<Utf32, 512>;
	static_assert(sizeof(Utf32String512) == 512, "natl: Utf32String512 should be 512 bytes");

	using String = String32;
	using AsciiString = String32;
	using Utf32String = Utf32String64;

	namespace literals {
		constexpr String32 operator ""_string(const Ascii* str, StdSize len) noexcept {
			return String32(str, static_cast<Size>(len));
		}
		constexpr String32 operator ""_string_32(const Ascii* str, StdSize len) noexcept {
			return String32(str, static_cast<Size>(len));
		}
		constexpr String64 operator ""_string_64(const Ascii* str, StdSize len) noexcept {
			return String64(str, static_cast<Size>(len));
		}
		constexpr String96 operator ""_string_96(const Ascii* str, StdSize len) noexcept {
			return String96(str, static_cast<Size>(len));
		}
		constexpr String128 operator ""_string_128(const Ascii* str, StdSize len) noexcept {
			return String128(str, static_cast<Size>(len));
		}
		constexpr String256 operator ""_string_256(const Ascii* str, StdSize len) noexcept {
			return String256(str, static_cast<Size>(len));
		}

		constexpr AsciiString32 operator ""_assci_string(const Ascii* str, StdSize len) noexcept {
			return AsciiString32(str, static_cast<Size>(len));
		}
		constexpr AsciiString32 operator ""_assci_string_32(const Ascii* str, StdSize len) noexcept {
			return AsciiString32(str, static_cast<Size>(len));
		}
		constexpr AsciiString64 operator ""_assci_string_64(const Ascii* str, StdSize len) noexcept {
			return AsciiString64(str, static_cast<Size>(len));
		}
		constexpr AsciiString96 operator ""_assci_string_96(const Ascii* str, StdSize len) noexcept {
			return AsciiString96(str, static_cast<Size>(len));
		}
		constexpr AsciiString128 operator ""_assci_string_128(const Ascii* str, StdSize len) noexcept {
			return AsciiString128(str, static_cast<Size>(len));
		}
		constexpr AsciiString256 operator ""_assci_string_256(const Ascii* str, StdSize len) noexcept {
			return AsciiString256(str, static_cast<Size>(len));
		}


		constexpr Utf32String operator ""_utf32_string(const char32_t* str, StdSize len) noexcept {
			return Utf32String(str, static_cast<Size>(len));
		}
		constexpr Utf32String32 operator ""_utf32_string_32(const char32_t* str, StdSize len) noexcept {
			return Utf32String32(str, static_cast<Size>(len));
		}
		constexpr Utf32String64 operator ""_utf32_string_64(const char32_t* str, StdSize len) noexcept {
			return Utf32String64(str, static_cast<Size>(len));
		}
		constexpr Utf32String128 operator ""_utf32_string_128(const char32_t* str, StdSize len) noexcept {
			return Utf32String128(str, static_cast<Size>(len));
		}
		constexpr Utf32String256 operator ""_utf32_string_256(const char32_t* str, StdSize len) noexcept {
			return Utf32String256(str, static_cast<Size>(len));
		}
		constexpr Utf32String512 operator ""_utf32_string_512(const char32_t* str, StdSize len) noexcept {
			return Utf32String512(str, static_cast<Size>(len));
		}

		constexpr Utf32String operator ""_utf32_string(const Ascii* str, StdSize len) noexcept {
			return Utf32String(str, static_cast<Size>(len));
		}
		constexpr Utf32String32 operator ""_utf32_string_32(const Ascii* str, StdSize len) noexcept {
			return Utf32String32(str, static_cast<Size>(len));
		}
		constexpr Utf32String64 operator ""_utf32_string_64(const Ascii* str, StdSize len) noexcept {
			return Utf32String64(str, static_cast<Size>(len));
		}
		constexpr Utf32String128 operator ""_utf32_string_128(const Ascii* str, StdSize len) noexcept {
			return Utf32String128(str, static_cast<Size>(len));
		}
		constexpr Utf32String256 operator ""_utf32_string_256(const Ascii* str, StdSize len) noexcept {
			return Utf32String256(str, static_cast<Size>(len));
		}
		constexpr Utf32String512 operator ""_utf32_string_512(const Ascii* str, StdSize len) noexcept {
			return Utf32String512(str, static_cast<Size>(len));
		}
	}

	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyCompareableV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>> 
		: FalseType {};

	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyRelocatableV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>>
		: TrueType {};
	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyConstructibleV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>>
		: TrueType {};
	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyDestructibleV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>>
		: FalseType {};

	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyConstRefConstructibleV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>>
		: FalseType {};
	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyMoveConstructibleV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>>
		: FalseType {};

	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyConstRefAssignableV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>>
		: FalseType {};
	template<typename CharType, Size bufferSize, typename Alloc, Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
	struct IsTriviallyMoveAssignableV<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>>
		: FalseType {};
}