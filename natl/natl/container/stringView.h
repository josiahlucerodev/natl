#pragma once 

//@begin_non_modules
//own
#include <initializer_list>
//@end_non_modules

//own
#include "../util/stringOperations.h"
#include "../util/iterators.h"
#include "../util/algorithm.h"
#include "../util/dataMovement.h"
#include "../util/compare.h"
#include "../util/option.h"

//@export
namespace natl {
	template<typename CharType>
	struct BaseStringView {
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

		using iterator = ContiguousIterator<value_type>;
		using const_iterator = ConstContiguousIterator<value_type>;
		using reverse_iterator = ReverseContiguousIterator<value_type>;
		using const_reverse_iterator = ReverseConstContiguousIterator<const value_type>;

		static constexpr size_type npos = size_type(-1);

		using ConstBaseStringView = BaseStringView<const CharType>;
	public:
		CharType* _dataPtr;
		size_type _stringLength;
	public:
		//constructors
		constexpr BaseStringView() noexcept : _dataPtr(nullptr), _stringLength(0) {}
		constexpr BaseStringView(const BaseStringView& str) noexcept = default;
		constexpr BaseStringView(CharType* stringPtr, const size_type length) noexcept : _dataPtr(stringPtr), _stringLength(length) {}
		constexpr BaseStringView(CharType* str) noexcept : _dataPtr(str), _stringLength(cstringLength(str)) {}
		//template<Size StrArraySize> 
		//constexpr BaseStringView(const CharType(&str)[StrArraySize]) noexcept : _dataPtr(str), _stringLength(cstringLength(str)) {}


		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, RemoveConctVolatile<CharType>>)
		explicit constexpr BaseStringView(const StringViewLike& stringViewLike) noexcept : 
			_dataPtr(stringViewLike.data()), _stringLength(stringViewLike.size()) {}

		//destrutor
		constexpr ~BaseStringView() = default;

		//convert 
		constexpr operator BaseStringView<const CharType>() const requires(IsNotConst<CharType>) {
			return BaseStringView<const CharType>(data(), size());
		}

		//util
		constexpr BaseStringView& self() noexcept { return *this; }
		constexpr const BaseStringView& self() const noexcept { return *this; }

		//iterators
		constexpr pointer beginPtr() requires(IsNotConst<CharType>) { return _dataPtr; }
		constexpr const_pointer beginPtr() const noexcept { return _dataPtr; }
		constexpr pointer endPtr() requires(IsNotConst<CharType>) { return _dataPtr + _stringLength; }
		constexpr const_pointer endPtr() const noexcept { return _dataPtr + _stringLength; }

		constexpr iterator begin() noexcept requires(IsNotConst<CharType>) { return iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr iterator end() noexcept requires(IsNotConst<CharType>) { return iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }

		constexpr reverse_iterator rbegin() noexcept requires(IsNotConst<CharType>) { return reverse_iterator(end()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
		constexpr reverse_iterator rend() noexcept requires(IsNotConst<CharType>) { return reverse_iterator(begin()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

		//element access
		constexpr reference operator[] (const size_type index) requires(IsNotConst<CharType>) { return at(index); }
		constexpr const_reference operator[] (const size_type index) const { return at(index); }

		constexpr reference at(const size_type index) noexcept requires(IsNotConst<CharType>) { return _dataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return _dataPtr[index]; };

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() ? size() - 1 : 0; }

		constexpr reference front() noexcept requires(IsNotConst<CharType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }
		constexpr reference back() noexcept requires(IsNotConst<CharType>) { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr CharType* data() noexcept requires(IsNotConst<CharType>) { return _dataPtr; }
		constexpr const CharType* data() const noexcept { return _dataPtr; }

		constexpr CharType* c_str() noexcept requires(IsNotConst<CharType>) { return _dataPtr; }
		constexpr const CharType* c_str() const noexcept { return _dataPtr; }

		//capacity 
		constexpr size_type size() const noexcept { return _stringLength; }
		constexpr size_type length() const noexcept { return _stringLength; }
		constexpr size_type max_size() const noexcept { return ~size_type(1); };
		constexpr Bool empty() const noexcept { return !_dataPtr || _stringLength == 0; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }

		//modifiers 
		constexpr void swap(BaseStringView& other) noexcept {
			natl::swap<BaseStringView>(self(), other);
		}
		constexpr BaseStringView removePrefix(const size_type n) noexcept {
			const size_type removeCount = min<size_type>(n, size());
			_dataPtr += removeCount;
			_stringLength -= removeCount;
			return self();
		}
		constexpr BaseStringView removeSuffix(const size_type n) noexcept {
			const size_type removeCount = min<size_type>(n, size());
			_stringLength -= removeCount;
			return self();
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

		constexpr BaseStringView substr(const size_type pos = 0, const size_type count = npos) noexcept requires(IsNotConst<CharType>) {
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

		constexpr Bool startsWith(const BaseStringView& sv) const noexcept {
			return BaseStringView(data(), min<size_type>(size(), sv.size())) == sv;
		}
		constexpr Bool startsWith(const CharType ch) const noexcept {
			return !empty() && front() == ch;
		}
		constexpr Bool startsWith(const CharType* s) const noexcept {
			return startsWith(BaseStringView(s));
		}

		constexpr Bool endsWith(const BaseStringView& sv) const noexcept {
			return size() >= sv.size() && compare(size() - sv.size(), npos, sv) == 0;
		}
		constexpr Bool endsWith(const CharType ch) const noexcept {
			return !empty() && back() == ch;
		}
		constexpr Bool endsWith(const CharType* s) const noexcept {
			return endsWith(BaseStringView(s));
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

		template<typename TargetCharType>
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
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, CharType> && !IsSameC<StringViewLike, BaseStringView<CharType>>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) noexcept {
			return find(BaseStringView(sv.data(), sv.size()), pos);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept {
			return find(static_cast<BaseStringView<CharType>>(sv), pos);
		}

		constexpr size_type find(const BaseStringView<const char>& v, size_type pos = 0) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return find<char>(v, pos);
		}
		constexpr size_type find(char ch, const size_type pos = 0) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return find(BaseStringView<const char>(addressof<char>(ch), 1), pos);
		}
		constexpr size_type find(const char* s, const size_type pos, const size_type count) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return find(BaseStringView<const char>(s, count), pos);
		}
		constexpr size_type find(const char* s, const size_type pos = 0) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return find(BaseStringView<const char>(s), pos);
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, char>)
		constexpr size_type find(const StringViewLike& sv, const size_type pos = 0) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return find(BaseStringView<const char>(sv.data(), sv.size()), pos);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<const char>> && !IsStringViewLike<StringLike, const char>)
		constexpr size_type find(const StringLike& sv, const size_type pos = 0) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return find(static_cast<BaseStringView<const char>>(sv), pos);
		}

		template<typename TargetCharType>
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
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<const CharType>> && !IsStringViewLike<StringLike, const CharType>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept {
			return rfind(static_cast<BaseStringView<const CharType>>(sv), pos);
		}


		constexpr size_type rfind(const BaseStringView<const char>& v, const size_type pos = npos) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return rfind<char>(v, pos);
		}
		constexpr size_type rfind(char ch, const size_type pos = npos) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(addressof<char>(ch), 1), pos);
		}
		constexpr size_type rfind(const char* s, const size_type pos, const size_type count) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(s, count), pos);
		}
		constexpr size_type rfind(const char* s, const size_type pos = npos) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(s), pos);
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, const char>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) const noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return rfind(BaseStringView<const char>(sv.data(), sv.size()), pos);
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<const char>> && !IsStringViewLike<StringLike, const char>)
		constexpr size_type rfind(const StringLike& sv, const size_type pos = npos) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return rfind(static_cast<BaseStringView<const char>>(sv), pos);
		}

		constexpr BaseStringView& reverse() noexcept requires(IsNotConst<CharType>) {
			natl::reverse<iterator>(begin(), end());
			return self();
		}

		//compare functions 
		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
		constexpr static Bool compareEqual(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			const LhsCharType* lhsPtr = lhs.data();
			const LhsCharType* lhsEndPtr = lhsPtr + lhs.size();
			const RhsCharType* rhsPtr = rhs;
			for (; lhsPtr < lhsEndPtr && *rhsPtr != RhsCharType(0); ++lhsPtr, ++rhsPtr) {
				if (*lhsPtr != static_cast<LhsCharType>(*rhsPtr)) { return false; }
			}
			return (lhsPtr == lhsEndPtr && *rhsPtr == RhsCharType(0));
		}

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
		constexpr static Bool compareNotEqual(const BaseStringView<const LhsCharType>& lhs, const BaseStringView<const RhsCharType>& rhs) noexcept {
			return !compareEqual<LhsCharType, RhsCharType>(lhs, rhs);
		}

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
		constexpr static Bool compareNotEqual(const BaseStringView<LhsCharType>& lhs, const RhsCharType* rhs) noexcept {
			return !compareEqual<LhsCharType, RhsCharType>(lhs, rhs);
		}

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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

		template<typename LhsCharType, typename RhsCharType>
			requires(IsConvertibleC<LhsCharType, RhsCharType>)
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
		friend constexpr Bool operator==(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConst<CharType>) {
			return compareEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator==(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator==(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const char* rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, const char rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator==(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs == BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator==(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return lhs == BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator!=(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConst<CharType>) {
			return compareNotEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator!=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator!=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareNotEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const char* rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, const char rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator!=(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareNotEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs != BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator!=(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return lhs != BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator<(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConst<CharType>) {
			return compareLessThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThan<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const char* rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, const char rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator<(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThan<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs < BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator<(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return lhs < BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator<=(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConst<CharType>) {
			return compareLessThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator<=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareLessThanEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const char* rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, const char rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator<=(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareLessThanEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs <= BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator<=(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return lhs <= BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator>(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConst<CharType>) {
			return compareGreaterThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThan<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const char* rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, const char rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator>(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThan<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs > BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator>(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return lhs > BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator>=(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConst<CharType>) {
			return compareGreaterThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr Bool operator>=(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareGreaterThanEqual<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const char* rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, rhs);
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, const char rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr Bool operator>=(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareGreaterThanEqual<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs >= BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator>=(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return lhs >= BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const BaseStringView& rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const BaseStringView<const CharType>& rhs) noexcept requires(IsNotConst<CharType>) {
			return compareSpaceship<CharType, CharType>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const CharType* rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const CharType rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, BaseStringView(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, CharType>)
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, BaseStringView(rhs.data(), rhs.size()));
		}
		template<typename StringLike>
			requires(IsConvertibleC<StringLike, BaseStringView<CharType>> && !IsStringViewLike<StringLike, CharType>)
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const StringLike& rhs) noexcept {
			return compareSpaceship<CharType, CharType>(lhs, static_cast<BaseStringView<CharType>>(rhs));
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const BaseStringView<const char>& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const char* rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, rhs);
		}
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const char rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, BaseStringView<const char>(&rhs, 1));
		}
		template<typename StringLike>
			requires(IsStringViewLike<StringLike, char>)
		friend constexpr StrongOrdering operator<=>(const BaseStringView& lhs, const StringLike& rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return compareSpaceship<CharType, char>(lhs, BaseStringView<const char>(rhs.data(), rhs.size()));
		}
		friend constexpr Bool operator<=>(const BaseStringView& lhs, std::initializer_list<CharType> rhs) noexcept {
			return lhs <=> BaseStringView<const CharType>(rhs.begin(), rhs.size());
		}
		friend constexpr Bool operator<=>(const BaseStringView& lhs, std::initializer_list<char> rhs) noexcept requires(IsSameC<Decay<CharType>, Utf32>) {
			return lhs <=> BaseStringView<const char>(rhs.begin(), rhs.size());
		}

		//hash
		constexpr size_type hash() const noexcept {
			return fnv1aHash(data(), size());
		}
		constexpr static size_type staticHash(const BaseStringView& stringView) noexcept {
			return fnv1aHash(stringView.data(), stringView.size());
		}
	};

	template<typename CharType>
	struct IsTriviallyCompareableV<BaseStringView<CharType>>
		: FalseType {};

	template<typename CharType>
	struct IsTriviallyRelocatableV<BaseStringView<CharType>>
		: TrueType {};
	template<typename CharType>
	struct IsTriviallyConstructibleV<BaseStringView<CharType>>
		: TrueType {};
	template<typename CharType>
	struct IsTriviallyDestructibleV<BaseStringView<CharType>>
		: TrueType {};

	template<typename CharType>
	struct IsTriviallyConstRefConstructibleV<BaseStringView<CharType>>
		: TrueType {};
	template<typename CharType>
	struct IsTriviallyMoveConstructibleV<BaseStringView<CharType>>
		: TrueType {};

	template<typename CharType>
	struct IsTriviallyConstRefAssignableV<BaseStringView<CharType>>
		: TrueType {};
	template<typename CharType>
	struct IsTriviallyMoveAssignableV<BaseStringView<CharType>>
		: TrueType {};

	using StringView = BaseStringView<Ascii>;
	using ConstStringView = BaseStringView<const Ascii>;
	using AsciiStringView = StringView;
	using ConstAsciiStringView = ConstStringView;
	using Utf32StringView = BaseStringView<Utf32>;
	using ConstUtf32StringView = BaseStringView<const Utf32>;

	//@export
	namespace literals {
		constexpr ConstStringView operator ""_natl_sv(const char* str, StdSize len) noexcept {
			return ConstStringView{ str, static_cast<Size>(len) };
		}
		constexpr ConstAsciiStringView operator ""_natl_assci_sv(const char* str, StdSize len) noexcept {
			return ConstAsciiStringView{ str, static_cast<Size>(len) };
		}
		constexpr ConstUtf32StringView operator ""_natl_utf32_sv(const char32_t* str, StdSize len) noexcept {
			return ConstUtf32StringView{ str, static_cast<Size>(len) };
		}
	}

	template<typename DataType>
	struct IsCopyableStorageDstT<BaseStringView<DataType>> : TrueType {};

	struct DummyString {
	public:
		constexpr DummyString() noexcept;
		constexpr ~DummyString() noexcept;
		constexpr Size size() noexcept;
		constexpr void reserve(Size) noexcept;
		constexpr void resize(Size) noexcept;
		constexpr void pushBack(Ascii) noexcept;
		constexpr Ascii operator[](Size) noexcept;
		constexpr DummyString& operator+=(DummyString) noexcept;
		constexpr DummyString operator+(DummyString) noexcept;
		constexpr DummyString& operator+=(Ascii) noexcept;
		constexpr DummyString operator+(Ascii) noexcept;
		constexpr DummyString operator+=(ConstAsciiStringView) noexcept;
		constexpr DummyString operator+(ConstAsciiStringView) noexcept;
	};

	template<typename StringLike, typename CharType = Ascii>
	concept IsDynSizedStringC = requires(StringLike string) {
		{ string.size() } -> IsConvertibleC<Size>;
		{ string.reserve(declval<Size>()) };
		{ string.resize(declval<Size>()) };
		{ string.pushBack(declval<Size>()) };
		{ string[0] } -> IsConvertibleC<CharType>;
		{ string += declval<CharType>() };
		{ string + declval<CharType>() };
		{ string += string };
		{ string + string };
		{ string += declval<ConstAsciiStringView>() };
		{ string + declval<ConstAsciiStringView>() };
	};

	template<typename StringLike, typename CharType = Ascii>
	constexpr inline Bool IsDynSizedString = IsDynSizedStringC<StringLike, CharType>
	template<typename StringLike, typename CharType = Ascii>
	struct IsDynSizedStringV = BoolConstant<IsDynSizedStringC<StringLike, CharType>> {};

	static_assert(IsDynSizedStringC<DummyString>);
}