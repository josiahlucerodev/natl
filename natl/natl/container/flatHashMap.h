#pragma once 

//own
#include "../util/hash.h"
#include "../util/iterators.h"
#include "../util/allocator.h"
#include "../util/dataMovement.h"
#include "../processing/format.h"
#include "../fundamental/option.h"
#include "dynArray.h"
#include "smallDynArray.h"

//interface
namespace natl {
	template<typename Key, typename Value>
	struct KeyValuePair {
	public:
		using key_type = Key;
		using map_type = Value;

		Key key;
		Value value;

		//constructor 
		constexpr KeyValuePair() noexcept = default;
		constexpr KeyValuePair(const KeyValuePair& other) noexcept
			: key(other.key), value(other.value) {}
		constexpr KeyValuePair(KeyValuePair&& other) noexcept
			: key(natl::move(other.key)), value(natl::move(other.value)) {}
		constexpr KeyValuePair(const Key& keyIn, const Value& dataIn) noexcept
			: key(keyIn), value(dataIn) {}
		constexpr KeyValuePair(Key&& keyIn, Value&& dataIn) noexcept
			: key(natl::forward<Key>(keyIn)), value(natl::forward<Value>(dataIn)) {}

		//destructor
		constexpr ~KeyValuePair() noexcept = default;

		//util 
		constexpr KeyValuePair& self() noexcept { return *this; };
		constexpr const KeyValuePair& self() const noexcept { return *this; };
	};

	template<typename Key, typename Value>
	using FlatHashMapEntry = Option<KeyValuePair<Key, Value>>;

	template<typename entry_type, typename Alloc>
	class FlatHashMapIterator {
	public:
		using iterator_traits = DefaultIteratorTraits<typename entry_type::value_type>;
		using entry_iterator_traits = DefaultIteratorTraits<entry_type>;
		using iterator_category = BidirectionalIteratorTag;

		using value_type = iterator_traits::value_type;
		using reference = iterator_traits::reference;
		using const_reference = iterator_traits::const_reference;
		using pointer = iterator_traits::pointer;
		using const_pointer = iterator_traits::const_pointer;
		using difference_type = iterator_traits::difference_type;
		using size_type = iterator_traits::size_type;

		using allocator_type = Alloc::template rebind_alloc<value_type>;

		using entry_allocator_type = Alloc;
		using entry_value_type = entry_iterator_traits::value_type;
		using entry_reference = entry_iterator_traits::reference;
		using entry_const_reference = entry_iterator_traits::const_reference;
		using entry_pointer = entry_iterator_traits::pointer;
		using entry_const_pointer = entry_iterator_traits::const_pointer;
		using entry_difference_type = entry_iterator_traits::difference_type;
		using entry_size_type = entry_iterator_traits::size_type;

		using iterator = FlatHashMapIterator;

		//friends
		template<typename, typename, typename, typename, typename, typename>
		friend class BaseFlatHashMap;
	private:
		entry_pointer dataPtr;
		entry_pointer beginPtr;
		entry_pointer endPtr;
	public:
		constexpr FlatHashMapIterator() : dataPtr(nullptr), beginPtr(nullptr), endPtr(nullptr) {};
		constexpr FlatHashMapIterator(entry_pointer dataPtrIn, entry_pointer beginPtrIn, entry_pointer endPtrIn) noexcept
			: dataPtr(dataPtrIn), beginPtr(beginPtrIn), endPtr(endPtrIn) {}
	private:
		constexpr iterator& self() noexcept { return *this; }
		constexpr const iterator& self() const noexcept { return *this; }
	public:
		constexpr reference operator*() noexcept requires(IsNotConst<entry_type>) {
			return dataPtr->value();
		}
		constexpr const_reference operator*() const noexcept {
			return dataPtr->value();
		}
		constexpr pointer operator->() noexcept requires(IsNotConst<entry_type>) {
			return dataPtr->valueAsPtr();
		}
		constexpr const_pointer operator->() const noexcept {
			return dataPtr->valueAsPtr();
		}
		constexpr reference entry() noexcept requires(IsNotConst<entry_type>) {
			return dataPtr->value();
		}
		constexpr const_reference entry() const noexcept {
			return dataPtr->value();
		}
		constexpr iterator& operator++() noexcept { 
			while (true) {
				dataPtr++;
				if (dataPtr == endPtr) {
					return self();
				}
				if (dataPtr->hasValue()) { 
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
				if (dataPtr->hasValue()) { 
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

	template<class DataType>
	class FlatMapHashCompare {
	public:
		constexpr static Bool compare(const DataType& lhs, const DataType& rhs) {
			return lhs == rhs;
		}
	};

	template<
		typename DynamicArrayType,
		typename Key, typename DataType,
		typename Hash = Hash<Key>,
		typename Compare = FlatMapHashCompare<Key>,
		typename Alloc = DefaultAllocatorByte>
	class BaseFlatHashMap {
	public:
		using key_type = Key;
		using map_type = DataType;
		using key_value_pair = KeyValuePair<const key_type, map_type>;
		using entry_type = Option<key_value_pair>;

		using allocator_type = Alloc::template rebind_alloc<key_value_pair>;
		using entry_allocator_type = Alloc::template rebind_alloc<entry_type>;

		using iterator = FlatHashMapIterator<entry_type, entry_allocator_type>;
		using const_iterator = FlatHashMapIterator<const entry_type, entry_allocator_type>;
		using reverse_iterator = ReverseIterator<FlatHashMapIterator<entry_type, entry_allocator_type>>;
		using const_reverse_iterator = ReverseIterator<FlatHashMapIterator<const entry_type, entry_allocator_type>>;

		using key_value_traits = DefaultIteratorTraits<key_value_pair>;

		using value_type = key_value_traits::value_type;
		using reference = key_value_traits::reference;
		using const_reference = key_value_traits::const_reference;
		using pointer = key_value_traits::pointer;
		using const_pointer = key_value_traits::const_pointer;
		using difference_type = key_value_traits::difference_type;
		using size_type = key_value_traits::size_type;

		using optional_pointer = Option<pointer>;
		using optional_const_pointer = Option<const_pointer>;

		using entry_value_type = typename entry_allocator_type::value_type;
		using entry_reference = typename entry_allocator_type::reference;
		using entry_const_reference = typename entry_allocator_type::const_reference;
		using entry_pointer = typename entry_allocator_type::pointer;
		using entry_const_pointer = typename entry_allocator_type::const_pointer;
		using entry_difference_type = typename entry_allocator_type::difference_type;
		using entry_size_type = typename entry_allocator_type::size_type;

		static constexpr f64 load_factor = 0.7;

		//movement info 
		constexpr static Bool triviallyRelocatable = true;
		constexpr static Bool triviallyDefaultConstructible = true;
		constexpr static Bool triviallyCompareable = false;
		constexpr static Bool triviallyDestructible = false;
		constexpr static Bool triviallyConstRefConstructedable = false;
		constexpr static Bool triviallyMoveConstructedable = false;
	private:
		size_type internalSize;
		DynamicArrayType table;
	public:
		//constructor
		constexpr BaseFlatHashMap() noexcept : internalSize(0), table() {}
		constexpr BaseFlatHashMap(const BaseFlatHashMap& other) noexcept 
			: internalSize(other.internalSize), table(other.table) {}
		constexpr BaseFlatHashMap(BaseFlatHashMap&& other) noexcept 
			: internalSize(other.internalSize), table(natl::move(other.table)) {}

		//destructor
		constexpr ~BaseFlatHashMap() noexcept = default;

		//util 
		constexpr BaseFlatHashMap& self() noexcept { return *this; }
		constexpr const BaseFlatHashMap& self() const noexcept { return *this; }

		//assignment 
		constexpr BaseFlatHashMap& operator=(const BaseFlatHashMap& other) noexcept {
			internalSize = other.internalSize;
			table = other.table;
			return self();
		}

		constexpr BaseFlatHashMap& operator=(BaseFlatHashMap&& other) noexcept {
			internalSize = other.internalSize;
			table = natl::move(other.table);
			return self();
		}

		//capacity
		constexpr size_type size() const noexcept { return internalSize; }
		constexpr size_type count() const noexcept { return internalSize; }
		constexpr size_type capacity() const noexcept { return table.size(); }
		constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
		constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
		explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

		//iterators
	private:
		constexpr entry_pointer beginPtr() noexcept requires(IsNotConst<DataType>) { return table.data(); }
		constexpr entry_const_pointer beginPtr() const noexcept { return table.data(); }
		constexpr entry_pointer endPtr() noexcept requires(IsNotConst<DataType>) { return table.data() + table.size(); }
		constexpr entry_const_pointer endPtr() const noexcept { return table.data() + table.size(); }

	public:
		constexpr iterator begin() noexcept requires(IsNotConst<DataType>) { 
			entry_pointer dataPtr = beginPtr();
			iterator firstIter = iterator(dataPtr, beginPtr(), endPtr());
			if (dataPtr->hasValue()) {
				return firstIter;
			} else {
				return ++firstIter;
			}
		}
		constexpr const_iterator begin() const noexcept { 
			entry_const_pointer dataPtr = beginPtr();
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

		constexpr iterator end() noexcept requires(IsNotConst<DataType>) { return iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr(), beginPtr(), endPtr()); }

		constexpr reverse_iterator rbegin() noexcept requires(IsNotConst<DataType>) { 
			entry_pointer dataPtr = endPtr();
			reverse_iterator firstIter = reverse_iterator(dataPtr, beginPtr(), endPtr());
			if (dataPtr->hasValue()) {
				return firstIter;
			} else {
				return ++firstIter;
			}
		}
		constexpr const_reverse_iterator rbegin() const noexcept { 
			entry_const_pointer dataPtr = endPtr();
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

		constexpr reverse_iterator rend() noexcept requires(IsNotConst<DataType>) { return reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr(), beginPtr(), endPtr()); }

	public:
		//modifiers 
		constexpr void clear() noexcept {
			table.clear();
			internalSize = 0;
		}
		constexpr void reserve(const size_type& newCapacity) noexcept {
			resizeAndRehash(newCapacity);
		}

		constexpr iterator insert(const Key& key, const DataType& value) noexcept {
			resizeAndRehash();

			entry_pointer dataLocation = nullptr;
			size_type index = Hash::hash(key) % capacity();
			while (table[index].hasValue()) {
				if (Compare::compare(table[index].value().key, key)) {
					dataLocation = &table[index];
					dataLocation->value().value = value;
					return iterator(dataLocation, beginPtr(), endPtr());
				}
				index = (index + 1) % capacity();
			}

			dataLocation = &table[index];
			dataLocation->construct(key, value);
			++internalSize;

			return iterator(dataLocation, beginPtr(), endPtr());
		};

		constexpr iterator insert(Key&& key, DataType&& value) noexcept {
			resizeAndRehash();

			entry_pointer dataLocation = nullptr;
			size_type index = Hash::hash(key) % capacity();
			while (table[index].hasValue()) {
				if (Compare::compare(table[index].value().key, key)) {
					dataLocation = &table[index];
					dataLocation->value().value = natl::forward<DataType>(value);
					return iterator(dataLocation, beginPtr(), endPtr());
				}
				index = (index + 1) % capacity();
			}

			dataLocation = &table[index];
			dataLocation->construct(natl::forward<Key>(key), natl::forward<DataType>(value));
			++internalSize;

			return iterator(dataLocation, beginPtr(), endPtr());
		};

		constexpr Bool erase(const Key& key) noexcept {
			iterator location = findIterator(key);
			if (location != end()) {
				internalSize -= 1;
				location.dataPtr->deconstruct();
				return true;
			} else {
				return false;
			}
		}

		//lookups
		constexpr iterator findIterator(const Key& key) noexcept {
			if (size() == 0) { return end(); }
			size_type index = Hash::hash(key) % capacity();
			size_type originalIndex = index;

			while (true) {
				if (table[index].hasValue() && Compare::compare(table[index].value().key, key)) {
					return iterator(&table[index], beginPtr(), endPtr());
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
				if (Compare::compare(table[index].value().key, key)) {
					return const_iterator(&table[index], beginPtr(), endPtr());
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
				if (table[index].hasValue() && Compare::compare(table[index].value().key, key)) {
					return optional_pointer(table[index].valueAsPtr());
				}

				index = (index + 1) % capacity();
				if (index == originalIndex) {
					break; 
				}
			}
			return optional_pointer(natl::OptionEmpty()); 
		}

		constexpr optional_const_pointer find(const Key& key) const noexcept {
			if (size() == 0) { return optional_const_pointer(natl::OptionEmpty()); }
			size_type index = Hash::hash(key) % capacity();
			size_type originalIndex = index;

			while (true) {
				if (table[index].hasValue() && Compare::compare(table[index].value().key, key)) {
					return optional_const_pointer(table[index].valueAsPtr());
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
	private:
		constexpr void resizeAndRehash() noexcept {
			 if (internalSize >= static_cast<size_type>(static_cast<f64>(capacity()) * load_factor)) {
				if (internalSize < static_cast<size_type>(static_cast<f64>(table.capacity()) * load_factor)) {
					table.resize(table.capacity());
					return;
				}

				resizeAndRehash((capacity() + 10) * 2);
			}
		}
		constexpr void resizeAndRehash(const size_type newCapacity) noexcept {
			if (newCapacity <= capacity()) { return; }

			DynamicArrayType newTable(newCapacity);

			for (entry_type& entry : table) {
				if (entry.hasValue()) {
					size_type index = Hash::hash(entry.value().key) % newCapacity;

					while (newTable[index].hasValue()) {
						index = (index + 1) % newCapacity;
					}

					newTable[index] = natl::forward<entry_type>(entry);
				}
			}
			table = natl::move(newTable);
		}
	};

	template<class Key, class DataType,
		class Hash = Hash<Key>,
		class Compare = FlatMapHashCompare<Key>,
		class Alloc = DefaultAllocatorByte>
	using FlatHashMap = BaseFlatHashMap<DynArray<FlatHashMapEntry<const Key, DataType>, typename Alloc::template rebind_alloc<FlatHashMapEntry<const Key, DataType>>>, Key, DataType, Hash, Compare, Alloc>;

	template<class Key, class DataType,
		Size bufferSize,
		class Hash = Hash<Key>,
		class Compare = FlatMapHashCompare<Key>,
		class Alloc = DefaultAllocatorByte>
	using SmallFlatHashMap = BaseFlatHashMap<SmallDynArray<FlatHashMapEntry<const Key, DataType>, bufferSize, typename Alloc::template rebind_alloc<FlatHashMapEntry<const Key, DataType>>>, Key, DataType, Hash, Compare, Alloc>;

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
		struct IsFormatKeyV<FormatKey<TemplateFlags...>> : TrueType{};
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
				: argStorage(natl::forward<ArgTypes>(formatArgs)...) {}
		};

		template<typename... ArgTypes>
		struct FormatValueArg {
			using arg_types = TypePack<ArgTypes...>;
			using storage_tuple_type = Tuple<ArgTypes&&...>;
		public:
			storage_tuple_type argStorage;
			constexpr FormatValueArg(ArgTypes&&... formatArgs) noexcept
				: argStorage(natl::forward<ArgTypes>(formatArgs)...) {}
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
					outputIter = KeyFormatter::format(outputIter, entry.key);
					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';
					outputIter = ValueFormatter::format(outputIter, entry.value);
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
						outputIter = KeyFormatter::format(outputIter, entry.key);
					} else {
						outputIter = KeyFormatter::format(outputIter, entry.key,
							natl::forward<KeyFormatArgsTypes>(formatKeyArgs.argStorage.template get<KeyIndices>())...);
					}

					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';
					outputIter = ValueFormatter::format(outputIter, entry.value);
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
						outputIter = KeyFormatter::format(outputIter, entry.key);
					} else {
						outputIter = KeyFormatter::format(outputIter, entry.key,
							natl::forward<KeyFormatArgsTypes>(formatKeyArgs.argStorage.template get<KeyIndices>())...);
					}

					outputIter = ' ';
					outputIter = ':';
					outputIter = ' ';

					if constexpr (sizeof...(KeyFormatArgsTypes) == 0) {
						outputIter = ValueFormatter::format(outputIter, entry.value);
					} else {
						outputIter = ValueFormatter::format(outputIter, entry.value,
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
		typename DynamicArrayType,
		typename Key, typename Value,
		typename Hash, typename Compare, typename Alloc,
		typename CharType>
	struct Formatter<BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare, Alloc>, CharType> 
		: impl::BaseFlatHashMapBaseFormatImpl<
			BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare, Alloc>,
			MakeFormatter<Key, CharType>,
			MakeFormatter<Value, CharType>
		> {
		using value_type = BaseFlatHashMap<DynamicArrayType, Key, Value, Hash, Compare, Alloc>;
		using hash_map_type = value_type;

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

//additional includes for end use
#include "../util/commonHashs.h"