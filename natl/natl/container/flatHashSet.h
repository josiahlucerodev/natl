#pragma once 

//own
#include "../util/hash.h"
#include "../util/iterators.h"
#include "../util/allocator.h"
#include "../util/dataMovement.h"
#include "../util/funcArgs.h"
#include "../util/compare.h"
#include "../processing/format.h"
#include "../util/option.h"
#include "dynArray.h"
#include "smallDynArray.h"

//@export
namespace natl {
	template<typename IterType>
	struct SetInsertResult {
	public:
		using iterator = IterType;

	private:
		Bool successful;
		iterator location;

	public:
		//constructor
		constexpr SetInsertResult() noexcept = default;
		constexpr SetInsertResult(Bool successfulIn, iterator locationIn) noexcept
			: successful(successfulIn), location(locationIn) {
		};

		//deconstructor 
		constexpr ~SetInsertResult() noexcept = default;

		//accessors
		constexpr Bool insertFailed() const noexcept { return !successful; }
		constexpr Bool insertSuccessful() const noexcept { return successful; }
		explicit constexpr operator Bool() const noexcept { return successful; }
		constexpr iterator iter() noexcept { return location; }
	};

	namespace impl {
		template<typename unit_type, typename Alloc>
			requires(IsAllocatorC<Alloc>)
		struct BaseFlatHashSetIterator {
		public:
			using iterator_traits = DefaultIteratorTraits<typename unit_type::value_type>;
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
			using iterator = BaseFlatHashSetIterator;

			//friends
			template<template<typename> typename, typename, typename, typename, typename>
			friend struct BaseFlatHashMap;
		private:
			unit_type* dataPtr;
			unit_type* beginPtr;
			unit_type* endPtr;
		public:
			constexpr BaseFlatHashSetIterator() : dataPtr(nullptr), beginPtr(nullptr), endPtr(nullptr) {};
			constexpr BaseFlatHashSetIterator(unit_type* dataPtrIn, unit_type* beginPtrIn, unit_type* endPtrIn) noexcept
				: dataPtr(dataPtrIn), beginPtr(beginPtrIn), endPtr(endPtrIn) {
			}
		private:
			constexpr iterator& self() noexcept { return *this; }
			constexpr const iterator& self() const noexcept { return *this; }
		public:
			constexpr reference operator*() noexcept requires(IsNotConst<unit_type>) { return dataPtr->valueAsRef(); }
			constexpr const_reference operator*() const noexcept { return dataPtr->valueAsRef(); }
			constexpr pointer operator->() noexcept requires(IsNotConst<unit_type>) { return dataPtr->valueAsPtr(); }
			constexpr const_pointer operator->() const noexcept { return dataPtr->valueAsPtr(); }
			constexpr reference deref() noexcept requires(IsNotConst<unit_type>) { return dataPtr->valueAsRef(); }
			constexpr const_reference deref() const noexcept { return dataPtr->valueAsRef(); }
			constexpr unit_type& storage() noexcept requires(IsNotConst<unit_type>) { return *dataPtr; }
			constexpr const unit_type& storage() const noexcept { return *dataPtr; }

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

		template<
			template<typename> typename DynamicArrayType,
			typename DataType,
			typename Hash = Hash<DataType>,
			typename Compare = CompareEqual<DataType>
		> struct BaseFlatHashSet {
		public:
			using key_type = DataType;

			using unit_type = Option<key_type>;
			using storage_array = DynamicArrayType<unit_type>;
			using allocator_type = storage_array::allocator_type;
			using typed_allocator_type = allocator_type::template rebind<DataType>;

			using iterator_traits = DefaultIteratorTraits<unit_type>;
			using iterator_category = BidirectionalIteratorTag;

			using value_type = typed_allocator_type::value_type;
			using reference = typed_allocator_type::reference;
			using const_reference = typed_allocator_type::const_reference;
			using pointer = typed_allocator_type::pointer;
			using const_pointer = typed_allocator_type::const_pointer;
			using difference_type = typed_allocator_type::difference_type;
			using size_type = typed_allocator_type::size_type;

			using option_value_type = OptionPtr<value_type>;
			using const_option_value_type = OptionPtr<const value_type>;

			using iterator = BaseFlatHashSetIterator<unit_type, allocator_type>;
			using const_iterator = BaseFlatHashSetIterator<const unit_type, allocator_type>;
			using reverse_iterator = ReverseIterator<BaseFlatHashSetIterator<unit_type, allocator_type>>;
			using const_reverse_iterator = ReverseIterator<BaseFlatHashSetIterator<const unit_type, allocator_type>>;

			using insert_result = SetInsertResult<iterator>;

			static constexpr f64 loadFactor = 0.75;
		private:
			size_type setSize;
			storage_array storageArray;

		public:
			//constructor
			constexpr BaseFlatHashSet() noexcept : setSize(0), storageArray() {}
			constexpr BaseFlatHashSet(const BaseFlatHashSet& other) noexcept
				: setSize(other.setSize), storageArray(other.storageArray) {
			}
			constexpr BaseFlatHashSet(BaseFlatHashSet&& other) noexcept
				: setSize(other.setSize), storageArray(move(other.storageArray)) {
			}
			//deconstructor 
			constexpr ~BaseFlatHashSet() noexcept {}

			//util 
			constexpr BaseFlatHashSet& self() noexcept { return *this; }
			constexpr const BaseFlatHashSet& self() const noexcept { return *this; }


			//assignment 
			constexpr BaseFlatHashSet& operator=(const BaseFlatHashSet& other) noexcept {
				setSize = other.setSize;
				storageArray = other.storageArray;
				return self();
			}
			constexpr BaseFlatHashSet& operator=(BaseFlatHashSet&& other) noexcept {
				setSize = other.setSize;
				storageArray = natl::move(other.storageArray);
				return self();
			}

			//capacity
			constexpr size_type size() const noexcept { return setSize; }
			constexpr size_type count() const noexcept { return setSize; }
			constexpr size_type capacity() const noexcept { return storageArray.size(); }
			constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
			constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
			explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

			//iterators
		private:
			constexpr unit_type* beginPtr() noexcept { return storageArray.data(); }
			constexpr const unit_type* beginPtr() const noexcept { return storageArray.data(); }
			constexpr unit_type* endPtr() noexcept { return storageArray.data() + storageArray.size(); }
			constexpr const unit_type* endPtr() const noexcept { return storageArray.data() + storageArray.size(); }

		public:
			constexpr iterator begin() noexcept {
				unit_type* dataPtr = beginPtr();
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
				const unit_type* dataPtr = beginPtr();
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
				unit_type* dataPtr = endPtr();
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
				const unit_type* dataPtr = endPtr();
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
				setSize = 0;
			}
			constexpr void reserve(const size_type& newCapacity) noexcept {
				resizeAndRehash(static_cast<size_type>(static_cast<f64>(newCapacity) / loadFactor));
			}

			constexpr insert_result insert(const value_type& value) noexcept {
				resizeAndRehash();

				unit_type* dataLocation = nullptr;
				size_type index = Hash::hash(value) % capacity();
				while (storageArray[index].hasValue()) {
					if (Compare::compare(storageArray[index].value(), value)) {
						return insert_result(false, iterator(dataLocation, beginPtr(), endPtr()));
					}
					index = (index + 1) % capacity();
				}

				dataLocation = &storageArray[index];
				*dataLocation = value;
				++setSize;

				return insert_result(true, iterator(dataLocation, beginPtr(), endPtr()));
			};

			constexpr insert_result insert(value_type&& value) noexcept {
				resizeAndRehash();

				unit_type* dataLocation = nullptr;
				size_type index = Hash::hash(value) % capacity();
				while (storageArray[index].hasValue()) {
					if (Compare::compare(storageArray[index].value(), value)) {
						return insert_result(false, iterator(dataLocation, beginPtr(), endPtr()));
					}
					index = (index + 1) % capacity();
				}

				dataLocation = &storageArray[index];
				*dataLocation = natl::forward<value_type>(value);
				++setSize;

				return insert_result(true, iterator(dataLocation, beginPtr(), endPtr()));
			};

			constexpr iterator insertWithReplacement(const value_type& value) noexcept {
				insert_result insertResult = insert(value);
				if (insertResult.insertFailed()) {
					*insertResult.iter() = value;
				}
				return insertResult.iter();
			}

			constexpr iterator insertWithReplacement(value_type&& value) noexcept {
				insert_result insertResult = insert(forward<value_type>(value));
				if (insertResult.insertFailed()) {
					*insertResult.iter() = forward<value_type>(value);
				}
				return insertResult.iter();
			}

			constexpr Bool erase(const value_type& value) noexcept {
				iterator location = findIter(value);
				return erase(location);
			}
			constexpr Bool erase(iterator location) noexcept {
				if (location != end()) {
					setSize -= 1;
					location.storage().reset();
					return true;
				} else {
					return false;
				}
			}
			constexpr Bool erase(const_iterator location) noexcept {
				if (location != end()) {
					setSize -= 1;
					location.storage().reset();
					return true;
				} else {
					return false;
				}
			}

			//lookups
			constexpr iterator findIter(const value_type& value) noexcept {
				if (size() == 0) { return end(); }
				size_type index = Hash::hash(value) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (storageArray[index].hasValue() && Compare::compare(storageArray[index].value(), value)) {
						return iterator(&storageArray[index], beginPtr(), endPtr());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return end();
			}

			constexpr const_iterator findIter(const value_type& value) const noexcept {
				if (size() == 0) { return end(); }
				size_type index = Hash::hash(value) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (Compare::compare(storageArray[index].value(), value)) {
						return const_iterator(&storageArray[index], beginPtr(), endPtr());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return end();
			}

			constexpr option_value_type find(const value_type& value) noexcept {
				if (size() == 0) { return option_value_type(natl::OptionEmpty()); }
				size_type index = Hash::hash(value) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (storageArray[index].hasValue() && Compare::compare(storageArray[index].value(), value)) {
						return option_value_type(&storageArray[index].value());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return option_value_type(natl::OptionEmpty());
			}

			constexpr const_option_value_type find(const value_type& value) const noexcept {
				if (size() == 0) { return const_option_value_type(natl::OptionEmpty()); }
				size_type index = Hash::hash(value) % capacity();
				size_type originalIndex = index;

				while (true) {
					if (storageArray[index].hasValue() && Compare::compare(storageArray[index].value(), value)) {
						return const_option_value_type(&storageArray[index].value());
					}

					index = (index + 1) % capacity();
					if (index == originalIndex) {
						break;
					}
				}
				return const_option_value_type();
			}

			constexpr Bool contains(const value_type& value) const noexcept {
				return find(value).hasValue();
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
					if (entry.hasValue()) {
						size_type index = Hash::hash(entry.value()) % newCapacity;
						while (newTable[index].hasValue()) {
							index = (index + 1) % newCapacity;
						}
						newTable[index] = forward<decltype(entry)>(entry);
					}
				}
				storageArray = natl::move(newTable);
			}
		};
	}

	template<typename DataType,
		typename Hash = Hash<DataType>,
		typename Compare = CompareEqual<DataType>,
		typename Alloc = DefaultAllocator>
	using FlatHashSet = impl::BaseFlatHashSet<DynArrayUnboundTypeT<Alloc>::template type, DataType, Hash, Compare>;

	template<typename DataType,
		typename Alloc = DefaultAllocator,
		typename Hash = Hash<DataType>,
		typename Compare = CompareEqual<DataType>>
	using FlatHashSetAlloc = impl::BaseFlatHashSet<DynArrayUnboundTypeT<Alloc>::template type, DataType, Hash, Compare>;

	template<typename DataType, Size bufferSize,
		typename Hash = Hash<DataType>,
		typename Compare = CompareEqual<DataType>,
		typename Alloc = DefaultAllocator>
	using SmallFlatHashSet = impl::BaseFlatHashSet<SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, DataType, Hash, Compare>;

	template<typename DataType, Size bufferSize,
		typename Alloc = DefaultAllocator,
		typename Hash = Hash<DataType>,
		typename Compare = CompareEqual<DataType>>
	using SmallFlatHashSetAlloc = impl::BaseFlatHashSet<SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, DataType, Hash, Compare>;

	namespace impl {
		template<typename SetType, typename ValueFormatter>
		struct SetBaseFormatImpl {
			template<typename Type>
			constexpr static auto getValue(const Type& value) noexcept {
				if constexpr (requires(const Type& value) { value.value(); }) {
					return value.value();
				} else {
					return value;
				}
			}

			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const SetType& set) noexcept {
				Size index = 0;
				const Size setSize = set.size();

				outputIter = '{';
				for (typename SetType::const_reference entry : set) {
					outputIter = ValueFormatter::format(outputIter, getValue(entry));
					if (index != setSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
				return outputIter;
			}

			template<typename OutputIter, typename... ValueFormatArgsTypes>
			constexpr static OutputIter format(OutputIter outputIter, const SetType& set,
				impl::FormatValueArg<ValueFormatArgsTypes...>&& formatValueArgs) noexcept {
				Size index = 0;
				const Size setSize = set.size();

				outputIter = '{';
				for (typename SetType::const_reference entry : set) {
					[&]<Size... Indices> (IndexSequence<Indices...>) {
						outputIter = ValueFormatter::format(outputIter, getValue(entry),
							natl::forward<ValueFormatArgsTypes>(formatValueArgs.argStorage.template get<Indices>())...);
					}(MakeIndexSequence<sizeof...(ValueFormatArgsTypes)>{});

					if (index != setSize - 1) {
						outputIter = ',';
						outputIter = ' ';
					}

					index++;
				}
				outputIter = '}';
				return outputIter;
			}
		};
	}

	template<
		template<typename> typename DynamicArrayType,
		typename DataType, typename Hash, typename Compare,
		typename CharType
	> struct Formatter<impl::BaseFlatHashSet<DynamicArrayType, DataType, Hash, Compare>, CharType>
		: impl::SetBaseFormatImpl<
			impl::BaseFlatHashSet<DynamicArrayType, DataType, Hash, Compare>,
			MakeFormatter<DataType, DataType>> {
		using hash_set_type = impl::BaseFlatHashSet<DynamicArrayType, DataType, Hash, Compare>;

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