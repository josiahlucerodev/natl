#pragma once 

//std
#include <cstdint>
#include <string>

//own
#include "iterators.h"

//interface
namespace natl {
	class StringView {
	public:
		using value_type = char;
		using reference = value_type&;
		using const_reference = const value_type&;
		using optional_reference = std::optional<std::reference_wrapper<value_type>>;
		using optional_const_reference = std::optional<std::reference_wrapper<const value_type>>;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using optional_pointer = std::optional<value_type*>;
		using optional_const_pointer = std::optional<const value_type*>;
		using difference_value_type = std::ptrdiff_t;
		using size_value_type = std::size_t;

		using iterator = RandomAccessIterator<value_type>;
		using const_iterator = RandomAccessIterator<const value_type>;
		using reverse_iterator = ReverseRandomAccessIterator<value_type>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const value_type>;
	private:
		std::size_t stringLength;
		const char* dataPtr;
	public:
		StringView() = default;
		~StringView() = default;
		constexpr StringView(const char string[]) : dataPtr(string), stringLength(0) {
			const char* tempDataPtr = string;
			while (*tempDataPtr++) { stringLength++; }
		}
		constexpr StringView(const char* stringPtr, const std::size_t length)
			: dataPtr(stringPtr), stringLength(length) {}
		constexpr StringView(const std::string& string)
			: dataPtr(string.c_str()), stringLength(string.length()) {}
		constexpr StringView(std::string& string)
			: dataPtr(string.c_str()), stringLength(string.length()) {}
	public:
		constexpr const char* c_str() const noexcept { return dataPtr; }
		constexpr const char* cString() const noexcept { return dataPtr; }
		constexpr std::string toString() const noexcept { return std::string(dataPtr, size()); }
		constexpr std::size_t length() const noexcept { return stringLength; }
		constexpr std::size_t size() const noexcept { return stringLength; }
		constexpr std::size_t hash() const noexcept {
			std::size_t c = 0;
			std::size_t hashValue = 5381;
			const char* dataPtr = dataPtr;
			while ((c = *dataPtr++))
				hashValue = ((hashValue << 5) + hashValue) + c;
			return hashValue;
		}
		constexpr bool operator==(const StringView& other) const noexcept {
			if (length() != other.length()) {
				return false;
			}

			for (std::size_t i = 0; i < length(); i++) {
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
	};
}