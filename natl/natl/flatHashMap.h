#pragma once 

//std
#include <cstdint>

//own
#include "hash.h"
#include "iterators.h"
#include "allocator.h"
#include "dynamicArray.h"
#include "option.h"

//interface
namespace natl {
	template<class Entry>
	class FlatHashMapIterator {
	public:
		using iterator = FlatHashMapIterator<Entry>;
		using difference_type = std::ptrdiff_t;
		using value_type = Entry;
		using reference = Entry&;
		using pointer = Entry*;
		using iterator_category = std::random_access_iterator_tag;
		using size_type = std::size_t;
	private:
		pointer dataPtr;
		pointer beginPtr;
		pointer endPtr;
	public:
		constexpr FlatHashMapIterator() : dataPtr(nullptr), beginPtr(nullptr), endPtr(nullptr) {};
		constexpr FlatHashMapIterator(pointer const dataPtr, pointer const beginPtr, pointer const endPtr) noexcept
			: dataPtr(dataPtr), beginPtr(beginPtr), endPtr(endPtr) {}
	private:
		constexpr iterator& getSelf() noexcept { return *this; }
		constexpr const iterator& getSelf() const noexcept { return *this; }
	public:
		constexpr reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() const noexcept { return dataPtr; }


		constexpr iterator& operator++() noexcept { 
			while (dataPtr != endPtr) {
				dataPtr++;
				if (dataPtr->used) { return getSelf(); }
			}
			dataPtr++; return getSelf(); 
		}
		constexpr iterator operator++(int) noexcept { iterator tempIt = getSelf(); ++getSelf().dataPtr; return tempIt; }

		constexpr iterator& operator--() noexcept { 
			while (dataPtr > beginPtr) {
				dataPtr--;
				if (dataPtr->used) { return getSelf(); }
			}
			return getSelf(); 
		}

		constexpr iterator operator--(int) noexcept { iterator tempIt = getSelf(); --getSelf().dataPtr; return tempIt; }


		constexpr bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
		constexpr bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
		constexpr bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
		constexpr bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }
	};

	template<class DataType>
	class FlatMapHashCompare {
	public:
		constexpr static bool compare(const DataType& lhs, const DataType& rhs) {
			return lhs == rhs;
		}
	};

	template<class Key, class DataType>
	class FlatHashMapEntry {
	public:
		Key key;
		DataType data;
	private:
		bool used;
	public:
		constexpr FlatHashMapEntry() : key(), data(), used(false) {};
		constexpr FlatHashMapEntry(const Key& key, const DataType& data, const bool used)
			: key(key), data(data), used(used) {}
		constexpr ~FlatHashMapEntry() {}

		constexpr FlatHashMapEntry(const FlatHashMapEntry& src) {
			key = src.key;
			data = src.data;
			used = src.used;
		}

		constexpr FlatHashMapEntry(FlatHashMapEntry&&)
			requires(std::is_trivially_constructible_v<Key>&& std::is_trivially_constructible_v<DataType>) = default;
		constexpr FlatHashMapEntry(FlatHashMapEntry&& src) {
			key = std::move(src.key);
			data = std::move(src.data);
			used = src.used;
		}

		constexpr FlatHashMapEntry& operator=(FlatHashMapEntry&& src) noexcept {
			key = std::move(src.key);
			data = std::move(src.data);
			used = src.used;
			return *this;
		}

		constexpr FlatHashMapEntry& operator=(const FlatHashMapEntry& src) noexcept {
			key = src.key;
			data = src.data;
			used = src.used;
			return *this;
		}


		constexpr void setAsUsed() { used = true; }
		constexpr void setAsNotUsed() { used = false; }
		constexpr bool isUsed() const { return used; }
	};

	template<class Key, class DataType,
		class Hash = Hash<Key>,
		class Compare = FlatMapHashCompare<Key>,
		class Alloc = DefaultAllocator<FlatHashMapEntry<Key, DataType>>>
	class FlatHashMap {
		using Entry = FlatHashMapEntry<Key, DataType>;
	public:
		using key_type = Key;
		using map_type = DataType;
		using value_type = Entry&;
		using reference = Entry&;
		using const_reference = const Entry&;
		using pointer = Entry*;
		using const_pointer = const Entry*;
		using optional_pointer = Option<Entry*>;
		using optional_const_pointer = Option<const Entry*>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;

		using iterator = FlatHashMapIterator<Entry>;
		using const_iterator = FlatHashMapIterator<const Entry>;
		using reverse_iterator = std::reverse_iterator<FlatHashMapIterator<Entry>>;
		using const_reverse_iterator = std::reverse_iterator<FlatHashMapIterator<const Entry>>;

		static constexpr double load_factor = 0.7;
	private:
		size_type inSize;
		DynamicArray<Entry, Alloc> table;
	public:
		constexpr FlatHashMap() : inSize(0), table() {}
		constexpr FlatHashMap(const FlatHashMap& src) : inSize{}, table{} {
			inSize = src.inSize;
			table = src.table;
		}
		constexpr FlatHashMap(FlatHashMap&& src) : inSize{}, table{} {
			inSize = src.inSize;
			table = std::move(src.table);
		}

		constexpr ~FlatHashMap() = default;

		constexpr size_type size() const noexcept { return inSize; }
		constexpr size_type count() const noexcept { return inSize; }
		constexpr size_type capacity() const noexcept { return table.capacity(); }

		constexpr bool isEmpty() const noexcept { return !bool(size()); }
		constexpr bool isNotEmpty() const noexcept { return bool(size()); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

	private:
		constexpr pointer beginPtr() noexcept requires(isNotConst<DataType>) { return table.data(); }
		constexpr const_pointer beginPtr() const noexcept { return table.data(); }
		constexpr pointer endPtr() noexcept requires(isNotConst<DataType>) { return table.data() + table.size(); }
		constexpr const_pointer endPtr() const noexcept { return table.data() + table.size(); }

	public:
		constexpr iterator begin() noexcept requires(isNotConst<DataType>) { return iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr iterator end() noexcept requires(isNotConst<DataType>) { return iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(isNotConst<DataType>) { return reverse_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(isNotConst<DataType>) { return reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }

	public:
		constexpr iterator insert(const Key& key, const DataType& data) {
			resizeAndRehash();

			return end();

			pointer dataLocation = nullptr;
			std::size_t index = Hash::hash(key) % capacity();
			while (table[index].isUsed()) {
				if (table[index].key == key) {
					dataLocation = &table[index];
					dataLocation->data = data;
					dataLocation->key = key;
					return iterator(dataLocation, beginPtr(), endPtr());
				}
				index = (index + 1) % capacity();
			}

			dataLocation = &table[index];
			*dataLocation = Entry(key, data, true);
			++inSize;

			return iterator(dataLocation, beginPtr(), endPtr());
		};

		constexpr iterator insert(Key&& key, DataType&& data) {
			resizeAndRehash();

			pointer dataLocation = nullptr;
			std::size_t index = Hash::hash(key) % capacity();
			while (table[index].isUsed()) {
				if (Compare::compare(table[index].key, key)) {
					dataLocation = &table[index];
					dataLocation->data = data;
					dataLocation->key = key;

					return iterator(dataLocation, beginPtr(), endPtr());
				}
				index = (index + 1) % capacity();
			}

			dataLocation = &table[index];
			*dataLocation = Entry(key, data, true);
			++inSize;

			return iterator(dataLocation, beginPtr(), endPtr());
		};

		constexpr iterator findIterator(const Key& key) {
			size_type index = Hash::hash(key) % capacity();
			size_type originalIndex = index;

			while (true) {
				if (Compare::compare(table[index].key, key)) {
					return iterator(&table[index].data, beginPtr(), endPtr());
				}

				index = (index + 1) % capacity();
				if (index == originalIndex) {
					break;
				}
			}
			return end();
		}

		constexpr const_iterator findIterator(const Key& key) const {
			size_type index = Hash::hash(key) % capacity();
			size_type originalIndex = index;

			while (true) {
				if (Compare::compare(table[index].key, key)) {
					return const_iterator(&table[index].data, beginPtr(), endPtr());
				}

				index = (index + 1) % capacity();
				if (index == originalIndex) {
					break;
				}
			}
			return end();
		}

		constexpr optional_pointer find(const Key& key) {
			size_type index = Hash::hash(key) % capacity();
			size_type originalIndex = index;

			while (true) {
				if (Compare::compare(table[index].key, key)) {
					return optional_pointer(&table[index]);
				}

				index = (index + 1) % capacity();
				if (index == originalIndex) {
					break; 
				}
			}
			return optional_pointer(natl::OptionEmpty()); 
		}

		constexpr optional_const_pointer find(const Key& key) const {
			size_type index = Hash::hash(key) % capacity();
			size_type originalIndex = index;

			while (true) {
				if (Compare::compare(table[index].key, key)) {
					return optional_const_pointer(&table[index]);
				}

				index = (index + 1) % capacity();
				if (index == originalIndex) {
					break;
				}
			}
			return optional_const_pointer();
		}

		bool contains(const Key& key) const {
			return find(key).hasValue();
		}
		constexpr void resizeAndRehash() {
			if (inSize >= capacity() * load_factor) {
				resizeAndRehash((capacity() + 10) * 2);
			}
		}
		constexpr void resizeAndRehash(const size_type newCapacity) {
			if (newCapacity <= capacity()) { return; }

			DynamicArray<Entry, Alloc> newTable(newCapacity);

			for (const Entry& entry : table) {
				if (entry.isUsed()) {
					std::size_t index = Hash::hash(entry.key) % newCapacity;

					while (newTable[index].isUsed()) {
						index = (index + 1) % newCapacity;
					}

					pointer dataLocation = &newTable[index];
					*dataLocation = entry;
				}
			}
			table = static_cast<DynamicArray<Entry, Alloc>&&>(newTable);
		}

		constexpr void clear() {
			table.clear();
			inSize = 0;
		}

		constexpr void reserve(const size_type& newCapacity) {
			resizeAndRehash(newCapacity);
		}

		constexpr FlatHashMap& operator=(FlatHashMap& src) {
			table = src.table;
			inSize = src.inSize;

			return *this;
		}

		constexpr FlatHashMap& operator=(FlatHashMap&& src) {
			table = std::move(src.table);
			inSize = src.inSize;

			return *this;
		}
	};
}

//additional includes for end use
#include "commonHashs.h"