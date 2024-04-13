#pragma once 

//std
#include <string_view>

//own
#include "iterators.h"
#include "option.h"
#include "algorithm.h"
#include "dataMovement.h"
#include "commonHashs.h"
#include "compare.h"

//interface
namespace natl {
	constexpr Size cstringLength(const char* str) noexcept {
		if (isConstantEvaluated()) {
			if (str == nullptr) { return 0; }
			Size size = 0;
			while (str[size] != '\0') { size += 1; }
			return size;
		} else {
			return std::strlen(str);
		}
	}
	constexpr Size cstringLength(const Utf32* str) noexcept {
		if (str == nullptr) { return 0; }
		Size size = 0;
		while (str[size] != '\0') { size += 1; }
		return size;
	}

	template <typename StringView, class CharType>
	concept IsStringViewLike = requires(StringView stringView) {
		{ stringView.data() } -> std::convertible_to<const CharType*>;
		{ stringView.size() } -> std::convertible_to<Size>;
		{ stringView[std::declval<Size>()] } -> std::convertible_to<CharType>;
	};

	template<class CharType>
	class BaseStringView {
	public:
		using value_type = CharType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using optional_pointer = Option<value_type*>;
		using optional_const_pointer = Option<const value_type*>;
		using difference_type = PtrDiff;
		using size_type = Size;

		using iterator = RandomAccessIterator<value_type>;
		using const_iterator = RandomAccessIterator<const value_type>;
		using reverse_iterator = ReverseRandomAccessIterator<value_type>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const value_type>;

		static constexpr size_type npos = size_type(-1);

		//movement info 
		constexpr static Bool triviallyRelocatable = true;
		constexpr static Bool triviallyDefaultConstructible = true;
		constexpr static Bool triviallyCompareable = false;
		constexpr static Bool triviallyDestructible = true;

		using ConstBaseStringView = BaseStringView<const CharType>;
	private:
		CharType* dataPtr;
		size_type stringLength;
	public:
		//constructors
		constexpr BaseStringView() noexcept : dataPtr(nullptr), stringLength(0) {}
		constexpr BaseStringView(const BaseStringView& str) noexcept = default;
		constexpr BaseStringView(CharType* stringPtr, const size_type length) noexcept : dataPtr(stringPtr), stringLength(length) {}
		constexpr BaseStringView(CharType* str) noexcept : dataPtr(str), stringLength(cstringLength(str)) {}

		//destrutor
		constexpr ~BaseStringView() = default;

		//convert 
		constexpr operator BaseStringView<const CharType>() const requires(IsNotConstV<CharType>) {
			return BaseStringView<const CharType>(data(), size());
		}

		//util
		constexpr BaseStringView& self() noexcept { return *this; }
		constexpr const BaseStringView& self() const noexcept { return *this; }

		//iterators
		constexpr pointer beginPtr() requires(IsNotConstV<CharType>) { return dataPtr; }
		constexpr const_pointer beginPtr() const noexcept { return dataPtr; }
		constexpr pointer endPtr() requires(IsNotConstV<CharType>) { return dataPtr + stringLength; }
		constexpr const_pointer endPtr() const noexcept { return dataPtr + stringLength; }

		constexpr iterator begin() noexcept requires(IsNotConstV<CharType>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(IsNotConstV<CharType>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }

		constexpr reverse_iterator rbegin() noexcept requires(IsNotConstV<CharType>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(IsNotConstV<CharType>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		//element access
		constexpr reference operator[] (const size_type index) requires(IsNotConstV<CharType>) { return at(index); }
		constexpr const_reference operator[] (const size_type index) const { return at(index); }

		constexpr reference at(const size_type index) noexcept requires(IsNotConstV<CharType>) { return dataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return dataPtr[index]; };

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() ? size() - 1 : 0; }

		constexpr reference front() noexcept requires(IsNotConstV<CharType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }
		constexpr reference back() noexcept requires(IsNotConstV<CharType>) { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr CharType* data() noexcept requires(IsNotConstV<CharType>) { return dataPtr; }
		constexpr const CharType* data() const noexcept { return dataPtr; }

		constexpr CharType* c_str() noexcept requires(IsNotConstV<CharType>) { return dataPtr; }
		constexpr const CharType* c_str() const noexcept { return dataPtr; }

		//capacity 
		constexpr size_type size() const noexcept { return stringLength; }
		constexpr size_type length() const noexcept { return stringLength; }
		constexpr size_type max_size() const noexcept { return ~size_type(1); };
		constexpr Bool empty() const noexcept { return !dataPtr || stringLength == 0; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }

		//modifiers 
		constexpr void swap(BaseStringView& other) noexcept {
			swap<BaseStringView>(self(), other);
		}
		constexpr void remove_prefix(const size_type n) noexcept {
			const size_type removeCount = min<size_type>(n, size());
			dataPtr += removeCount;
			stringLength -= removeCount;
		}
		constexpr void remove_suffix(const size_type n) noexcept {
			const size_type removeCount = min<size_type>(n, size());
			stringLength -= removeCount;
		}

		//operations
		constexpr size_type copy(CharType* dest, const size_type count, size_type pos = 0) const noexcept {
			const size_type copyCount = min<size_type>(count, size() - pos);
			const CharType* srcPtr = data() + pos;
			const CharType* srcPtrEnd = srcPtr + copyCount;
			CharType* dstPtr = dest;
			uninitializedCopyNoOverlap<const CharType*, CharType*>(srcPtr, srcPtrEnd, dstPtr);
			return copyCount;
		}

		constexpr BaseStringView substr(const size_type pos = 0, const size_type count = npos) noexcept requires(IsNotConstV<CharType>) {
			if (pos > size()) {
				return BaseStringView(nullptr, 0);
			} else {
				return BaseStringView(data() + pos, min<size_type>(size() - pos, count));
			}
		}

		constexpr ConstBaseStringView substr(const size_type pos = 0, const size_type count = npos) const noexcept {
			if (pos > size()) { 
				return ConstBaseStringView(nullptr, 0);
			} else {
				return ConstBaseStringView(data() + pos, min<size_type>(size() - pos, count));
			}
		}

		constexpr int compare(const BaseStringView& v) const noexcept {
			const CharType* lhsPtr = data();
			const CharType* lhsEndPtr = lhsPtr + size();
			const CharType* rhsPtr = v.data();
			const CharType* rhsEndPtr = rhsPtr + v.size();

			for (; lhsPtr < lhsEndPtr && rhsPtr < rhsEndPtr; ++lhsEndPtr, ++rhsEndPtr) {
				if (*lhsPtr < *rhsPtr) return -1;
				if (*lhsPtr > *rhsPtr) return 1;
			}
			if (size() < v.size()) return -1;
			if (size() > v.size()) return 1;
			return 0;
		}
		constexpr int compare(const size_type pos1, const size_type count1, const BaseStringView& v) const noexcept {
			return substr(pos1, count1).compare(v);
		}
		constexpr int compare(const size_type pos1, const size_type count1, const BaseStringView& v, const size_type pos2, const size_type count2) const noexcept {
			return substr(pos1, count1).compare(v.substr(pos2, count2));
		}
		constexpr int compare(const CharType* s) const noexcept {
			return compare(BaseStringView(s));
		}
		constexpr int compare(const size_type pos1, const size_type count1, const CharType* s) const noexcept {
			return substr(pos1, count1).compare(BaseStringView(s));
		}
		constexpr int compare(const size_type pos1, const size_type count1, const CharType* s, const size_type count2) const noexcept {
			return substr(pos1, count1).compare(BaseStringView(s, count2));
		}

		constexpr Bool starts_with(const BaseStringView& sv) const noexcept {
			return BaseStringView(data(), min<size_type>(size(), sv.size())) == sv;
		}
		constexpr Bool starts_with(const CharType ch) const noexcept {
			return !empty() && front() == ch;
		}
		constexpr Bool starts_with(const CharType* s) const noexcept {
			return starts_with(BaseStringView(s));
		}

		constexpr Bool ends_with(const BaseStringView& sv) const noexcept {
			return size() >= sv.size() && compare(size() - sv.size(), npos, sv) == 0;
		}
		constexpr Bool ends_with(const CharType ch) const noexcept {
			return !empty() && back() == ch;
		}
		constexpr Bool ends_with(const CharType* s) const noexcept {
			return ends_with(BaseStringView(s));
		}

		constexpr Bool contains(const BaseStringView sv) const noexcept {
			return find(sv) != npos;
		}
		constexpr Bool contains(const CharType c) const noexcept {
			return find(c) != npos;
		}
		constexpr Bool contains(const CharType* s) const {
			return find(s) != npos;
		}

		template<class TargetCharType>
		constexpr size_type find(const BaseStringView<TargetCharType>& v, size_type pos = 0) const noexcept {
			const size_type strSize = size();
			const size_type targetSize = v.size();
			if (pos > strSize || strSize - pos < targetSize) { return npos; }

			const CharType* strPtr = data() + pos;
			const CharType* strEndPtr = strPtr + strSize - targetSize;
			const TargetCharType* targetPtr = v.data();
			const TargetCharType* targetPtrEnd = targetPtr + v.size();
			size_type findIndex = pos;
			for (; strPtr < strEndPtr; ++strPtr, ++findIndex) {
				Bool match = true;

				const CharType* strTestPtr = strPtr;
				const TargetCharType* targetTestPtr = targetPtr;
				for (; targetTestPtr < targetPtrEnd; ++strTestPtr, ++targetTestPtr) {
					if (*strTestPtr != static_cast<CharType>(*targetTestPtr)) {
						match = false;
						break;
					}
				}
				if (match) {
					return findIndex;
				}
			}
			return npos;
		}

		constexpr size_type find(const BaseStringView& v, size_type pos = 0) const noexcept {
			return find<CharType>(v, pos);
		}
		constexpr size_type find(const CharType ch, size_type pos = 0) const noexcept {
			return find(BaseStringView(addressof<CharType>(ch), 1), pos);
		}
		constexpr size_type find(const CharType* s, const size_type pos, const size_type count) const noexcept {
			return find(BaseStringView(s, count), pos);
		}
		constexpr size_type find(const CharType* s, const size_type pos = 0) const noexcept {
			return find(BaseStringView(s), pos);
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, CharType> && !std::is_same_v<StringViewLike, BaseStringView<CharType>>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) noexcept {
			return find(BaseStringView(sv.data(), sv.size()), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept {
			return find(static_cast<BaseStringView<CharType>>(sv), pos);
		}

		constexpr size_type find(const BaseStringView<const char>& v, size_type pos = 0) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return find<char>(v, pos);
		}
		constexpr size_type find(char ch, const size_type pos = 0) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return find(BaseStringView<const char>(addressof<char>(ch), 1), pos);
		}
		constexpr size_type find(const char* s, const size_type pos, const size_type count) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return find(BaseStringView<const char>(s, count), pos);
		}
		constexpr size_type find(const char* s, const size_type pos = 0) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return find(BaseStringView<const char>(s), pos);
		}
		template<class StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return find(BaseStringView<const char>(sv.data(), sv.size()), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<const char>> && !IsStringViewLike<StringLike, const char>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return find(static_cast<BaseStringView<const char>>(sv), pos);
		}

		template<class TargetCharType>
		constexpr size_type rfind(const BaseStringView<const TargetCharType>& v, size_type pos = npos) const noexcept {
			if (v.empty()) { return npos; }

			const size_type strSize = size();
			const size_type targetSize = v.size();
			pos = min<size_type>(pos, size() - targetSize);
			if (pos > strSize || strSize - pos < targetSize) { return npos; }

			const CharType* strPtrStart = data();
			const CharType* strPtrEnd = strPtrStart + pos;
			const TargetCharType* targetPtr = v.data();
			const TargetCharType* targetPtrEnd = targetPtr + v.size();

			size_type findIndex = pos;
			for (const CharType* strPtr = strPtrEnd; strPtr >= strPtrStart; --strPtr, --findIndex) {
				Bool match = true;

				const CharType* strTestPtr = strPtr;
				const TargetCharType* targetTestPtr = targetPtr;
				for (; targetTestPtr < targetPtrEnd; ++strTestPtr, ++targetTestPtr) {
					if (*strTestPtr != static_cast<CharType>(*targetTestPtr)) {
						match = false;
						break;
					}
				}
				if (match) {
					return findIndex;
				}
			}
			return npos;
		}

		constexpr size_type rfind(const BaseStringView& v, size_type pos = npos) const noexcept {
			return rfind<CharType>(v, pos);
		};
		constexpr size_type rfind(CharType ch, size_type pos = npos) const noexcept {
			return rfind(BaseStringView(addressof<CharType>(ch), 1), pos);
		}
		constexpr size_type rfind(const CharType* s, size_type pos, size_type count) const {
			return rfind(BaseStringView(s, count), pos);
		}
		constexpr size_type rfind(const CharType* s, const size_type pos = npos) const {
			return rfind(BaseStringView(s), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<const CharType>> && !IsStringViewLike<StringLike, const CharType>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept {
			return rfind(static_cast<BaseStringView<const CharType>>(sv), pos);
		}


		constexpr size_type rfind(const BaseStringView<const char>& v, const size_type pos = npos) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return rfind<char>(v, pos);
		}
		constexpr size_type rfind(char ch, const size_type pos = npos) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(addressof<char>(ch), 1), pos);
		}
		constexpr size_type rfind(const char* s, const size_type pos, const size_type count) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(s, count), pos);
		}
		constexpr size_type rfind(const char* s, const size_type pos = npos) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(s), pos);
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, const char>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) const noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(sv.data(), sv.size()), pos);
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<const char>> && !IsStringViewLike<StringLike, const char>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return rfind(static_cast<BaseStringView<const char>>(sv), pos);
		}

		constexpr BaseStringView& reverse() noexcept requires(IsNotConstV<CharType>) {
			reverse<iterator>(begin(), end());
			return self();
		}

		//compare functions 
		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareEqual(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			if (lhs.size() != rhs.size()) { return false; }
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs.data();
			for (; lhsPtr < lhsEndPtr; ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr != static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return true;
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareEqual(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs;
			for (; lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0); ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr != static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return (lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0)) ||
				(lhsPtr == lhsEndPtr && *rhsPtr == RhsCharType(0));
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareNotEqual(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			return !compareEqual<LhsCharType, RhsCharType>(lhs, rhs);
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareNotEqual(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			return !compareEqual<LhsCharType, RhsCharType>(lhs, rhs);
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareLessThan(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			const Size commonSize = min<Size>(lhs.size(), rhs.size());
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + commonSize;
			const RhsCharType* rhsPtr = rhs.data();
			for (; lhsPtr < lhsEndPtr; ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return lhs.size() < rhs.size();
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareLessThan(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs;
			for (; lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0); ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return lhsPtr == lhsEndPtr && !(*rhsPtr == RhsCharType(0));
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareLessThanEqual(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			const Size commonSize = min<Size>(lhs.size(), rhs.size());
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + commonSize;
			const RhsCharType* rhsPtr = rhs.data();
			for (; lhsPtr < lhsEndPtr; ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return lhs.size() <= rhs.size();
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareLessThanEqual(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs;
			for (; lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0); ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return lhsPtr == lhsEndPtr;
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareGreaterThan(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			const Size commonSize = min<Size>(lhs.size(), rhs.size());
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + commonSize;
			const RhsCharType* rhsPtr = rhs.data();
			for (; lhsPtr < lhsEndPtr; ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return lhs.size() > rhs.size();
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareGreaterThan(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs;
			for (; lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0); ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return !(*rhsPtr == RhsCharType(0));
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareGreaterThanEqual(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			const Size commonSize = min<Size>(lhs.size(), rhs.size());
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + commonSize;
			const RhsCharType* rhsPtr = rhs.data();
			for (; lhsPtr < lhsEndPtr; ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return lhs.size() >= rhs.size();
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static Bool compareGreaterThanEqual(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs;
			for (; lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0); ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr > static_cast<LhsCharType>(*rhsPtr)) { return true; }
				if (*lhsPtr < static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			const Bool lhsAtEnd = lhsPtr == lhsEndPtr;
			const Bool rhsAtEnd = *rhsPtr == RhsCharType(0);
			return !rhsAtEnd || (lhsAtEnd && rhsAtEnd);
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static StrongOrdering compareSpaceship(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			const Size commonSize = min<Size>(lhs.size(), rhs.size());
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + commonSize;
			const RhsCharType* rhsPtr = rhs.data();
			for (; lhsPtr < lhsEndPtr; ++lhsPtr, ++rhsPtr) {
				StrongOrdering cmpResult = (*lhsPtr <=> static_cast<LhsCharType>(*rhsPtr));
				if (cmpResult != 0) { return cmpResult; }
			}
			return lhs.size() <=> rhs.size();
		}

		template<class LhsCharType, class RhsCharType>
			requires(std::is_convertible_v<LhsCharType, RhsCharType>)
		constexpr static StrongOrdering compareSpaceship(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs;
			for (; lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0); ++lhsPtr, ++rhsPtr) {
				StrongOrdering cmpResult = (*lhsPtr <=> static_cast<LhsCharType>(*rhsPtr));
				if (cmpResult != 0) { return cmpResult; }
			}
			const Bool lhsAtEnd = lhsPtr == lhsEndPtr;
			const Bool rhsAtEnd = *rhsPtr == RhsCharType(0);
			const Size lhsReleativeSize = lhsAtEnd ? 1 : 0;
			const Size rhsReleativeSize = rhsAtEnd ? 1 : 0;
			return lhsReleativeSize <=> rhsReleativeSize;
		}

		//compare operators 
		friend constexpr Bool operator==(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConstV<CharType>) {
			return compareEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator==(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator==(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator==(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs == BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs == BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator!=(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConstV<CharType>) {
			return compareNotEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator!=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator!=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator!=(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs != BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs != BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator<(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConstV<CharType>) {
			return compareLessThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator<(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs < BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs < BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator<=(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConstV<CharType>) {
			return compareLessThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator<=(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs <= BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs <= BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator>(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConstV<CharType>) {
			return compareGreaterThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator>(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs > BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs > BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator>=(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConstV<CharType>) {
			return compareGreaterThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator>=(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs >= BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs >= BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConstV<CharType>) {
			return compareSpaceship<CharType, CharType>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<class StringLike>
			requires(std::is_convertible_v<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const char* rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const char rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<class StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator<=>(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs <=> BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator<=>(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(std::is_same_v<std::decay_t<CharType>, Utf32>) {
			return lhs <=> BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		//hash
		constexpr size_type hash() const noexcept {
			return fnv1aHash(data(), size());
		}
		constexpr static size_type staticHash(const BaseStringView& stringView) noexcept {
			return fnv1aHash(stringView.data(), stringView.size());
		}

		//stream
		friend std::ostream& operator<<(std::ostream& os, const BaseStringView& str) {
			os << std::string_view(str.data(), static_cast<std::size_t>(str.size()));
			return os;
		}
	};

	using StringView = BaseStringView<const char>;
	using ConstStringView = BaseStringView<const char>;
	using AsciiStringView = StringView;
	using ConstAsciiStringView = ConstStringView;
	using Utf32StringView = BaseStringView<char32_t>;
	using ConstUtf32StringView = BaseStringView<const char32_t>;

	namespace literals {
		constexpr ConstStringView operator ""_natl_sv(const char* str, std::size_t len) noexcept {
			return ConstStringView{ str, static_cast<Size>(len) };
		}
		constexpr ConstAsciiStringView operator ""_natl_assci_sv(const char* str, std::size_t len) noexcept {
			return ConstAsciiStringView{ str, static_cast<Size>(len) };
		}
		constexpr ConstUtf32StringView operator ""_natl_utf32_sv(const char32_t* str, std::size_t len) noexcept {
			return ConstUtf32StringView{ str, static_cast<Size>(len) };
		}
	}
}