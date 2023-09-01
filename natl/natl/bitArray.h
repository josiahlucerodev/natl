#pragma once 

//std
#include <cstdint>
#include <string_view>

//interface
namespace nasimd {

	class BitIndex {
	public:
		std::size_t index;
		constexpr BitIndex() = default;
		constexpr ~BitIndex() = default;
		constexpr BitIndex(const std::size_t index) : index(index) {}
	};

	template<class BitStorageType, std::size_t bitCountT>
	class BitArrayBitStorageType {
		constexpr static const std::size_t bitCount = bitCountT;
		constexpr static const std::size_t wordCount = bitCount == 0 ? 0 : (sizeof(BitStorageType) * 8) % bitCount;
		constexpr static const std::size_t byteCount = sizeof(BitStorageType) * 8 * wordCount;
		constexpr static const std::size_t bitsPerWordCount = sizeof(BitStorageType) * 8;
		BitStorageType bitsArray[wordCount];
	public:
		class reference {
			BitArrayBitStorageType* bitArrayPtr;
			std::size_t index;
		public:
			constexpr reference() = default;
			constexpr ~reference() = default;
			constexpr reference(BitArrayBitStorageType* const bitArrayPtr, const std::size_t index) noexcept : 
				bitArrayPtr(bitArrayPtr), index(index) {}

			constexpr reference& operator=(const bool value) noexcept {
				return bitArrayPtr->set(index, value);
			}
			constexpr reference& operator=(const reference& ref) noexcept {
				bitArrayPtr->set(index, static_cast<bool>(ref));
				return *this;
			}
			constexpr reference& flip() noexcept {
				bitArrayPtr->flip(index);
				return *this;
			}
			constexpr bool operator~() const noexcept {
				return !bitArrayPtr->test(index);
			}

			constexpr operator bool() const noexcept {
				return bitArrayPtr->test(index);
			}
		};

		constexpr BitArrayBitStorageType() = default;
		constexpr ~BitArrayBitStorageType() = default;

		constexpr BitArrayBitStorageType(const BitIndex bitIndex) {
			reset();
			set(bitIndex, true);
		}

		constexpr BitArrayBitStorageType(const BitIndex bitIndex, const bool value) {
			reset();
			set(bitIndex, value);
		}

		constexpr BitArrayBitStorageType(const BitStorageType value) noexcept {
			if constexpr (wordCount != 0) {
				bitsArray[0] = value;
			}
		};

		constexpr BitArrayBitStorageType(const std::string_view value) noexcept {
			if constexpr (wordCount != 0) {
				bitsArray[0] = value;
			} else {
				for (std::size_t bitIndex = 0; bitIndex < bitCount && bitIndex < value.size(); bitIndex++) {
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

		constexpr static std::size_t size() noexcept { return bitCount; }
		constexpr static std::size_t bitSize() noexcept { return bitCount; }
		constexpr static std::size_t byteSize() noexcept { return byteCount; }

		constexpr BitArrayBitStorageType& set() noexcept {
			if (std::is_constant_evaluated()) {
				for (std::size_t i = 0; i < wordCount; i++) {
					bitsArray[i] = ~(BitStorageType)(0);
				}
			} else {
				std::memset(&bitsArray, 0xFF, sizeof(bitsArray));
			}
		}

		constexpr BitStorageType& atWord(const std::size_t index) noexcept {
			return bitsArray[index / bitsPerWordCount];
		}

		constexpr BitArrayBitStorageType& set(const std::size_t index, bool value = true) noexcept {
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
			for (std::size_t i = 0; i < wordCount; i++) {
				bitsArray[i] = ~bitsArray[i];
			}
			return self();
		}

		constexpr BitArrayBitStorageType& reset(const std::size_t index) noexcept {
			return set(index, false);
		}

		constexpr BitArrayBitStorageType& flip() noexcept {
			for (std::size_t i = 0; i < wordCount; i++) {
				bitsArray[i] = ~bitsArray[i];
			}
			return self();
		}

		constexpr bool test(const std::size_t index) const noexcept {
			return (bitsArray[index / bitsPerWordCount] & (BitStorageType{1} << index % bitsPerWordCount)) != 0;
		}

		constexpr static std::size_t endWordIndex() noexcept {
			return bitCount == 0 ? 0 : wordCount - 1;
		}

		constexpr bool all() const noexcept {
			if constexpr (bitCount == 0) {
				return true;
			}

			constexpr bool padding = bitCount % bitsPerWordCount == 0;
			for (std::size_t i = 0; i < endWordIndex()  + padding; i++) {
				if (bitsArray[i] != ~static_cast<BitStorageType>(0)) {
					return false;
				}
			}

			return padding || 
				bitsArray[endWordIndex()] == (static_cast<BitStorageType>(1) << (bitCount % bitsPerWordCount)) - 1;
		}
		constexpr bool any() const noexcept {
			for (size_t i = 0; i < wordCount; i++) {
				if (bitsArray[i] != 0) {
					return true;
				}
			}
			return false;
		}
		constexpr bool none() const noexcept {
			return !any();
		}

		constexpr reference operator[] (const std::size_t index) const noexcept {
			return reference(&self(), index);
		}
		constexpr bool operator[] (const std::size_t index) const noexcept {
			return test(index);
		}

		constexpr bool operator==(const BitArrayBitStorageType& rhs) const noexcept {
			if (std::is_constant_evaluated()) {
				for (std::size_t i = 0; i < wordCount; i++) {
					if (bitsArray[i] != rhs.bitsArray[i]) {
						return false;
					}
				}
				return true;
			} else {
				return std::memcmp(&bitsArray, rhs.bitsArray, sizeof(bitsArray)) == 0;
			}
		}
		constexpr bool operator!=(const BitArrayBitStorageType& rhs) const noexcept {
			return !(*this == rhs);
		}

		constexpr BitArrayBitStorageType& operator&=(const BitArrayBitStorageType& other) noexcept {
			for (std::size_t i = 0; i < wordCount; i++) {
				bitsArray[i] &= other.bitsArray[i];
			}
		}
		constexpr BitArrayBitStorageType& operator|=(const BitArrayBitStorageType& other) noexcept {
			for (std::size_t i = 0; i < wordCount; i++) {
				bitsArray[i] |= other.bitsArray[i];
			}
		}
		constexpr BitArrayBitStorageType& operator^=(const BitArrayBitStorageType& other) noexcept {
			for (std::size_t i = 0; i < wordCount; i++) {
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
			for(std::size_t i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] & rhs.bitsArray[i];
			}
			return temp;
		}
		constexpr BitArrayBitStorageType operator|(BitArrayBitStorageType& rhs) noexcept {
			BitArrayBitStorageType temp;
			for (std::size_t i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] |  rhs.bitsArray[i];
			}
			return temp;
		}
		constexpr BitArrayBitStorageType operator^(BitArrayBitStorageType& rhs) noexcept {
			BitArrayBitStorageType temp;
			for (std::size_t i = 0; i < wordCount; i++) {
				temp.bitsArray[i] = self().bitsArray[i] ^ rhs.bitsArray[i];
			}
			return temp;
		}

		constexpr operator bool() const noexcept {
			return any(); 
		}
	};

	template<std::size_t bitCount>	
	using BitArray = BitArrayBitStorageType<std::uint32_t, bitCount>;
}