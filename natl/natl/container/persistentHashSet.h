#pragma once 

//own
#include "flatHashSet.h"
#include "batchPool.h"

//interface 
namespace natl {
	namespace impl {
		template<typename DataType, typename storage_iterator, typename Alloc>
			requires(IsAllocatorC<Alloc>)
		struct BasePersistentHashSetIterator {
		public:
			using key_type = DataType;

			using iterator_traits = DefaultIteratorTraits<DataType>;
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
			using iterator = BasePersistentHashSetIterator;
			
		private:
			storage_iterator baseIterator;

		public:
			//constructor 
			constexpr BasePersistentHashSetIterator() noexcept = default;
			constexpr BasePersistentHashSetIterator(storage_iterator baseIteratorIn) noexcept
				: baseIterator(baseIteratorIn) {
			}

			//destructor 
			constexpr ~BasePersistentHashSetIterator() noexcept = default;

			//util
			constexpr iterator& self() noexcept { return *this; }
			constexpr const iterator& self() const noexcept { return *this; }

			//iter ops
			constexpr storage_iterator getBase() const noexcept { return baseIterator; }

			constexpr reference operator*() noexcept requires(IsNotConst<DataType>) { 
				return **baseIterator;
			}
			constexpr const_reference operator*() const noexcept {
				return **baseIterator;
			}
			constexpr pointer operator->() noexcept requires(IsNotConst<DataType>) {
				return *baseIterator;
			}
			constexpr const_pointer operator->() const noexcept {
				return *baseIterator;
			}
			constexpr reference deref() noexcept requires(IsNotConst<DataType>) {
				return **baseIterator;
			}
			constexpr const_reference deref() const noexcept {
				return **baseIterator;
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
			, typename DataType
			, typename Hash = Hash<DataType>
			, typename Compare = CompareEqual<DataType>
		> struct BasePersistentHashSet {
			using value_type = DataType;

			struct ValueHash {
				constexpr static Size hash(const value_type* value) {
					return hashValue<value_type>(*value);
				}
			};
			struct ValueCompare {
				constexpr static Bool compare(const value_type* lhs, const value_type* rhs) noexcept {
					return *lhs == *rhs;
				}
				constexpr Bool operator()(const value_type* lhs, const value_type*& rhs) noexcept {
					return *lhs == *rhs;
				}
			};

			using storage_pool = PoolType<DataType>;
			using storage_set = impl::BaseFlatHashSet<DynamicArrayType, value_type*, ValueHash, ValueCompare>;
			using storage_array = storage_set::storage_array;

			using allocator_type = storage_set::allocator_type;
			using typed_allocator_type = storage_set::typed_allocator_type;

			using storage_map_iterator = typename storage_set::iterator;

			using iterator_traits = DefaultIteratorTraits<value_type>;
			using iterator_category = BidirectionalIteratorTag;

			//using value_type = iterator_traits::value_type;
			using reference = iterator_traits::reference;
			using const_reference = iterator_traits::const_reference;
			using pointer = iterator_traits::pointer;
			using const_pointer = iterator_traits::const_pointer;
			using difference_type = iterator_traits::difference_type;
			using size_type = iterator_traits::size_type;

			using iterator = BasePersistentHashSetIterator<value_type, typename storage_set::iterator, allocator_type>;
			using const_iterator = BasePersistentHashSetIterator<const value_type, typename storage_set::const_iterator, allocator_type>;
			using reverse_iterator = BasePersistentHashSetIterator<value_type, typename storage_set::reverse_iterator, allocator_type>;
			using const_reverse_iterator = BasePersistentHashSetIterator<const value_type, typename storage_set::const_reverse_iterator, allocator_type>;

			using insert_result = SetInsertResult<iterator>;

			using option_value_type = OptionPtr<value_type>;
			using const_option_value_type = OptionPtr<const value_type>;

			static constexpr f64 loadFactor = storage_set::loadFactor;

		private:
			storage_set storageSet;
			storage_pool pool;

		public:
			//constructor
			constexpr BasePersistentHashSet() noexcept : storageSet(), pool() {}
			constexpr BasePersistentHashSet(const BasePersistentHashSet& other) noexcept
				: storageSet(), pool() {
				assign(other);
			}
			constexpr BasePersistentHashSet(BasePersistentHashSet&& other) noexcept
				: storageSet(move(other.storageSet)), pool(move(other.pool)) {}

			//deconstructor 
			constexpr ~BasePersistentHashSet() noexcept {}

			//util 
			constexpr BasePersistentHashSet& self() noexcept { return *this; }
			constexpr const BasePersistentHashSet& self() const noexcept { return *this; }

			//assignment 
			constexpr BasePersistentHashSet& operator=(const BasePersistentHashSet& other) noexcept {
				return assign(other);
			}
			constexpr BasePersistentHashSet& operator=(BasePersistentHashSet&& other) noexcept {
				storageSet = move(other.storageSet);
				pool = move(other.pool);
				return self();
			}

			constexpr BasePersistentHashSet& assign(const BasePersistentHashSet& other) noexcept {
				storageSet = other.storageSet;
				pool.reserve(other.size());

				const storage_array& setElements = storageSet.restrictedGetStorageArray();
				for (const typename storage_array::value_type& setElement : setElements) {
					if (setElement.hasValue()) {
						value_type* newValue = pool.allocateConstructed();
						*newValue = *setElement.value();
						setElement.constructValue(newValue);
					}
				}

				return self();
			}

			//capacity
			constexpr size_type size() const noexcept { return storageSet.size(); }
			constexpr size_type count() const noexcept { storageSet.size(); }
			constexpr size_type capacity() const noexcept { return  storageSet.capacity(); }
			constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
			constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
			explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

			//iterators
			constexpr iterator begin() noexcept { return iterator(storageSet.begin()); }
			constexpr const_iterator begin() const noexcept { return const_iterator(storageSet.begin()); }
			constexpr const_iterator cbegin() const noexcept { return const_iterator(storageSet.cbegin()); }
			constexpr iterator end() noexcept { return iterator(storageSet.end()); }
			constexpr const_iterator end() const noexcept { return const_iterator(storageSet.end()); }
			constexpr const_iterator cend() const noexcept { return const_iterator(storageSet.cend()); }

			constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(storageSet.rbegin()); }
			constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(storageSet.rbegin()); }
			constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(storageSet.crbegin()); }
			constexpr reverse_iterator rend() noexcept { return reverse_iterator(storageSet.rend()); }
			constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(storageSet.rend()); }
			constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(storageSet.crend()); }

			//modifiers
			constexpr void clear() noexcept {
				storageSet.clear();
				pool.clear();
			}
			constexpr void reserve(const size_type& newCapacity) noexcept {
				storageSet.reserve(newCapacity);
				pool.reserve(newCapacity);
			}

			constexpr insert_result insert(const value_type& value) noexcept {
				value_type tempValue = value;
				auto insertResult = storageSet.insert(&tempValue);
				if (insertResult.insertSuccessful()) {
					value_type* newValueLocation = pool.allocateConstructed();
					*newValueLocation = value;
					*insertResult.iter() = newValueLocation;
				}
				return insert_result(insertResult.insertSuccessful(), iterator(insertResult.iter()));
			}
			constexpr insert_result insert(value_type&& value) noexcept {
				auto insertResult = storageSet.insert(&value);
				if (insertResult.insertSuccessful()) {
					value_type* newValueLocation = pool.allocateConstructed();
					*newValueLocation = forward<value_type>(value);
					*insertResult.iter() = newValueLocation;
				}
				return insert_result(insertResult.insertSuccessful(), iterator(insertResult.iter()));
			}

			constexpr iterator insertWithReplacement(const value_type& value) noexcept {
				value_type tempValue = value;
				auto insertResult = storageSet.insert(&tempValue);
				if (insertResult.insertSuccessful()) {
					value_type* newValueLocation = pool.allocateConstructed();
					*newValueLocation = value;
					*insertResult.iter() = newValueLocation;
				} else {
					*insertResult.iter() = value;
				}
				return iterator(insertResult.iter());
			}
			constexpr iterator insertWithReplacement(value_type&& value) noexcept {
				auto insertResult = storageSet.insert(&value);
				if (insertResult.insertSuccessful()) {
					value_type* newValueLocation = pool.allocateConstructed();
					*newValueLocation = forward<value_type>(value);
					*insertResult.iter() = newValueLocation;
				} else {
					*insertResult.iter() = forward<value_type>(value);
				}
				return iterator(insertResult.iter());
			}

			constexpr Bool erase(const value_type& value) noexcept {
				auto location = storageSet.findIter(value);
				return erase(location);
			}
			constexpr Bool erase(iterator location) noexcept {
				if (location == storageSet.end()) {
					return false;
				}

				value_type* valueLocation = location->value();
				pool.recycle(valueLocation);
				storageSet.erase(location.getBase());
				return true;
			}
			constexpr Bool erase(const_iterator location) noexcept {
				if (location == storageSet.end()) {
					return false;
				}

				value_type* valueLocation = location->value();
				pool.recycle(valueLocation);
				storageSet.erase(location.getBase());
				return true;
			}

			//lookups 
			constexpr iterator findIter(const value_type& value) noexcept {
				return iterator(storageSet.findIter(value));
			}
			constexpr const_iterator findIter(const value_type& value) const noexcept {
				return const_iterator(storageSet.findIter(value));
			}
			constexpr option_value_type find(const value_type& value) noexcept {
				auto foundOption = storageSet.find(value);
				if (foundOption.hasValue()) {
					return OptionEmpty{};
				}
				return option_value_type(foundOption.value());
			}
			constexpr const_option_value_type find(const value_type& value) const noexcept {
				auto foundOption = storageSet.find(value);
				if (foundOption.hasValue()) {
					return OptionEmpty{};
				}
				return const_option_value_type(foundOption.value());
			}
			constexpr Bool contains(const value_type& value) const noexcept {
				return storageSet.find(value).hasValue();
			}
		};
	}

	template<typename DataType, Size BatchSize = defaultBatchSize
		, typename Hash = Hash<DataType>, typename Compare = CompareEqual<DataType>, typename Alloc = DefaultAllocator>
	using PersistentHashSet = impl::BasePersistentHashSet<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
		, DynArrayUnboundTypeT<Alloc>::template type, DataType, Hash, Compare>;

	template<typename DataType, typename Alloc = DefaultAllocator,
		Size BatchSize = defaultBatchSize, typename Hash = Hash<DataType>, typename Compare = CompareEqual<DataType>>
		using PersistentHashSetAlloc = impl::BasePersistentHashSet<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
		, DynArrayUnboundTypeT<Alloc>::template type, DataType, Hash, Compare>;

	template<typename DataType, Size bufferSize, Size BatchSize = defaultBatchSize
		, typename Hash = Hash<DataType>, typename Compare = CompareEqual<DataType>, typename Alloc = DefaultAllocator>
	using SmallPersistentHashSet = impl::BasePersistentHashSet<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
		, SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, DataType, Hash, Compare>;

	template<typename DataType, Size bufferSize, typename Alloc = DefaultAllocator
		, Size BatchSize = defaultBatchSize, typename Hash = Hash<DataType>, typename Compare = CompareEqual<DataType>>
		using SmallPersistentHashSetAlloc = impl::BasePersistentHashSet<
		BatchPoolUnboundTypeT<BatchSize, Alloc>::template type
		, SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, DataType, Hash, Compare>;

	template<
		template<typename> typename PoolType
		, template<typename> typename DynamicArrayType
		, typename DataType, typename Hash, typename Compare
		, typename CharType
	> struct Formatter<impl::BasePersistentHashSet<PoolType, DynamicArrayType, DataType, Hash, Compare>, CharType>
		: impl::SetBaseFormatImpl<
		impl::BasePersistentHashSet<PoolType, DynamicArrayType, DataType, Hash, Compare>,
		MakeFormatter<DataType, DataType>> {
		using hash_set_type = impl::BasePersistentHashSet<PoolType, DynamicArrayType, DataType, Hash, Compare>;

		template<typename... TemplateFlags>
		struct WithTemplateFlagsImplT {
			using template_flags = TypePack<TemplateFlags...>;
			using unreduced_value_template_flags = TypePackFilter<IsValueFormatTemplateFlagV, template_flags>;
			using value_template_flags = TypePackTransform<ReduceToTemplateFlagsT, unreduced_value_template_flags>;
			using value_formatter = InstantiateFormatterWithTemplateFlagsTypePack<Decay<DataType>, CharType, value_template_flags>;
			using type = impl::SetBaseFormatImpl<hash_set_type, value_formatter>;
		};

		template<typename... TemplateFlags>
			requires(IsValueFormatTemplateFlagC<TemplateFlags> && ...)
		struct WithTemplateFlags : WithTemplateFlagsImplT<TemplateFlags...>::type {};
	};
}