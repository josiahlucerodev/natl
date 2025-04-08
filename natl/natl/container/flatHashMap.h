#pragma once 

//own
#include "../util/hash.h"
#include "../util/iterators.h"
#include "../util/allocator.h"
#include "../util/dataMovement.h"
#include "../util/funcArgs.h"
#include "../util/compare.h"
#include "../processing/format.h"
#include "../fundamental/option.h"
#include "dynArray.h"
#include "smallDynArray.h"

//interface
namespace natl {
	template<typename KeyType, typename ValueType>
	struct KeyValueRef {
	public:
		using key_type = KeyType;
		using mapped_type = ValueType;

	private:
		key_type* keyPtr;
		mapped_type* valuePtr;

	public:
		//constructor 
		constexpr KeyValueRef() noexcept = default;
		constexpr KeyValueRef(key_type* keyPtrIn, mapped_type* valuePtrIn) 
			: keyPtr(keyPtrIn), valuePtr(valuePtrIn) {}

		//destructor 
		constexpr ~KeyValueRef() noexcept = default;

		//convert
		constexpr operator KeyValueRef<const KeyType, const ValueType>() noexcept
			requires(!IsConstC<KeyType> && !IsConstC<ValueType>) {
			return KeyValueRef<const KeyType, const ValueType>(keyPtr, valuePtr);
		}

		//accessors
		constexpr key_type& key() noexcept { return *keyPtr; }
		constexpr const key_type& key() const noexcept { return *keyPtr; }
		constexpr key_type* keyAsPtr() noexcept { return keyPtr; }
		constexpr const key_type* keyAsPtr() const noexcept { return keyPtr; }
		constexpr mapped_type& value() noexcept { return *valuePtr; }
		constexpr const mapped_type& value() const noexcept { return *valuePtr; }
		constexpr mapped_type* valueAsPtr() noexcept { return valuePtr; }
		constexpr const mapped_type* valueAsPtr() const noexcept { return valuePtr; }
	};

	template<typename IterType>
	struct MapInsertResult {
	public:
		using iterator = IterType;

	private:
		Bool successful;
		iterator location;

	public:
		//constructor
		constexpr MapInsertResult() noexcept = default;
		constexpr MapInsertResult(Bool successfulIn, iterator locationIn) noexcept 
			: successful(successfulIn), location(locationIn) {};

		//deconstructor 
		constexpr ~MapInsertResult() noexcept = default;

		//accessors
		constexpr Bool insertFailed() const noexcept { return !successful; }
		constexpr Bool insertSuccessful() const noexcept { return successful; }
		explicit constexpr operator Bool() const noexcept { return successful; }
		constexpr iterator iter() noexcept { return location; }
	};

	namespace impl {
		template<typename KeyType, typename ValueType>
		struct FlatMapHashKeyValueUnit {
		public:
			using key_type = KeyType;
			using mapped_type = ValueType;

			using key_value_ref = KeyValueRef<key_type, mapped_type>;
			using key_value_const_ref = KeyValueRef<const key_type, const mapped_type>;
		public:
			natl::Bool isActive;

			union KeyDetachedLifetime{
				Dummy dummy;
				key_type key;
				KeyDetachedLifetime() {}
				~KeyDetachedLifetime() {}
			} key_detachedLifetime;
			union ValueDetachedLifetime {
				Dummy dummy;
				mapped_type value;
				ValueDetachedLifetime() {}
				~ValueDetachedLifetime() {}
			} value_detachedLifetime;

		public:
			//constructor
			constexpr FlatMapHashKeyValueUnit() noexcept : isActive(false) {}
			constexpr FlatMapHashKeyValueUnit(const FlatMapHashKeyValueUnit& other) noexcept {
				isActive = other.isActive;
				if (other.isActive) {
					natl::construct(&key_detachedLifetime.key, other.key_detachedLifetime.key);
					natl::construct(&value_detachedLifetime.value, other.value_detachedLifetime.value);
				}
			}
			constexpr FlatMapHashKeyValueUnit(FlatMapHashKeyValueUnit&& other) noexcept {
				isActive = other.isActive;
				if (other.isActive) {
					natl::construct(&key_detachedLifetime.key, move(other.key_detachedLifetime.key));
					natl::construct(&value_detachedLifetime.value, move(other.value_detachedLifetime.value));
					natl::deconstruct(&other.key_detachedLifetime.key);
					natl::deconstruct(&other.value_detachedLifetime.value);
					other.isActive = false;
				}
			}

			//destructor 
			constexpr ~FlatMapHashKeyValueUnit() noexcept {
				if (isActive) {
					natl::deconstruct(&key_detachedLifetime.key);
					natl::deconstruct(&value_detachedLifetime.value);
				}
			}

			//util
			constexpr FlatMapHashKeyValueUnit& self() noexcept { return *this; }
			constexpr const FlatMapHashKeyValueUnit& self() const noexcept { return *this; }

			//assignment
			constexpr FlatMapHashKeyValueUnit& operator=(const FlatMapHashKeyValueUnit& other) noexcept {
				if (isActive) {
					if (other.isActive) {
						key_detachedLifetime.key = other.key_detachedLifetime.key;
						value_detachedLifetime.value = other.value_detachedLifetime.value;
					} else {
						natl::deconstruct(&key_detachedLifetime.key);
						natl::deconstruct(&value_detachedLifetime.value);
					}
				} else {
					if (other.isActive) {
						natl::construct(&key_detachedLifetime.key, other.key_detachedLifetime.key);
						natl::construct(&value_detachedLifetime.value, other.value_detachedLifetime.value);
					}
				}
				isActive = other.isActive;
				return self();
			}

			constexpr FlatMapHashKeyValueUnit& operator=(FlatMapHashKeyValueUnit&& other) noexcept {
				if (isActive) {
					if (other.isActive) {
						key_detachedLifetime.key = forward(other.key_detachedLifetime.key);
						value_detachedLifetime.value = forward(other.value_detachedLifetime.value);
					} else {
						natl::deconstruct(&key_detachedLifetime.key);
						natl::deconstruct(&value_detachedLifetime.value);
					}
				} else {
					if (other.isActive) {
						natl::construct(&key_detachedLifetime.key, forward(other.key_detachedLifetime.key));
						natl::construct(&value_detachedLifetime.value, forward(other.value_detachedLifetime.value));
					}
				}
				isActive = other.isActive;
				return self();
			}

			//accessors 
			constexpr Bool hasValue() const noexcept { return isActive; }
			constexpr Bool doesNotHaveValue() const noexcept { return !hasValue(); }

			constexpr key_type& key() noexcept { return key_detachedLifetime.key; }
			constexpr const key_type& key() const noexcept { return key_detachedLifetime.key; }
			constexpr key_type* keyAsPtr() noexcept { return &key_detachedLifetime.key; }
			constexpr const key_type* keyAsPtr() const noexcept { return &key_detachedLifetime.key; }
			constexpr mapped_type& value() noexcept { return value_detachedLifetime.value; }
			constexpr const mapped_type& value() const noexcept { return value_detachedLifetime.value; }
			constexpr mapped_type* valueAsPtr() noexcept { return &value_detachedLifetime.value; }
			constexpr const mapped_type* valueAsPtr() const noexcept { return &value_detachedLifetime.value; }

			constexpr key_value_ref ref() noexcept {
				return key_value_ref{ &key_detachedLifetime.key, &value_detachedLifetime.value };
			}
			constexpr key_value_const_ref ref() const noexcept {
				return key_value_const_ref{ &key_detachedLifetime.key, &value_detachedLifetime.value };
			}

			constexpr void deconstruct() noexcept {
				if (isActive) {
					natl::deconstruct(&key_detachedLifetime.key);
					natl::deconstruct(&value_detachedLifetime.value);
				}
				isActive = false;
			}

			template<typename OtherKeyType>
			constexpr void constructKey(OtherKeyType&& otherKey) noexcept {
				natl::construct(&key_detachedLifetime.key, forward<OtherKeyType>(otherKey));
			}
			template<typename OtherValueType>
			constexpr void constructValue(OtherValueType&& otherValue) noexcept {
				natl::construct(&value_detachedLifetime.value, forward<OtherValueType>(otherValue));
			}
			constexpr void setActive() noexcept {
				isActive = true;
			}
			constexpr void setInactive() noexcept {
				isActive = false;
			}

			template<typename OtherKeyType, typename OtherValueType>
			constexpr void constructKeyValue(OtherKeyType&& otherKey, OtherValueType&& otherValue) noexcept {
				constructKey<OtherKeyType>(forward<OtherKeyType>(otherKey));
				constructValue<OtherValueType>(forward<OtherValueType>(otherValue));
				setActive();
			}

		};
	}

	template<typename KeyType, typename ValueType>
	struct IsTriviallyCompareableV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyCompareableC<KeyType>> {
	};

	template<typename KeyType, typename ValueType>
	struct IsTriviallyRelocatableV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyRelocatableC<KeyType>&& IsTriviallyRelocatableC<ValueType>> {
	};
	template<typename KeyType, typename ValueType>
	struct IsTriviallyConstructibleV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyConstructibleC<KeyType>&& IsTriviallyConstructibleC<ValueType>> {
	};
	template<typename KeyType, typename ValueType>
	struct IsTriviallyDestructibleV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyDestructibleC<KeyType>&& IsTriviallyDestructibleC<ValueType>> {
	};

	template<typename KeyType, typename ValueType>
	struct IsTriviallyConstRefConstructibleV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyConstRefConstructibleC<KeyType>&& IsTriviallyConstRefConstructibleC<ValueType>> {
	};
	template<typename KeyType, typename ValueType>
	struct IsTriviallyMoveConstructibleV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyMoveConstructibleC<KeyType>&& IsTriviallyMoveConstructibleC<ValueType>> {
	};

	template<typename KeyType, typename ValueType>
	struct IsTriviallyConstRefAssignableV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyConstRefAssignableC<KeyType>&& IsTriviallyConstRefAssignableC<ValueType>> {
	};
	template<typename KeyType, typename ValueType>
	struct IsTriviallyMoveAssignableV<impl::FlatMapHashKeyValueUnit<KeyType, ValueType>>
		: BoolConstant<IsTriviallyMoveAssignableC<KeyType>&& IsTriviallyMoveAssignableC<ValueType>> {
	};

	namespace impl {
		template<typename unit_type, typename Alloc>
			requires(IsAllocatorC<Alloc>)
		struct BaseFlatHashMapIterator {
		public:
			using iterator_traits = DefaultIteratorTraits<unit_type>;
			using iterator_category = BidirectionalIteratorTag;

			using value_type = iterator_traits::value_type;
			using reference = iterator_traits::reference;
			using const_reference = iterator_traits::const_reference;
			using pointer = iterator_traits::pointer;
			using const_pointer = iterator_traits::const_pointer;
			using difference_type = iterator_traits::difference_type;
			using size_type = iterator_traits::size_type;

			using allocator_type = Alloc;
			using typed_allocator_type = Alloc::template rebind<value_type>;
			using iterator = BaseFlatHashMapIterator;

			//friends
			template<template<typename> typename, typename, typename, typename, typename>
			friend struct BaseFlatHashMap;
		private:
			pointer dataPtr;
			pointer beginPtr;
			pointer endPtr;
		public:
			constexpr BaseFlatHashMapIterator() : dataPtr(nullptr), beginPtr(nullptr), endPtr(nullptr) {};
			constexpr BaseFlatHashMapIterator(pointer dataPtrIn, pointer beginPtrIn, pointer endPtrIn) noexcept
				: dataPtr(dataPtrIn), beginPtr(beginPtrIn), endPtr(endPtrIn) {
			}
		private:
			constexpr iterator& self() noexcept { return *this; }
			constexpr const iterator& self() const noexcept { return *this; }
		public:
			constexpr reference operator*() noexcept requires(IsNotConst<unit_type>) { return *dataPtr; }
			constexpr const_reference operator*() const noexcept { return *dataPtr; }
			constexpr pointer operator->() noexcept requires(IsNotConst<unit_type>) { return dataPtr; }
			constexpr const_pointer operator->() const noexcept { return dataPtr; }
			constexpr reference deref() noexcept requires(IsNotConst<unit_type>) { return *dataPtr; }
			constexpr const_reference deref() const noexcept { return *dataPtr; }
			constexpr reference keyValue() noexcept requires(IsNotConst<unit_type>) { return *dataPtr; }
			constexpr const_reference keyValue() const noexcept { return *dataPtr; }

			constexpr iterator& operator++() noexcept {
				while (true) {
					dataPtr++;
					if (dataPtr == endPtr) {
						return self();
					}
					if (dataPtr->isActive) {
						return self();
					}
				}
			}
			constexpr iterator operator++(int) noexcept {
				iterator tempIt = self();
				++self();
				return tempIt;
			}
			constexpr iterator& operator--() noexcept {
				while (dataPtr > beginPtr) {
					dataPtr--;
					if (dataPtr->isActive) {
						return self();
					}
				}
				return self();
			}
			constexpr iterator operator--(int) noexcept {
				iterator tempIt = self();
				--self();
				return tempIt;
			}

			constexpr Bool operator== (const iterator rhs) const noexcept { return dataPtr == rhs.dataPtr; }
			constexpr Bool operator!= (const iterator rhs) const noexcept { return dataPtr != rhs.dataPtr; }
			constexpr Bool operator<(const iterator rhs) const noexcept { return dataPtr < rhs.dataPtr; }
			constexpr Bool operator>(const iterator rhs) const noexcept { return dataPtr > rhs.dataPtr; }
			constexpr Bool operator<=(const iterator rhs) const noexcept { return dataPtr <= rhs.dataPtr; }
			constexpr Bool operator>=(const iterator rhs) const noexcept { return dataPtr >= rhs.dataPtr; }
		};

		template<
			template<typename> typename DynamicArrayType,
			typename KeyType, typename ValueType,
			typename Hash = Hash<KeyType>,
			typename Compare = CompareEqual<KeyType>
		> struct BaseFlatHashMap {
		public:
			using size_type = Size;
			using key_type = KeyType;
			using mapped_type = ValueType;

			using key_value_unit = FlatMapHashKeyValueUnit<key_type, mapped_type>;
			using storage_array = DynamicArrayType<key_value_unit>;

			using allocator_type = storage_array::allocator_type;
			using typed_allocator_type = storage_array::typed_allocator_type;

			using key_value_ref = KeyValueRef<key_type, mapped_type>;
			using key_value_const_ref = KeyValueRef<const key_type, const mapped_type>;

			using option_key_value_ref = Option<key_value_ref>;
			using option_key_value_const_ref = Option<key_value_const_ref>;

			using iterator_traits = DefaultIteratorTraits<key_value_unit>;
			using iterator_category = BidirectionalIteratorTag;

			using value_type = typed_allocator_type::value_type;
			using reference = typed_allocator_type::reference;
			using const_reference = typed_allocator_type::const_reference;
			using pointer = typed_allocator_type::pointer;
			using const_pointer = typed_allocator_type::const_pointer;
			using difference_type = typed_allocator_type::difference_type;

			using iterator = BaseFlatHashMapIterator<key_value_unit, allocator_type>;
			using const_iterator = BaseFlatHashMapIterator<const key_value_unit, allocator_type>;
			using reverse_iterator = ReverseIterator<BaseFlatHashMapIterator<key_value_unit, allocator_type>>;
			using const_reverse_iterator = ReverseIterator<BaseFlatHashMapIterator<const key_value_unit, allocator_type>>;

			using insert_result = MapInsertResult<iterator>;

			static constexpr f64 loadFactor = 0.75;
		private:
			size_type mapSize;
			storage_array storageArray;
		public:
			//constructor
			constexpr BaseFlatHashMap() noexcept : mapSize(0), storageArray() {}
			constexpr BaseFlatHashMap(const BaseFlatHashMap& other) noexcept
				: mapSize(other.mapSize), storageArray(other.storageArray) {
			}
			constexpr BaseFlatHashMap(BaseFlatHashMap&& other) noexcept
				: mapSize(other.mapSize), storageArray(natl::move(other.storageArray)) {
			}

			//deconstructor 
			constexpr ~BaseFlatHashMap() noexcept {}

			//util 
			constexpr BaseFlatHashMap& self() noexcept { return *this; }
			constexpr const BaseFlatHashMap& self() const noexcept { return *this; }

			//assignment 
			constexpr BaseFlatHashMap& operator=(const BaseFlatHashMap& other) noexcept {
				mapSize = other.mapSize;
				storageArray = other.storageArray;
				return self();
			}
			constexpr BaseFlatHashMap& operator=(BaseFlatHashMap&& other) noexcept {
				mapSize = other.mapSize;
				storageArray = natl::move(other.storageArray);
				return self();
			}

			//capacity
			constexpr size_type size() const noexcept { return mapSize; }
			constexpr size_type count() const noexcept { return mapSize; }
			constexpr size_type capacity() const noexcept { return storageArray.size(); }
			constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
			constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
			explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

			//iterators
		private:
			constexpr pointer beginPtr() noexcept { return storageArray.data(); }
			constexpr const_pointer beginPtr() const noexcept { return storageArray.data(); }
			constexpr pointer endPtr() noexcept { return storageArray.data() + storageArray.size(); }
			constexpr const_pointer endPtr() const noexcept { return storageArray.data() + storageArray.size(); }
		public:
			constexpr iterator begin() noexcept {
				pointer dataPtr = beginPtr();
				iterator firstIter = iterator(dataPtr, beginPtr(), endPtr());
				if (dataPtr == nullptr) {
					return firstIter;
				}

				if (dataPtr->hasValue()) {
					return firstIter;
				} else {
					return ++firstIter;
				}
			}
			constexpr const_iterator begin() const noexcept {
				const_pointer dataPtr = beginPtr();
				const_iterator firstIter = const_iterator(dataPtr, beginPtr(), endPtr());
				if (dataPtr == nullptr) {
					return firstIter;
				}

				if (dataPtr->hasValue()) {
					return firstIter;
				} else {
					return ++firstIter;
				}
			}
			constexpr const_iterator cbegin() const noexcept {
				return begin();
			}

			constexpr iterator end() noexcept { return iterator(endPtr(), beginPtr(), endPtr()); }
			constexpr const_iterator end() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }
			constexpr const_iterator cend() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }

			constexpr reverse_iterator rbegin() noexcept {
				pointer dataPtr = endPtr();
				reverse_iterator firstIter = reverse_iterator(dataPtr, beginPtr(), endPtr());
				if (dataPtr == nullptr) {
					return firstIter;
				}

				if (dataPtr->hasValue()) {
					return firstIter;
				} else {
					return ++firstIter;
				}
			}
			constexpr const_reverse_iterator rbegin() const noexcept {
				const_pointer dataPtr = endPtr();
				const_reverse_iterator firstIter = const_reverse_iterator(dataPtr, beginPtr(), endPtr());
				if (dataPtr == nullptr) {
					return firstIter;
				}

				if (dataPtr->hasValue()) {
					return firstIter;
				} else {
					return ++firstIter;
				}
			}
			constexpr const_reverse_iterator crbegin() const noexcept {
				return rbegin();
			}

			constexpr reverse_iterator rend() noexcept { return reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
			constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
			constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }

			//modifiers
			constexpr void clear() noexcept {
				storageArray.clear();
				mapSize = 0;
			}
			constexpr void reserve(const size_type& newCapacity) noexcept {
				resizeAndRehash(static_cast<size_type>(static_cast<f64>(newCapacity) / loadFactor));
			}

			constexpr insert_result insert(const key_type& key, const mapped_type& value) noexcept {
				resizeAndRehash();

				pointer dataLocation = nullptr;
				size_type index = Hash::hash(key) % capacity();
				while (storageArray[index].hasValue()) {
					if (Compare::compare(storageArray[index].key(), key)) {
						dataLocation = &storageArray[index];
						return insert_result(false, iterator(dataLocation, beginPtr(), endPtr()));
					}
					index = (index + 1) % capacity();
				}

				dataLocation = &storageArray[index];
				dataLocation->constructKeyValue(key, value);
				++mapSize;

				return insert_result(true, iterator(dataLocation, beginPtr(), endPtr()));
			};

			constexpr insert_result insert(key_type&& key, mapped_type&& value) noexcept {
				resizeAndRehash();

				pointer dataLocation = nullptr;
				size_type index = Hash::hash(key) % capacity();
				while (storageArray[index].hasValue()) {
					if (Compare::compare(storageArray[index].key(), key)) {
						dataLocation = &storageArray[index];
						return insert_result(false, iterator(dataLocation, beginPtr(), endPtr()));
					}
					index = (index + 1) % capacity();
				}

				dataLocation = &storageArray[index];
				dataLocation->constructKeyValue(forward<key_type>(key), forward<mapped_type>(value));
				++mapSize;

				return insert_result(true, iterator(dataLocation, beginPtr(), endPtr()));
			};

			constexpr iterator insertWithReplacement(const key_type& key, const mapped_type& value) noexcept {
				insert_result insertResult = insert(key, value);
				if (insertResult.insertFailed()) {
					insertResult.iter()->value() = value;
				}
				return insertResult.iter();
			}

			constexpr iterator insertWithReplacement(key_type&& key, mapped_type&& value) noexcept {
				insert_result insertResult = insert(forward<key_type>(key), forward<mapped_type>(value));
				if (insertResult.insertFailed()) {
					insertResult.iter()->value() = forward<mapped_type>(value);
				}
				return insertResult.iter();
			}

			constexpr Bool erase(const key_type& key) noexcept {
				iterator location = findIter(key);
				return erase(location);
			}
			constexpr Bool erase(iterator location) noexcept {
				if (location != end()) {
					mapSize -= 1;
					location.dataPtr->deconstruct();
					return true;
				} else {
					return false;
				}
			}
			constexpr Bool erase(const_iterator location) noexcept {
				if (location != end()) {
					mapSize -= 1;
					location.dataPtr->deconstruct();
					return true;
				} else {
					return false;
				}
			}

			//lookups
			constexpr iterator findIter(const key_type& key) noexcept {
				if (size() == 0) { return end(); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (storageArray[index].hasValue() && Compare::compare(storageArray[index].key(), key)) {
						return iterator(&storageArray[index], beginPtr(), endPtr());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return end();
			}

			constexpr const_iterator findIter(const key_type& key) const noexcept {
				if (size() == 0) { return end(); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (Compare::compare(storageArray[index].key(), key)) {
						return const_iterator(&storageArray[index], beginPtr(), endPtr());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return end();
			}

			constexpr option_key_value_ref find(const key_type& key) noexcept {
				if (size() == 0) { return option_key_value_ref(natl::OptionEmpty()); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (storageArray[index].hasValue() && Compare::compare(storageArray[index].key(), key)) {
						return option_key_value_ref(storageArray[index].ref());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return option_key_value_ref(natl::OptionEmpty());
			}

			constexpr option_key_value_const_ref find(const key_type& key) const noexcept {
				if (size() == 0) { return option_key_value_const_ref(natl::OptionEmpty()); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (storageArray[index].hasValue() && Compare::compare(storageArray[index].key(), key)) {
						return option_key_value_const_ref(storageArray[index].ref());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return option_key_value_const_ref();
			}

			constexpr Bool contains(const key_type& key) const noexcept {
				return find(key).hasValue();
			}

			//restricted
			constexpr storage_array& restrictedGetStorageArray() noexcept {
				return storageArray;
			}
			constexpr const storage_array& restrictedGetStorageArray() const noexcept {
				return storageArray;
			}
		private:
			constexpr void resizeAndRehash() noexcept {
				if (size() >= static_cast<size_type>(static_cast<f64>(capacity()) * loadFactor)) {
					if (size() < static_cast<size_type>(static_cast<f64>(storageArray.capacity()) * loadFactor)) {
						storageArray.resize(storageArray.capacity());
						return;
					}

					resizeAndRehash((capacity() + 10) * 2);
				}
			}
			constexpr void resizeAndRehash(const size_type newCapacity) noexcept {
				if (newCapacity <= capacity()) { return; }

				storage_array newTable(newCapacity);
				for (auto& entry : storageArray) {
					if (entry.isActive) {
						size_type index = Hash::hash(entry.key()) % newCapacity;
						while (newTable[index].isActive) {
							index = (index + 1) % newCapacity;
						}
						newTable[index] = forward<decltype(entry)>(entry);
					}
				}
				storageArray = natl::move(newTable);
			}
		};
	}

	template<typename Key, typename DataType,
		typename Hash = Hash<Key>,
		typename Compare = CompareEqual<Key>,
		typename Alloc = DefaultAllocator>
	using FlatHashMap = impl::BaseFlatHashMap<DynArrayUnboundTypeT<Alloc>::template type, Key, DataType, Hash, Compare>;

	template<typename Key, typename DataType,
		typename Alloc = DefaultAllocator,
		typename Hash = Hash<Key>,
		typename Compare = CompareEqual<Key>>
	using FlatHashMapAlloc = impl::BaseFlatHashMap<DynArrayUnboundTypeT<Alloc>::template type, Key, DataType, Hash, Compare>;

	template<typename Key, typename DataType, Size bufferSize,
		typename Hash = Hash<Key>,
		typename Compare = CompareEqual<Key>,
		typename Alloc = DefaultAllocator>
	using SmallFlatHashMap = impl::BaseFlatHashMap<SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, Key, DataType, Hash, Compare>;

	template<typename Key, typename DataType, Size bufferSize,
		typename Alloc = DefaultAllocator,
		typename Hash = Hash<Key>,
		typename Compare = CompareEqual<Key>>
	using SmallFlatHashMapAlloc = impl::BaseFlatHashMap<SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, Key, DataType, Hash, Compare>;

	namespace impl {
		template<typename MapType, typename KeyFormatter, typename ValueFormatter>
		struct MapBaseFormatImpl {
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const MapType& map) noexcept {
				Size index = 0;
				const Size mapSize = map.size();

				outputIter = '{';
				for (typename MapType::const_reference entry : map) {
					outputIter = '{';
					outputIter = KeyFormatter::format(outputIter, entry.key());
					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';
					outputIter = ValueFormatter::format(outputIter, entry.value());
					outputIter = '}';

					if (index != mapSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
				return outputIter;
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes, Size... KeyIndices>
			constexpr static void formatWithKeyArgs(OutputIter& outputIter, const MapType& map,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs, IndexSequence<KeyIndices...>) noexcept {
				Size index = 0;
				const Size mapSize = map.size();

				outputIter = '{';
				for (typename MapType::const_reference entry : map) {
					outputIter = '{';

					if constexpr (sizeof...(KeyFormatArgsTypes) == 0) {
						outputIter = KeyFormatter::format(outputIter, entry.key());
					} else {
						outputIter = KeyFormatter::format(outputIter, entry.key(),
							forward<KeyFormatArgsTypes>(formatKeyArgs.argStorage.template get<KeyIndices>())...);
					}

					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';
					outputIter = ValueFormatter::format(outputIter, entry.value());
					outputIter = '}';

					if (index != mapSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes>
			constexpr static OutputIter format(OutputIter outputIter, const MapType& map,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs) noexcept {
				using format_key_arg_type = impl::FormatKeyArg<KeyFormatArgsTypes...>;
				formatWithKeyArgs(outputIter, map,
					forward<format_key_arg_type>(formatKeyArgs),
					MakeIndexSequence<sizeof...(KeyFormatArgsTypes)>{}
				);
				return outputIter;
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes, typename... ValueFromatArgTypes,
				Size... KeyIndices, Size... ValueIndices>
			constexpr static void formatWithKeyArgsAndValueArgs(OutputIter& outputIter,
				const MapType& map,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs,
				impl::FormatValueArg<ValueFromatArgTypes...>&& formatValueArgs,
				IndexSequence<KeyIndices...>, IndexSequence<ValueIndices...>) noexcept {
				Size index = 0;
				const Size mapSize = map.size();

				outputIter = '{';
				for (typename MapType::const_reference entry : map) {
					outputIter = '{';

					if constexpr (sizeof...(KeyFormatArgsTypes) == 0) {
						outputIter = KeyFormatter::format(outputIter, entry.key());
					} else {
						outputIter = KeyFormatter::format(outputIter, entry.key(),
							forward<KeyFormatArgsTypes>(formatKeyArgs.argStorage.template get<KeyIndices>())...);
					}

					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';

					if constexpr (sizeof...(KeyFormatArgsTypes) == 0) {
						outputIter = ValueFormatter::format(outputIter, entry.value());
					} else {
						outputIter = ValueFormatter::format(outputIter, entry.value(),
							forward<ValueFromatArgTypes>(formatValueArgs.argStorage.template get<ValueIndices>())...);
					}
					outputIter = '}';

					if (index != mapSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes, typename... ValueFromatArgTypes>
			constexpr static OutputIter format(OutputIter outputIter, const MapType& map,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs,
				impl::FormatValueArg<ValueFromatArgTypes...>&& formatValueArgs) noexcept {
				using format_key_arg_type = impl::FormatKeyArg<KeyFormatArgsTypes...>;
				using format_value_arg_type = impl::FormatValueArg<ValueFromatArgTypes...>;
				formatWithKeyArgsAndValueArgs(outputIter, map,
					forward<format_key_arg_type>(formatKeyArgs),
					forward<format_value_arg_type>(formatValueArgs),
					MakeIndexSequence<sizeof...(KeyFormatArgsTypes)>{},
					MakeIndexSequence<sizeof...(ValueFromatArgTypes)>{}
				);
				return outputIter;
			}
		};
	}

	template<
		template<typename> typename DynamicArrayType,
		typename Key, typename Value,
		typename Hash, typename Compare,
		typename CharType>
	struct Formatter<impl::BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare>, CharType>
		: impl::MapBaseFormatImpl<
			impl::BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare>,
				MakeFormatter<Key, CharType>,
				MakeFormatter<Value, CharType>> {
		using mapped_type = impl::BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare>;
		using hash_map_type = mapped_type;

		template<typename... TemplateFlags>
		struct WithTemplateFlagsFunctionImplT {
			using template_flags = TypePack<TemplateFlags...>;

			using unreduced_key_template_flags = TypePackFilter<IsKeyFormatTemplateFlagV, template_flags>;
			using unreduced_value_template_flags = TypePackFilter<IsValueFormatTemplateFlagV, template_flags>;

			using key_template_flags = TypePackTransform<ReduceToTemplateFlagsT, unreduced_key_template_flags>;
			using value_template_flags = TypePackTransform<ReduceToTemplateFlagsT, unreduced_value_template_flags>;

			using key_formatter = InstantiateFormatterWithTemplateFlagsTypePack<Decay<Key>, CharType, key_template_flags>;
			using value_formatter = InstantiateFormatterWithTemplateFlagsTypePack<Decay<Value>, CharType, value_template_flags>;

			using type = impl::MapBaseFormatImpl<hash_map_type, key_formatter, value_formatter>;
		};

		template<typename... TemplateFlags>
			requires(IsKeyOrValueFormatTemplateFlagC<TemplateFlags> && ...)
		struct WithTemplateFlags : WithTemplateFlagsFunctionImplT<TemplateFlags...>::type {};
	};
}

namespace natl {
	template <std::size_t Index, typename KeyType, typename ValueType>
	constexpr decltype(auto) get(const natl::KeyValueRef<KeyType, ValueType>& obj) {
		if constexpr (Index == 0) {
			return obj.key();
		} else if constexpr (Index == 1) {
			return obj.value();
		} else {
			static_assert(Index < 2, "natl: index out of bounds");
		}
	}
	template <std::size_t Index, typename KeyType, typename ValueType>
	constexpr decltype(auto) get(natl::KeyValueRef<KeyType, ValueType>& obj) {
		if constexpr (Index == 0) {
			return obj.key();
		} else if constexpr (Index == 1) {
			return obj.value();
		} else {
			static_assert(Index < 2, "natl: index out of bounds");
		}
	}
	template <std::size_t Index, typename KeyType, typename ValueType>
	constexpr decltype(auto) get(natl::KeyValueRef<KeyType, ValueType>&& obj) {
		if constexpr (Index == 0) {
			return obj.key();
		} else if constexpr (Index == 1) {
			return obj.value();
		} else {
			static_assert(Index < 2, "natl: index out of bounds");
		}
	}
}

namespace natl::impl {
	template <std::size_t Index, typename KeyType, typename ValueType>
	constexpr decltype(auto) get(const natl::impl::FlatMapHashKeyValueUnit<KeyType, ValueType>& obj) {
		if constexpr (Index == 0) {
			return obj.key();
		} else if constexpr (Index == 1) {
			return obj.value();
		} else {
			static_assert(Index < 2, "natl: index out of bounds");
		}
	}
	template <std::size_t Index, typename KeyType, typename ValueType>
	constexpr decltype(auto) get(natl::impl::FlatMapHashKeyValueUnit<KeyType, ValueType>& obj) {
		if constexpr (Index == 0) {
			return obj.key();
		} else if constexpr (Index == 1) {
			return obj.value();
		} else {
			static_assert(Index < 2, "natl: index out of bounds");
		}
	}
	template <std::size_t Index, typename KeyType, typename ValueType>
	constexpr decltype(auto) get(natl::impl::FlatMapHashKeyValueUnit<KeyType, ValueType>&& obj) {
		if constexpr (Index == 0) {
			return obj.key();
		} else if constexpr (Index == 1) {
			return obj.value();
		} else {
			static_assert(Index < 2, "natl:  index out of bounds");
		}
	}
}

namespace std {
	template<typename KeyType, typename ValueType>
	struct tuple_size<natl::KeyValueRef<KeyType, ValueType>> {
		constexpr static inline natl::StdSize value = 2;
	};

	template<natl::Size Index, typename KeyType, typename ValueType>
	struct tuple_element<Index, natl::KeyValueRef<KeyType, ValueType>> {
		using type = natl::Conditional<Index == 0, KeyType, ValueType>;
	};

	
	template<typename KeyType, typename ValueType>
	struct tuple_size<natl::impl::FlatMapHashKeyValueUnit<KeyType, ValueType>> {
		constexpr static inline natl::StdSize value = 2;
	};
	template<natl::Size Index, typename KeyType, typename ValueType>
	struct tuple_element<Index, natl::impl::FlatMapHashKeyValueUnit<KeyType, ValueType>> {
		using type = natl::Conditional<Index == 0, KeyType, ValueType>;
	};
}