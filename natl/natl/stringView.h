#pragma once 

//std
#include <string>

//own
#include "iterators.h"
#include "option.h"
#include "string.h"

//interface
namespace natl {

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
		using difference_value_type = std::ptrdiff_t;
		using size_type = std::size_t;

		using iterator = RandomAccessIterator<value_type>;
		using const_iterator = RandomAccessIterator<const value_type>;
		using reverse_iterator = ReverseRandomAccessIterator<value_type>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const value_type>;
	private:
		size_type stringLength;
		const char* dataPtr;
	public:
		constexpr BaseStringView() : stringLength(0), dataPtr(nullptr) {}
		constexpr ~BaseStringView() = default;
		constexpr BaseStringView(const char string[]) : dataPtr(string), stringLength(0) {
			const char* tempDataPtr = string;
			while (*tempDataPtr++) { stringLength++; }
		}
		constexpr BaseStringView(const char* stringPtr, const size_type length)
			: dataPtr(stringPtr), stringLength(length) {}
		constexpr BaseStringView(const String& string)
			: dataPtr(string.c_str()), stringLength(string.length()) {}
	public:
		constexpr const char* c_str() const noexcept { return dataPtr; }
		constexpr const char* cStr() const noexcept { return dataPtr; }
		constexpr const char* cString() const noexcept { return dataPtr; }
		constexpr String toString() const noexcept { return String(dataPtr, size()); }
		constexpr size_type length() const noexcept { return stringLength; }
		constexpr size_type size() const noexcept { return stringLength; }
		constexpr size_type hash() const noexcept {
			size_type c = 0;
			size_type hashValue = 5381;
			const char* dataPtr = dataPtr;
			while ((c = *dataPtr++))
				hashValue = ((hashValue << 5) + hashValue) + c;
			return hashValue;
		}
		constexpr static size_type staticHash(const BaseStringView& stringView) noexcept {
			size_type c = 0;
			size_type hashValue = 5381;
			const char* dataPtr = stringView.dataPtr;
			while ((c = *dataPtr++))
				hashValue = ((hashValue << 5) + hashValue) + c;
			return hashValue;
		}

		constexpr bool operator==(const BaseStringView& other) const noexcept {
			if (length() != other.length()) {
				return false;
			}

			for (size_type i = 0; i < length(); i++) {
				if (dataPtr[i] != other.dataPtr[i]) {
					return false;
				}
			}

			return true;
		}
	public:
		constexpr const_pointer beginPtr() const noexcept { return dataPtr; }
		constexpr const_pointer endPtr() const noexcept { return dataPtr + stringLength; }

		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		constexpr reference at(const size_type index) noexcept requires(isNotConst<CharType>) { return dataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return dataPtr[index]; };
		constexpr reference operator[] (const size_type index) { return at(index); }
		constexpr const_reference operator[] (const size_type index) const { return at(index); }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() ? size() - 1 : 0; }
		constexpr reference front() noexcept requires(isNotConst<CharType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }
		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr String toString() { return String(cStr(), size()); }
	};

	using StringView = BaseStringView<char>;
}