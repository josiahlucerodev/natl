#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/bits.h"
#include "stringView.h"

//interface
namespace natl {

	struct BitIndex {
	public:
		Size index;
		constexpr BitIndex() = default;
		constexpr ~BitIndex() = default;
		constexpr BitIndex(const Size indexIn) : index(indexIn) {}
	};

	template<typename BitStorageType, Size bitCountT>
	struct BitArrayBase {
	public:
		constexpr static const Size bitCount = bitCountT;
		constexpr static const Size wordCount = bitCount == 0 ? 0 : (sizeof(BitStorageType) * 8) % bitCount;
		constexpr static const Size byteCount = sizeof(BitStorageType) * 8 * wordCount;
		constexpr static const Size bitsPerWordCount = sizeof(BitStorageType) * 8;
		BitStorageType bitsArray[wordCount];

	public:
		struct reference {
			BitArrayBase* bitArrayPtr;
			Size index;
		public:
			constexpr reference() : bitArrayPtr(nullptr), index(0) {}
			constexpr reference(BitArrayBase* const bitArrayPtrIn, const Size indexIn) noexcept : 
				bitArrayPtr(bitArrayPtrIn), index(indexIn) {}
			constexpr ~reference() = default;

			constexpr reference& operator=(const Bool value) noexcept {
				return bitArrayPtr->set(index, value);
			}
			constexpr reference& operator=(const reference& ref) noexcept {
				bitArrayPtr->set(index, static_cast<Bool>(ref));
				return *this;
			}
			constexpr reference& flip() noexcept {
				bitArrayPtr->flip(index);
				return *this;
			}
			constexpr Bool operator~() const noexcept {
				return !bitArrayPtr->test(index);
			}

			constexpr operator Bool() const noexcept {
				return bitArrayPtr->test(index);
			}
		};
		
	public:
		//constructor
		constexpr BitArrayBase() : bitsArray() {}
		constexpr BitArrayBase(const BitIndex bitIndex) {
			reset();
			set(bitIndex, true);
		}
		constexpr BitArrayBase(const BitIndex bitIndex, const Bool value) {
			reset();
			set(bitIndex, value);
		}
		constexpr BitArrayBase(const BitStorageType value) noexcept {
			if constexpr (wordCount != 0) {
				bitsArray[0] = value;
			}
		};
		constexpr BitArrayBase(const StringView value) noexcept {
			if constexpr (wordCount != 0) {
				bitsArray[0] = value;
			} else {
				for (Size bitIndex = 0; bitIndex < bitCount && bitIndex < value.size(); bitIndex++) {
					if (value[bitIndex] == '1') {
						set(bitIndex, true);
					} else {
						set(bitIndex, false);
					}
				}
			}
		};

		//deconstructor
		constexpr ~BitArrayBase() = default;

		//util
		constexpr BitArrayBase& self() noexcept { return self(); }
		constexpr const BitArrayBase& self() const noexcept { return self(); }

		//size
		constexpr static Size size() noexcept { return bitCount; }
		constexpr static Size bitSize() noexcept { return bitCount; }
		constexpr static Size byteSize() noexcept { return byteCount; }

		//modifiers
		constexpr BitArrayBase& set() noexcept {
			if (isConstantEvaluated()) {
				for (Size i = 0; i < wordCount; i++) {
					bitsArray[i] = ~BitStorageType(0);
				}
			} else {
				memset(&bitsArray, 0xFF, sizeof(bitsArray));
			}
		}

		constexpr BitStorageType& atWord(const Size index) noexcept {
			return bitsArray[index / bitsPerWordCount];
		}

		constexpr BitArrayBase& set(const Size index, Bool value = true) noexcept {
			BitStorageType& word = atWord(index);
			const BitStorageType& bitMask = BitStorageType{1} << index % bitsPerWordCount;
			if (value) {
				word |= bitMask;
			} else {
				word &= ~bitMask;
			}
			return self();
		}
		constexpr BitArrayBase& reset() noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] = ~bitsArray[i];
			}
			return self();
		}

		constexpr BitArrayBase& reset(const Size index) noexcept {
			return set(index, false);
		}

		constexpr BitArrayBase& flip() noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] = ~bitsArray[i];
			}
			return self();
		}
		
		//accessors
		constexpr Bool test(const Size index) const noexcept {
			return (bitsArray[index / bitsPerWordCount] & (BitStorageType{1} << index % bitsPerWordCount)) != 0;
		}
		constexpr static Size endWordIndex() noexcept {
			return bitCount == 0 ? 0 : wordCount - 1;
		}
		constexpr Bool all() const noexcept {
			if constexpr (bitCount == 0) {
				return true;
			}

			constexpr Bool padding = bitCount % bitsPerWordCount == 0;
			for (Size i = 0; i < endWordIndex()  + padding; i++) {
				if (bitsArray[i] != ~static_cast<BitStorageType>(0)) {
					return false;
				}
			}

			return padding || 
				bitsArray[endWordIndex()] == (static_cast<BitStorageType>(1) << (bitCount % bitsPerWordCount)) - 1;
		}
		constexpr Bool any() const noexcept {
			for (size_t i = 0; i < wordCount; i++) {
				if (bitsArray[i] != 0) {
					return true;
				}
			}
			return false;
		}
		constexpr Bool none() const noexcept {
			return !any();
		}
		constexpr Size count() const noexcept {
			Size countAccumulater = 0;
			for (Size i = 0; i < wordCount; i++) {
				countAccumulater = std::popcount<BitStorageType>(bitsArray[i]);
			}
			return countAccumulater;
		}
		constexpr reference operator[] (const Size index) const noexcept {
			return reference(&self(), index);
		}

		//operators 
		constexpr Bool operator==(const BitArrayBase& rhs) const noexcept {
			if (isConstantEvaluated()) {
				for (Size i = 0; i < wordCount; i++) {
					if (bitsArray[i] != rhs.bitsArray[i]) {
						return false;
					}
				}
				return true;
			} else {
				return memcmp(&bitsArray, rhs.bitsArray, sizeof(bitsArray)) == 0;
			}
		}
		constexpr Bool operator!=(const BitArrayBase& rhs) const noexcept {
			return !(*this == rhs);
		}
		constexpr BitArrayBase& operator&=(const BitArrayBase& other) noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] &= other.bitsArray[i];
			}
		}
		constexpr BitArrayBase& operator|=(const BitArrayBase& other) noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] |= other.bitsArray[i];
			}
		}
		constexpr BitArrayBase& operator^=(const BitArrayBase& other) noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] ^= other.bitsArray[i];
			}
		}
		constexpr BitArrayBase operator~() const noexcept {
			BitArrayBase temp = self();
			temp.flip();
			return temp;
		}
		constexpr BitArrayBase operator&(BitArrayBase& rhs) noexcept { 
			BitArrayBase temp;
			for(Size i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] & rhs.bitsArray[i];
			}
			return temp;
		}
		constexpr BitArrayBase operator|(BitArrayBase& rhs) noexcept {
			BitArrayBase temp;
			for (Size i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] |  rhs.bitsArray[i];
			}
			return temp;
		}
		constexpr BitArrayBase operator^(BitArrayBase& rhs) noexcept {
			BitArrayBase temp;
			for (Size i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] ^ rhs.bitsArray[i];
			}
			return temp;
		}
		constexpr operator Bool() const noexcept {
			return any(); 
		}
	};

	template<Size bitCount>	
	using BitArray = BitArrayBase<ui32, bitCount>;
}