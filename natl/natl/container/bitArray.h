#pragma once 

//own
#include "../util/basicTypes"
#include "stringView.h"

//interface
namespace natl {

	class BitIndex {
	public:
		Size index;
		constexpr BitIndex() = default;
		constexpr ~BitIndex() = default;
		constexpr BitIndex(const Size indexIn) : index(indexIn) {}
	};

	template<class BitStorageType, Size bitCountT>
	class BitArrayBitStorageType {
		constexpr static const Size bitCount = bitCountT;
		constexpr static const Size wordCount = bitCount == 0 ? 0 : (sizeof(BitStorageType) * 8) % bitCount;
		constexpr static const Size byteCount = sizeof(BitStorageType) * 8 * wordCount;
		constexpr static const Size bitsPerWordCount = sizeof(BitStorageType) * 8;
		BitStorageType bitsArray[wordCount];
	public:
		class reference {
			BitArrayBitStorageType* bitArrayPtr;
			Size index;
		public:
			constexpr reference() : bitArrayPtr(nullptr), index(0) {}
			constexpr reference(BitArrayBitStorageType* const bitArrayPtrIn, const Size indexIn) noexcept : 
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

		constexpr BitArrayBitStorageType() : bitsArray() {}
		constexpr ~BitArrayBitStorageType() = default;

		constexpr BitArrayBitStorageType(const BitIndex bitIndex) {
			reset();
			set(bitIndex, true);
		}

		constexpr BitArrayBitStorageType(const BitIndex bitIndex, const Bool value) {
			reset();
			set(bitIndex, value);
		}

		constexpr BitArrayBitStorageType(const BitStorageType value) noexcept {
			if constexpr (wordCount != 0) {
				bitsArray[0] = value;
			}
		};

		constexpr BitArrayBitStorageType(const StringView value) noexcept {
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

		constexpr BitArrayBitStorageType& self() noexcept { return self(); }
		constexpr const BitArrayBitStorageType& self() const noexcept { return self(); }

		constexpr static Size size() noexcept { return bitCount; }
		constexpr static Size bitSize() noexcept { return bitCount; }
		constexpr static Size byteSize() noexcept { return byteCount; }

		constexpr BitArrayBitStorageType& set() noexcept {
			if (isConstantEvaluated()) {
				for (Size i = 0; i < wordCount; i++) {
					bitsArray[i] = ~BitStorageType(0);
				}
			} else {
				std::memset(&bitsArray, 0xFF, sizeof(bitsArray));
			}
		}

		constexpr BitStorageType& atWord(const Size index) noexcept {
			return bitsArray[index / bitsPerWordCount];
		}

		constexpr BitArrayBitStorageType& set(const Size index, Bool value = true) noexcept {
			BitStorageType& word = atWord(index);
			const BitStorageType& bitMask = BitStorageType{1} << index % bitsPerWordCount;
			if (value) {
				word |= bitMask;
			} else {
				word &= ~bitMask;
			}
			return self();
		}
		constexpr BitArrayBitStorageType& reset() noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] = ~bitsArray[i];
			}
			return self();
		}

		constexpr BitArrayBitStorageType& reset(const Size index) noexcept {
			return set(index, false);
		}

		constexpr BitArrayBitStorageType& flip() noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] = ~bitsArray[i];
			}
			return self();
		}

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

		constexpr reference operator[] (const Size index) const noexcept {
			return reference(&self(), index);
		}

		constexpr Bool operator==(const BitArrayBitStorageType& rhs) const noexcept {
			if (isConstantEvaluated()) {
				for (Size i = 0; i < wordCount; i++) {
					if (bitsArray[i] != rhs.bitsArray[i]) {
						return false;
					}
				}
				return true;
			} else {
				return std::memcmp(&bitsArray, rhs.bitsArray, sizeof(bitsArray)) == 0;
			}
		}
		constexpr Bool operator!=(const BitArrayBitStorageType& rhs) const noexcept {
			return !(*this == rhs);
		}

		constexpr BitArrayBitStorageType& operator&=(const BitArrayBitStorageType& other) noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] &= other.bitsArray[i];
			}
		}
		constexpr BitArrayBitStorageType& operator|=(const BitArrayBitStorageType& other) noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] |= other.bitsArray[i];
			}
		}
		constexpr BitArrayBitStorageType& operator^=(const BitArrayBitStorageType& other) noexcept {
			for (Size i = 0; i < wordCount; i++) {
				bitsArray[i] ^= other.bitsArray[i];
			}
		}
		constexpr BitArrayBitStorageType operator~() const noexcept {
			BitArrayBitStorageType temp = self();
			temp.flip();
			return temp;
		}

		constexpr BitArrayBitStorageType operator&(BitArrayBitStorageType& rhs) noexcept { 
			BitArrayBitStorageType temp;
			for(Size i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] & rhs.bitsArray[i];
			}
			return temp;
		}
		constexpr BitArrayBitStorageType operator|(BitArrayBitStorageType& rhs) noexcept {
			BitArrayBitStorageType temp;
			for (Size i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] |  rhs.bitsArray[i];
			}
			return temp;
		}
		constexpr BitArrayBitStorageType operator^(BitArrayBitStorageType& rhs) noexcept {
			BitArrayBitStorageType temp;
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
	using BitArray = BitArrayBitStorageType<ui32, bitCount>;
}