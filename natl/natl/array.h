#pragma once 

//std
#include <initializer_list>

//own
#include "typeTraits.h"
#include "iterators.h"
#include "container.h"
#include "option.h"
#include "dataMovement.h"

//interface
namespace natl {
	template<class DataType, std::size_t number>
	class Array {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;

		using iterator = RandomAccessIterator<DataType>;
		using const_iterator = RandomAccessIterator<const DataType>;
		using reverse_iterator = ReverseRandomAccessIterator<DataType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const DataType>;
	private:
		DataType dataStorage[number];
	public:
		constexpr Array() = default;
		constexpr Array(std::initializer_list<DataType> dataSrc) { 
			uninitializedCopyCountNoOverlap<const_pointer, pointer>(dataSrc.begin(), dataStorage, std::min<std::size_t>(dataSrc.size(), size()));
		};
		constexpr ~Array() = default;

		constexpr size_type size() const noexcept { return number; }
		constexpr pointer data() noexcept requires(isNotConst<DataType>) { return dataStorage; }
		constexpr const_pointer data() const noexcept { return dataStorage; };

		constexpr pointer beginPtr() noexcept requires(isNotConst<DataType>) { return dataStorage; }
		constexpr const_pointer beginPtr() const noexcept { return dataStorage; }
		constexpr pointer endPtr() noexcept requires(isNotConst<DataType>) { return dataStorage + number; }
		constexpr const_pointer endPtr() const noexcept { return dataStorage + number; }

		constexpr iterator begin() noexcept requires(isNotConst<DataType>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(isNotConst<DataType>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(isNotConst<DataType>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(isNotConst<DataType>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		constexpr bool isEmpty() const noexcept { return !bool(number); }
		constexpr bool isNotEmpty() const noexcept { return bool(number); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr reference at(const size_type index) noexcept requires(isNotConst<DataType>) { return dataStorage[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return dataStorage[index]; };

		constexpr size_type clamp(const size_type value, const size_type min, const size_type max) const noexcept {
			const size_type t = value < min ? min : value;
			return t > max ? max : t;
		}

		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp(index, 0, number - 1); }

		constexpr reference atClamped(const size_type index) noexcept requires(isNotConst<DataType>) { return dataStorage[clampIndex(index)]; }
		constexpr const_pointer atClamped(const size_type index) const noexcept { return dataStorage[clampIndex(index)]; }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return number ? number - 1 : 0; }

		constexpr reference front() noexcept requires(isNotConst<DataType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr bool has(const size_type index) const noexcept { return index < number; }
		constexpr bool notHave(const size_type index) const noexcept { return index >= number; }

		constexpr optional_pointer optionalAt(const size_type index) noexcept {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(at(index));
		};
		constexpr optional_const_pointer optionalAt(const size_type index) const noexcept {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(at(index));
		};

		constexpr optional_pointer optionalFront() noexcept { return optionalAt(frontIndex()); }
		constexpr optional_const_pointer optionalFront() const noexcept { return optionalAt(frontIndex()); }
		constexpr optional_pointer optionalBack() noexcept { return optionalAt(backIndex()); }
		constexpr optional_const_pointer optionalBack() const noexcept { return optionalAt(backIndex()); }

		constexpr void fill(const DataType& value) noexcept {
			if (isEmpty()) { return; }
			fillCount<pointer, DataType>(data(), value, size());
		}

		constexpr Array& operator=(std::initializer_list<DataType> dataSrc) noexcept {
			copyCountNoOverlap<const_pointer, pointer>(dataSrc.begin(), dataStorage, std::min<std::size_t>(dataSrc.size(), size()));
			return *this;
		}
	};
}