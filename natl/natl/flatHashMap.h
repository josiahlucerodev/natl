#pragma once 

//std
#include <cstdint>

//own
#include "hash.h"
#include "iterators.h"
#include "allocator.h"
#include "dynArray.h"
#include "smallDynArray.h"
#include "option.h"
#include "dataMovement.h"

//interface
namespace natl {
	template<class Entry, class Alloc>
	class FlatHashMapIterator {
	public:
		using iterator = FlatHashMapIterator<Entry, Alloc>;

		using allocator_type = Alloc;
		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;
	private:
		pointer dataPtr;
		pointer beginPtr;
		pointer endPtr;
	public:
		constexpr FlatHashMapIterator() : dataPtr(nullptr), beginPtr(nullptr), endPtr(nullptr) {};
		constexpr FlatHashMapIterator(pointer dataPtrIn, pointer beginPtrIn, pointer endPtrIn) noexcept
			: dataPtr(dataPtrIn), beginPtr(beginPtrIn), endPtr(endPtrIn) {}
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


		constexpr Bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
		constexpr Bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
		constexpr Bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
		constexpr Bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
		constexpr Bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
		constexpr Bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }
	};

	template<class DataType>
	class FlatMapHashCompare {
	public:
		constexpr static Bool compare(const DataType& lhs, const DataType& rhs) {
			return lhs == rhs;
		}
	};

	template<class Key, class DataType>
	class FlatHashMapEntry {
	public:
		Key key;
		DataType data;
	private:
		Bool used;
	public:
		constexpr FlatHashMapEntry() : key(), data(), used(false) {};
		constexpr FlatHashMapEntry(const Key& keyIn, const DataType& dataIn, const Bool usedIn)
			: key(keyIn), data(dataIn), used(usedIn) {}
		constexpr ~FlatHashMapEntry() {}

		constexpr FlatHashMapEntry(const FlatHashMapEntry& src) {
			key = src.key;
			data = src.data;
			used = src.used;
		}

		constexpr FlatHashMapEntry(FlatHashMapEntry&&)
			requires(std::is_trivially_constructible_v<Key>&& std::is_trivially_constructible_v<DataType>) = default;
		constexpr FlatHashMapEntry(FlatHashMapEntry&& src) {
			key = move(src.key);
			data = move(src.data);
			used = src.used;
		}

		constexpr FlatHashMapEntry& operator=(FlatHashMapEntry&& src) noexcept {
			key = move(src.key);
			data = move(src.data);
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
		constexpr Bool isUsed() const { return used; }
	};

	template<
		class DynamicArrayType,
		class Key, class DataType,
		class Hash = Hash<Key>,
		class Compare = FlatMapHashCompare<Key>,
		class Alloc = DefaultAllocator<FlatHashMapEntry<Key, DataType>>>
	class FlatHashMapCustomDynArray {
		using Entry = FlatHashMapEntry<Key, DataType>;
	public:
		using key_type = Key;
		using map_type = DataType;
		using entry_type = Entry;

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

		using iterator = FlatHashMapIterator<value_type, Alloc>;
		using const_iterator = FlatHashMapIterator<const value_type, Alloc>;
		using reverse_iterator = ReverseIterator<FlatHashMapIterator<value_type, Alloc>>;
		using const_reverse_iterator = ReverseIterator<FlatHashMapIterator<const value_type, Alloc>>;

		static constexpr f64 load_factor = 0.7;

		//movement info 
		constexpr static Bool triviallyRelocatable = true;
		constexpr static Bool triviallyDefaultConstructible = true;
		constexpr static Bool triviallyCompareable = false;
		constexpr static Bool triviallyDestructible = false;
		constexpr static Bool triviallyConstRefConstructedable = false;
		constexpr static Bool triviallyMoveConstructedable = false;
	private:
		size_type inSize;
		DynamicArrayType table;
	public:
		constexpr FlatHashMapCustomDynArray() noexcept : inSize(0), table() {}
		constexpr FlatHashMapCustomDynArray(const FlatHashMapCustomDynArray& src) noexcept : inSize{}, table() {
			inSize = src.inSize;
			table = src.table;
		}
		constexpr FlatHashMapCustomDynArray(FlatHashMapCustomDynArray&& src) noexcept : inSize{}, table() {
			inSize = src.inSize;
			table = move(src.table);
		}

		constexpr ~FlatHashMapCustomDynArray() noexcept = default;

		constexpr size_type size() const noexcept { return inSize; }
		constexpr size_type count() const noexcept { return inSize; }
		constexpr size_type capacity() const noexcept { return table.size(); }

		constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
		constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
		constexpr operator Bool() const noexcept { return isNotEmpty(); }

	private:
		constexpr pointer beginPtr() noexcept requires(IsNotConstV<DataType>) { return table.data(); }
		constexpr const_pointer beginPtr() const noexcept { return table.data(); }
		constexpr pointer endPtr() noexcept requires(IsNotConstV<DataType>) { return table.data() + table.size(); }
		constexpr const_pointer endPtr() const noexcept { return table.data() + table.size(); }

	public:
		constexpr iterator begin() noexcept requires(IsNotConstV<DataType>) { return iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr iterator end() noexcept requires(IsNotConstV<DataType>) { return iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(IsNotConstV<DataType>) { return reverse_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(IsNotConstV<DataType>) { return reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }

	public:
		constexpr iterator insert(const Key& key, const DataType& data) noexcept {
			resizeAndRehash();

			pointer dataLocation = nullptr;
			size_type index = Hash::hash(key) % capacity();
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

		constexpr iterator insert(Key&& key, DataType&& data) noexcept {
			resizeAndRehash();

			pointer dataLocation = nullptr;
			size_type index = Hash::hash(key) % capacity();
			while (table[index].isUsed()) {
				if (Compare::compare(table[index].key, key)) {
					dataLocation = &table[index];
					dataLocation->data = forward<DataType>(data);
					dataLocation->key = forward<Key>(key);

					return iterator(dataLocation, beginPtr(), endPtr());
				}
				index = (index + 1) % capacity();
			}

			dataLocation = &table[index];
			dataLocation->data = forward<DataType>(data);
			dataLocation->key = forward<Key>(key);
			dataLocation->setAsUsed();
			++inSize;

			return iterator(dataLocation, beginPtr(), endPtr());
		};

		constexpr iterator findIterator(const Key& key) noexcept {
			if (size() == 0) { return end(); }
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

		constexpr const_iterator findIterator(const Key& key) const noexcept {
			if (size() == 0) { return end(); }
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

		constexpr optional_pointer find(const Key& key) noexcept {
			if (size() == 0) { return optional_pointer(natl::OptionEmpty()); }
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

		constexpr optional_const_pointer find(const Key& key) const noexcept {
			if (size() == 0) { return optional_pointer(natl::OptionEmpty()); }
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

		Bool contains(const Key& key) const noexcept {
			return find(key).hasValue();
		}
		constexpr void resizeAndRehash() noexcept {
			 if (inSize >= static_cast<size_type>(static_cast<f64>(capacity()) * load_factor)) {
				if (inSize < static_cast<size_type>(static_cast<f64>(table.capacity()) * load_factor)) {
					table.resize(table.capacity());
					return;
				}

				resizeAndRehash((capacity() + 10) * 2);
			}
		}
		constexpr void resizeAndRehash(const size_type newCapacity) noexcept {
			if (newCapacity <= capacity()) { return; }

			DynamicArrayType newTable(newCapacity);

			for (Entry& entry : table) {
				if (entry.isUsed()) {
					size_type index = Hash::hash(entry.key) % newCapacity;

					while (newTable[index].isUsed()) {
						index = (index + 1) % newCapacity;
					}

					newTable[index] = forward<Entry>(entry);
				}
			}
			table = move(newTable);
		}

		constexpr void clear() noexcept {
			table.clear();
			inSize = 0;
		}

		constexpr void reserve(const size_type& newCapacity) noexcept {
			resizeAndRehash(newCapacity);
		}

		constexpr FlatHashMapCustomDynArray& operator=(FlatHashMapCustomDynArray& src) noexcept {
			table = src.table;
			inSize = src.inSize;

			return *this;
		}

		constexpr FlatHashMapCustomDynArray& operator=(FlatHashMapCustomDynArray&& src) noexcept {
			table = move(src.table);
			inSize = src.inSize;

			return *this;
		}
	};

	template<class Key, class DataType,
		class Hash = Hash<Key>,
		class Compare = FlatMapHashCompare<Key>,
		class Alloc = DefaultAllocator<FlatHashMapEntry<Key, DataType>>>
	using FlatHashMap = FlatHashMapCustomDynArray<DynArray<FlatHashMapEntry<Key, DataType>, Alloc>, Key, DataType, Hash, Compare, Alloc>;

	template<class Key, class DataType,
		Size bufferSize,
		class Hash = Hash<Key>,
		class Compare = FlatMapHashCompare<Key>,
		class Alloc = DefaultAllocator<FlatHashMapEntry<Key, DataType>>>
	using SmallFlatHashMap = FlatHashMapCustomDynArray<SmallDynArray<FlatHashMapEntry<Key, DataType>, bufferSize>, Key, DataType, Hash, Compare, Alloc>;
}

//additional includes for end use
#include "commonHashs.h"