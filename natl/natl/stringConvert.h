#pragma once 

//std
#include <cstdint>

//own
#include "basicTypes.h"
#include "stringView.h"
#include "expect.h"
#include "string.h"

//interface
namespace natl {
	enum class StringNumericConvertError {
		unknown = 0,
		invalid,
		valueExceedsLimits,
		none,
	};

	constexpr StringView convertErrorToStringView(const StringNumericConvertError convertError) {
		switch (convertError) {
		case StringNumericConvertError::unknown:
			return "unknown";
		case StringNumericConvertError::invalid:
			return "invalid";
		case StringNumericConvertError::valueExceedsLimits:
			return "value exceeds numeric limits";
		case StringNumericConvertError::none:
			return "none";
		default:
			return "unknown case";
		}
	}


	constexpr ui64 convertDecimalCharacterToNumber(const char& character) noexcept {
		return static_cast<ui64>(character) - 48ULL;
	}
	constexpr bool checkIfStringConvertInRange(const bool hardEnd, const bool softEnd, const ui64 maxEndingPosValue,
		const ui64 maxTrailingValue, const ui64 number, const ui64 value) noexcept {
		return hardEnd || (number > maxEndingPosValue && softEnd) || (static_cast<ui64>(number) == maxEndingPosValue && value > maxTrailingValue);
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

		const Size endIndex = std::is_signed_v<Interger> && string.c_str()[0] == '-' ? 0 : static_cast<Size>(-1);
		const Size length = string.length();
		std::conditional_t<std::is_signed_v<Interger>, i64, ui64> value = 0;
		ui64 mul = 1; ui32 pos = 0;

		for (Size i = length - 1; i > endIndex; i--) {
			const char numberCharacter = string.c_str()[i];
			if (numberCharacter == ',') {
				continue;
			}
			if (!isCharacterDecimalDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const ui64 number = convertDecimalCharacterToNumber(numberCharacter);

			if constexpr (std::same_as<i64, Interger>) { //9223372036854775807
				if (checkIfStringConvertInRange(pos > 18, pos == 18, 9, 223372036854775807, number, static_cast<ui64>(value))) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return string.c_str()[0] == '-' ? std::numeric_limits<i64>::min() : std::numeric_limits<i64>::max();
				}
			}
			else if constexpr (std::same_as<ui64, Interger>) { //18,446,744,073,709,551,615
				if (checkIfStringConvertInRange(pos > 19, pos == 19, 1, 8446744073709551615, number, static_cast<ui64>(value))) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<ui64>::max();
				}
			}

			value += number * mul;
			mul = mul * 10;
			pos += 1;

			if constexpr (!std::same_as<i64, Interger> && !std::same_as<ui64, Interger>) {
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

	constexpr ui64 convertHexCharacterToNumber(const char& character) noexcept {
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

		const Size length = string.length();
		ui64 value = 0;
		ui64 mul = 1; ui32 pos = 0;

		Size endIndex = static_cast<Size>(-1);
		if (string.length() >= 2 && string.at(0) == '0' && string.at(1) == 'x') {
			endIndex += 2;
		}

		for (Size index = length - 1; index > endIndex; index--) {
			const char numberCharacter = string.c_str()[index];
			if (numberCharacter == ',' || numberCharacter == '_') {
				continue;
			}
			if (!isCharacterHexadecimalDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const ui64 number = convertHexCharacterToNumber(numberCharacter);

			if constexpr (std::same_as<ui64, Interger>) { //0xFFFFFFFFFFFFFFFF
				if (pos > 15) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<ui64>::max();
				}
			}

			value += number * mul;
			mul = mul * 16;
			pos += 1;

			if constexpr (!std::same_as<ui64, Interger>) {
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

	constexpr ui64 convertBinaryCharacterToNumber(const char& character) noexcept {
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
		const Size length = string.length();
		ui64 value = 0;
		ui64 mul = 1; ui32 pos = 0;

		for (i64 i = static_cast<i64>(length) - 1; i > -1; i--) {
			const char numberCharacter = string.c_str()[i];
			if (numberCharacter == ',' || numberCharacter == '_') {
				continue;
			}
			if (!isCharacterBinaryDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const ui64 number = convertBinaryCharacterToNumber(numberCharacter);

			if constexpr (std::same_as<ui64, Interger>) { //1111111111111111111111111111111111111111111111111111111111111111
				if (pos > 63) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return std::numeric_limits<ui64>::max();
				}
			}

			value += number * mul;
			mul = mul * 2;
			pos += 1;

			if constexpr (!std::same_as<ui64, Interger>) {
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

		const Size length = string.length();
		Float result = 0.0f; 
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
				const ui64 number = convertDecimalCharacterToNumber(numberCharacter);
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
			result.push_back(digitChar);
			number /= 10; 
		}

		if constexpr (std::is_signed_v<Interger>) {
			if (isNegative) {
				result.push_back('-');
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

		ui64 temp = static_cast<ui64>(number);
		ui64 numberOfDigits = 0;
		while (temp != 0) {
			temp >>= 4;
			numberOfDigits++;
		}

		char hexString[18]; 

		for (i64 i = static_cast<i64>(numberOfDigits) - 1; i >= 0; i--) {
			ui64 digitValue = (number >> (i * 4)) & 0xF;
			hexString[numberOfDigits - 1 - static_cast<ui64>(i)] = hexDigits[digitValue];
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
	constexpr String floatToString(const Float number, const ui64 precision) noexcept {
		String result{};
		const i64 integerPart = static_cast<i64>(number);

		result = intToStringDecimal<i64>(integerPart);
		result.push_back('.');

		Float fractionalPart = number - static_cast<Float>(static_cast<int>(number));
		for (std::size_t i = 0; i < precision && fractionalPart < Float(1.0); ++i) {
			fractionalPart *= 10;
			const char digitCharacter = '0' + static_cast<char>(static_cast<i64>(fractionalPart));
			result.push_back(digitCharacter);
			fractionalPart -= static_cast<Float>(static_cast<i64>(fractionalPart));
		}

		return result;
	}

	template<typename Float>
	constexpr String floatToString(const Float number) noexcept {
		return floatToString<Float>(number, std::numeric_limits<ui64>::max());
	}
	
	constexpr i64 stringDecimalToInt(const StringView& stringView) noexcept {
		return stringDecimalToInt<i64>(stringView);
	}
	constexpr ui64 stringHexadecimalToInt(const StringView& stringView) noexcept {
		return stringHexadecimalToInt<ui64>(stringView);
	}
	constexpr ui64 stringBinaryToInt(const StringView& stringView) noexcept {
		return stringBinaryToInt<ui64>(stringView);
	}
	constexpr f32 stringToFloat(const StringView& stringView) noexcept {
		return stringToFloat<f32>(stringView);
	}

	constexpr i64 stringDecimalToInt(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringDecimalToInt<i64>(stringView, convertError);
	}
	constexpr ui64 stringHexadecimalToInt(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringHexadecimalToInt<ui64>(stringView, convertError);
	}
	constexpr ui64 stringBinaryToInt(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringBinaryToInt<ui64>(stringView, convertError);
	}
	constexpr f32 stringToFloat(const StringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringToFloat<f32>(stringView, convertError);
	}

	constexpr Expect<i64, StringNumericConvertError> stringDecimalToIntExpect(const StringView& stringView) noexcept {
		return stringDecimalToIntExpect<i64>(stringView);
	}
	constexpr Expect<ui64, StringNumericConvertError> stringHexadecimalToIntExpect(const StringView& stringView) noexcept {
		return stringHexadecimalToIntExpect<ui64>(stringView);
	}
	constexpr Expect<ui64, StringNumericConvertError> stringBinaryToIntExpect(const StringView& stringView) noexcept {
		return stringBinaryToIntExpect<ui64>(stringView);
	}
	constexpr Expect<f32, StringNumericConvertError> stringToFloatExpect(const StringView& stringView) noexcept {
		return stringToFloat<f32>(stringView);
	}

	constexpr String intToStringDecimal(const i8 number) noexcept { return intToStringDecimal<i8>(number); };
	constexpr String intToStringDecimal(const i16 number) noexcept { return intToStringDecimal<i16>(number); };
	constexpr String intToStringDecimal(const i32 number) noexcept { return intToStringDecimal<i32>(number); };
	constexpr String intToStringDecimal(const i64 number) noexcept { return intToStringDecimal<i64>(number); };
	constexpr String intToStringDecimal(const ui8 number) noexcept { return intToStringDecimal<ui8>(number); };
	constexpr String intToStringDecimal(const ui16 number) noexcept { return intToStringDecimal<ui16>(number); };
	constexpr String intToStringDecimal(const ui32 number) noexcept { return intToStringDecimal<ui32>(number); };
	constexpr String intToStringDecimal(const ui64 number) noexcept { return intToStringDecimal<ui64>(number); };

	constexpr String intToStringHexadecimal(const i8 number) noexcept { return intToStringHexadecimal<i8>(number); };
	constexpr String intToStringHexadecimal(const i16 number) noexcept { return intToStringHexadecimal<i16>(number); };
	constexpr String intToStringHexadecimal(const i32 number) noexcept { return intToStringHexadecimal<i32>(number); };
	constexpr String intToStringHexadecimal(const i64 number) noexcept { return intToStringHexadecimal<i64>(number); };
	constexpr String intToStringHexadecimal(const ui8 number) noexcept { return intToStringHexadecimal<ui8>(number); };
	constexpr String intToStringHexadecimal(const ui16 number) noexcept { return intToStringHexadecimal<ui16>(number); };
	constexpr String intToStringHexadecimal(const ui32 number) noexcept { return intToStringHexadecimal<ui32>(number); };
	constexpr String intToStringHexadecimal(const ui64 number) noexcept { return intToStringHexadecimal<ui64>(number); };

	constexpr String intToStringBinary(const i8 number) noexcept { return intToStringBinary<i8>(number); };
	constexpr String intToStringBinary(const i16 number) noexcept { return intToStringBinary<i16>(number); };
	constexpr String intToStringBinary(const i32 number) noexcept { return intToStringBinary<i32>(number); };
	constexpr String intToStringBinary(const i64 number) noexcept { return intToStringBinary<i64>(number); };
	constexpr String intToStringBinary(const ui8 number) noexcept { return intToStringBinary<ui8>(number); };
	constexpr String intToStringBinary(const ui16 number) noexcept { return intToStringBinary<ui16>(number); };
	constexpr String intToStringBinary(const ui32 number) noexcept { return intToStringBinary<ui32>(number); };
	constexpr String intToStringBinary(const ui64 number) noexcept { return intToStringBinary<ui64>(number); };

	constexpr String floatToString(const f32 number) noexcept { return floatToString<f32>(number); };
	constexpr String floatToString(const f64 number) noexcept { return floatToString<f64>(number); };
}