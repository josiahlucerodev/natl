#pragma once 

//std
#include <cstdint>

//own
#include "stringView.h"
#include "expect.h"

//interface
namespace natl {
	enum class StringNumericConvertError {
		unknown = 0,
		invalid,
		valueExceedsLimits,
		none,
	};

	constexpr std::int32_t convertDecimalCharacterToNumber(const char& character) {
		return character - 48;
	}
	constexpr bool checkIfStringConvertInRange(const bool hardEnd, const bool softEnd, const std::uint64_t maxEndingPosValue,
		const std::uint64_t maxTrailingValue, const std::int32_t number, const std::uint64_t value) {
		return hardEnd || (number > maxEndingPosValue && softEnd) || (number == maxEndingPosValue && value > maxTrailingValue);
	}
	constexpr bool isCharacterDecimalDigit(const char character) {
		switch (character) {
		case '0': case '1': case '2': case '3': case '4':
		case '5':case '6': case '7': case '8': case '9':
			return true;
		default:
			return false;
		}
	}
	template<typename Interger>
	constexpr  Interger stringDecimalToInt(const StringView& string, StringNumericConvertError& convertError) {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}
		if (string.c_str()[0] == '-') {
			if constexpr (std::is_unsigned_v<Interger>) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
		}

		const std::int64_t endIndex = std::is_signed_v<Interger> && string.c_str()[0] == '-' ? 0 : -1;
		const std::uint32_t length = string.length();
		std::conditional_t<std::is_signed_v<Interger>, std::int64_t, std::uint64_t> value = 0;
		std::uint64_t mul = 1; std::uint32_t pos = 0;

		for (std::int64_t i = length - 1; i > endIndex; i--) {
			const char numberCharacter = string.c_str()[i];
			if (numberCharacter == ',') {
				continue;
			}
			if (!isCharacterDecimalDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const std::int32_t number = convertDecimalCharacterToNumber(numberCharacter);

			if constexpr (std::same_as<std::int64_t, Interger>) { //9223372036854775807
				if (checkIfStringConvertInRange(pos > 18, pos == 18, 9, 223372036854775807, number, value)) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return string.c_str()[0] == '-' ? std::numeric_limits<std::int64_t>::min() : std::numeric_limits<std::int64_t>::max();
				}
			}
			else if constexpr (std::same_as<std::uint64_t, Interger>) { //18,446,744,073,709,551,615
				if (checkIfStringConvertInRange(pos > 19, pos == 19, 1, 8446744073709551615, number, value)) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<std::uint64_t>::max();
				}
			}

			value += number * mul;
			mul = mul * 10;
			pos += 1;

			if constexpr (!std::same_as<std::int64_t, Interger> && !std::same_as<std::uint64_t, Interger>) {
				if (value > std::numeric_limits<Interger>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<Interger>::max();
				}
			}
		}

		if (string.c_str()[0] == '-') {
			value *= -1;
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Interger>
	constexpr  Interger stringDecimalToInt(const StringView& string) {
		StringNumericConvertError convertError;
		return stringDecimalToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringDecimalToIntExpect(const StringView& string) {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Interger value = stringDecimalToInt<Interger>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	constexpr std::int32_t convertHexCharacterToNumber(const char& character) {
		switch (character) {
		case '0': return 0; case '1': return 1;
		case '2': return 2; case '3': return 3;
		case '4': return 4; case '5': return 5;
		case '6': return 6; case '7': return 7;
		case '8': return 8; case '9': return 9;
		case 'A': return 10; case 'a': return 10;
		case 'B': return 11; case 'b': return 11;
		case 'C': return 12; case 'c': return 12;
		case 'D': return 13; case 'd': return 13;
		case 'E': return 14; case 'e': return 14;
		case 'F': return 15; case 'f': return 15;
		default: return 0;
		}
	}

	constexpr bool isCharacterHexadecimalDigit(const char character) {
		switch (character) {
		case '0': case '1': case '2': case '3': case '4':
		case '5':case '6': case '7': case '8': case '9':
		case 'A': case 'a': case 'B': case 'b':
		case 'C': case 'c': case 'D': case 'd':
		case 'E': case 'e': case 'F': case 'f':
			return true;
		default:
			return false;
		}
	}

	template<typename Interger>
	constexpr Interger stringHexadecimalToInt(const StringView& string, StringNumericConvertError& convertError) {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

		static_assert(std::is_signed_v<Interger>, "stringHexadecimalToInt: Interger cannot be signed");
		const std::uint32_t length = string.length();
		std::uint64_t value = 0;
		std::uint64_t mul = 1; std::uint32_t pos = 0;

		for (std::int64_t i = length - 1; i > -1; i--) {
			const char numberCharacter = string.c_str()[i];
			if (numberCharacter == ',' || numberCharacter == '_') {
				continue;
			}
			if (!isCharacterHexadecimalDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const std::int32_t number = convertHexCharacterToNumber(numberCharacter);

			if constexpr (std::same_as<std::uint64_t, Interger>) { //0xFFFFFFFFFFFFFFFF
				if (pos > 15) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<std::uint64_t>::max();
				}
			}

			value += number * mul;
			mul = mul * 16;
			pos += 1;

			if constexpr (!std::same_as<std::uint64_t, Interger>) {
				if (value > std::numeric_limits<Interger>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<Interger>::max();
				}
			}
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Interger>
	constexpr Interger stringHexadecimalToInt(const StringView& string) {
		StringNumericConvertError convertError;
		return stringHexadecimalToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringHexadecimalToIntExpect(const StringView& string) {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Interger value = stringHexadecimalToInt<Interger>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	constexpr std::int32_t convertBinaryCharacterToNumber(const char& character) {
		switch (character) {
		case '0': return 0; case '1': return 1;
		default: return 0;
		}
	}

	constexpr bool isCharacterBinaryDigit(const char character) {
		switch (character) {
		case '0': case '1':
			return true;
		default:
			return false;
		}
	}

	template<typename Interger>
	constexpr Interger stringBinaryToInt(const StringView& string, StringNumericConvertError& convertError) {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

		static_assert(std::is_signed_v<Interger>, "stringBinaryToInt Interger cannot be signed");
		const std::uint32_t length = string.length();
		std::uint64_t value = 0;
		std::uint64_t mul = 1; std::uint32_t pos = 0;

		for (std::int64_t i = length - 1; i > -1; i--) {
			const char numberCharacter = string.c_str()[i];
			if (numberCharacter == ',' || numberCharacter == '_') {
				continue;
			}
			if (!isCharacterBinaryDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const std::int32_t number = convertBinaryCharacterToNumber(numberCharacter);

			if constexpr (std::same_as<std::uint64_t, Interger>) { //1111111111111111111111111111111111111111111111111111111111111111
				if (pos > 63) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<std::uint64_t>::max();
				}
			}

			value += number * mul;
			mul = mul * 2;
			pos += 1;

			if constexpr (!std::same_as<std::uint64_t, Interger>) {
				if (value > std::numeric_limits<Interger>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<Interger>::max();
				}
			}
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Interger>
	constexpr Interger stringBinaryToInt(const StringView& string) {
		StringNumericConvertError convertError;
		return stringBinaryToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringBinaryToIntExpect(const StringView& string) {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Interger value = stringBinaryToInt<Interger>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	template<typename Float>
	constexpr Float stringDecimalToFloat(const StringView& string, StringNumericConvertError& convertError) {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

		const std::uint32_t length = string.length();
		Float result = 0.0f; Float fraction = 0.1f;
		bool isNegative = false;
		bool decimalFound = false;
		Float decimalMul = 10.0f;
		std::size_t index = 0;

		// Check for a sign
		if (string.c_str()[0] == '-') {
			isNegative = true;
			++index;
		}
		else if (string.c_str()[0] == '+') {
			isNegative = false;
			++index;
		}

		for (; index < length; ++index) {
			const char numberCharacter = string.c_str()[index];

			if (isCharacterDecimalDigit(numberCharacter)) {
				const std::int32_t number = convertDecimalCharacterToNumber(numberCharacter);
				if (decimalFound) {
					result = result + (static_cast<Float>(number) / decimalMul);
					decimalMul *= 10.0f;
				}
				else {
					result = result * 10.0f + static_cast<Float>(number);
				}

			}
			else if (numberCharacter == '.') {
				if (decimalFound) {
					convertError = StringNumericConvertError::invalid;
					return 0;
				}

				decimalFound = true;
			}
			else {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
		}

		//Check if the resulting float is too big or too small
		if (result > std::numeric_limits<Float>::max() && !isNegative) {
			convertError = StringNumericConvertError::valueExceedsLimits;
			return std::numeric_limits<Float>::max();
		} else if (result < std::numeric_limits<Float>::lowest() && isNegative) {
			convertError = StringNumericConvertError::valueExceedsLimits;
			return std::numeric_limits<Float>::lowest();
		}

		convertError = StringNumericConvertError::none;
		return isNegative ? -result : result;
	}

	template<typename Float>
	constexpr Float stringDecimalToFloat(const StringView& string) {
		StringNumericConvertError convertError;
		return stringDecimalToFloat<Float>(string, convertError);
	}

	template<typename Float>
	constexpr Expect<Float, StringNumericConvertError> stringDecimalToFloatExpect(const StringView& string) {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Float value = stringDecimalToFloat<Float>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}
}