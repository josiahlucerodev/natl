#pragma once

// own
#include "../util/basicTypes.h"
#include "../util/bits.h"
#include "stringView.h"

#include <type_traits>

//@export
namespace natl {
	struct BitIndex {
		Size index = 0;

		constexpr BitIndex() noexcept = default;
		constexpr explicit BitIndex(const Size indexIn) noexcept
			: index(indexIn) {
		}
	};

	template<typename BitStorageType, Size bitCountT>
		requires (bitCountT > 0)
	struct BitArrayBase {
	public:
		using storage_type = BitStorageType;

		static constexpr Size bitCount = bitCountT;
		static constexpr Size bitsPerWordCount = sizeof(BitStorageType) * 8;

		// ceil(bitCount / bitsPerWordCount)
		static constexpr Size wordCount = (bitCount / bitsPerWordCount) + ((bitCount % bitsPerWordCount) != 0);

		static constexpr Size byteCount = sizeof(BitStorageType) * wordCount;

	private:
		static constexpr Size lastWordIndex = wordCount - 1;
		static constexpr Size lastWordBitCount =
			bitCount % bitsPerWordCount;

		 static constexpr BitStorageType allBitsSet() noexcept {
			return ~BitStorageType{ 0 };
		}

		 static constexpr BitStorageType lastWordMask() noexcept {
			 if constexpr (lastWordBitCount == 0) {
				 return allBitsSet();
			 } else {
				 return static_cast<BitStorageType>((BitStorageType{ 1 } << lastWordBitCount) - 1);
			 }
		}

		constexpr void clearUnusedBits() noexcept {
			bitsArray[lastWordIndex] &= lastWordMask();
		}

		 static constexpr Size wordIndex(const Size bitIndex) noexcept {
			return bitIndex / bitsPerWordCount;
		}

		 static constexpr BitStorageType bitMask(const Size bitIndex) noexcept {
			return static_cast<BitStorageType>(BitStorageType{ 1 } << (bitIndex % bitsPerWordCount));
		}

	public:
		BitStorageType bitsArray[wordCount]{};


		struct Reference {
		private:
			BitArrayBase* bitArrayPtr = nullptr;
			Size index = 0;

		public:
			constexpr Reference() noexcept = default;

			constexpr Reference(BitArrayBase* bitArrayPtrIn, const Size indexIn) noexcept
				: bitArrayPtr(bitArrayPtrIn), index(indexIn) {}

			constexpr Reference& operator=(const Bool value) noexcept {
				bitArrayPtr->set(index, value);
				return *this;
			}
			constexpr Reference& operator=(const Reference& other) noexcept {
				return *this = static_cast<Bool>(other);
			}

			constexpr Bool operator~() const noexcept {
				return !bitArrayPtr->test(index);
			}
			constexpr operator Bool() const noexcept {
				return bitArrayPtr->test(index);
			}

			constexpr Reference& flip() noexcept {
				bitArrayPtr->flip(index);
				return *this;
			}
		};

		using reference = Reference;

		struct ConstReference {
		private:
			const BitArrayBase* bitArrayPtr = nullptr;
			Size index = 0;

		public:
			constexpr ConstReference() noexcept = default;

			constexpr ConstReference(const BitArrayBase* bitArrayPtrIn, const Size indexIn) noexcept
				: bitArrayPtr(bitArrayPtrIn), index(indexIn) {}

			 constexpr Bool operator~() const noexcept {
				return !bitArrayPtr->test(index);
			}
			 constexpr operator Bool() const noexcept {
				return bitArrayPtr->test(index);
			}
		};

		using const_reference = ConstReference;


		// constructors
		constexpr BitArrayBase() noexcept = default;

		constexpr BitArrayBase(const BitIndex bitIndex) noexcept {
			set(bitIndex.index);
		}

		constexpr BitArrayBase(const BitIndex bitIndex, const Bool value) noexcept {
			set(bitIndex.index, value);
		}
		constexpr explicit BitArrayBase(const BitStorageType value) noexcept {
			bitsArray[0] = value;
			clearUnusedBits();
		}

		constexpr explicit BitArrayBase(const StringView value) noexcept {
			const Size countToRead = value.size() < bitCount ? value.size() : bitCount;
			for (Size i = 0; i < countToRead; ++i) {
				if (value[i] == '1') {
					set(i);
				}
			}
		}

		// util
	private:
		 constexpr BitArrayBase& self() noexcept {
			return *this;
		}
		 constexpr const BitArrayBase& self() const noexcept {
			return *this;
		}

	public:
		// size
		static constexpr Size size() noexcept { return bitCount; }
		static constexpr Size bitSize() noexcept { return bitCount; }
		static constexpr Size byteSize() noexcept { return byteCount; }


		// modifiers
		constexpr BitArrayBase& set() noexcept {
			for (Size i = 0; i < wordCount; ++i) {
				bitsArray[i] = allBitsSet();
			}

			clearUnusedBits();
			return self();
		}

		constexpr BitArrayBase& set(const Size index, const Bool value = true) noexcept {
			if (index >= bitCount) {
				return self();
			}

			BitStorageType& word = bitsArray[wordIndex(index)];
			const BitStorageType mask = bitMask(index);

			if (value) {
				word |= mask;
			} else {
				word &= static_cast<BitStorageType>(~mask);
			}

			return self();
		}

		constexpr BitArrayBase& reset() noexcept {
			for (Size i = 0; i < wordCount; ++i) {
				bitsArray[i] = BitStorageType{ 0 };
			}
			return self();
		}

		constexpr BitArrayBase& reset(const Size index) noexcept {
			return set(index, false);
		}

		constexpr BitArrayBase& flip() noexcept {
			for (Size i = 0; i < wordCount; ++i) {
				bitsArray[i] = static_cast<BitStorageType>(~bitsArray[i]);
			}
			clearUnusedBits();
			return self();
		}

		constexpr BitArrayBase& flip(const Size index) noexcept {
			if (index >= bitCount) {
				return self();
			}
			bitsArray[wordIndex(index)] ^= bitMask(index);
			return self();
		}

		// accessors
		constexpr Bool test(const Size index) const noexcept {
			if (index >= bitCount) {
				return false;
			}
			return (bitsArray[wordIndex(index)] & bitMask(index)) != 0;
		}

		constexpr Bool all() const noexcept {
			for (Size i = 0; i + 1 < wordCount; ++i) {
				if (bitsArray[i] != allBitsSet()) {
					return false;
				}
			}
			return (bitsArray[lastWordIndex] & lastWordMask()) == lastWordMask();
		}

		constexpr Bool any() const noexcept {
			for (Size i = 0; i + 1 < wordCount; ++i) {
				if (bitsArray[i] != 0) {
					return true;
				}
			}
			return (bitsArray[lastWordIndex] & lastWordMask()) != 0;
		}

		constexpr Bool none() const noexcept {
			return !any();
		}

		constexpr Size count() const noexcept {
			Size accumulator = 0;
			for (Size i = 0; i + 1 < wordCount; ++i) {
				accumulator += popcount(bitsArray[i]);
			}
			accumulator += popcount(static_cast<BitStorageType>(bitsArray[lastWordIndex] & lastWordMask()));
			return accumulator;
		}

		// indexing
		constexpr reference operator[](const Size index) noexcept {
			return reference(this, index);
		}
		constexpr const_reference operator[](const Size index) const noexcept {
			return const_reference(this, index);
		}


		// operators
		constexpr Bool operator==(const BitArrayBase& rhs) const noexcept {
			for (Size i = 0; i + 1 < wordCount; ++i) {
				if (bitsArray[i] != rhs.bitsArray[i]) {
					return false;
				}
			}
			return (bitsArray[lastWordIndex] & lastWordMask()) == (rhs.bitsArray[lastWordIndex] & lastWordMask());
		}
		constexpr Bool operator!=(const BitArrayBase& rhs) const noexcept {
			return !(*this == rhs);
		}
		constexpr BitArrayBase& operator&=(const BitArrayBase& rhs) noexcept {
			for (Size i = 0; i < wordCount; ++i) {
				bitsArray[i] &= rhs.bitsArray[i];
			}
			clearUnusedBits();
			return self();
		}
		constexpr BitArrayBase& operator|=(const BitArrayBase& rhs) noexcept {
			for (Size i = 0; i < wordCount; ++i) {
				bitsArray[i] |= rhs.bitsArray[i];
			}
			clearUnusedBits();
			return self();
		}
		constexpr BitArrayBase& operator^=(const BitArrayBase& rhs
			) noexcept {
			for (Size i = 0; i < wordCount; ++i) {
				bitsArray[i] ^= rhs.bitsArray[i];
			}
			clearUnusedBits();
			return self();
		}
		constexpr BitArrayBase operator~() const noexcept {
			BitArrayBase result = *this;
			result.flip();
			return result;
		}
		constexpr BitArrayBase operator&(const BitArrayBase& rhs) const noexcept {
			BitArrayBase result = *this;
			result &= rhs;
			return result;
		}
		constexpr BitArrayBase operator|(const BitArrayBase& rhs) const noexcept {
			BitArrayBase result = *this;
			result |= rhs;
			return result;
		}
		constexpr BitArrayBase operator^(const BitArrayBase& rhs) const noexcept {
			BitArrayBase result = *this;
			result ^= rhs;
			return result;
		}
		constexpr explicit operator Bool() const noexcept {
			return any();
		}
	};


	template<Size bitCount>
	using BitArray = BitArrayBase<ui32, bitCount>;
}