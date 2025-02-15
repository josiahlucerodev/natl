#pragma once 

//own
#include "../util/hash.h"
#include "../util/iterators.h"
#include "../util/allocator.h"
#include "../util/dataMovement.h"
#include "../util/funcArgs.h"
#include "../processing/format.h"
#include "../fundamental/option.h"
#include "dynArray.h"
#include "smallDynArray.h"

//interface
namespace natl {
	template<typename KeyType, typename ValueType>
	struct KeyValueRef {
		using key_type = KeyType;
		using mapped_type = ValueType;

		key_type* keyPtr;
		mapped_type* valuePtr;

		constexpr operator KeyValueRef<const KeyType, const ValueType>() noexcept
			requires(!IsConstC<KeyType> && !IsConstC<ValueType>) {
			return KeyValueRef<const KeyType, const ValueType>(keyPtr, valuePtr);
		}

		constexpr key_type& key() noexcept { return *keyPtr; }
		constexpr const key_type& key() const noexcept { return *keyPtr; }
		constexpr mapped_type& value() noexcept { return *valuePtr; }
		constexpr const mapped_type& value() const noexcept { return *valuePtr; }
	};

	template<typename DataType>
	struct FlatMapHashCompare {
	public:
		constexpr static Bool compare(const DataType& lhs, const DataType& rhs) noexcept {
			return lhs == rhs;
		}
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
			constexpr mapped_type& value() noexcept { return value_detachedLifetime.value; }
			constexpr const mapped_type& value() const noexcept { return value_detachedLifetime.value; }

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

			template<typename OtherKeyType, typename OtherValueType>
			constexpr void constructKeyValue(OtherKeyType&& otherKey, OtherValueType&& otherValue) noexcept {
				natl::construct(&key_detachedLifetime.key, forward<OtherKeyType>(otherKey));
				natl::construct(&value_detachedLifetime.value, forward<OtherValueType>(otherValue));
				isActive = true;
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

			using allocator_type = Alloc::template rebind_alloc<value_type>;
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
			constexpr reference value() noexcept requires(IsNotConst<unit_type>) { return *dataPtr; }
			constexpr const_reference value() const noexcept { return *dataPtr; }

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
			typename Compare = FlatMapHashCompare<KeyType>>
		struct BaseFlatHashMap {
		public:
			using size_type = Size;
			using key_type = KeyType;
			using mapped_type = ValueType;

			using key_value_unit = FlatMapHashKeyValueUnit<key_type, mapped_type>;
			using storage_array = DynamicArrayType<key_value_unit>;

			using allocator_type = storage_array::allocator_type;

			using key_value_ref = KeyValueRef<key_type, mapped_type>;
			using key_value_const_ref = KeyValueRef<const key_type, const mapped_type>;

			using option_key_value_ref = Option<key_value_ref>;
			using option_key_value_const_ref = Option<key_value_const_ref>;


			using iterator_traits = DefaultIteratorTraits<key_value_unit>;
			using iterator_category = BidirectionalIteratorTag;

			using value_type = allocator_type::value_type;
			using reference = allocator_type::reference;
			using const_reference = allocator_type::const_reference;
			using pointer = allocator_type::pointer;
			using const_pointer = allocator_type::const_pointer;
			using difference_type = allocator_type::difference_type;

			using iterator = BaseFlatHashMapIterator<key_value_unit, allocator_type>;
			using const_iterator = BaseFlatHashMapIterator<const key_value_unit, allocator_type>;
			using reverse_iterator = ReverseIterator<BaseFlatHashMapIterator<key_value_unit, allocator_type>>;
			using const_reverse_iterator = ReverseIterator<BaseFlatHashMapIterator<const key_value_unit, allocator_type>>;

			static constexpr f64 loadFactor = 0.75;
		private:
			size_type _size;
			storage_array _storageArray;
		public:
			//constructor
			constexpr BaseFlatHashMap() noexcept : _size(0), _storageArray() {}
			constexpr BaseFlatHashMap(const BaseFlatHashMap& other) noexcept
				: _size(other._size), _storageArray(other._storageArray) {
			}
			constexpr BaseFlatHashMap(BaseFlatHashMap&& other) noexcept
				: _size(other._size), _storageArray(natl::move(other._storageArray)) {
			}

			//deconstructor 
			constexpr ~BaseFlatHashMap() noexcept {}

			//util 
			constexpr BaseFlatHashMap& self() noexcept { return *this; }
			constexpr const BaseFlatHashMap& self() const noexcept { return *this; }

			//assignment 
			constexpr BaseFlatHashMap& operator=(const BaseFlatHashMap& other) noexcept {
				_size = other._size;
				_storageArray = other._storageArray;
				return self();
			}
			constexpr BaseFlatHashMap& operator=(BaseFlatHashMap&& other) noexcept {
				_size = other._size;
				_storageArray = natl::move(other._storageArray);
				return self();
			}

			//capacity
			constexpr size_type size() const noexcept { return _size; }
			constexpr size_type count() const noexcept { return _size; }
			constexpr size_type capacity() const noexcept { return _storageArray.size(); }
			constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
			constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
			explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

			//iterators
		private:
			constexpr pointer beginPtr() noexcept { return _storageArray.data(); }
			constexpr const_pointer beginPtr() const noexcept { return _storageArray.data(); }
			constexpr pointer endPtr() noexcept { return _storageArray.data() + _storageArray.size(); }
			constexpr const_pointer endPtr() const noexcept { return _storageArray.data() + _storageArray.size(); }
		public:
			constexpr iterator begin() noexcept {
				pointer dataPtr = beginPtr();
				iterator firstIter = iterator(dataPtr, beginPtr(), endPtr());
				if (dataPtr->hasValue()) {
					return firstIter;
				} else {
					return ++firstIter;
				}
			}
			constexpr const_iterator begin() const noexcept {
				const_pointer dataPtr = beginPtr();
				const_iterator firstIter = const_iterator(dataPtr, beginPtr(), endPtr());
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
				if (dataPtr->hasValue()) {
					return firstIter;
				} else {
					return ++firstIter;
				}
			}
			constexpr const_reverse_iterator rbegin() const noexcept {
				const_pointer dataPtr = endPtr();
				const_reverse_iterator firstIter = const_reverse_iterator(dataPtr, beginPtr(), endPtr());
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
				_storageArray.clear();
				_size = 0;
			}
			constexpr void reserve(const size_type& newCapacity) noexcept {
				resizeAndRehash(static_cast<size_type>(static_cast<f64>(newCapacity) / loadFactor));
			}

			constexpr iterator insert(const key_type& key, const mapped_type& value) noexcept {
				resizeAndRehash();

				pointer dataLocation = nullptr;
				size_type index = Hash::hash(key) % capacity();
				while (_storageArray[index].hasValue()) {
					if (Compare::compare(_storageArray[index].key(), key)) {
						dataLocation = &_storageArray[index];
						dataLocation->value() = value;
						return iterator(dataLocation, beginPtr(), endPtr());
					}
					index = (index + 1) % capacity();
				}

				dataLocation = &_storageArray[index];
				dataLocation->constructKeyValue(key, value);
				++_size;

				return iterator(dataLocation, beginPtr(), endPtr());
			};

			constexpr iterator insert(key_type&& key, mapped_type&& value) noexcept {
				resizeAndRehash();

				pointer dataLocation = nullptr;
				size_type index = Hash::hash(key) % capacity();
				while (_storageArray[index].hasValue()) {
					if (Compare::compare(_storageArray[index].key(), key)) {
						dataLocation = &_storageArray[index];
						dataLocation->value() = natl::forward<mapped_type>(value);
						return iterator(dataLocation, beginPtr(), endPtr());
					}
					index = (index + 1) % capacity();
				}

				dataLocation = &_storageArray[index];
				dataLocation->constructKeyValue(natl::forward<key_type>(key), natl::forward<mapped_type>(value));
				++_size;

				return iterator(dataLocation, beginPtr(), endPtr());
			};

			constexpr Bool erase(const key_type& key) noexcept {
				iterator location = findIterator(key);
				if (location != end()) {
					_size -= 1;
					location.dataPtr->deconstruct();
					return true;
				} else {
					return false;
				}
			}

			//lookups
			constexpr iterator findIterator(const key_type& key) noexcept {
				if (size() == 0) { return end(); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (_storageArray[index].hasValue() && Compare::compare(_storageArray[index].key(), key)) {
						return iterator(&_storageArray[index], beginPtr(), endPtr());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return end();
			}

			constexpr const_iterator findIterator(const key_type& key) const noexcept {
				if (size() == 0) { return end(); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (Compare::compare(_storageArray[index].key(), key)) {
						return const_iterator(&_storageArray[index], beginPtr(), endPtr());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return end();
			}

			constexpr option_key_value_const_ref find(const key_type& key) noexcept {
				if (size() == 0) { return option_key_value_const_ref(natl::OptionEmpty()); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (_storageArray[index].hasValue() && Compare::compare(_storageArray[index].key(), key)) {
						return option_key_value_const_ref(_storageArray[index].ref());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return option_key_value_const_ref(natl::OptionEmpty());
			}

			constexpr option_key_value_const_ref find(const key_type& key) const noexcept {
				if (size() == 0) { return option_key_value_const_ref(natl::OptionEmpty()); }
				size_type index = Hash::hash(key) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (_storageArray[index].hasValue() && Compare::compare(_storageArray[index].key(), key)) {
						return option_key_value_const_ref(_storageArray[index].ref());
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

		private:
			constexpr void resizeAndRehash() noexcept {
				if (size() >= static_cast<size_type>(static_cast<f64>(capacity()) * loadFactor)) {
					if (size() < static_cast<size_type>(static_cast<f64>(_storageArray.capacity()) * loadFactor)) {
						_storageArray.resize(_storageArray.capacity());
						return;
					}

					resizeAndRehash((capacity() + 10) * 2);
				}
			}
			constexpr void resizeAndRehash(const size_type newCapacity) noexcept {
				if (newCapacity <= capacity()) { return; }

				storage_array newTable(newCapacity);
				for (auto& entry : _storageArray) {
					if (entry.isActive) {
						size_type index = Hash::hash(entry.key()) % newCapacity;
						while (newTable[index].isActive) {
							index = (index + 1) % newCapacity;
						}
						newTable[index] = forward<decltype(entry)>(entry);
					}
				}
				_storageArray = natl::move(newTable);
			}
		};
	}

	template<typename Key, typename DataType,
		typename Hash = Hash<Key>,
		typename Compare = FlatMapHashCompare<Key>,
		typename Alloc = DefaultAllocatorByte>
	using FlatHashMap = impl::BaseFlatHashMap<DynArrayUnboundTypeT<Alloc>::template type, Key, DataType, Hash, Compare>;

	template<typename Key, typename DataType, Size bufferSize,
		typename Hash = Hash<Key>,
		typename Compare = FlatMapHashCompare<Key>,
		typename Alloc = DefaultAllocatorByte>
	using SmallFlatHashMap = impl::BaseFlatHashMap<SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, Key, DataType, Hash, Compare>;


	template<typename... TemplateFlags>
	struct FormatKey {
		using template_flags = TypePack<TemplateFlags...>;
	};
	template<typename... TemplateFlags>
	struct FormatValue {
		using template_flags = TypePack<TemplateFlags...>;
	};

	namespace impl {
		template<typename Type>
		struct IsFormatKeyV : FalseType {};
		template<typename... TemplateFlags>
		struct IsFormatKeyV<FormatKey<TemplateFlags...>> : TrueType {};
		template<typename Type>
		constexpr inline Bool IsFormatKey = IsFormatKeyV<Type>::value;


		template<typename Type>
		struct IsFormatValueV : FalseType {};
		template<typename... TemplateFlags>
		struct IsFormatValueV<FormatValue<TemplateFlags...>> : TrueType {};
		template<typename Type>
		constexpr inline Bool IsFormatValue = IsFormatValueV<Type>::value;

		template<typename Type>
		constexpr inline Bool IsFormatKeyOrValue = IsFormatKey<Type> || IsFormatValue<Type>;

		template<typename... ArgTypes>
		struct FormatKeyArg {
			using arg_types = TypePack<ArgTypes...>;
			using storage_tuple_type = Tuple<ArgTypes&&...>;
		public:
			storage_tuple_type argStorage;
			constexpr FormatKeyArg(ArgTypes&&... formatArgs) noexcept
				: argStorage(natl::forward<ArgTypes>(formatArgs)...) {
			}
		};

		template<typename... ArgTypes>
		struct FormatValueArg {
			using arg_types = TypePack<ArgTypes...>;
			using storage_tuple_type = Tuple<ArgTypes&&...>;
		public:
			storage_tuple_type argStorage;
			constexpr FormatValueArg(ArgTypes&&... formatArgs) noexcept
				: argStorage(natl::forward<ArgTypes>(formatArgs)...) {
			}
		};
	}

	template<typename... ArgTypes>
	constexpr impl::FormatKeyArg<ArgTypes...> formatKey(ArgTypes&&... formatArgs) noexcept {
		return impl::FormatKeyArg<ArgTypes...>(natl::forward<ArgTypes>(formatArgs)...);
	}
	template<typename... ArgTypes>
	constexpr impl::FormatValueArg<ArgTypes...> formatValue(ArgTypes&&... formatArgs) noexcept {
		return impl::FormatValueArg<ArgTypes...>(natl::forward<ArgTypes>(formatArgs)...);
	}

	namespace impl {
		template<typename BaseFlatHashMapType, typename KeyFormatter, typename ValueFormatter>
		struct BaseFlatHashMapBaseFormatImpl {
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const BaseFlatHashMapType& flatHashMap) noexcept {
				Size index = 0;
				const Size flatHashMapSize = flatHashMap.size();

				outputIter = '{';
				for (typename BaseFlatHashMapType::const_reference entry : flatHashMap) {
					outputIter = '{';
					outputIter = KeyFormatter::format(outputIter, entry.key());
					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';
					outputIter = ValueFormatter::format(outputIter, entry.value());
					outputIter = '}';

					if (index != flatHashMapSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
				return outputIter;
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes, Size... KeyIndices>
			constexpr static void formatWithKeyArgs(OutputIter& outputIter, const BaseFlatHashMapType& flatHashMap,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs, IndexSequence<KeyIndices...>) noexcept {
				Size index = 0;
				const Size flatHashMapSize = flatHashMap.size();

				outputIter = '{';
				for (typename BaseFlatHashMapType::const_reference entry : flatHashMap) {
					outputIter = '{';

					if constexpr (sizeof...(KeyFormatArgsTypes) == 0) {
						outputIter = KeyFormatter::format(outputIter, entry.key());
					} else {
						outputIter = KeyFormatter::format(outputIter, entry.key(),
							natl::forward<KeyFormatArgsTypes>(formatKeyArgs.argStorage.template get<KeyIndices>())...);
					}

					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';
					outputIter = ValueFormatter::format(outputIter, entry.value());
					outputIter = '}';

					if (index != flatHashMapSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes>
			constexpr static OutputIter format(OutputIter outputIter, const BaseFlatHashMapType& flatHashMap,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs) noexcept {
				using format_key_arg_type = impl::FormatKeyArg<KeyFormatArgsTypes...>;
				formatWithKeyArgs(outputIter, flatHashMap,
					natl::forward<format_key_arg_type>(formatKeyArgs),
					MakeIndexSequence<sizeof...(KeyFormatArgsTypes)>{}
				);
				return outputIter;
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes, typename... ValueFromatArgTypes,
				Size... KeyIndices, Size... ValueIndices>
			constexpr static void formatWithKeyArgsAndValueArgs(OutputIter& outputIter,
				const BaseFlatHashMapType& flatHashMap,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs,
				impl::FormatValueArg<ValueFromatArgTypes...>&& formatValueArgs,
				IndexSequence<KeyIndices...>, IndexSequence<ValueIndices...>) noexcept {
				Size index = 0;
				const Size flatHashMapSize = flatHashMap.size();

				outputIter = '{';
				for (typename BaseFlatHashMapType::const_reference entry : flatHashMap) {
					outputIter = '{';

					if constexpr (sizeof...(KeyFormatArgsTypes) == 0) {
						outputIter = KeyFormatter::format(outputIter, entry.key());
					} else {
						outputIter = KeyFormatter::format(outputIter, entry.key(),
							natl::forward<KeyFormatArgsTypes>(formatKeyArgs.argStorage.template get<KeyIndices>())...);
					}

					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';

					if constexpr (sizeof...(KeyFormatArgsTypes) == 0) {
						outputIter = ValueFormatter::format(outputIter, entry.value());
					} else {
						outputIter = ValueFormatter::format(outputIter, entry.value(),
							natl::forward<ValueFromatArgTypes>(formatValueArgs.argStorage.template get<ValueIndices>())...);
					}
					outputIter = '}';

					if (index != flatHashMapSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
			}

			template<typename OutputIter, typename... KeyFormatArgsTypes, typename... ValueFromatArgTypes>
			constexpr static OutputIter format(OutputIter outputIter, const BaseFlatHashMapType& flatHashMap,
				impl::FormatKeyArg<KeyFormatArgsTypes...>&& formatKeyArgs,
				impl::FormatValueArg<ValueFromatArgTypes...>&& formatValueArgs) noexcept {
				using format_key_arg_type = impl::FormatKeyArg<KeyFormatArgsTypes...>;
				using format_value_arg_type = impl::FormatValueArg<ValueFromatArgTypes...>;
				formatWithKeyArgsAndValueArgs(outputIter, flatHashMap,
					natl::forward<format_key_arg_type>(formatKeyArgs),
					natl::forward<format_value_arg_type>(formatValueArgs),
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
		: impl::BaseFlatHashMapBaseFormatImpl<
			impl::BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare>,
				MakeFormatter<Key, CharType>,
				MakeFormatter<Value, CharType>> {
		using mapped_type = impl::BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare>;
		using hash_map_type = mapped_type;

		template<typename TemplateFlag>
		struct IsMapFormatTemplateFlagV {
			consteval static Bool testTemplateFlag() noexcept {
				if constexpr (IsStringLiteral<TemplateFlag>) {
					constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
					if constexpr (tflagName.starts_with("k: ")) {
						return true;
					} else if constexpr (tflagName.starts_with("v: ")) {
						return true;
					} else {
						return false;
					}
				} else {
					return impl::IsFormatKeyOrValue<TemplateFlag>;
				}
			}
			constexpr static Bool value = testTemplateFlag();
		};
		template<typename TemplateFlag>
		constexpr static Bool IsMapFormatTemplateFlag = IsMapFormatTemplateFlagV<TemplateFlag>::value;

		template<typename... TemplateFlags>
		struct WithTemplateFlagsFunctionImplT {
			template<typename TemplateFlag>
			struct IsKeyTemplateFlagV {
				consteval static Bool testTemplateFlag() noexcept {
					if constexpr (IsStringLiteral<TemplateFlag>) {
						constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
						return tflagName.starts_with("k: ");
					} else {
						return impl::IsFormatKey<TemplateFlag>;
					}
				}
				constexpr static Bool value = testTemplateFlag();
			};

			template<typename TemplateFlag>
			struct IsValueTemplateFlagV {
				consteval static Bool testTemplateFlag() noexcept {
					if constexpr (IsStringLiteral<TemplateFlag>) {
						constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
						return tflagName.starts_with("v: ");
					} else {
						return impl::IsFormatValue<TemplateFlag>;
					}
				}
				constexpr static Bool value = testTemplateFlag();
			};

			using template_flags = TypePack<TemplateFlags...>;

			using unreduced_key_template_flags = TypePackFilter<IsKeyTemplateFlagV, template_flags>;
			using unreduced_value_template_flags = TypePackFilter<IsValueTemplateFlagV, template_flags>;

			template<typename TemplateFlag>
			struct ReduceToTemplateFlagsT {
				using type = TypePack<TemplateFlag>;
			};
			template<typename... RTemplateFlags>
			struct ReduceToTemplateFlagsT<FormatKey<RTemplateFlags...>> {
				using type = TypePack<RTemplateFlags...>;
			};
			template<typename... RTemplateFlags>
			struct ReduceToTemplateFlagsT<FormatValue<RTemplateFlags...>> {
				using type = TypePack<RTemplateFlags...>;
			};
			template<typename TemplateFlag>
				requires(IsStringLiteral<TemplateFlag>)
			struct ReduceToTemplateFlagsT<TemplateFlag> {
				using type = TemplateFlag::template Substr<3>;
			};

			using key_template_flags = TypePackTransform<ReduceToTemplateFlagsT, unreduced_key_template_flags>;
			using value_template_flags = TypePackTransform<ReduceToTemplateFlagsT, unreduced_value_template_flags>;

			using key_formatter = InstantiateFormatterWithTemplateFlagsTypePack<Decay<Key>, CharType, key_template_flags>;
			using value_formatter = InstantiateFormatterWithTemplateFlagsTypePack<Decay<Key>, CharType, value_template_flags>;

			using type = impl::BaseFlatHashMapBaseFormatImpl<hash_map_type, key_formatter, value_formatter>;
		};

		template<typename... TemplateFlags>
			requires(IsMapFormatTemplateFlag<TemplateFlags> && ...)
		struct WithTemplateFlags : WithTemplateFlagsFunctionImplT<TemplateFlags...>::type {};
	};
}

namespace NATL_NAMESPACE_NAME_FOR_TUPLE_GET(natl) {
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

namespace NATL_NAMESPACE_NAME_FOR_TUPLE_GET(natl::impl) {
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


//additional includes for end use
#include "../util/commonHashs.h"