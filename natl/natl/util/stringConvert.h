#pragma once 

//own
#include "basicTypes.h"
#include "string.h"
#include "limits.h"
#include "../util/characterTest.h"
#include "../container/stringView.h"
#include "../container/string.h"
#include "../fundamental/expect.h"

//interface
namespace natl {
	enum class StringNumericConvertError {
		unknown = 0,
		invalid,
		valueExceedsLimits,
		none,
	};

	constexpr ConstStringView convertErrorToStringView(const StringNumericConvertError convertError) {
		switch (convertError) {
		case StringNumericConvertError::unknown:
			return "unknown";
		case StringNumericConvertError::invalid:
			return "invalid";
		case StringNumericConvertError::valueExceedsLimits:
			return "value exceeds numeric Limits";
		case StringNumericConvertError::none:
			return "none";
		default:
			return "unknown case";
		}
	}


	constexpr ui64 convertDecimalCharacterToNumber(const char& character) noexcept {
		return static_cast<ui64>(character) - ui64(48);
	}
	constexpr Bool checkIfStringConvertInRange(const Bool hardEnd, const Bool softEnd, const ui64 maxEndingPosValue,
		const ui64 maxTrailingValue, const ui64 number, const ui64 value) noexcept {
		return hardEnd || (number > maxEndingPosValue && softEnd) || (static_cast<ui64>(number) == maxEndingPosValue && value > maxTrailingValue);
	}
	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr Integer stringDecimalToInt(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}
		if (string.c_str()[0] == '-') {
			if constexpr (IsBuiltInUnsignedInteger<Integer>) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
		}

		const i64 endIndex = IsBuiltInSignedInteger<Integer> && string.c_str()[0] == '-' ? 0 : static_cast<i64>(-1);
		const Size length = string.length();
		Conditional<IsBuiltInSignedInteger<Integer>, i64, ui64> value = 0;
		ui64 mul = 1; ui32 pos = 0;

		for (i64 i = static_cast<i64>(length) - 1; static_cast<i64>(i) > endIndex; i--) {
			const char numberCharacter = string.c_str()[static_cast<Size>(i)];
			if (numberCharacter == ',') {
				continue;
			}
			if (!isDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const ui64 number = convertDecimalCharacterToNumber(numberCharacter);

			if constexpr (SameAs<i64, Integer>) { //9223372036854775807
				if (checkIfStringConvertInRange(pos > 18, pos == 18, 9, 223372036854775807, number, static_cast<ui64>(value))) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return string.c_str()[0] == '-' ? Limits<i64>::min() : Limits<i64>::max();
				}
			}
			else if constexpr (SameAs<ui64, Integer>) { //18,446,744,073,709,551,615
				if (checkIfStringConvertInRange(pos > 19, pos == 19, 1, 8446744073709551615, number, static_cast<ui64>(value))) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<ui64>::max();
				}
			}

			value += static_cast<decltype(value)>(number * mul);
			mul = mul * 10;
			pos += 1;

			if constexpr (!SameAs<i64, Integer> && !SameAs<ui64, Integer>) {
				if (value > Limits<Integer>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<Integer>::max();
				}
			}
		}

		if constexpr (IsBuiltInSignedInteger<Integer>) {
			if (string.c_str()[0] == '-') {
				value *= -1;
			}
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr  Integer stringDecimalToInt(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringDecimalToInt<Integer>(string, convertError);
	}

	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr Expect<Integer, StringNumericConvertError> stringDecimalToIntExpect(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Integer value = stringDecimalToInt<Integer>(string, convertError);
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

	template<typename Integer>
		requires(IsBuiltInUnsignedInteger<Integer>)
	constexpr Integer stringHexadecimalToInt(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

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
			if (!isHexadecimalDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const ui64 number = convertHexCharacterToNumber(numberCharacter);

			if constexpr (SameAs<ui64, Integer>) { //0xFFFFFFFFFFFFFFFF
				if (pos > 15) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<ui64>::max();
				}
			}

			value += number * mul;
			mul = mul * 16;
			pos += 1;

			if constexpr (!SameAs<ui64, Integer>) {
				if (value > Limits<Integer>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<Integer>::max();
				}
			}
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Integer>
	constexpr Integer stringHexadecimalToInt(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringHexadecimalToInt<Integer>(string, convertError);
	}

	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr Expect<Integer, StringNumericConvertError> stringHexadecimalToIntExpect(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Integer value = stringHexadecimalToInt<Integer>(string, convertError);
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

	template<typename Integer>
		requires(IsBuiltInUnsignedInteger<Integer>)
	constexpr Integer stringBinaryToInt(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

		const Size length = string.length();
		ui64 value = 0;
		ui64 mul = 1; ui32 pos = 0;

		for (i64 i = static_cast<i64>(length) - 1; i > -1; i--) {
			const char numberCharacter = string.c_str()[i];
			if (numberCharacter == ',' || numberCharacter == '_') {
				continue;
			}
			if (!isBinaryDigit(numberCharacter)) {
				convertError = StringNumericConvertError::invalid;
				return 0;
			}
			const ui64 number = convertBinaryCharacterToNumber(numberCharacter);

			if constexpr (SameAs<ui64, Integer>) { //1111111111111111111111111111111111111111111111111111111111111111
				if (pos > 63) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<ui64>::max();
				}
			}

			value += number * mul;
			mul = mul * 2;
			pos += 1;

			if constexpr (!SameAs<ui64, Integer>) {
				if (value > Limits<Integer>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<Integer>::max();
				}
			}
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr Integer stringBinaryToInt(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringBinaryToInt<Integer>(string, convertError);
	}

	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr Expect<Integer, StringNumericConvertError> stringBinaryToIntExpect(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Integer value = stringBinaryToInt<Integer>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	template<typename Float>
		requires(IsBuiltInFloatingPointC<Float>)
	constexpr Float stringDecimalToFloat(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
		if (string.size() == 0) {
			convertError = StringNumericConvertError::invalid;
			return 0;
		}

		const Size length = string.length();
		Float result = 0.0f; 
		Bool isNegative = false;
		Bool decimalFound = false;
		Float decimalMul = 10.0f;
		Size index = 0;

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

			if (isDigit(numberCharacter)) {
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

		if (result > Limits<Float>::max() && !isNegative) {
			convertError = StringNumericConvertError::valueExceedsLimits;
			return Limits<Float>::max();
		} else if (result < Limits<Float>::min() && isNegative) {
			convertError = StringNumericConvertError::valueExceedsLimits;
			return Limits<Float>::min();
		}

		convertError = StringNumericConvertError::none;
		return isNegative ? -result : result;
	}

	template<typename Float>
		requires(IsBuiltInFloatingPointC<Float>)
	constexpr Float stringDecimalToFloat(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringDecimalToFloat<Float>(string, convertError);
	}

	template<typename Float>
		requires(IsBuiltInFloatingPointC<Float>)
	constexpr Expect<Float, StringNumericConvertError> stringToFloatExpect(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Float value = stringDecimalToFloat<Float>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	template<typename DynStringContainer>
	concept IsConvertDynStringContainer =
		ContainerHasPushBackFunction<DynStringContainer, char>
		&& HasIteratorType<DynStringContainer>
		&& HasBeginIterator<DynStringContainer>
		&& HasEndIterator<DynStringContainer> && 
		requires(DynStringContainer& string) { 
			{ string.append("test string", 11) }; 
			{ string.reserve(natl::declval<Size>()) }; 
	};

	template<typename DynStringContainer, typename Integer>
		requires(IsConvertDynStringContainer<DynStringContainer> && IsBuiltInIntegerC<Integer>)
	constexpr void intToStringDecimal(DynStringContainer& output, Integer number) noexcept {
		if (number == 0) { 
			output.push_back('0');
			return;
		}

		Bool isNegative = false;

		if constexpr (IsBuiltInSignedInteger<Integer>) {
			if (number < 0) {
				isNegative = true;
				number = -number; 
			}
		}

		Size count = 0;
		while (number > 0) {
			const char digitChar = '0' + static_cast<char>(number % 10); 
			output.push_back(digitChar);
			number /= 10; 
			count += 1;
		}

		if constexpr (IsBuiltInSignedInteger<Integer>) {
			if (isNegative) {
				output.push_back('-');
				count += 1;
			}
		}

		reverse<typename DynStringContainer::iterator>(output.end() - static_cast<PtrDiff>(count), output.end());
	}

	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr String intToStringDecimal(Integer number) noexcept {
		String result{};
		intToStringDecimal<String, Integer>(result, number);
		return result;
	}

	template<typename DynStringContainer, typename Integer>
		requires(IsConvertDynStringContainer<DynStringContainer> && IsBuiltInIntegerC<Integer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, Integer number, const Bool addPrefix) noexcept {
		if (number == 0) {
			if (addPrefix) {
				output.append("0x0", 3);
			} else {
				output.push_back('0');
			}
			return;
		}

		const char hexDigits[] = "0123456789ABCDEF";

		ui64 temp = static_cast<ui64>(number);
		Size numberOfDigits = 0;
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

		output.reserve(20);

		if (addPrefix) {
			output.append("0x", 2);
		}

		output.append(hexString, numberOfDigits);
	}

	template<typename Integer>
	constexpr String intToStringHexadecimal(Integer number, const Bool addPrefix) noexcept {
		String result{};
		intToStringHexadecimal<String, Integer>(result, number, addPrefix);
		return result;
	}

	template<typename Integer>
	constexpr String intToStringHexadecimal(const Integer number) noexcept {
		return intToStringHexadecimal<Integer>(number, true);
	}

	template<typename DynStringContainer, typename Integer>
		requires(IsConvertDynStringContainer<DynStringContainer> && IsBuiltInIntegerC<Integer>)
	constexpr void intToStringBinary(DynStringContainer& output, Integer n) noexcept {
		if (n == 0) {
			output.push_back('0');
			return;
		}

		Size index = 0;
		char binaryString[65];
		while (n > 0) {
			if (n % 2 == 0) {
				binaryString[index] = '0';
			} else {
				binaryString[index] = '1';
			}
			n /= 2;
			index++;
		}
		binaryString[index] = '\0';

		char* reverseBegin = binaryString;
		char* reverseEnd = reverseBegin + index;
		reverse<char*>(reverseBegin, reverseEnd);

		const Size count = index;
		output.append(binaryString, count);
	}

	template<typename Integer>
	constexpr String intToStringBinary(Integer n) noexcept {
		String result{};
		intToStringBinary<String, Integer>(result, n);
		return result;
	}


	template<typename DynStringContainer, typename Float>
		requires(IsConvertDynStringContainer<DynStringContainer> && IsBuiltInFloatingPointC<Float>)
	constexpr void floatToStringDecimal(DynStringContainer& output, const Float number, Size precision) noexcept {
		if (number > Float(-1) && number < Float(0.0)) {
			output.push_back('-');
		}

		const i64 integerPart = static_cast<i64>(number);
		intToStringDecimal<DynStringContainer, i64>(output, integerPart);
		output.push_back('.');
		Float fractionalPart = number - static_cast<Float>(static_cast<i64>(number));
		if (fractionalPart == Float(0.0) || (fractionalPart - Float(0.000001)) / fractionalPart <= Float(0.0)) {
			output.push_back('0');
			return;
		}

		if constexpr (sizeof(Float) == 4) {
			precision = natl::min<Size>(precision, 10);
		}
		if constexpr (sizeof(Float) == 8) {
			precision = natl::min<Size>(precision, 20);
		}

		if (fractionalPart < Float(0.0)) {
			fractionalPart *= -1;
		}

		for (Size i = 0; i < precision && fractionalPart > Float(0.00001); ++i) {
			fractionalPart *= 10;
			const char digitCharacter = '0' + static_cast<char>(static_cast<i64>(fractionalPart));
			output.push_back(digitCharacter);
			fractionalPart -= static_cast<Float>(static_cast<i64>(fractionalPart));
		}
	}

	template<typename Float>
		requires(IsBuiltInFloatingPointC<Float>)
	constexpr String floatToStringDecimal(const Float number, const Size precision) noexcept {
		String result{};
		floatToStringDecimal<String, Float>(result, number, precision);
		return result;
	}

	template<typename Float>
		requires(IsBuiltInFloatingPointC<Float>)
	constexpr String floatToStringDecimal(const Float number) noexcept {
		return floatToStringDecimal<Float>(number, Limits<Size>::max());
	}	
}