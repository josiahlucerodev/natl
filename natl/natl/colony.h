#pragma once 

//own
#include "allocator.h"
#include "limits.h"
#include "algorithm.h"
#include "dataMovement.h"
#include "memory.h"
#include "stringConvert.h"
#include "option.h"
#include "numerics.h"

//interface 
namespace natl {
	using ColonySkipFieldType = bool;
	struct ColonyLimits {
		Size minGroupSize;
		Size maxGroupSize;
		constexpr ColonyLimits() noexcept = default;
		constexpr ColonyLimits(const Size minGroupSizeIn, const Size maxGroupSizeIn) noexcept : 
			minGroupSize(minGroupSizeIn), maxGroupSize(maxGroupSizeIn) {}
	};

	template<typename DataType>
	class ColonyBlock {
	public:
		using SkipFieldType = ColonySkipFieldType;
	public:
		Size index;
		Size colonyBlockSize;
		Size colonyBlockCapacity;
		ColonyBlock* previousColonyBlock;
		ColonyBlock* nextColonyBlock;
		SkipFieldType* skipField;

		Size emptySkipFieldStartIndex;
		SkipFieldType* emptySkipFieldStartPos;


		DataType* data;
		Byte* blockByteStorage;
		Size byteSize;

		constexpr bool isFull() noexcept {
			return colonyBlockSize == colonyBlockCapacity;
		}
	};

	template<typename DataType, typename Alloc>
	class ColonyIterator {
	public:
		using iterator = ColonyIterator;

		using allocator_type = Alloc;

		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;

		using colony_block = ColonyBlock<std::remove_cv_t<DataType>>;
		using SkipFieldType = ColonySkipFieldType;
	public:
		mutable colony_block* colonyBlock;
		natl::i64 skipFieldIndex;
		mutable SkipFieldType* skipFieldPos;
	public:
		//construtor
		constexpr ColonyIterator() noexcept = default;
		constexpr ColonyIterator(colony_block* colonyBlockIn, const natl::i64 skipFieldIndexIn, SkipFieldType* skipFieldPosIn) noexcept :
			colonyBlock(colonyBlockIn), skipFieldIndex(skipFieldIndexIn), skipFieldPos(skipFieldPosIn) {}

		//destructor 
		constexpr ~ColonyIterator() noexcept = default;

		//util
	private:
		constexpr iterator& self() noexcept { return *this; }
		constexpr const iterator& self() const noexcept { return *this; }
	public:
		using const_iterator = ColonyIterator<const DataType, Alloc>;
		constexpr operator const_iterator() const noexcept requires(IsNotConstV<DataType>) {
			return const_iterator(colonyBlock, skipFieldIndex, skipFieldPos);
		}

		constexpr pointer getAddress() noexcept requires(IsNotConstV<DataType>) { return &colonyBlock->data[skipFieldIndex]; }
		constexpr const_pointer getAddress() const noexcept { return &colonyBlock->data[skipFieldIndex]; }

		constexpr reference operator*() noexcept requires(IsNotConstV<DataType>) { return colonyBlock->data[skipFieldIndex]; }
		constexpr const_reference operator*() const noexcept { return colonyBlock->data[skipFieldIndex]; }
		constexpr pointer operator->() noexcept requires(IsNotConstV<DataType>) { return &colonyBlock->data[skipFieldIndex]; }
		constexpr const_pointer operator->() const noexcept { return &colonyBlock->data[skipFieldIndex]; }

		constexpr bool operator==(const iterator rhs) const noexcept { 
			return colonyBlock == rhs.colonyBlock && skipFieldIndex == rhs.skipFieldIndex;
		}
		constexpr bool operator!= (const iterator rhs) const noexcept { 
			return !(self() == rhs);
		}
		constexpr bool operator<(const iterator rhs) const noexcept { 
			return colonyBlock->index <= rhs.colonyBlock->index && skipFieldIndex < rhs.skipFieldIndex;
		}
		constexpr bool operator<=(const iterator rhs) const noexcept { 
			return colonyBlock->index <= rhs.colonyBlock->index && skipFieldIndex <= rhs.skipFieldIndex;
		}
		constexpr bool operator>(const iterator rhs) const noexcept { 
			return colonyBlock->index >= rhs.colonyBlock->index && skipFieldIndex > rhs.skipFieldIndex;
		}
		constexpr bool operator>=(const iterator rhs) const noexcept { 
			return colonyBlock->index >= rhs.colonyBlock->index && skipFieldIndex >= rhs.skipFieldIndex;
		}

		constexpr iterator& operator++() noexcept { 
			while (true) {
				skipFieldPos += 1;
				skipFieldIndex += 1;

				if (skipFieldIndex >= static_cast<i64>(colonyBlock->colonyBlockCapacity)) {
					if (colonyBlock->nextColonyBlock) {
						colonyBlock = colonyBlock->nextColonyBlock;
						skipFieldIndex = 0;
						skipFieldPos = colonyBlock->skipField;
					} else {
						return self();
					}
				}

				if (*skipFieldPos) {
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
			while (true) {
				skipFieldIndex -= 1;
				skipFieldPos -= 1;

				if (skipFieldIndex == 0) {
					if (colonyBlock->previousColonyBlock) {
						colonyBlock = colonyBlock->previousColonyBlock;
						skipFieldIndex = colonyBlock->colonyBlockCapacity - 1;
						skipFieldPos = colonyBlock->skipField + skipFieldIndex;
					} else {
						return self();
					}
				} else {
					if (*skipFieldPos) {
						return self();
					}
				}
			}
		}
		constexpr iterator operator--(int) noexcept { 
			iterator tempIt = self(); 
			--self();
			return tempIt; 
		}


	public:
	};

	template<typename DataType, typename Alloc = DefaultAllocator<DataType>>
		requires(IsAllocator<Alloc>)
	class Colony {
	public:
		using allocator_type = Alloc;

		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;

		using iterator = ColonyIterator<DataType, Alloc>;
		using const_iterator = ColonyIterator<const DataType, Alloc>;
		using reverse_iterator = ReverseIterator<ColonyIterator<DataType, Alloc>>;
		using const_reverse_iterator = ReverseIterator<ColonyIterator<const DataType, Alloc>>;

		using SkipFieldType = ColonySkipFieldType;
		using colony_block = ColonyBlock<std::remove_cv_t<DataType>>;
	private:
		Size colonySize;
		Size colonyCapacity;
		colony_block* headColonyBlock;
		colony_block* tailColonyBlock;
		Size internalGrowthFactor;
		ColonyLimits internalColonyLimits;
		Size index;
	public:
		//constructor
		constexpr Colony() noexcept : 
			colonySize(0), colonyCapacity(0), 
			headColonyBlock(nullptr), tailColonyBlock(nullptr), 
			internalGrowthFactor(0), internalColonyLimits(ColonyLimits(0, 0)), index(0) {}
		constexpr Colony(const ColonyLimits colonyLimits) noexcept : 
			colonySize(0), colonyCapacity(0), 
			headColonyBlock(0), tailColonyBlock(nullptr),
			internalGrowthFactor(0), internalColonyLimits(colonyLimits), index(0) {}

		constexpr Colony(const Colony& other) noexcept {
			colonySize = other.colonySize;
			colonyCapacity = other.colonyCapacity;
			internalGrowthFactor = other.internalGrowthFactor;
			internalColonyLimits = other.internalColonyLimits;
			index = other.index;

			if (other.headColonyBlock == nullptr) {
				headColonyBlock = nullptr;
				tailColonyBlock = nullptr;
				return;
			} 

			colony_block* copyColonyBlock = other.headColonyBlock;
			colony_block* previousColonyBlock = nullptr;
			while (copyColonyBlock) {
				colony_block* newColonyBlock = createColonyBlock(copyColonyBlock->colonyBlockCapacity);

				newColonyBlock->index = copyColonyBlock->index;
				newColonyBlock->colonyBlockSize = copyColonyBlock->colonyBlockSize;
				newColonyBlock->colonyBlockCapacity = copyColonyBlock->colonyBlockCapacity;

				if (copyColonyBlock->emptySkipFieldStartPos == nullptr) {
					newColonyBlock->emptySkipFieldStartIndex = 0;
					newColonyBlock->emptySkipFieldStartPos = nullptr;
				} else {
					newColonyBlock->emptySkipFieldStartIndex = copyColonyBlock->emptySkipFieldStartIndex;
					newColonyBlock->emptySkipFieldStartPos = &newColonyBlock->skipField[newColonyBlock->emptySkipFieldStartIndex];
				}
				
				if (copyColonyBlock->previousColonyBlock == nullptr) {
					headColonyBlock = newColonyBlock;
				}
				if (copyColonyBlock->nextColonyBlock == nullptr) {
					tailColonyBlock = newColonyBlock;
				}

				newColonyBlock->previousColonyBlock = previousColonyBlock;
				if (previousColonyBlock) {
					previousColonyBlock->nextColonyBlock = newColonyBlock;
				}
				
				uninitializedCopy<SkipFieldType*, SkipFieldType*>(
					copyColonyBlock->skipField, 
					copyColonyBlock->skipField + copyColonyBlock->colonyBlockCapacity,
					newColonyBlock->skipField);

				previousColonyBlock = newColonyBlock;
				copyColonyBlock = copyColonyBlock->nextColonyBlock;
			}

			const_iterator copyBegin = other.cbegin();
			const_iterator copyEnd = other.cend();
			const_iterator copyIter = copyBegin;
			iterator dstIter = begin();

			for (; copyIter < copyEnd; ++copyIter, ++dstIter) {
				std::construct_at<DataType>(dstIter.getAddress(), *copyIter);
			}
		}

		constexpr Colony(Colony&& other) noexcept {
			colonySize = other.colonySize;
			colonyCapacity = other.colonyCapacity;
			headColonyBlock = other.headColonyBlock;
			tailColonyBlock = other.tailColonyBlock;
			internalGrowthFactor = other.internalGrowthFactor;
			internalColonyLimits = other.internalColonyLimits;
			index = other.index;

			other.colonySize = 0;
			other.colonyCapacity = 0;
			other.headColonyBlock = nullptr;
			other.tailColonyBlock = nullptr;
			other.internalGrowthFactor = 0;
			other.internalColonyLimits = ColonyLimits(0, 0);
			other.index = 0;
		}

	private:
		constexpr void destructColonyBlockMemory(colony_block* colonyBlock) noexcept {
			if (isConstantEvaluated()) {
				using colony_block_allocator_type = Alloc::template rebind_alloc<colony_block>;
				using skipfield_allocator_type = Alloc::template rebind_alloc<SkipFieldType>;

				allocator_type::deallocate(colonyBlock->data, colonyBlock->colonyBlockSize);
				skipfield_allocator_type::deallocate(colonyBlock->skipField, colonyBlock->colonyBlockSize);
				colony_block_allocator_type::deallocate(colonyBlock, 1);
			} else {
				using byte_allocator_type = Alloc::template rebind_alloc<Byte>;
				byte_allocator_type::deallocate(colonyBlock->blockByteStorage, colonyBlock->byteSize);
			}
		}
	public:

		//destructor 
		constexpr ~Colony() noexcept {
			colony_block* currentColonyBlock = headColonyBlock;
			internalClear<false>();

			while (currentColonyBlock) {
				colony_block* nextColonyBlock = currentColonyBlock->nextColonyBlock;
				destructColonyBlockMemory(currentColonyBlock);
				currentColonyBlock = nextColonyBlock;
			} 
		}

		constexpr colony_block* createColonyBlock() noexcept {
			Size newColonyBlockCapacity;
			if (tailColonyBlock == nullptr) {
				newColonyBlockCapacity = internalColonyLimits.minGroupSize;
			} else {
				newColonyBlockCapacity = natl::min<size_type>(tailColonyBlock->colonyBlockCapacity * internalGrowthFactor, internalColonyLimits.maxGroupSize);
			}
			return createColonyBlock(newColonyBlockCapacity);
		}

		//assignment 
		constexpr Colony& operator=(const Colony& other) noexcept {
			defaultDeconstruct<Colony>(&self());
			std::construct_at(&self(), other);
			return self();
		}
		constexpr Colony& operator=(Colony&& other) noexcept {
			defaultDeconstruct<Colony>(&self());
			std::construct_at(&self(), natl::move(other));
			return self();
		}

		constexpr colony_block* createColonyBlock(const Size newColonyBlockCapacity) noexcept {
			colony_block* newColonyBlock = nullptr;
			if (isConstantEvaluated()) {
				using colony_block_allocator_type = Alloc::template rebind_alloc<colony_block>;
				using skipfield_allocator_type = Alloc::template rebind_alloc<SkipFieldType>;

				newColonyBlock = colony_block_allocator_type::allocate(1);
				newColonyBlock->colonyBlockSize = 0;
				newColonyBlock->colonyBlockCapacity = newColonyBlockCapacity;
				newColonyBlock->skipField = skipfield_allocator_type::allocate(newColonyBlockCapacity);
				newColonyBlock->data = allocator_type::allocate(newColonyBlockCapacity);
				newColonyBlock->previousColonyBlock = nullptr;
				newColonyBlock->nextColonyBlock = nullptr;
				newColonyBlock->emptySkipFieldStartIndex = 0;
				newColonyBlock->emptySkipFieldStartPos = newColonyBlock->skipField;

				defaultConstructAll<SkipFieldType>(newColonyBlock->skipField, newColonyBlockCapacity);
			} else {
				using byte_allocator_type = Alloc::template rebind_alloc<Byte>;
				const Size storageByteSize =
					(sizeof(colony_block) + alignof(colony_block)) +
					((sizeof(SkipFieldType) * newColonyBlockCapacity) + alignof(SkipFieldType)) +
					((sizeof(DataType) * newColonyBlockCapacity) + alignof(DataType));

				Byte* byteStorage = byte_allocator_type::allocate(storageByteSize);

				AlignPtrInfo alignInfo = createAlignPtrInfo(castToVoidPtr<Byte>(byteStorage), storageByteSize);
				AlignPtrExpect colonyBlockAlignPtr = alignPtrWithType<colony_block>(alignInfo);

				if (colonyBlockAlignPtr.hasValue()) {
					newColonyBlock = std::construct_at<colony_block>(reinterpret_cast<colony_block*>(colonyBlockAlignPtr.value().alignedPtr));
					alignInfo = colonyBlockAlignPtr.value();
				} else {
					handelStandardAlignPtrError(colonyBlockAlignPtr);
				}

				newColonyBlock->blockByteStorage = byteStorage;
				newColonyBlock->byteSize = storageByteSize;

				AlignPtrExpect skipFieldAlignPtr = alignPtrWithArray<SkipFieldType>(newColonyBlockCapacity, alignInfo);
				if (skipFieldAlignPtr.hasValue()) {
					newColonyBlock->skipField = reinterpret_cast<SkipFieldType*>(skipFieldAlignPtr.value().alignedPtr);
					defaultConstructAll<SkipFieldType>(newColonyBlock->skipField, newColonyBlockCapacity);
					alignInfo = skipFieldAlignPtr.value();
				} else {
					handelStandardAlignPtrError(skipFieldAlignPtr);
				}

				AlignPtrExpect dataAlignPtr = alignPtrWithArray<DataType>(newColonyBlockCapacity, alignInfo);
				if (dataAlignPtr.hasValue()) {
					newColonyBlock->data = reinterpret_cast<DataType*>(dataAlignPtr.value().alignedPtr);
					alignInfo = dataAlignPtr.value();
				} else {
					handelStandardAlignPtrError(dataAlignPtr);
				}

				newColonyBlock->colonyBlockSize = 0;
				newColonyBlock->colonyBlockCapacity = newColonyBlockCapacity;
				newColonyBlock->previousColonyBlock = nullptr;
				newColonyBlock->nextColonyBlock = nullptr;
				newColonyBlock->emptySkipFieldStartIndex = 0;
				newColonyBlock->emptySkipFieldStartPos = newColonyBlock->skipField;
			}

			newColonyBlock->index = index;
			index += 1;
			return newColonyBlock;
		}

		//util
		constexpr Colony& self() noexcept { return *this; }
		constexpr const Colony& self() const noexcept { return *this; }

		// iterators
		constexpr iterator begin() noexcept {
			if (headColonyBlock) {
				return iterator(headColonyBlock, 0, headColonyBlock->skipField);
			} else {
				return iterator(nullptr, 0, nullptr);
			}
		}
		constexpr const_iterator begin() const noexcept {
			if (headColonyBlock) {
				return const_iterator(headColonyBlock, 0, headColonyBlock->skipField);
			} else {
				return const_iterator(nullptr, 0, nullptr);
			}
		}
		constexpr iterator end() noexcept {
			if (tailColonyBlock) {
				Size colonyBlockCapacity = tailColonyBlock->colonyBlockCapacity;
				return iterator(
					tailColonyBlock, 
					static_cast<difference_type>(colonyBlockCapacity), 
					tailColonyBlock->skipField + colonyBlockCapacity);
			} else {
				return iterator(nullptr, 0, nullptr);
			}
		}
		constexpr const_iterator end() const noexcept {
			if (tailColonyBlock) {
				Size colonyBlockCapacity = tailColonyBlock->colonyBlockCapacity;
				return const_iterator(
					tailColonyBlock, 
					static_cast<difference_type>(colonyBlockCapacity),
					tailColonyBlock->skipField + colonyBlockCapacity);
			} else {
				return const_iterator(nullptr, 0, nullptr);
			}
		}

		constexpr reverse_iterator rbegin() noexcept {
			if (headColonyBlock) {
				return reverse_iterator(headColonyBlock, -1, headColonyBlock->skipField - 1);
			} else {
				return reverse_iterator(nullptr, 0, nullptr);
			}
		}
		constexpr const_reverse_iterator rbegin() const noexcept {
			if (headColonyBlock) {
				return const_reverse_iterator(headColonyBlock, -1, headColonyBlock->skipField - 1);
			} else {
				return const_reverse_iterator(nullptr, 0, nullptr);
			}
		}
		constexpr reverse_iterator rend() noexcept {
			if (tailColonyBlock) {
				Size skipFiledIndex = tailColonyBlock->colonyBlockCapacity - 1;
				return iterator(
					tailColonyBlock, 
					static_cast<difference_type>(skipFiledIndex),
					tailColonyBlock->skipField + skipFiledIndex);
			} else {
				return iterator(nullptr, 0, nullptr);
			}
		}
		constexpr const_reverse_iterator rend() const noexcept {
			if (tailColonyBlock) {
				Size skipFiledIndex = tailColonyBlock->colonyBlockCapacity - 1;
				return const_reverse_iterator(
					tailColonyBlock, 
					static_cast<difference_type>(skipFiledIndex), 
					tailColonyBlock->skipField + skipFiledIndex);
			} else {
				return const_reverse_iterator(nullptr, 0, nullptr);
			}
		}

		constexpr const_iterator cbegin() const noexcept { return begin(); }
		constexpr const_iterator cend() const noexcept { return end(); }
		constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
		constexpr const_reverse_iterator crend() const noexcept { return rend(); }
	

		//capacity
		[[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
		[[nodiscard]] constexpr bool isEmpty() const noexcept { return empty(); }
		[[nodiscard]] constexpr bool isNotEmpty() const noexcept { return !empty(); }
		constexpr size_type size() const noexcept { return colonySize; }
		constexpr size_type max_size() const noexcept { return Limits<Size>::max(); }
		constexpr size_type capacity() const noexcept { return colonyCapacity; }
		constexpr ColonyLimits colonyLimits() const noexcept { return internalColonyLimits; }
		constexpr Size growthFactor() const noexcept { return internalGrowthFactor; }
		constexpr static ColonyLimits hardColonyLimits() noexcept { return ColonyLimits(1, Limits<Size>::max()); }

		constexpr void reserve(const size_type newCapacity) noexcept {
			if (colonyCapacity >= newCapacity) [[likely]] { 
				return; 
			}
			if (internalGrowthFactor == 0) [[unlikely]] {
				internalGrowthFactor = 2;
			};
			if (internalColonyLimits.minGroupSize == 0) [[unlikely]] {
				internalColonyLimits.minGroupSize = 10;
				internalColonyLimits.maxGroupSize = max_size();
			};

			while (colonyCapacity < newCapacity) {
				colony_block* newColonyBlock = createColonyBlock();
				if (tailColonyBlock) {
					tailColonyBlock->nextColonyBlock = newColonyBlock;
					newColonyBlock->previousColonyBlock = tailColonyBlock;
					tailColonyBlock = newColonyBlock;
				} else {
					headColonyBlock = newColonyBlock;
					tailColonyBlock = newColonyBlock;
				}
				colonyCapacity += newColonyBlock->colonyBlockCapacity;
			}
		}
		constexpr void shrink() noexcept {
			colony_block* currentColonyBlock = headColonyBlock;
			while (currentColonyBlock) {
				colony_block* nextColonyBlock = currentColonyBlock->nextColonyBlock;
				if (currentColonyBlock->colonyBlockSize == 0) {
					colonyCapacity -= currentColonyBlock->colonyBlockCapacity;

					if (currentColonyBlock->previousColonyBlock) {
						currentColonyBlock->previousColonyBlock->nextColonyBlock = currentColonyBlock->nextColonyBlock;
					}

					if (currentColonyBlock->nextColonyBlock) {
						currentColonyBlock->nextColonyBlock->previousColonyBlock = currentColonyBlock->previousColonyBlock;
					}

					if (currentColonyBlock == headColonyBlock) {
						headColonyBlock = currentColonyBlock->nextColonyBlock;
					}
					if (currentColonyBlock == tailColonyBlock) {
						tailColonyBlock = currentColonyBlock->previousColonyBlock;
					}

					destructColonyBlockMemory(currentColonyBlock);
				}

				currentColonyBlock = nextColonyBlock;
			}
		}


		//modifiers
	private:
		constexpr iterator colonyBlockInsertGetIter(colony_block* colonyBlock) noexcept {
			colonyBlock->colonyBlockSize += 1;

			SkipFieldType* skipFieldPos;
			Size skipFieldIndex;

			skipFieldPos = colonyBlock->skipField;
			skipFieldIndex = 0;

			if (colonyBlock->emptySkipFieldStartPos != nullptr &&
				*colonyBlock->emptySkipFieldStartPos == false) {
				skipFieldPos = colonyBlock->emptySkipFieldStartPos;
				skipFieldIndex = colonyBlock->emptySkipFieldStartIndex;
			}

			while (*skipFieldPos == true) {
				skipFieldIndex += 1;
				skipFieldPos += 1;
			}

			*skipFieldPos = true;

			if (skipFieldIndex + 1 < colonyBlock->colonyBlockCapacity) {
				colonyBlock->emptySkipFieldStartIndex = skipFieldIndex + 1;
				colonyBlock->emptySkipFieldStartPos = skipFieldPos + 1;
			}

			return iterator(colonyBlock, 
				static_cast<difference_type>(skipFieldIndex),
				skipFieldPos);
		}
		constexpr iterator insertGetIter() {
			reserve(colonySize + 1); 
			colonySize += 1;

			colony_block* currentColonyBlock = headColonyBlock;
			while (true) {
				if (currentColonyBlock->isFull()) {
					currentColonyBlock = currentColonyBlock->nextColonyBlock;
					continue;
				}

				return colonyBlockInsertGetIter(currentColonyBlock);
			}
		}

	public:
		constexpr iterator insert(const value_type& value) noexcept {
			iterator iter = insertGetIter();
			std::construct_at<value_type>(iter.getAddress(), value);
			return iter;
		}
		constexpr iterator insert(value_type&& value) noexcept {
			iterator iter = insertGetIter();
			std::construct_at<value_type>(iter.getAddress(), natl::move(value));
			return iter;
		}
	private:
		constexpr iterator getNextElement(const_iterator iter) noexcept {
			iter++;
			return iterator(iter.colonyBlock, iter.skipFieldIndex, iter.skipFieldPos);
		}
	public:
		constexpr iterator erase(const_iterator iter) noexcept {
#ifdef NATL_IN_DEBUG
			if (!isActive(iter)) [[unlikely]] {
				fatalError("natl: colony erase error - element does not exist");
			}
			if (iter == end()) [[unlikely]] {
				fatalError("natl: colony erase error - iter points at end");
			}
#endif
			iterator nextElementIter = getNextElement(iter);
			SkipFieldType* emptySkipFieldPos = iter.skipFieldPos;
			Size emptySkipFieldIndex = static_cast<Size>(iter.skipFieldIndex);

			colony_block* colonyBlock = iter.colonyBlock;
			colonyBlock->colonyBlockSize -= 1;
			std::destroy_at<value_type>(&iter.colonyBlock->data[iter.skipFieldIndex]);

			*iter.skipFieldPos = false;

			if (colonyBlock->emptySkipFieldStartPos == nullptr) {
				colonyBlock->emptySkipFieldStartPos = emptySkipFieldPos;
				colonyBlock->emptySkipFieldStartIndex = emptySkipFieldIndex;
			} else if (*colonyBlock->emptySkipFieldStartPos == true) {
				colonyBlock->emptySkipFieldStartPos = emptySkipFieldPos;
				colonyBlock->emptySkipFieldStartIndex = emptySkipFieldIndex;
			} else if (emptySkipFieldPos < colonyBlock->emptySkipFieldStartPos) {
				colonyBlock->emptySkipFieldStartPos = emptySkipFieldPos;
				colonyBlock->emptySkipFieldStartIndex = emptySkipFieldIndex;
			}

			return nextElementIter;
		}
		constexpr void swap(Colony& other) noexcept {
			Colony temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(other);
		}

	private:
		template<bool ClearSkipFeild>
		constexpr void internalClear() noexcept {
			if (!ClearSkipFeild && !typeHasToBeDestructed<DataType>()) { return; }
			if (isEmpty()) { return; }

			iterator beingIter = begin();
			iterator endIter = end();

			iterator iter = beingIter;
			iterator previous = iter;
			++iter;

			if (typeHasToBeDestructed<DataType>()) {
				natl::defaultDeconstruct<DataType>(previous.getAddress());
			}

			for (; iter < endIter; ++iter) {
				if (typeHasToBeDestructed<DataType>()) {
					natl::defaultDeconstruct<DataType>(iter.getAddress());
				}
				if constexpr (ClearSkipFeild) {
					previous.skipFieldPos->setSkipCount(0);
				}
				previous = iter;
			}

			if constexpr (ClearSkipFeild) {
				previous.skipFieldPos->setSkipCount(0);
			}
		}
	public:
		constexpr void clear() noexcept {
			internalClear<true>();
		}

	private:
		constexpr void printSkipFieldState(colony_block* colonyBlock) {
			std::cout << "size: " << colonyBlock->colonyBlockSize << "\n";
			for (Size i = 0; i < colonyBlock->colonyBlockCapacity; i++) {
				SkipFieldType skipFieldPos = colonyBlock->skipField[i];
				if (skipFieldPos) {
					std::cout << "/";
				} else {
					std::cout << "0";
				}
			}
			std::cout << "\n";
		}

		//colony operations 
	public:
		constexpr Option<iterator> getIteratorAtIndex(const size_type atIndex) noexcept {
			if (atIndex >= colonyCapacity) {
				return OptionEmpty();
			}

			size_type relativeIndex = 0;
			colony_block* currentColonyBlock = headColonyBlock;
			while (atIndex + 1 > relativeIndex + currentColonyBlock->colonyBlockCapacity) {
				relativeIndex += currentColonyBlock->colonyBlockCapacity;
				currentColonyBlock = currentColonyBlock->nextColonyBlock;
			}

			const size_type newIndex = atIndex - relativeIndex;
			return iterator(
				currentColonyBlock, 
				static_cast<difference_type>(newIndex),
				currentColonyBlock->skipField + newIndex);
		}
		constexpr Option<iterator> getIteratorAtIndex(const size_type atIndex) const noexcept {
			if (atIndex >= colonyCapacity) {
				return OptionEmpty();
			}

			size_type relativeIndex = 0;
			colony_block* currentColonyBlock = headColonyBlock;
			while (atIndex > relativeIndex + currentColonyBlock->colonyBlockCapacity) {
				relativeIndex += currentColonyBlock->colonyBlockCapacity;
				currentColonyBlock = currentColonyBlock->nextColonyBlock;
			}

			const size_type newIndex = atIndex - relativeIndex;
			return const_iterator(currentColonyBlock, newIndex, currentColonyBlock->skipField + newIndex);
		}

		constexpr Option<iterator> getIterator(const_pointer p) noexcept {
			colony_block* currentColonyBlock = headColonyBlock;
			while (currentColonyBlock) {
				SkipFieldType* skipField = currentColonyBlock;

				if (skipField <= p && p < skipField + currentColonyBlock->colonyBlockCapacity) {
					Size skipFiledIndex = std::bit_cast<Size>(p - skipField);
					return iterator(currentColonyBlock, currentColonyBlock, skipField + skipFiledIndex);
				}

				currentColonyBlock = currentColonyBlock->nextColonyBlock;
			}

			return OptionEmpty();
		}
		constexpr Option<const_iterator> getIterator(const_pointer p) const noexcept {
			colony_block* currentColonyBlock = headColonyBlock;
			while (currentColonyBlock) {
				SkipFieldType* skipField = currentColonyBlock;

				if (skipField <= p && p < skipField + currentColonyBlock->colonyBlockCapacity) {
					Size skipFiledIndex = std::bit_cast<Size>(p - skipField);
					return const_iterator(currentColonyBlock, currentColonyBlock, skipField + skipFiledIndex);
				}

				currentColonyBlock = currentColonyBlock->nextColonyBlock;
			}

			return OptionEmpty();
		}
		constexpr bool isActive(const_iterator iter) const noexcept {
			return *iter.skipFieldPos == true;
		}

		//colony modifier operations 
		constexpr void reshape(const ColonyLimits colonyLimits) noexcept {
			reshape(colonyLimits, internalGrowthFactor);
		}
		constexpr void reshape(const ColonyLimits colonyLimits, const Size inputGrowthFactor) noexcept {
			if (headColonyBlock == nullptr) {
				internalColonyLimits = colonyLimits;
				return;
			}

			if (colonyLimits.minGroupSize == 0) { return; }

			if (colonyLimits.minGroupSize == internalColonyLimits.minGroupSize &&
				colonyLimits.maxGroupSize == internalColonyLimits.maxGroupSize) { 
				return; 
			}

			if (headColonyBlock->colonyBlockCapacity >= colonyLimits.minGroupSize &&
				tailColonyBlock->colonyBlockCapacity <= colonyLimits.maxGroupSize) {
				internalColonyLimits = colonyLimits;
				return;
			}

			Colony tempColony = natl::move(self());

			internalColonyLimits = colonyLimits;
			internalGrowthFactor = inputGrowthFactor;

			reserve(tempColony.capacity());
			
			for (DataType& value : tempColony) {
				insert(natl::move(value));
			}
		}
	};
}