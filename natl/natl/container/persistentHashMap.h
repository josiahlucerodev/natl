#pragma once 

//own
#include "flatHashMap.h"
#include "batchPool.h"

//@export
namespace natl {
	namespace impl {
		template<typename KeyType, typename ValueType, typename storage_iterator, typename Alloc>
			requires(IsAllocatorC<Alloc>)
		struct BasePersistentHashMapIterator {
		public:
			using key_type = KeyType;
			using mapped_type = ValueType;

			using key_value_ref = KeyValueRef<key_type, mapped_type>;
			using key_value_const_ref = KeyValueRef<const key_type, const mapped_type>;

			struct IteratorTraits {
				using value_type = key_value_ref;
				using reference = key_value_ref;
				using const_reference = key_value_const_ref;
				using pointer = key_value_const_ref;
				using const_pointer = key_value_const_ref;
				using difference_type = PtrDiff;
				using size_type = Size;
			};

			using iterator_traits = IteratorTraits;
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
			using iterator = BasePersistentHashMapIterator;

		private:
			storage_iterator baseIterator;

		public:
			//constructor 
			constexpr BasePersistentHashMapIterator() noexcept = default;
			constexpr BasePersistentHashMapIterator(storage_iterator baseIteratorIn) noexcept
				: baseIterator(baseIteratorIn) {}

			//destructor 
			constexpr ~BasePersistentHashMapIterator() noexcept = default;

			//util
			constexpr iterator& self() noexcept { return *this; }
			constexpr const iterator& self() const noexcept { return *this; }

			//iter ops
			constexpr storage_iterator getBase() const noexcept { return baseIterator; }

			constexpr key_value_ref operator*() noexcept requires(IsNotConst<KeyType>) {
				return key_value_ref(baseIterator->keyAsPtr(), baseIterator->value());
			}
			constexpr key_value_const_ref operator*() const noexcept {
				return key_value_const_ref(baseIterator->keyAsPtr(), baseIterator->value());
			}
			constexpr key_value_ref deref() noexcept requires(IsNotConst<KeyType>) {
				return key_value_ref(baseIterator->keyAsPtr(), baseIterator->value());
			}
			constexpr key_value_const_ref deref() const noexcept {
				return key_value_const_ref(baseIterator->keyAsPtr(), baseIterator->value());
			}
			constexpr key_value_ref keyValue() noexcept requires(IsNotConst<KeyType>) {
				return key_value_ref(baseIterator->keyAsPtr(), baseIterator->value());
			}
			constexpr key_value_const_ref keyValue() const noexcept {
				return key_value_const_ref(baseIterator->keyAsPtr(), baseIterator->value());
			}

			constexpr iterator& operator++() noexcept { 
				++baseIterator;
				return self();
			}
			constexpr iterator operator++(int) noexcept { 
				return iterator(baseIterator++);
			}
			constexpr iterator& operator--() noexcept { 
				--baseIterator; 
				return self();
			}
			constexpr iterator operator--(int) noexcept { 
				return iterator(baseIterator--); 
			}

			constexpr Bool operator== (const iterator rhs) const noexcept { return baseIterator == rhs.baseIterator; }
			constexpr Bool operator!= (const iterator rhs) const noexcept { return baseIterator != rhs.baseIterator; }
			constexpr Bool operator<(const iterator rhs) const noexcept { return baseIterator < rhs.baseIterator; }
			constexpr Bool operator>(const iterator rhs) const noexcept { return baseIterator > rhs.baseIterator; }
			constexpr Bool operator<=(const iterator rhs) const noexcept { return baseIterator <= rhs.baseIterator; }
			constexpr Bool operator>=(const iterator rhs) const noexcept { return baseIterator >= rhs.baseIterator; }
		};

		template<
			template<typename> typename PoolType
			, template<typename> typename DynamicArrayType
			, typename KeyType, typename ValueType
			, typename Hash = Hash<KeyType>
			, typename Compare = CompareEqual<KeyType>
		> struct BasePersistentHashMap {
		public:
			using storage_pool = PoolType<ValueType>;
			using storage_map = impl::BaseFlatHashMap<DynamicArrayType, KeyType, ValueType*, Hash, Compare>;
			using storage_array = storage_map::storage_array;

			using key_type = KeyType;
			using mapped_type = ValueType;

			using allocator_type = storage_map::allocator_type;
			using typed_allocator_type = storage_map::typed_allocator_type;

			using storage_map_iterator = typename storage_map::iterator;

			using key_value_ref = KeyValueRef<key_type, mapped_type>;
			using key_value_const_ref = KeyValueRef<const key_type, const mapped_type>;
			using option_key_value_ref = Option<key_value_ref>;
			using option_key_value_const_ref = Option<key_value_const_ref>;

			struct IteratorTraits {
				using value_type = key_value_ref;
				using reference = key_value_ref;
				using const_reference = key_value_const_ref;
				using pointer = key_value_const_ref;
				using const_pointer = key_value_const_ref;
				using difference_type = PtrDiff;
				using size_type = Size;
			};

			using iterator_traits = IteratorTraits;
			using iterator_category = BidirectionalIteratorTag;

			using value_type = iterator_traits::value_type;
			using reference = iterator_traits::reference;
			using const_reference = iterator_traits::const_reference;
			using pointer = iterator_traits::pointer;
			using const_pointer = iterator_traits::const_pointer;
			using difference_type = iterator_traits::difference_type;
			using size_type = iterator_traits::size_type;

			using iterator = BasePersistentHashMapIterator<key_type, mapped_type, typename storage_map::iterator, allocator_type>;
			using const_iterator = BasePersistentHashMapIterator<const key_type, const mapped_type, typename storage_map::const_iterator, allocator_type>;
			using reverse_iterator = BasePersistentHashMapIterator<key_type, mapped_type, typename storage_map::reverse_iterator, allocator_type>;
			using const_reverse_iterator = BasePersistentHashMapIterator<const key_type, const mapped_type, typename storage_map::const_reverse_iterator, allocator_type>;

			using insert_result = MapInsertResult<iterator>;

			static constexpr f64 loadFactor = storage_map::loadFactor;
		private:
			storage_map storageMap;
			storage_pool pool;

		public:
			//constructor
			constexpr BasePersistentHashMap() noexcept : storageMap(), pool() {}
			constexpr BasePersistentHashMap(const BasePersistentHashMap& other) noexcept 
				: storageMap(), pool() {
				assign(other);
			}
			constexpr BasePersistentHashMap(BasePersistentHashMap&& other) noexcept
				: storageMap(move(other.storageMap)), pool(move(other.pool)) {}

			//deconstructor 
			constexpr ~BasePersistentHashMap() noexcept {}

			//util 
			constexpr BasePersistentHashMap& self() noexcept { return *this; }
			constexpr const BasePersistentHashMap& self() const noexcept { return *this; }
			
			//assignment 
			constexpr BasePersistentHashMap& operator=(const BasePersistentHashMap& other) noexcept {
				return assign(other);
			}
			constexpr BasePersistentHashMap& operator=(BasePersistentHashMap&& other) noexcept {
				storageMap = move(other.storageMap);
				pool = move(other.pool);
				return self();
			}

			constexpr BasePersistentHashMap& assign(const BasePersistentHashMap& other) noexcept {
				storageMap = other.storageMap;
				pool.reserve(other.size());

				storage_array mapElements = storageMap.restrictedGetStorageArray();
				for (typename storage_array::value_type& mapElement : mapElements) {
					if (mapElement.isActive) {
						mapped_type* newValue = pool.allocate();
						construct<mapped_type>(newValue, *mapElement.value());
						mapElement.constructValue(newValue);
					}
				}

				return self();
			}

			//capacity
			constexpr size_type size() const noexcept { return storageMap.size(); }
			constexpr size_type count() const noexcept { storageMap.size(); }
			constexpr size_type capacity() const noexcept { return  storageMap.capacity(); }
			constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
			constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
			explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

			//iterators
			constexpr iterator begin() noexcept { return iterator(storageMap.begin()); }
			constexpr const_iterator begin() const noexcept { return const_iterator(storageMap.begin()); }
			constexpr const_iterator cbegin() const noexcept { return const_iterator(storageMap.cbegin()); }
			constexpr iterator end() noexcept { return iterator(storageMap.end()); }
			constexpr const_iterator end() const noexcept { return const_iterator(storageMap.end()); }
			constexpr const_iterator cend() const noexcept { return const_iterator(storageMap.cend()); }

			constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(storageMap.rbegin()); }
			constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(storageMap.rbegin()); }
			constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(storageMap.crbegin()); }
			constexpr reverse_iterator rend() noexcept { return reverse_iterator(storageMap.rend()); }
			constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(storageMap.rend()); }
			constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(storageMap.crend()); }

			//modifiers
			constexpr void clear() noexcept {
				storageMap.clear();
				pool.clear();
			}
			constexpr void reserve(const size_type& newCapacity) noexcept {
				storageMap.reserve(newCapacity);
				pool.reserve(newCapacity);
			}

			constexpr insert_result insert(const key_type& key, const mapped_type& value) noexcept {
				auto insertResult = storageMap.insert(key, nullptr);
				if (insertResult.insertSuccessful()) {
					mapped_type* newValueLocation = pool.allocate();
					insertResult.iter().deref().value() = newValueLocation;
					construct<mapped_type>(newValueLocation, value);
				}
				return insert_result(insertResult.insertSuccessful(), iterator(insertResult.iter()));
			}
			constexpr insert_result insert(key_type&& key, mapped_type&& value) noexcept {
				auto insertResult = storageMap.insert(forward<key_type>(key), nullptr);
				if (insertResult.insertSuccessful()) {
					mapped_type* newValueLocation = pool.allocate();
					insertResult.iter().deref().value() = newValueLocation;
					construct<mapped_type>(newValueLocation, forward<mapped_type>(value));
				}
				return insert_result(insertResult.insertSuccessful(), iterator(insertResult.iter()));
			}
			constexpr iterator insertWithReplacement(const key_type& key, const mapped_type& value) noexcept {
				insert_result insertResult = storageMap.insert(key, nullptr);
				if (insertResult.insertSuccessful()) {
					mapped_type valueLocation = insertResult.iter().deref().value();
					deconstruct<mapped_type>(valueLocation);
					construct<mapped_type>(valueLocation, value);
				} else {
					mapped_type* newValueLocation = pool.allocate();
					insertResult.iter().deref().value() = newValueLocation;
					construct<mapped_type>(newValueLocation, value);
				}
				return iterator(insertResult.iter());
			}
			constexpr iterator insertWithReplacement(key_type&& key, mapped_type&& value) noexcept {
				insert_result insertResult = insert(forward<key_type>(key), forward<mapped_type>(value));
				if (insertResult.insertSuccessful()) {
					mapped_type valueLocation = insertResult.iter().deref().value();
					deconstruct<mapped_type>(valueLocation);
					construct<mapped_type>(valueLocation, forward<mapped_type>(value));
				} else {
					mapped_type* newValueLocation = pool.allocate();
					insertResult.iter().deref().value() = newValueLocation;
					construct<mapped_type>(newValueLocation, forward<mapped_type>(value));
				}
				return iterator(insertResult.iter());
			}

			constexpr Bool erase(const key_type& key) noexcept {
				auto location = storageMap.findIter(key);
				return erase(location);
			}
			constexpr Bool erase(iterator location) noexcept {
				if (location == storageMap.end()) {
					return false;
				}

				mapped_type* valueLocation = location.deref().valueAsPtr();
				deconstruct<mapped_type>(valueLocation);
				pool.deallocate(valueLocation);
				storageMap.erase(location.getBase());
				return true;
			}
			constexpr Bool erase(const_iterator location) noexcept {
				if (location == storageMap.end()) {
					return false;
				}

				mapped_type* valueLocation = location.deref().valueAsPtr();
				deconstruct<mapped_type>(valueLocation);
				pool.deallocate(valueLocation);
				storageMap.erase(location.getBase());
				return true;
			}

			//lookup 
			constexpr iterator findIter(const key_type& key) noexcept {
				return iterator(storageMap.findIter(key));
			}
			constexpr const_iterator findIter(const key_type& key) const noexcept {
				return const_iterator(storageMap.findIter(key));
			}
			constexpr option_key_value_ref find(const key_type& key) noexcept {
				auto keyValueOption = storageMap.find(key);
				if (!keyValueOption.hasValue()) {
					return OptionEmpty{};
				}
				auto& keyValue = keyValueOption.value();
				return key_value_ref(keyValue.keyAsPtr(), keyValue.value());
			}
			constexpr option_key_value_const_ref find(const key_type& key) const noexcept {
				const auto keyValueOption = storageMap.find(key);
				if (!keyValueOption.hasValue()) {
					return OptionEmpty{};
				}
				const auto& keyValue = keyValueOption.value();
				return key_value_const_ref(keyValue.keyAsPtr(), keyValue.value());
			}
			constexpr Bool contains(const key_type& key) const noexcept {
				return storageMap.find(key).hasValue();
			}
		};
	}

	template<typename Key, typename DataType, Size BatchSize = 100
		, typename Hash = Hash<Key>, typename Compare = CompareEqual<Key>, typename Alloc = DefaultAllocator>
	using PersistentHashMap = impl::BasePersistentHashMap<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
		, DynArrayUnboundTypeT<Alloc>::template type, Key, DataType, Hash, Compare>;

	template<typename Key, typename DataType, typename Alloc = DefaultAllocator, 
		Size BatchSize = 100, typename Hash = Hash<Key>, typename Compare = CompareEqual<Key>>
		using PersistentHashMapAlloc = impl::BasePersistentHashMap<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
			, DynArrayUnboundTypeT<Alloc>::template type, Key, DataType, Hash, Compare>;

	template<typename Key, typename DataType, Size bufferSize, Size BatchSize = 100
		, typename Hash = Hash<Key>, typename Compare = CompareEqual<Key>, typename Alloc = DefaultAllocator>
	using SmallPersistentHashMap = impl::BasePersistentHashMap<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
			, SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, Key, DataType, Hash, Compare>;

	template<typename Key, typename DataType, Size bufferSize, typename Alloc = DefaultAllocator
		, Size BatchSize = 100, typename Hash = Hash<Key>, typename Compare = CompareEqual<Key>>
	using SmallPersistentHashMapAlloc = impl::BasePersistentHashMap<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
		, SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, Key, DataType, Hash, Compare>;

	template<
		template<typename> typename PoolType
		, template<typename> typename DynamicArrayType
		, typename KeyType, typename ValueType
		, typename Hash, typename Compare
		, typename CharType
	> struct Formatter<impl::BasePersistentHashMap<PoolType, DynamicArrayType, KeyType, ValueType, Hash, Compare>, CharType>
		: impl::MapBaseFormatImpl<
			impl::BasePersistentHashMap<PoolType, DynamicArrayType, KeyType, ValueType, Hash, Compare>,
			MakeFormatter<KeyType, CharType>,
			MakeFormatter<ValueType, CharType>> {
		using hash_map_type = impl::BasePersistentHashMap<PoolType, DynamicArrayType, KeyType, ValueType, Hash, Compare>;

		template<typename... TemplateFlags>
		struct WithTemplateFlagsFunctionImplT {
			using template_flags = TypePack<TemplateFlags...>;

			using unreduced_key_template_flags = TypePackFilter<IsKeyFormatTemplateFlagV, template_flags>;
			using unreduced_value_template_flags = TypePackFilter<IsValueFormatTemplateFlagV, template_flags>;

			using key_template_flags = TypePackTransform<ReduceToTemplateFlagsT, unreduced_key_template_flags>;
			using value_template_flags = TypePackTransform<ReduceToTemplateFlagsT, unreduced_value_template_flags>;

			using key_formatter = InstantiateFormatterWithTemplateFlagsTypePack<Decay<KeyType>, CharType, key_template_flags>;
			using value_formatter = InstantiateFormatterWithTemplateFlagsTypePack<Decay<ValueType>, CharType, value_template_flags>;

			using type = impl::MapBaseFormatImpl<hash_map_type, key_formatter, value_formatter>;
		};

		template<typename... TemplateFlags>
			requires(IsKeyOrValueFormatTemplateFlagC<TemplateFlags> && ...)
		struct WithTemplateFlags : WithTemplateFlagsFunctionImplT<TemplateFlags...>::type {};
	};
}