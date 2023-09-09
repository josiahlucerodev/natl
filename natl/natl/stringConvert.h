#pragma once 

//std
#include <cstdint>

//own
#include "stringView.h"
#include "string.h"
#include "expect.h"

//interface
namespace natl {
	enum class StringNumericConvertError {
		unknown = 0,
		invalid,
		valueExceedsLimits,
		none,
	};

	constexpr std::int32_t convertDecimalCharacterToNumber(const char& character) noexcept {
		return character - 48;
	}
	constexpr bool checkIfStringConvertInRange(const bool hardEnd, const bool softEnd, const std::uint64_t maxEndingPosValue,
		const std::uint64_t maxTrailingValue, const std::int32_t number, const std::uint64_t value) noexcept {
		return hardEnd || (number > maxEndingPosValue && softEnd) || (number == maxEndingPosValue && value > maxTrailingValue);
	}
	constexpr bool isCharacterDecimalDigit(const char character) noexcept {
		switch (character) {
		case '0': case '1': case '2': case '3': case '4':
		case '5':case '6': case '7': case '8': case '9':
			return true;
		default:
			return false;
		}
	}
	template<typename Interger>
	constexpr  Interger stringDecimalToInt(const StringView& string, StringNumericConvertError& convertError) noexcept {
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
	constexpr  Interger stringDecimalToInt(const StringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringDecimalToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringDecimalToIntExpect(const StringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Interger value = stringDecimalToInt<Interger>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	constexpr std::int32_t convertHexCharacterToNumber(const char& character) noexcept {
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

	constexpr bool isCharacterHexadecimalDigit(const char character) noexcept {
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
	constexpr Interger stringHexadecimalToInt(const StringView& string, StringNumericConvertError& convertError) noexcept {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

		static_assert(std::is_unsigned_v<Interger>, "stringHexadecimalToInt: Interger cannot be signed");

		const std::uint32_t length = string.length();
		std::uint64_t value = 0;
		std::uint64_t mul = 1; std::uint32_t pos = 0;

		std::int64_t endIndex = -1;
		if (string.length() >= 2 && string.at(0) == '0' && string.at(1) == 'x') {
			endIndex += 2;
		}

		for (std::int64_t index = length - 1; index > endIndex; index--) {
			const char numberCharacter = string.c_str()[index];
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
	constexpr Interger stringHexadecimalToInt(const StringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringHexadecimalToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringHexadecimalToIntExpect(const StringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Interger value = stringHexadecimalToInt<Interger>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	constexpr std::int32_t convertBinaryCharacterToNumber(const char& character) noexcept {
		switch (character) {
		case '0': return 0; case '1': return 1;
		default: return 0;
		}
	}

	constexpr bool isCharacterBinaryDigit(const char character) noexcept {
		switch (character) {
		case '0': case '1':
			return true;
		default:
			return false;
		}
	}

	template<typename Interger>
	constexpr Interger stringBinaryToInt(const StringView& string, StringNumericConvertError& convertError) noexcept {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

		static_assert(std::is_unsigned_v<Interger>, "stringBinaryToInt Interger cannot be signed");
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
	constexpr Interger stringBinaryToInt(const StringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringBinaryToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringBinaryToIntExpect(const StringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Interger value = stringBinaryToInt<Interger>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	template<typename Float>
	constexpr Float stringToFloat(const StringView& string, StringNumericConvertError& convertError) noexcept {
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
	constexpr Float stringToFloat(const StringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringToFloat<Float>(string, convertError);
	}

	template<typename Float>
	constexpr Expect<Float, StringNumericConvertError> stringToFloatExpect(const StringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Float value = stringToFloat<Float>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	template<typename Interger>
	constexpr String intToStringDecimal(Interger number) noexcept {
		if (number == 0) { return String("0"); }

		String result{};
		bool isNegative = false;

		if constexpr (std::is_signed_v<Interger>) {
			if (number < 0) {
				isNegative = true;
				number = -number; 
			}
		}

		while (number > 0) {
			const char digitChar = '0' + (number % 10); 
			result.pushBack(digitChar);
			number /= 10; 
		}

		if constexpr (std::is_signed_v<Interger>) {
			if (isNegative) {
				result.pushBack('-');
			}
		}

		reverse<String::iterator>(result.begin(), result.end());
		return result;
	}

	template<typename Interger>
	constexpr String intToStringHexadecimal(Interger number, const bool addPrefix) noexcept {
		if (number == 0) {
			if (addPrefix) {
				return "0x0";
			} else {
				return "0";
			}
		}

		const char hexDigits[] = "0123456789ABCDEF";

		int temp = number;
		int numberOfDigits = 0;
		while (temp != 0) {
			temp >>= 4;
			numberOfDigits++;
		}

		char hexString[18]; 

		for (std::int32_t i = numberOfDigits - 1; i >= 0; i--) {
			int digitValue = (number >> (i * 4)) & 0xF;
			hexString[numberOfDigits - 1 - i] = hexDigits[digitValue];
		}
		hexString[numberOfDigits] = '\0';

		String result{};
		result.reserve(20);

		if (addPrefix) {
			result = "0x";
		}
		result.append(hexString);

		return result;
	}

	template<typename Interger>
	constexpr String intToStringHexadecimal(const Interger number) noexcept {
		return intToStringHexadecimal<Interger>(number, true);
	}

	template<typename Interger>
	constexpr String intToStringBinary(Interger n) noexcept {
		if (n == 0) { return String("0"); }

		std::size_t index = 0;
		char binaryString[65];
		while (n > 0) {
			if (n % 2 == 0) {
				binaryString[index] = '0';
			} else {
				binaryString[index] = '1';;
			}
			n /= 2;
			index++;
		}
		binaryString[index] = '\0';


		String result{};
		result.reserve(index + 1);
		result = binaryString;

		reverse<String::iterator>(result.begin(), result.end());
		return result;
	}

	template<typename Float>
	constexpr String floatToString(const Float number, const std::uint64_t precision) noexcept {
		String result{};
		const std::int64_t integerPart = static_cast<std::int64_t>(number);

		result = intToStringDecimal<std::int64_t>(integerPart);
		result.pushBack('.');

		float fractionalPart = number - static_cast<Float>(static_cast<int>(number));
		for (std::size_t i = 0; i < precision && fractionalPart < 1.0; ++i) {
			fractionalPart *= 10;
			const char digitCharacter = '0' + static_cast<std::int64_t>(fractionalPart);
			result.pushBack(digitCharacter);
			fractionalPart -= static_cast<std::int64_t>(fractionalPart);
		}

		return result;
	}

	template<typename Float>
	constexpr String floatToString(const Float number) noexcept {
		return floatToString<Float>(number, std::numeric_limits<std::uint64_t>::max());
	}
	
	constexpr std::int64_t stringDecimalToInt(const StringView& stringView) noexcept {
		return stringDecimalToInt<std::int64_t>(stringView);
	}
	constexpr std::uint64_t stringHexadecimalToInt(const StringView& stringView) noexcept {
		return stringHexadecimalToInt<std::uint64_t>(stringView);
	}
	constexpr std::uint64_t stringBinaryToInt(const StringView& stringView) noexcept {
		return stringBinaryToInt<std::uint64_t>(stringView);
	}
	constexpr float stringToFloat(const StringView& stringView) noexcept {
		return stringToFloat<float>(stringView);
	}

	constexpr std::int64_t stringDecimalToInt(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringDecimalToInt<std::int64_t>(stringView, convertError);
	}
	constexpr std::uint64_t stringHexadecimalToInt(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringHexadecimalToInt<std::uint64_t>(stringView, convertError);
	}
	constexpr std::uint64_t stringBinaryToInt(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringBinaryToInt<std::uint64_t>(stringView, convertError);
	}
	constexpr float stringToFloat(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringToFloat<float>(stringView, convertError);
	}

	constexpr Expect<std::int64_t, StringNumericConvertError> stringDecimalToIntExpect(const StringView& stringView) noexcept {
		return stringDecimalToIntExpect<std::int64_t>(stringView);
	}
	constexpr Expect<std::uint64_t, StringNumericConvertError> stringHexadecimalToIntExpect(const StringView& stringView) noexcept {
		return stringHexadecimalToIntExpect<std::uint64_t>(stringView);
	}
	constexpr Expect<std::uint64_t, StringNumericConvertError> stringBinaryToIntExpect(const StringView& stringView) noexcept {
		return stringBinaryToIntExpect<std::uint64_t>(stringView);
	}
	constexpr Expect<float, StringNumericConvertError> stringToFloatExpect(const StringView& stringView) noexcept {
		return stringToFloat<float>(stringView);
	}

	constexpr String intToStringDecimal(const std::int8_t number) noexcept { return intToStringDecimal<std::int8_t>(number); };
	constexpr String intToStringDecimal(const std::int16_t number) noexcept { return intToStringDecimal<std::int16_t>(number); };
	constexpr String intToStringDecimal(const std::int32_t number) noexcept { return intToStringDecimal<std::int32_t>(number); };
	constexpr String intToStringDecimal(const std::int64_t number) noexcept { return intToStringDecimal<std::int64_t>(number); };
	constexpr String intToStringDecimal(const std::uint8_t number) noexcept { return intToStringDecimal<std::uint8_t>(number); };
	constexpr String intToStringDecimal(const std::uint16_t number) noexcept { return intToStringDecimal<std::uint16_t>(number); };
	constexpr String intToStringDecimal(const std::uint32_t number) noexcept { return intToStringDecimal<std::uint32_t>(number); };
	constexpr String intToStringDecimal(const std::uint64_t number) noexcept { return intToStringDecimal<std::uint64_t>(number); };

	constexpr String intToStringHexadecimal(const std::int8_t number) noexcept { return intToStringHexadecimal<std::int8_t>(number); };
	constexpr String intToStringHexadecimal(const std::int16_t number) noexcept { return intToStringHexadecimal<std::int16_t>(number); };
	constexpr String intToStringHexadecimal(const std::int32_t number) noexcept { return intToStringHexadecimal<std::int32_t>(number); };
	constexpr String intToStringHexadecimal(const std::int64_t number) noexcept { return intToStringHexadecimal<std::int64_t>(number); };
	constexpr String intToStringHexadecimal(const std::uint8_t number) noexcept { return intToStringHexadecimal<std::uint8_t>(number); };
	constexpr String intToStringHexadecimal(const std::uint16_t number) noexcept { return intToStringHexadecimal<std::uint16_t>(number); };
	constexpr String intToStringHexadecimal(const std::uint32_t number) noexcept { return intToStringHexadecimal<std::uint32_t>(number); };
	constexpr String intToStringHexadecimal(const std::uint64_t number) noexcept { return intToStringHexadecimal<std::uint64_t>(number); };

	constexpr String intToStringBinary(const std::int8_t number) noexcept { return intToStringBinary<std::int8_t>(number); };
	constexpr String intToStringBinary(const std::int16_t number) noexcept { return intToStringBinary<std::int16_t>(number); };
	constexpr String intToStringBinary(const std::int32_t number) noexcept { return intToStringBinary<std::int32_t>(number); };
	constexpr String intToStringBinary(const std::int64_t number) noexcept { return intToStringBinary<std::int64_t>(number); };
	constexpr String intToStringBinary(const std::uint8_t number) noexcept { return intToStringBinary<std::uint8_t>(number); };
	constexpr String intToStringBinary(const std::uint16_t number) noexcept { return intToStringBinary<std::uint16_t>(number); };
	constexpr String intToStringBinary(const std::uint32_t number) noexcept { return intToStringBinary<std::uint32_t>(number); };
	constexpr String intToStringBinary(const std::uint64_t number) noexcept { return intToStringBinary<std::uint64_t>(number); };

	constexpr String floatToString(const float number) noexcept { return floatToString<float>(number); };
	constexpr String floatToString(const double number) noexcept { return floatToString<double>(number); };
}