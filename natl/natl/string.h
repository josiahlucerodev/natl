#pragma once 

//std
#include <string>
#include <string_view>

//own
#include "basicTypes.h"
#include "allocator.h"
#include "iterators.h"
#include "dataMovement.h"
#include "algorithm.h"
#include "container.h"
#include "stringView.h"
#include "arrayView.h"

//interface 
namespace natl {
	template<class CharType>
	class BaseStringBaseMembersRef {
		Size stringSizeAndSmallStringFlag;
		CharType* stringPtr;
		Size stringCapacity;
	};

	template<class CharType, Size bufferSize, class Alloc = DefaultAllocator<CharType>>
		requires(IsAllocator<Alloc>)
	class BaseString {
	public:
		using value_type = CharType;
		using reference = CharType&;
		using const_reference = const CharType&;
		using pointer = CharType*;
		using const_pointer = const CharType*;
		using optional_pointer = Option<CharType*>;
		using optional_const_pointer = Option<const CharType*>;
		using difference_type = PtrDiff;
		using size_type = Size;

		using iterator = RandomAccessIterator<CharType>;
		using const_iterator = RandomAccessIterator<const CharType>;
		using reverse_iterator = ReverseRandomAccessIterator<CharType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const CharType>;

		static constexpr size_type npos = size_type(-1);

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;

		constexpr static bool enableSmallString = true;

		using ConstBaseStringView = BaseStringView<const CharType>;
	private:
		Size stringSizeAndSmallStringFlag; 
		CharType* stringPtr;
		Size stringCapacity;
		CharType smallStringStorage[bufferSize];

		//small string 
	private:
		constexpr void setAsSmallString() noexcept { 
			if constexpr (enableSmallString) {
				stringSizeAndSmallStringFlag = setNthBitToZero(stringSizeAndSmallStringFlag, 63); 
			}
		}
		constexpr void setAsNotSmallString() noexcept { 
			if constexpr(enableSmallString) {
				stringSizeAndSmallStringFlag = setNthBitToOne(stringSizeAndSmallStringFlag, 63);
			}
		}
		constexpr void setSize(const Size newSize) noexcept {
			stringSizeAndSmallStringFlag = (stringSizeAndSmallStringFlag & ~0x7FFFFFFFFFFFFFFFULL) | newSize;
		}
	public:
		constexpr bool isSmallString() const noexcept { 
			if (!enableSmallString) {
				return false;
			} else {
				return !(stringSizeAndSmallStringFlag & ~0x7FFFFFFFFFFFFFFFULL);
			}
		}
		constexpr bool isNotSmallString() const noexcept { 
			return !isSmallString();
		}
		constexpr static Size smallStringCapacity() noexcept {
			if (std::is_constant_evaluated() || !enableSmallString) {
				return bufferSize;
			} else {
				return bufferSize + ((sizeof(stringPtr) + sizeof(stringCapacity)) / sizeof(CharType));
			}
		}
		constexpr const CharType* smallStringLocation() const noexcept {
			if (std::is_constant_evaluated()) {
				return smallStringStorage;
			} else {
				return reinterpret_cast<const CharType*>(reinterpret_cast<const ui8*>(this) + sizeof(stringSizeAndSmallStringFlag));
			}
		}
		constexpr CharType* smallStringLocation() noexcept {
			if (std::is_constant_evaluated()) {
				return smallStringStorage;
			} else {
				return reinterpret_cast<CharType*>(reinterpret_cast<ui8*>(this) + sizeof(stringSizeAndSmallStringFlag));
			}
		}

		constexpr Size capacity() const noexcept { return isSmallString() ? smallStringCapacity() : stringCapacity; };

		constexpr Size size() const noexcept { return stringSizeAndSmallStringFlag & 0x7FFFFFFFFFFFFFFFULL; }
		constexpr size_type length() const noexcept { return size(); };
		constexpr size_type max_size() const noexcept { return 0x7FFFFFFFFFFFFFFFULL; };
	private:
		constexpr void baseConstructorInit() noexcept {
			stringSizeAndSmallStringFlag = 0;
			stringPtr = 0;
			stringCapacity = 0;
			if (std::is_constant_evaluated()) {
				uninitializedFill<CharType*, CharType>(smallStringStorage, smallStringStorage + bufferSize, CharType(0));
			}
		}
		constexpr BaseString& self() noexcept { return *this; }
		constexpr const BaseString& self() const noexcept { return *this; }
	public:
		//constructors 
		constexpr BaseString() noexcept : stringSizeAndSmallStringFlag(0), stringPtr(0), stringCapacity(0), smallStringStorage{} {}
		constexpr BaseString(const BaseString& str) noexcept {
			baseConstructorInit();
			construct(str.data(), str.size());
		}
		constexpr BaseString(BaseString&& str) noexcept {
			baseConstructorInit();
			construct(forward<BaseString>(str));
		}
		constexpr BaseString(const CharType* str) noexcept {
			baseConstructorInit();
			construct(str, cstringLength(str));
		}
		constexpr BaseString(const CharType* str, const Size count) noexcept {
			baseConstructorInit();
			construct(str, count);
		}

		constexpr BaseString(const Size count, const CharType character) noexcept {
			baseConstructorInit();
			construct(count, character);
		}

		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString(const StringViewLike& str) noexcept {
			baseConstructorInit();
			construct(str.data(), str.size());
		}

		constexpr BaseString(const char* str, const Size count) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			baseConstructorInit();
			//TODO
			assign(str, count);
		}

		constexpr BaseString(const char* str) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			baseConstructorInit();
			//TODO
			assign(str);
		}

		constexpr BaseString(const Size count, const char character) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			baseConstructorInit();
			//TODO
			assign(character, count);
		}

		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		constexpr BaseString(const StringViewLike& str) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			baseConstructorInit();
			//TODO
			assignStringViewLike<StringViewLike>(str);
		}

		//destructor
		constexpr ~BaseString() {
			if (isNotSmallString() && stringPtr) {
				Alloc::deallocate(stringPtr, capacity());
			}
		}

		//construct
	private:
		constexpr BaseString& construct(const CharType* otherPtr, const Size& count) noexcept {
			if (count == 0) {
				stringSizeAndSmallStringFlag = 0;
				stringCapacity = 0;
				stringPtr = nullptr;
			} else if (count < smallStringCapacity()) {
				const CharType* srcDataPtrFirst = otherPtr;
				const CharType* srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcDataPtrFirst, srcDataPtrLast, smallStringLocation());

				setAsSmallString();

				const Size newSize = count;
				setSize(newSize);

				stringCapacity = 0;
				stringPtr = nullptr;
			} else {
				const Size newSize = count;
				reserve(newSize);

				const CharType* srcDataPtrFirst = otherPtr;
				const CharType* srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcDataPtrFirst, srcDataPtrLast, data());

				setSize(newSize);
			}

			return self();
		}
		constexpr BaseString& construct(BaseString&& other) noexcept {
			if (other.isEmpty()) {
				stringSizeAndSmallStringFlag = other.stringSizeAndSmallStringFlag;
				stringCapacity = 0;
				stringPtr = nullptr;
			} else if (other.isSmallString()) {
				const CharType* srcDataPtrFirst = other.data();
				const CharType* srcDataPtrLast = srcDataPtrFirst + other.size();
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcDataPtrFirst, srcDataPtrLast, smallStringLocation());

				stringSizeAndSmallStringFlag = other.stringSizeAndSmallStringFlag;
				stringCapacity = 0;
				stringPtr = nullptr;
			} else {
				stringSizeAndSmallStringFlag = other.stringSizeAndSmallStringFlag;
				stringCapacity = other.stringCapacity;
				stringPtr = other.stringPtr;
			}

			other.stringSizeAndSmallStringFlag = 0;
			other.stringCapacity = 0;
			other.stringPtr = nullptr;

			return self();
		}
		constexpr BaseString& construct(const size_type count, const CharType& value = CharType()) noexcept {
			if (count == 0) {
				stringSizeAndSmallStringFlag = 0;
				stringCapacity = 0;
				stringPtr = nullptr;
				return self();
			}

			reserve(count);

			CharType* fillDstPtr = data();
			CharType* fillDstPtrLast = fillDstPtr + count;
			uninitializedFill<CharType*, CharType>(fillDstPtr, fillDstPtrLast, value);

			setSize(count);
			return self();
		}

		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, CharType>)
		constexpr BaseString& construct(Iter first, Iter last) noexcept {
			if constexpr (std::contiguous_iterator<Iter>) {
				const Size count = iterDistance<Iter>(first, last);
				const CharType* firstPtr = iteratorToAddress<Iter>(first);
				return construct(firstPtr, count);
			}

			reserve(10);
			for (; first != last; first++) {
				push_back(*first);
			}
			return self();
		}

	public:

		//assignment 
		constexpr BaseString& operator=(const BaseString& str) noexcept {
			return assign(str);
		}
		constexpr BaseString& operator=(BaseString&& str) noexcept {
			return assign(forward<BaseString>(str));
		}
		constexpr BaseString& operator=(const CharType* str) noexcept {
			return assign(str);
		}
		constexpr BaseString& operator=(const CharType character) noexcept {
			return assign(character);
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& operator=(const StringViewLike& stringView) noexcept {
			return assign<StringViewLike>(stringView);
		}

		constexpr BaseString& operator=(const char* str) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return assign(str);
		}
		constexpr BaseString& operator=(const char character) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return assign(character);
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		constexpr BaseString& operator=(const StringViewLike& stringView) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return assign<StringViewLike>(stringView);
		}

		constexpr BaseString& assign(const BaseString& str) noexcept {
			if (str.isEmpty()) {
				release();

				stringSizeAndSmallStringFlag = 0;
				stringPtr = nullptr;
				stringCapacity = 0;
			} else {
				const Size newSize = str.size();
				reserve(newSize);
				uninitializedCopyNoOverlap<const CharType*, CharType*>(str.data(), str.data() + newSize, data());
				setSize(newSize);
				addNullTerminater();
			}

			return self();
		}

		constexpr BaseString& assign(BaseString&& str) noexcept {
			if (str.isSmallString()) {
				stringSizeAndSmallStringFlag = str.stringSizeAndSmallStringFlag;
				const CharType* srcStringPtrFirst = str.smallStringLocation();
				const CharType* srcStringPtrLast = srcStringPtrFirst + str.size();
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, smallStringLocation());

				addNullTerminater();

				str.stringSizeAndSmallStringFlag = 0;
			} else {
				release();

				stringSizeAndSmallStringFlag = str.stringSizeAndSmallStringFlag;
				stringPtr = str.stringPtr;
				stringCapacity = str.stringCapacity;

				str.stringSizeAndSmallStringFlag = 0;
				str.stringPtr = nullptr;
				str.stringCapacity = 0;
			}

			return self();
		}

		constexpr BaseString& assign(const CharType* str, const Size count) noexcept {
			if (str == nullptr) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const Size srcStringSize = count;
			const CharType* srcStringPtrFirst = str;
			const CharType* srcStringPtrLast = srcStringPtrFirst + srcStringSize;

			if (srcStringSize + 1 < smallStringCapacity()) {
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, smallStringLocation());
			} else {
				reserve(srcStringSize);
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, stringPtr);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& assign(const CharType* str) noexcept {
			if (str == nullptr) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			return assign(str, cstringLength(str));
		}

		constexpr BaseString& assign(const Size count, CharType character) noexcept {
			if (count == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			if (count + 1 < smallStringCapacity()) {
				CharType* srcStringPtrFirst = smallStringLocation();
				CharType* srcStringPtrLast = srcStringPtrFirst + count;
				uninitializedFill<CharType*, CharType>(srcStringPtrFirst, srcStringPtrLast, forward<CharType>(character));
			} else {
				reserve(count);
				CharType* srcStringPtrFirst = stringPtr;
				CharType* srcStringPtrLast = srcStringPtrFirst + count;
				uninitializedFill<CharType*, CharType>(srcStringPtrFirst, srcStringPtrLast, forward<CharType>(character));
			}
			
			setSize(count);
			addNullTerminater();
			return self();
		}

		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& assign(const StringViewLike& stringView) noexcept {
			if (stringView.size() == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const Size srcStringSize = stringView.size();
			const CharType* srcStringPtrFirst = stringView.data();
			const CharType* srcStringPtrLast = srcStringPtrFirst + srcStringSize;

			if (srcStringSize + 1 < smallStringCapacity()) {
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, smallStringLocation());
			} else {
				reserve(srcStringSize);
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, stringPtr);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& assign(const char* str, const Size count) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			const Size stringSize = count;
			if (stringSize == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			const Size srcStringSize = stringSize;
			const char* srcStringPtrFirst = str;
			const char* srcStringPtrLast = srcStringPtrFirst + srcStringSize;

			CharType* stringDst = nullptr;
			if (srcStringSize + 1 < smallStringCapacity()) {
				stringDst = smallStringLocation();
			}
			else {
				reserve(srcStringSize);
				stringDst = stringPtr;
			}

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, stringDst++) {
				*stringDst = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& assign(const char* str) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			if (str == nullptr) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}
			return assign(str, cstringLength(str));
		}

		constexpr BaseString& assign(const char character, const Size count = 1) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			if (count == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return self();
			}

			CharType* stringDst = nullptr;
			if (count + 1 < smallStringCapacity()) {
				stringDst = smallStringLocation();
			} else {
				reserve(count);
				stringDst = stringPtr;
			}

			for (Size i = 0; i < count; i++, stringDst++) {
				*stringDst = static_cast<Utf32>(character);
			}

			setSize(count);
			addNullTerminater();
			return self();
		}

		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		constexpr BaseString& assign(const StringViewLike& stringView) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			if (stringView.size() == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
				return;
			}

			const Size srcStringSize = stringView.size();
			const char* srcStringPtrFirst = stringView.data();
			const char* srcStringPtrLast = srcStringPtrLast + srcStringSize;

			CharType* stringDst = nullptr;
			if (srcStringSize + 1 < smallStringCapacity()) {
				stringDst = smallStringLocation();
			} else {
				reserve(srcStringSize);
				stringDst = stringPtr;
			}

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, stringDst++) {
				*stringDst = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(srcStringSize);
			addNullTerminater();
			return self();
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


		constexpr const CharType* data() const noexcept {
			return isSmallString() ? smallStringLocation() : stringPtr;
		}
		constexpr CharType* data() noexcept {
			return isSmallString() ? smallStringLocation() : stringPtr;
		}

		constexpr const CharType* c_str() const noexcept { return data(); }

		constexpr operator BaseStringView<CharType>() noexcept {
			return BaseStringView<CharType>(data(), size());
		}
		constexpr operator ConstBaseStringView() const noexcept {
			return ConstBaseStringView(data(), size());
		}

		constexpr BaseStringView<CharType> toStringView() noexcept {
			return BaseStringView<CharType>(data(), size());
		}
		constexpr ConstBaseStringView toStringView() const noexcept {
			return ConstBaseStringView(data(), size());
		}

		constexpr ArrayView<CharType> toArrayView() noexcept {
			return ArrayView<CharType>(data(), size());
		}
		constexpr ArrayView<const CharType> toArrayView() const noexcept {
			return ArrayView<const CharType>(data(), size());
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
		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }


		//capactity 
	private:
		constexpr void release() noexcept {
			if (!isSmallString() && stringPtr) {
				Alloc::deallocate(stringPtr, capacity());
				stringPtr = nullptr;
			}
		}
		constexpr static Size setNthBitToZero(const Size num, const Size n) noexcept {
			const Size mask = ~(Size(1) << n);
			const Size result = num & mask;
			return result;
		}
		constexpr static Size setNthBitToOne(const Size num, const Size n) {
			const Size mask = Size(1) << n;
			const Size result = num | mask;
			return result;
		}

	public:
		constexpr bool empty() const noexcept { return size() == 0; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }

		constexpr void reserve(Size newCapacity) noexcept {
			newCapacity += 1;
			if (newCapacity < capacity()) {
				return;
			} else if (newCapacity < smallStringCapacity()) {
				setAsSmallString();
				return;
			}

			newCapacity *= 2;

			CharType* newStringPtr = Alloc::allocate(newCapacity);

			const CharType* srcStringPtrFirst = data();
			const CharType* srcStringPtrLast = srcStringPtrFirst + size();
			uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, newStringPtr);

			if (isNotSmallString() && stringPtr) {
				Alloc::deallocate(stringPtr, capacity());
			} 

			stringPtr = newStringPtr;
			stringCapacity = newCapacity;
			setAsNotSmallString();
		}

		constexpr void shrink_to_fit() {
			const Size newCapacity = size() + 1;
			const bool sameSize = newCapacity == capacity();
			const bool canBeSmallString = newCapacity < smallStringCapacity();
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
				CharType* volatile oldStringPtr = stringPtr;
				Size volatile oldCapacity = capacity();

				CharType* dstStringPtr = smallStringLocation();
				const CharType* srcStringPtrFirst = stringPtr;
				const CharType* srcStringPtrLast = stringPtr + newCapacity;
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

				Alloc::deallocate(oldStringPtr, oldCapacity);

				setAsSmallString();
				return;
			} else if (sameSize) {
				return;
			}

			if (newCapacity < smallStringCapacity() || newCapacity == capacity()) {
				return;
			}

			if (size() == 0) {
				release();
				stringSizeAndSmallStringFlag = 0;
			} else {
				CharType* newStringPtr = Alloc::allocate(newCapacity);
				const CharType* srcStringPtrFirst = stringPtr;
				const CharType* srcStringPtrLast = stringPtr + newCapacity;
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, newStringPtr);
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
			const Size newSize = size() + sv.size();
			reserve(newSize);

			const Size relocateCount = size() - index;
			CharType* relocateDstPtr = data() + index + sv.size();
			const CharType* relocateSrcPtrFirst = data() + index;
			const CharType* relocateSrcPtrLast = relocateSrcPtrFirst + relocateCount;
			uninitializedCopy<const CharType*, CharType*>(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			CharType* insertStrDstPtr = data() + index;
			const CharType* insertStrSrcPtrFirst = sv.data();
			const CharType* insertStrSrcPtrLast = insertStrSrcPtrFirst + sv.size();
			uninitializedCopy<const CharType*, CharType*>(insertStrSrcPtrFirst, insertStrSrcPtrLast, insertStrDstPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& insert(const size_type index, const size_type count, const CharType character) noexcept {
			const Size newSize = size() + count;
			reserve(newSize);

			const Size relocateCount = size() - index;
			CharType* relocateDstPtr = data() + index + count;
			const CharType* relocateSrcPtrFirst = data() + index;
			const CharType* relocateSrcPtrLast = relocateSrcPtrFirst + relocateCount;
			uninitializedCopy<const CharType*, CharType*>(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			const CharType* insertDstPtrFirst = data() + index;
			const CharType* insertDstPtrLast = insertDstPtrFirst + count;
			uninitializedFill<CharType*, CharType>(insertDstPtrFirst, insertDstPtrLast, character);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& insert(const size_type index, const CharType* str) noexcept {
			return insert(index, ConstBaseStringView(str));
		}
		constexpr BaseString& insert(const size_type index, const CharType* str, const size_type count) noexcept {
			return insert(index, ConstBaseStringView(str, min<Size>(cstringLength(str), count)));
		}
		constexpr BaseString& insert(const size_type index, const BaseString& str) noexcept {
			return insert(index, str.toStringView());
		}
		constexpr BaseString& insert(size_type index, const BaseString& str, const size_type s_index, const size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			return insert(index, str.substrView(s_index, count));
		}

		constexpr iterator insert(const_iterator pos, const CharType character) noexcept {
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			insert(index, character, 1);
			return iterator(data() + index);
		}
		constexpr iterator insert(const_iterator pos, size_type count, const CharType character) noexcept {
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			insert(index, character, count);
			return iterator(data() + index);
		}
		
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType> && !std::is_same_v<StringViewLike, ConstBaseStringView>)
		constexpr BaseString& insert(const size_type index, const StringViewLike& stringView) noexcept {
			return insert(index, ConstBaseStringView(stringView.data(), stringView.size()));
		}
		template< class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& insert(const size_type index, const StringViewLike& stringView, size_type t_index, size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			return insert(index, ConstBaseStringView(stringView.data() + t_index, min<size_type>(stringView.size(), count)));
		}

		constexpr BaseString& erase(const size_type index, const size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			const Size newCount = min<Size>(count, size() - index);
			CharType* dstStringPtr = data() + index;
			const CharType* srcStringPtrFirst = dstStringPtr + newCount;
			const CharType* srcStringPtrLast = endPtr();
			uninitializedCopy<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			setSize(size() - newCount);
			addNullTerminater();
			return self();
		};
		constexpr BaseString& erase(const_iterator position) noexcept {
			CharType* dstStringPtr = data() + (&*position-data());
			const CharType* srcStringPtrFirst = dstStringPtr + 1;
			const CharType* srcStringPtrLast = endPtr();
			uninitializedCopy<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			setSize(size() - 1);
			addNullTerminater();
			return self();
		}
		constexpr iterator erase(const_iterator first, const_iterator last) noexcept {
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			CharType* dstStringPtr = data() + (&*first-data());
			const CharType* srcStringPtrFirst = dstStringPtr + count;
			const CharType* srcStringPtrLast = endPtr();
			uninitializedCopy<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			setSize(size() - count);
			addNullTerminater();
			return iterator(dstStringPtr);
		}

		constexpr void push_back(const CharType character) noexcept {
			const Size newSize = size() + 1;
			reserve(newSize);
			setEnd(character);
			setSize(newSize);
			addNullTerminater();
		}
		constexpr void pop_back() noexcept {
			setSize(size() - 1);
			addNullTerminater();
		}

		constexpr BaseString& append(size_type count, const CharType character) noexcept {
			const Size newSize = size() + count;
			reserve(newSize);

			CharType* srcStringPtrFirst = endPtr();
			CharType* srcStringPtrLast = srcStringPtrFirst + count;
			uninitializedFill<CharType*, CharType>(srcStringPtrFirst, srcStringPtrLast, character);

			setSize(newSize);
			addNullTerminater();
			return self();
		}
		constexpr BaseString& append(const BaseString& str) noexcept {
			const Size newSize = size() + str.size();
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			const CharType* srcStringPtrFirst = str.beginPtr();
			const CharType* srcStringPtrLast = str.endPtr();
			uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}
		constexpr BaseString& append(const CharType* str, const size_type count) noexcept {
			return append(BaseStringView<const CharType>(str, count));
		}
		constexpr BaseString& append(const BaseString& str, const size_type pos, const size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			const Size newCount = min<Size>(str.size() - pos, count);
			const Size newSize = size() + newCount;
			resize(newSize);

			CharType* dstStringPtr = endPtr();
			const CharType* srcStringPtrFirst = str.beginPtr() + pos;
			const CharType* srcStringPtrLast = str + newCount;
			uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const CharType* str) noexcept {
			const Size count = cstringLength(str);
			const Size newSize = size() + count;
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			const CharType* srcStringPtrFirst = str;
			const CharType* srcStringPtrLast = str + count;
			uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		template<class InputIt>
		constexpr BaseString& append(InputIt first, InputIt last) noexcept {
			const Size newSize = size() + iterDistance<InputIt>(first, last);
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			uninitializedCopyNoOverlap<InputIt, CharType*>(first, last, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& append(const StringViewLike& stringView) noexcept {
			const Size count = stringView.size();
			const Size newSize = size() + count;
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			const CharType* srcStringPtrFirst = stringView.data();
			const CharType* srcStringPtrLast = srcStringPtrFirst + count;
			uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& append(const StringViewLike& stringView, size_type pos, size_type count = 0xFFFFFFFFFFFFFFFF) noexcept {
			const Size newCount = min<Size>(stringView.size() - pos, count);
			const Size newSize = size() + newCount;
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			const CharType* srcStringPtrFirst = stringView.data() + pos;
			const CharType* srcStringPtrLast = srcStringPtrFirst + newCount;
			uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const char* str) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			const Size count = cstringLength(str);
			const Size newSize = size() + count;
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			const char* srcStringPtrFirst = str;
			const char* srcStringPtrLast = srcStringPtrFirst + count;

			for (; srcStringPtrFirst < srcStringPtrLast; srcStringPtrFirst++, dstStringPtr++) {
				*dstStringPtr = static_cast<Utf32>(*srcStringPtrFirst);
			}

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const Size count, const char character) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			const Size newSize = size() + count;
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			const CharType* dstStringPtrLast = dstStringPtr + count;
			for (; dstStringPtr < dstStringPtrLast; dstStringPtr++) {
				*dstStringPtr = static_cast<Utf32>(character);
			}

			setSize(newSize);
			addNullTerminater();
			return self();
		}

		constexpr BaseString& append(const char* str, const size_type count) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return append(BaseStringView<const char>(str, count));
		}

		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		constexpr BaseString& append(const StringViewLike& stringView) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			const Size count = stringView.size();
			const Size newSize = size() + count;
			reserve(newSize);

			CharType* dstStringPtr = endPtr();
			const char* srcStringPtrFirst = stringView.data();
			const char* srcStringPtrLast = srcStringPtrLast + count;

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
		constexpr BaseString& operator+=(const CharType* rhs) noexcept {
			return append(rhs);
		}
		constexpr BaseString& operator+=(const CharType rhs) noexcept {
			return append(rhs);
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		constexpr BaseString& operator+=(const StringLike& rhs) noexcept {
			return append(rhs);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, CharType>)
		constexpr BaseString& operator+=(const StringLike& rhs) noexcept {
			return append(rhs);
		}
		constexpr BaseString& operator+=(const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return append(rhs);
		}
		constexpr BaseString& operator+=(const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return append(rhs);
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		constexpr BaseString& operator+=(const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return append(rhs);
		}

		template <class SrcCharType>
		constexpr BaseString& replace(const size_type pos, const size_type count, const BaseStringView<const SrcCharType>& sv, const size_type pos2, const size_type count2 = npos) {
			const Size index = min<Size>(size(), pos);
			const Size srcIndex = min<Size>(sv.size(), pos2);
			const Size srcCount = min<Size>(sv.size() - srcIndex, count2);
			const Size replaceSize = min<Size>(size() - index, count);
			const Size endingSize = size() - index - replaceSize;

			if (endingSize > 0) {
				const Size newSize = index + srcCount + endingSize;
				reserve(newSize);

				CharType* dstShiftStringPtr = data() + index + srcCount;
				const CharType* srcShiftStringPtrFirst = data() + index + replaceSize;
				const CharType* srcShiftStringPtrLast = endPtr();
				uninitializedCopy<const CharType*, CharType*>(srcShiftStringPtrFirst, srcShiftStringPtrLast, dstShiftStringPtr);

				setSize(newSize);
				addNullTerminater();
			} 

			if constexpr (std::is_same_v<std::decay_t<CharType>, Utf32> && std::is_same_v<SrcCharType, char>) {
				CharType* dstStringPtr = data() + index;
				const char* srcStringPtrFirst = sv.data() + srcIndex;
				const char* srcStringPtrLast = srcStringPtrFirst + srcCount;
				uninitializedCopyConvert<const char*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			} else {
				CharType* dstStringPtr = data() + index;
				const CharType* srcStringPtrFirst = sv.data() + srcIndex;
				const CharType* srcStringPtrLast = srcStringPtrFirst + srcCount;
				uninitializedCopyNoOverlap<const CharType*, CharType*>(srcStringPtrFirst, srcStringPtrLast, dstStringPtr);
			}

			return self();
		}

		template <class SrcCharType>
		constexpr BaseString& replace(const size_type pos, const size_type count, const size_type count2, SrcCharType ch) {
			const Size index = min<Size>(size(), pos);
			const Size srcCount = count2;
			const Size replaceSize = min<Size>(size() - index, count);
			const Size endingSize = size() - index - replaceSize;

			if (endingSize > 0) {
				const Size newSize = index + srcCount + endingSize;
				reserve(newSize);

				CharType* dstShiftStringPtr = data() + index + srcCount;
				const CharType* srcShiftStringPtrFirst = data() + index + replaceSize;
				const CharType* srcShiftStringPtrLast = endPtr();
				uninitializedCopy<const CharType*, CharType*>(srcShiftStringPtrFirst, srcShiftStringPtrLast, dstShiftStringPtr);

				setSize(newSize);
				addNullTerminater();
			}

			if (std::is_same_v<std::decay_t<CharType>, Utf32> && std::is_same_v<SrcCharType, char>) {
				const char* dstCovertPtrFirst = data() + pos;
				const char* dstConvertPtrLast = dstCovertPtrFirst + srcCount;
				uninitializedFillConvert<CharType*, SrcCharType>(dstCovertPtrFirst, dstConvertPtrLast, std::forward<SrcCharType>(ch));
			} else {
				const CharType* dstCovertPtrFirst = data() + pos;
				const CharType* dstConvertPtrLast = dstCovertPtrFirst + srcCount;
				uninitializedFill<CharType*, CharType>(dstCovertPtrFirst, dstConvertPtrLast, std::forward<CharType>(ch));
			}

			return self();
		}

		constexpr BaseString& replace(const size_type pos, const size_type count, const BaseString& str) noexcept {
			return replace<CharType>(pos, count, str.toStringView(), 0, npos);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, const BaseString& str) noexcept {
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const Size pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<CharType>(pos, count, str.toStringView(), 0, npos);
		}

		constexpr BaseString& replace(const size_type pos, size_type count, const BaseString& str, const size_type pos2, size_type count2 = npos) noexcept {
			return replace<CharType>(pos, count, str.toStringView(), pos2, count2);
		}

		constexpr BaseString& replace(size_type pos, size_type count, const CharType* cstr, size_type count2) noexcept {
			return replace<CharType>(pos, count, ConstBaseStringView(cstr), 0, count2);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, const CharType* cstr, size_type count2) noexcept {
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const Size pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<CharType>(pos, count, ConstBaseStringView(cstr), 0, count2);
		}

		constexpr BaseString& replace(size_type pos, size_type count, const CharType* cstr) noexcept {
			return replace<CharType>(pos, count, ConstBaseStringView(cstr), 0, npos);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, const CharType* cstr) noexcept {
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const Size pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<CharType>(pos, count, ConstBaseStringView(cstr), 0, npos);
		}

		constexpr BaseString& replace(size_type pos, size_type count, size_type count2, const CharType character) noexcept {
			return replace<CharType>(pos, count, count2, character);
		}

		constexpr BaseString& replace(const_iterator first, const_iterator last, size_type count2, const CharType character) noexcept {
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const Size pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<CharType>(pos, count, count2, character);
		}

		template<class InputIt>
		constexpr BaseString& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2) noexcept {
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const Size pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			const Size count2 = iterDistance<typename const_iterator::pointer>(&*first2, &*last2);
			return replace<CharType>(pos, count, ConstBaseStringView(&*last, count2), 0, npos);
		}

		template< class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& replace(size_type pos, size_type count, const StringViewLike& stringView) noexcept {
			return replace<CharType>(pos, count, ConstBaseStringView(stringView.data(), stringView.size()), 0, npos);
		}

		template< class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& replace(const_iterator first, const_iterator last, const StringViewLike& stringView) noexcept {
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const Size pos = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*last);
			return replace<CharType>(pos, count, ConstBaseStringView(stringView.data(), stringView.size()), 0, npos);
		}

		template< class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		constexpr BaseString& replace(size_type pos, size_type count, const StringViewLike& stringView, size_type pos2, size_type count2 = npos) noexcept {
			return replace<CharType>(pos, count, ConstBaseStringView(stringView.data(), stringView.size()), pos2, count2);
		}

		constexpr void resize(const size_type count) noexcept {
			resize(count, CharType{});
		}

		constexpr void resize(const size_type count, const CharType character) noexcept {
			if (count < size()) {
				setSize(count);
				addNullTerminater();
			} else if (count > size()) {
				reserve(count);

				CharType* srcStringPtrFirst = endPtr();
				CharType* srcStringPtrLast = srcStringPtrFirst + count;
				uninitializedFill<CharType*, CharType>(srcStringPtrFirst, srcStringPtrLast, character);

				setSize(count);
				addNullTerminater();
			}
		}

		constexpr void swap(BaseString& other) noexcept {
			swap<BaseString>(self(), other);
		}

		//Search
		constexpr size_type find(const BaseString& v, size_type pos = 0) const noexcept {
			return toStringView().find(v.toStringView(), pos);
		}
		constexpr size_type find(const CharType ch, size_type pos = 0) const noexcept {
			return toStringView().find(ConstBaseStringView(addressof<CharType>(ch), 1), pos);
		}
		constexpr size_type find(const CharType* s, const size_type pos, const size_type count) const noexcept {
			return toStringView().find(ConstBaseStringView(s, count), pos);
		}
		constexpr size_type find(const CharType* s, const size_type pos = 0) const noexcept {
			return toStringView().find(ConstBaseStringView(s), pos);
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType> && !std::is_same_v<StringViewLike, ConstBaseStringView>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) noexcept {
			return toStringView().find(ConstBaseStringView(sv.data(), sv.size()), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept {
			return toStringView().find(static_cast<ConstBaseStringView>(sv), pos);
		}

		constexpr size_type find(char ch, const size_type pos = 0) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().find(BaseStringView<const char>(addressof<char>(ch), 1), pos);
		}
		constexpr size_type find(const char* s, const size_type pos, const size_type count) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().find(BaseStringView<const char>(s, count), pos);
		}
		constexpr size_type find(const char* s, const size_type pos = 0) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().find(BaseStringView<const char>(s), pos);
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().find(BaseStringView<const char>(sv.data(), sv.size()), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, char>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().find(static_cast<ConstBaseStringView>(sv), pos);
		}

		constexpr size_type rfind(const BaseString& v, size_type pos = npos) const noexcept {
			return toStringView().rfind(v.toStringView(), pos);
		};
		constexpr size_type rfind(CharType ch, size_type pos = npos) const noexcept {
			return toStringView().rfind(ConstBaseStringView(addressof<CharType>(ch), 1), pos);
		}
		constexpr size_type rfind(const CharType* s, size_type pos, size_type count) const {
			return toStringView().rfind(ConstBaseStringView(s, count), pos);
		}
		constexpr size_type rfind(const CharType* s, const size_type pos = npos) const {
			return toStringView().rfind(ConstBaseStringView(s), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, CharType>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept {
			return toStringView().rfind(static_cast<ConstBaseStringView>(sv), pos);
		}

		constexpr size_type rfind(char ch, const size_type pos = npos) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().rfind(BaseStringView<const char>(addressof<char>(ch), 1), pos);
		}
		constexpr size_type rfind(const char* s, const size_type pos, const size_type count) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().rfind(BaseStringView<const char>(s, count), pos);
		}
		constexpr size_type rfind(const char* s, const size_type pos = npos) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().rfind(BaseStringView<const char>(s), pos);
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().rfind(BaseStringView<const char>(sv.data(), sv.size()), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, ConstBaseStringView> && !IsStringViewLike<StringLike, char>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return toStringView().rfind(static_cast<ConstBaseStringView>(sv), pos);
		}

		//operations
		constexpr bool starts_with(const BaseStringView<CharType>& sv) const noexcept {
			return toStringView().starts_with(sv);
		}
		constexpr bool starts_with(const CharType ch) const noexcept {
			return toStringView().starts_with(ch);
		}
		constexpr bool starts_with(const CharType* s) const noexcept {
			return toStringView().starts_with(s);
		}

		constexpr bool ends_with(const BaseStringView<CharType>& sv) const noexcept {
			return toStringView().ends_with(sv);
		}
		constexpr bool ends_with(const CharType ch) const noexcept {
			return toStringView().ends_with(ch);
		}
		constexpr bool ends_with(const CharType* s) const noexcept {
			return toStringView().ends_with(s);
		}

		constexpr bool contains(const BaseStringView<CharType>& sv) const noexcept {
			return find(sv) != npos;
		}
		constexpr bool contains(const CharType ch) const noexcept {
			return find(ch) != npos;
		}
		constexpr bool contains(const CharType* s) const {
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
				return BaseStringView<CharType>(nullptr, 0);
			} else {
				return BaseStringView<CharType>(data() + pos, min<size_type>(size() - pos, count));
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
			temp.reserve(lhs.size() + rhs.size());
			temp.append(lhs.toStringView());
			temp.append(rhs);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const CharType* rhs) noexcept {
			BaseStringView<CharType> rhsSringView = BaseStringView<CharType>(rhs);

			BaseString temp;
			temp.reserve(lhs.size() + rhsSringView.size());
			temp.append(lhs.toStringView());
			temp.append(rhsSringView);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const CharType rhs) noexcept {
			BaseString temp;
			temp.reserve(lhs.size() + 1);
			temp.append(lhs.toStringView());
			temp.append(rhs);
			return temp;
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, const CharType>)
		friend constexpr BaseString operator+(const BaseString& lhs, const StringViewLike& rhs) noexcept {
			BaseString temp;
			temp.reserve(lhs.size() + rhs.size());
			temp.append(lhs.toStringView());
			temp.append<StringViewLike>(rhs);
			return temp;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr BaseString operator+(const BaseString& lhs, const StringLike& rhs) noexcept {
			BaseStringView<CharType> rhsSringView = static_cast<BaseStringView<CharType>>(rhs);

			BaseString temp;
			temp.reserve(lhs.size() + rhsSringView.size());
			temp.append(lhs.toStringView());
			temp.append(rhsSringView);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			BaseStringView<const char> rhsSringView = BaseStringView<const char>(rhs);

			BaseString temp;
			temp.reserve(lhs.size() + rhsSringView.size());
			temp.append(lhs.toStringView());
			temp.append(rhsSringView);
			return temp;
		}
		friend constexpr BaseString operator+(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			BaseString temp;
			temp.reserve(lhs.size() + 1);
			temp.append(lhs.toStringView());
			temp.append(rhs);
			return temp;
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		friend constexpr BaseString operator+(const BaseString& lhs, const StringViewLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			BaseString temp;
			temp.reserve(lhs.size() + rhs.size());
			temp.append(lhs.toStringView());
			temp.append<StringViewLike>(rhs);
			return temp;
		}


		//compare operators 
		friend constexpr bool operator==(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() == rhs.toStringView();
		}
		friend constexpr bool operator==(const BaseString& lhs, const CharType* rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		friend constexpr bool operator==(const BaseString& lhs, const CharType rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator==(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator==(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() == rhs;
		}
		friend constexpr bool operator==(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() == rhs;
		}
		friend constexpr bool operator==(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() == rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr bool operator==(const BaseString& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() == rhs;
		}

		friend constexpr bool operator!=(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() != rhs.toStringView();
		}
		friend constexpr bool operator!=(const BaseString& lhs, const CharType* rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		friend constexpr bool operator!=(const BaseString& lhs, const CharType rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator!=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator!=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() != rhs;
		}
		friend constexpr bool operator!=(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() != rhs;
		}
		friend constexpr bool operator!=(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() != rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr bool operator!=(const BaseString& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() != rhs;
		}

		friend constexpr bool operator<(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() < rhs.toStringView();
		}
		friend constexpr bool operator<(const BaseString& lhs, const CharType* rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		friend constexpr bool operator<(const BaseString& lhs, const CharType rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator<(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator<(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() < rhs;
		}
		friend constexpr bool operator<(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() < rhs;
		}
		friend constexpr bool operator<(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() < rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr bool operator<(const BaseString& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() < rhs;
		}

		friend constexpr bool operator<=(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() <= rhs.toStringView();
		}
		friend constexpr bool operator<=(const BaseString& lhs, const CharType* rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		friend constexpr bool operator<=(const BaseString& lhs, const CharType rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator<=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator<=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <= rhs;
		}
		friend constexpr bool operator<=(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() <= rhs;
		}
		friend constexpr bool operator<=(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() <= rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr bool operator<=(const BaseString& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() <= rhs;
		}

		friend constexpr bool operator>(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() > rhs.toStringView();
		}
		friend constexpr bool operator>(const BaseString& lhs, const CharType* rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		friend constexpr bool operator>(const BaseString& lhs, const CharType rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() > rhs;
		}
		friend constexpr bool operator>(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() > rhs;
		}
		friend constexpr bool operator>(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() > rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr bool operator>(const BaseString& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() > rhs;
		}

		friend constexpr bool operator>=(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() >= rhs.toStringView();
		}
		friend constexpr bool operator>=(const BaseString& lhs, const CharType* rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		friend constexpr bool operator>=(const BaseString& lhs, const CharType rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator>=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr bool operator>=(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() >= rhs;
		}
		friend constexpr bool operator>=(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() >= rhs;
		}
		friend constexpr bool operator>=(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() >= rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr bool operator>=(const BaseString& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() >= rhs;
		}

		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const BaseString& rhs) noexcept {
			return lhs.toStringView() <=> rhs.toStringView();
		}
		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const CharType* rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const CharType rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const StringLike& rhs) noexcept {
			return lhs.toStringView() <=> rhs;
		}
		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() <=> rhs;
		}
		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() <=> rhs;
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr std::strong_ordering operator<=>(const BaseString& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs.toStringView() <=> rhs;
		}

		//operations 
		constexpr BaseString& reverse() noexcept {
			reverse<iterator>(begin(), end());
			return self();
		}

		//hash
		constexpr size_type hash() const noexcept {
			return fnv1aHash(data(), size());
		}
		constexpr static size_type staticHash(const BaseString& stringView) noexcept {
			return fnv1aHash(stringView.data(), stringView.size());
		}

		//stream
		friend std::ostream& operator<<(std::ostream& os, const BaseString& str) {
			os << std::string_view(str.data(), str.size());
			return os;
		}

		friend std::istream& operator>>(std::istream& is, BaseString& str) {
			str.clear();
			constexpr const Size isBufferSize = 1024;
			char buffer[isBufferSize];

			if (is.getline(buffer, isBufferSize)) {
				str.append(buffer);
			}

			return is;
		}

		//set
		constexpr void setEnd(CharType value) noexcept {
			if (std::is_constant_evaluated()) {
				std::construct_at<CharType, CharType>(endPtr(), forward<CharType>(value));
			} else {
				*end() = value;
			}
		}
	};

	
	template<class CharType, Size byteSize, class Alloc = DefaultAllocator<CharType>>
		requires(byteSize >= 32)
	using BaseStringByteSize = BaseString<CharType, (byteSize - sizeof(BaseStringBaseMembersRef<CharType>)) / sizeof(CharType), Alloc>;

	using String32 = BaseStringByteSize<char, 32>;
	static_assert(sizeof(String32) == 32, "natl: String32 should be 32 bytes");

	using String64 = BaseStringByteSize<char, 64>;
	static_assert(sizeof(String64) == 64, "natl: String64 should be 64 bytes");

	using String96 = BaseStringByteSize<char, 96>;
	static_assert(sizeof(String96) == 96, "natl: String96 should be 96 bytes");

	using String128 = BaseStringByteSize<char, 128>;
	static_assert(sizeof(String128) == 128, "natl: String128 should be 128 bytes");

	using String256 = BaseStringByteSize<char, 256>;
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
		constexpr String32 operator ""_natl_dyn_s(const char* str, std::size_t len) noexcept {
			return String32(str, static_cast<Size>(len));
		}
		constexpr String32 operator ""_natl_dyn_s_32(const char* str, std::size_t len) noexcept {
			return String32(str, static_cast<Size>(len));
		}
		constexpr String64 operator ""_natl_dyn_s_64(const char* str, std::size_t len) noexcept {
			return String64(str, static_cast<Size>(len));
		}
		constexpr String96 operator ""_natl_dyn_s_96(const char* str, std::size_t len) noexcept {
			return String96(str, static_cast<Size>(len));
		}
		constexpr String128 operator ""_natl_dyn_s_128(const char* str, std::size_t len) noexcept {
			return String128(str, static_cast<Size>(len));
		}
		constexpr String256 operator ""_natl_dyn_s_256(const char* str, std::size_t len) noexcept {
			return String256(str, static_cast<Size>(len));
		}

		constexpr AsciiString32 operator ""_natl_assci_dyn_s(const char* str, std::size_t len) noexcept {
			return AsciiString32(str, static_cast<Size>(len));
		}
		constexpr AsciiString32 operator ""_natl_assci_dyn_s_32(const char* str, std::size_t len) noexcept {
			return AsciiString32(str, static_cast<Size>(len));
		}
		constexpr AsciiString64 operator ""_natl_assci_dyn_s_64(const char* str, std::size_t len) noexcept {
			return AsciiString64(str, static_cast<Size>(len));
		}
		constexpr AsciiString96 operator ""_natl_assci_dyn_s_96(const char* str, std::size_t len) noexcept {
			return AsciiString96(str, static_cast<Size>(len));
		}
		constexpr AsciiString128 operator ""_natl_assci_dyn_s_128(const char* str, std::size_t len) noexcept {
			return AsciiString128(str, static_cast<Size>(len));
		}
		constexpr AsciiString256 operator ""_natl_assci_dyn_s_256(const char* str, std::size_t len) noexcept {
			return AsciiString256(str, static_cast<Size>(len));
		}


		constexpr Utf32String operator ""_natl_utf32_dyn_s(const char32_t* str, std::size_t len) noexcept {
			return Utf32String(str, static_cast<Size>(len));
		}
		constexpr Utf32String32 operator ""_natl_utf32_dyn_s_32(const char32_t* str, std::size_t len) noexcept {
			return Utf32String32(str, static_cast<Size>(len));
		}
		constexpr Utf32String64 operator ""_natl_utf32_dyn_s_64(const char32_t* str, std::size_t len) noexcept {
			return Utf32String64(str, static_cast<Size>(len));
		}
		constexpr Utf32String128 operator ""_natl_utf32_dyn_s_128(const char32_t* str, std::size_t len) noexcept {
			return Utf32String128(str, static_cast<Size>(len));
		}
		constexpr Utf32String256 operator ""_natl_utf32_dyn_s_256(const char32_t* str, std::size_t len) noexcept {
			return Utf32String256(str, static_cast<Size>(len));
		}
		constexpr Utf32String512 operator ""_natl_utf32_dyn_s_512(const char32_t* str, std::size_t len) noexcept {
			return Utf32String512(str, static_cast<Size>(len));
		}

		constexpr Utf32String operator ""_natl_utf32_dyn_s(const char* str, std::size_t len) noexcept {
			return Utf32String(str, static_cast<Size>(len));
		}
		constexpr Utf32String32 operator ""_natl_utf32_dyn_s_32(const char* str, std::size_t len) noexcept {
			return Utf32String32(str, static_cast<Size>(len));
		}
		constexpr Utf32String64 operator ""_natl_utf32_dyn_s_64(const char* str, std::size_t len) noexcept {
			return Utf32String64(str, static_cast<Size>(len));
		}
		constexpr Utf32String128 operator ""_natl_utf32_dyn_s_128(const char* str, std::size_t len) noexcept {
			return Utf32String128(str, static_cast<Size>(len));
		}
		constexpr Utf32String256 operator ""_natl_utf32_dyn_s_256(const char* str, std::size_t len) noexcept {
			return Utf32String256(str, static_cast<Size>(len));
		}
		constexpr Utf32String512 operator ""_natl_utf32_dyn_s_512(const char* str, std::size_t len) noexcept {
			return Utf32String512(str, static_cast<Size>(len));
		}
	}

}