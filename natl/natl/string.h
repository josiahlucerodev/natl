#pragma once 

//own
#include "typeTraits.h"
#include "iterators.h"
#include "allocator.h"
#include "option.h"
#include "stringView.h"
#include "dynamicArray.h"

//interface 
namespace natl {
	template<
		class CharType,
		class Alloc = Allocator>
	class BaseString {
	public:
		using value_type = CharType;
		using reference = CharType&;
		using const_reference = const CharType&;
		using pointer = CharType*;
		using const_pointer = const CharType*;
		using optional_pointer = Option<CharType*>;
		using optional_const_pointer = Option<const CharType*>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		using allocator_type = Alloc;

		using iterator = RandomAccessIterator<CharType>;
		using const_iterator = RandomAccessIterator<const CharType>;
		using reverse_iterator = ReverseRandomAccessIterator<CharType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const CharType>;
		using alloc_traits = AllocatorTraits<Alloc>;

		using BaseStringViewType = BaseStringView<CharType>;
	private:
		DynamicArray<CharType, Allocator> characters;
	public:
		constexpr BaseString() = default;
		constexpr BaseString(const BaseString& src) : characters(src.characters) {}
		constexpr BaseString(BaseString&& src) : characters{} { characters = std::move(src.characters); }

		constexpr BaseString(const CharType* cString) : characters{} {
			if (!cString) { return; }
			size_type stringLength = 1;
			const_pointer tempDataPtr = cString;
			while (*tempDataPtr++) { stringLength++; }

			append(cString, stringLength - 1);
		}

		constexpr BaseString(const BaseStringViewType& stringView) : characters {} {
			append(stringView);
		}

		constexpr size_type size() const noexcept { 
			return characters.isEmpty() ? 0 : characters.size() - 1;
		}

		constexpr size_type length() const noexcept { return size(); }
		constexpr size_type capacity() const noexcept { return characters.capacity(); }
		constexpr size_type freeSpaceSize() const noexcept { return characters.freeSpaceSize(); }
		constexpr pointer data() noexcept requires(isNotConst<CharType>) { return characters.data(); }
		constexpr const_pointer data() const noexcept { return characters.data(); };
		constexpr const CharType* cStr() const noexcept { return characters.data(); }

		constexpr pointer beginPtr() noexcept requires(isNotConst<CharType>) { return characters.beginPtr(); }
		constexpr const_pointer beginPtr() const noexcept { return characters.beginPtr(); }

		constexpr pointer endPtr() noexcept requires(isNotConst<CharType>) { 
			return characters.isEmpty() ? characters.endPtr() : characters.endPtr() - 1;
		}
		constexpr const_pointer endPtr() const noexcept { 
			return characters.isEmpty() ? characters.endPtr() : characters.endPtr() - 1;
		}

		constexpr bool isEmpty() const noexcept { return !bool(size()); }
		constexpr bool isNotEmpty() const noexcept { return bool(size()); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr iterator begin() noexcept requires(isNotConst<CharType>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(isNotConst<CharType>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(isNotConst<CharType>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(isNotConst<CharType>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		constexpr reference at(const size_type index) noexcept requires(isNotConst<CharType>) { return data()[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return data()[index]; };
		constexpr reference operator[] (const size_type index) { return at(index); }
		constexpr const_reference operator[] (const size_type index) const { return at(index); }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() ? size() - 1 : 0; }

		constexpr reference front() noexcept requires(isNotConst<CharType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }
		constexpr BaseStringViewType toStringView() const noexcept { return BaseStringViewType(data(), size()); }
	private:
		constexpr void removeNullTerminatingCharacter() noexcept {
			if (isNotEmpty()) { characters.popBack(); }
		}
		constexpr void addNullTerminatingCharacter() {
			characters.pushBack(CharType('\0'));
		}
	public:
		constexpr void reserve(const size_type newCapacity) noexcept { characters.reserve(newCapacity + 1); }

		constexpr void resize(const size_type newSize) noexcept { 
			removeNullTerminatingCharacter();
			characters.resize(newSize);
			addNullTerminatingCharacter();
		}

		constexpr void clear() noexcept { characters.clear(); }
		constexpr void pushBack(const CharType& value) noexcept { 
			removeNullTerminatingCharacter();
			characters.pushBack(value); 
			addNullTerminatingCharacter();
		}
		constexpr void pushBack(CharType&& value) noexcept { 
			removeNullTerminatingCharacter();
			characters.pushBack(value);
			addNullTerminatingCharacter();
		}
		constexpr void popBack() noexcept { 
			removeNullTerminatingCharacter();
			characters.popBack();
			addNullTerminatingCharacter();
		}

		constexpr void append(const BaseString& str) noexcept {
			if (str.isEmpty()) { return; }
			removeNullTerminatingCharacter();
			reserve(size() + str.length() + 1);
			characters.append(str.characters.data(), str.length());
			addNullTerminatingCharacter();
		}

		constexpr void append(const BaseStringViewType& stringView) noexcept {
			if (stringView.size() == 0) { return; }
			removeNullTerminatingCharacter();
			reserve(size() + stringView.length() + 1);
			characters.append(stringView.cStr(), stringView.length());
			addNullTerminatingCharacter();
		}

		constexpr void append(const CharType* stringPtr, const size_type& length) noexcept {
			removeNullTerminatingCharacter();
			reserve(length + length + 1);
			characters.append(stringPtr, length);
			addNullTerminatingCharacter();
		}

		constexpr void append(const CharType* stringPtr) noexcept {
			append(BaseStringViewType(stringPtr));
		}

		constexpr void fill(const char character) noexcept {
			if (isEmpty()) { return; }
			removeNullTerminatingCharacter();
			characters.fill(character);
			addNullTerminatingCharacter();
		}

		constexpr size_type compare(const BaseString& str) const noexcept {
			size_type index = 0;
			for (index = 0; index < str.length() && index < length(); index++) {
				if (at(index) != str.at(index)) { return index; }
			}
			return index;
		}

		constexpr size_type compare(const BaseStringViewType& stringView) const noexcept {
			size_type index = 0;
			for (index = 0; index < stringView.length() && index < length(); index++) {
				if (at(index) != stringView.at(index)) { return index; }
			}
			return index;
		}

		constexpr bool operator==(const BaseString& str) const noexcept {
			if (length() != str.length()) { return false; }
			for (size_type i = 0; i < length(); i++) {
				if (at(i) != str.at(i)) { return false; }
			}
			return true;
		}

		constexpr BaseString& operator=(const BaseString& src) noexcept {
			characters = src.characters;
			return *this;
		}
		constexpr BaseString& operator=(BaseString&& src) noexcept {
			characters = std::move(src.characters);
			return *this;
		}

		constexpr BaseString& operator=(const BaseStringViewType& stringView) noexcept {
			clear();
			append(stringView);
			return *this;
		};

		constexpr BaseString& operator=(const CharType* cStr) noexcept {
			clear();
			append(BaseStringViewType(cStr));
			return *this;
		};

		constexpr BaseString& operator+=(const BaseString& src) noexcept {
			append(src);
			return *this;
		}

		constexpr BaseString& operator+=(const BaseStringViewType& stringView) noexcept {
			append(stringView);
			return *this;
		}

		constexpr BaseString& operator+=(const CharType* srcCStr) noexcept {
			append(BaseStringViewType(srcCStr));
			return *this;
		}

		constexpr BaseString operator+(const BaseString& rhsString) noexcept  {
			return merge<BaseString, BaseString>(*this, rhsString);
		}

		constexpr BaseString operator+(const StringView& rhsStringView) noexcept {
			return merge<BaseString, StringView>(*this, rhsStringView);
		}

		constexpr explicit operator BaseStringViewType() const noexcept {
			return toStringView();
		}

	private:
		template<class StringType>
		constexpr static void appendStringToString(BaseString& dstString, const StringType& srcString) {
			dstString.append(srcString);
		}
		constexpr static void appendStringsToString(BaseString& dstString) {}

		template<class FirstStringType, class... LastStringTypes>
		constexpr static void appendStringsToString(BaseString& dstString, const FirstStringType& firstSrcString, const LastStringTypes&... lastSrcStrings) {
			appendStringToString(dstString, firstSrcString);
			appendStringsToString(dstString, lastSrcStrings...);
		}
	public:

		template<class... StringTypes>
		constexpr static BaseString merge(const StringTypes&... srcStrings) noexcept {
			BaseString dstString{};
			size_type dstSize = sumContaniersSizes<StringTypes...>(srcStrings...);
			dstString.reserve(dstSize);
			appendStringsToString<StringTypes...>(dstString, srcStrings...);
			dstString.pushBack(CharType('\0'));

			return dstString;
		}
	};

	using String = BaseString<char>;
}