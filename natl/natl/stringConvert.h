#pragma once 

//std
#include <cstdint>

//own
#include "basicTypes.h"
#include "stringView.h"
#include "expect.h"
#include "string.h"
#include "limits.h"

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
	constexpr Bool isCharacterDecimalDigit(const char character) noexcept {
		switch (character) {
		case '0': case '1': case '2': case '3': case '4':
		case '5':case '6': case '7': case '8': case '9':
			return true;
		default:
			return false;
		}
	}
	template<typename Interger>
	constexpr Interger stringDecimalToInt(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
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

		const i64 endIndex = std::is_signed_v<Interger> && string.c_str()[0] == '-' ? 0 : static_cast<i64>(-1);
		const Size length = string.length();
		std::conditional_t<std::is_signed_v<Interger>, i64, ui64> value = 0;
		ui64 mul = 1; ui32 pos = 0;

		for (i64 i = static_cast<i64>(length) - 1; static_cast<i64>(i) > endIndex; i--) {
			const char numberCharacter = string.c_str()[static_cast<Size>(i)];
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
					return string.c_str()[0] == '-' ? Limits<i64>::min() : Limits<i64>::max();
				}
			}
			else if constexpr (std::same_as<ui64, Interger>) { //18,446,744,073,709,551,615
				if (checkIfStringConvertInRange(pos > 19, pos == 19, 1, 8446744073709551615, number, static_cast<ui64>(value))) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<ui64>::max();
				}
			}

			value += static_cast<decltype(value)>(number * mul);
			mul = mul * 10;
			pos += 1;

			if constexpr (!std::same_as<i64, Interger> && !std::same_as<ui64, Interger>) {
				if (value > Limits<Interger>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<Interger>::max();
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
	constexpr  Interger stringDecimalToInt(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringDecimalToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringDecimalToIntExpect(const ConstStringView& string) noexcept {
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

	constexpr Bool isCharacterHexadecimalDigit(const char character) noexcept {
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
	constexpr Interger stringHexadecimalToInt(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
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
					return Limits<ui64>::max();
				}
			}

			value += number * mul;
			mul = mul * 16;
			pos += 1;

			if constexpr (!std::same_as<ui64, Interger>) {
				if (value > Limits<Interger>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<Interger>::max();
				}
			}
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Interger>
	constexpr Interger stringHexadecimalToInt(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringHexadecimalToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringHexadecimalToIntExpect(const ConstStringView& string) noexcept {
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

	constexpr Bool isCharacterBinaryDigit(const char character) noexcept {
		switch (character) {
		case '0': case '1':
			return true;
		default:
			return false;
		}
	}

	template<typename Interger>
	constexpr Interger stringBinaryToInt(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
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
					return Limits<ui64>::max();
				}
			}

			value += number * mul;
			mul = mul * 2;
			pos += 1;

			if constexpr (!std::same_as<ui64, Interger>) {
				if (value > Limits<Interger>::max()) {
					convertError = StringNumericConvertError::valueExceedsLimits;
					return Limits<Interger>::max();
				}
			}
		}

		convertError = StringNumericConvertError::none;
		return value;
	}

	template<typename Interger>
	constexpr Interger stringBinaryToInt(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringBinaryToInt<Interger>(string, convertError);
	}

	template<typename Interger>
	constexpr Expect<Interger, StringNumericConvertError> stringBinaryToIntExpect(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Interger value = stringBinaryToInt<Interger>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	template<typename Float>
	constexpr Float stringToFloat(const ConstStringView& string, StringNumericConvertError& convertError) noexcept {
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

		if (result > Limits<Float>::max() && !isNegative) {
			convertError = StringNumericConvertError::valueExceedsLimits;
			return Limits<Float>::max();
		} else if (result < Limits<Float>::lowest() && isNegative) {
			convertError = StringNumericConvertError::valueExceedsLimits;
			return Limits<Float>::lowest();
		}

		convertError = StringNumericConvertError::none;
		return isNegative ? -result : result;
	}

	template<typename Float>
	constexpr Float stringToFloat(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError;
		return stringToFloat<Float>(string, convertError);
	}

	template<typename Float>
	constexpr Expect<Float, StringNumericConvertError> stringToFloatExpect(const ConstStringView& string) noexcept {
		StringNumericConvertError convertError = StringNumericConvertError::unknown;
		Float value = stringToFloat<Float>(string, convertError);
		if (convertError != StringNumericConvertError::none) {
			return unexpected<StringNumericConvertError>(convertError);
		} else {
			return value;
		}
	}

	template<class DynStringContainer>
	concept IsConvertDynStringContainer =
		ContainerHasPushBackFunction<DynStringContainer, char>
		&& HasIteratorType<DynStringContainer>
		&& HasBeginIterator<DynStringContainer>
		&& HasEndIterator<DynStringContainer> && 
		requires(DynStringContainer& string) { 
			{ string.append("test string", 11) }; 
			{ string.reserve(std::declval<Size>()) }; 
	};

	template<class DynStringContainer, typename Interger>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimalStringType(DynStringContainer& output, Interger number) noexcept {
		if (number == 0) { 
			output.push_back('0');
			return;
		}

		Bool isNegative = false;

		if constexpr (std::is_signed_v<Interger>) {
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

		if constexpr (std::is_signed_v<Interger>) {
			if (isNegative) {
				output.push_back('-');
				count += 1;
			}
		}

		reverse<typename DynStringContainer::iterator>(output.end() - static_cast<PtrDiff>(count), output.end());
	}

	template<typename Interger>
	constexpr String intToStringDecimalType(Interger number) noexcept {
		String result{};
		intToStringDecimalStringType<String, Interger>(result, number);
		return result;
	}




	template<class DynStringContainer, typename Interger>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimalStringType(DynStringContainer& output, Interger number, const Bool addPrefix) noexcept {
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

	template<typename Interger>
	constexpr String intToStringHexadecimalType(Interger number, const Bool addPrefix) noexcept {
		String result{};
		intToStringHexadecimalStringType<String, Interger>(result, number, addPrefix);
		return result;
	}

	template<typename Interger>
	constexpr String intToStringHexadecimalType(const Interger number) noexcept {
		return intToStringHexadecimalType<Interger>(number, true);
	}

	template<class DynStringContainer, typename Interger>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinaryStringType(DynStringContainer& output, Interger n) noexcept {
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
				binaryString[index] = '1';;
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

	template<typename Interger>
	constexpr String intToStringBinaryType(Interger n) noexcept {
		String result{};
		intToStringBinaryStringType<String, Interger>(result, n);
		return result;
	}


	template<class DynStringContainer, typename Float>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void floatToStringStringType(DynStringContainer& output, const Float number, const ui64 precision) noexcept {
		const i64 integerPart = static_cast<i64>(number);

		intToStringDecimalStringType<DynStringContainer, i64>(output, integerPart);
		output.push_back('.');

		Float fractionalPart = number - static_cast<Float>(static_cast<int>(number));
		for (Size i = 0; i < precision && fractionalPart < Float(1.0); ++i) {
			fractionalPart *= 10;
			const char digitCharacter = '0' + static_cast<char>(static_cast<i64>(fractionalPart));
			output.push_back(digitCharacter);
			fractionalPart -= static_cast<Float>(static_cast<i64>(fractionalPart));
		}
	}

	template<typename Float>
	constexpr String floatToStringType(const Float number, const ui64 precision) noexcept {
		String result{};
		floatToStringStringType<String, Float>(result, number, precision);
		return result;
	}

	template<typename Float>
	constexpr String floatToStringType(const Float number) noexcept {
		return floatToStringType<Float>(number, Limits<ui64>::max());
	}
	
	constexpr i64 stringDecimalToInt(const ConstStringView& stringView) noexcept {
		return stringDecimalToInt<i64>(stringView);
	}
	constexpr ui64 stringHexadecimalToInt(const ConstStringView& stringView) noexcept {
		return stringHexadecimalToInt<ui64>(stringView);
	}
	constexpr ui64 stringBinaryToInt(const ConstStringView& stringView) noexcept {
		return stringBinaryToInt<ui64>(stringView);
	}
	constexpr f32 stringToFloat(const ConstStringView& stringView) noexcept {
		return stringToFloat<f32>(stringView);
	}

	constexpr i64 stringDecimalToInt(const ConstStringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringDecimalToInt<i64>(stringView, convertError);
	}
	constexpr ui64 stringHexadecimalToInt(const ConstStringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringHexadecimalToInt<ui64>(stringView, convertError);
	}
	constexpr ui64 stringBinaryToInt(const ConstStringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringBinaryToInt<ui64>(stringView, convertError);
	}
	constexpr f32 stringToFloat(const ConstStringView& stringView, StringNumericConvertError& convertError) noexcept {
		return stringToFloat<f32>(stringView, convertError);
	}

	constexpr Expect<i64, StringNumericConvertError> stringDecimalToIntExpect(const ConstStringView& stringView) noexcept {
		return stringDecimalToIntExpect<i64>(stringView);
	}
	constexpr Expect<ui64, StringNumericConvertError> stringHexadecimalToIntExpect(const ConstStringView& stringView) noexcept {
		return stringHexadecimalToIntExpect<ui64>(stringView);
	}
	constexpr Expect<ui64, StringNumericConvertError> stringBinaryToIntExpect(const ConstStringView& stringView) noexcept {
		return stringBinaryToIntExpect<ui64>(stringView);
	}
	constexpr Expect<f32, StringNumericConvertError> stringToFloatExpect(const ConstStringView& stringView) noexcept {
		return stringToFloat<f32>(stringView);
	}


	constexpr String intToStringDecimal(const i8 number) noexcept { return intToStringDecimalType<i8>(number); };
	constexpr String intToStringDecimal(const i16 number) noexcept { return intToStringDecimalType<i16>(number); };
	constexpr String intToStringDecimal(const i32 number) noexcept { return intToStringDecimalType<i32>(number); };
	constexpr String intToStringDecimal(const i64 number) noexcept { return intToStringDecimalType<i64>(number); };
	constexpr String intToStringDecimal(const ui8 number) noexcept { return intToStringDecimalType<ui8>(number); };
	constexpr String intToStringDecimal(const ui16 number) noexcept { return intToStringDecimalType<ui16>(number); };
	constexpr String intToStringDecimal(const ui32 number) noexcept { return intToStringDecimalType<ui32>(number); };
	constexpr String intToStringDecimal(const ui64 number) noexcept { return intToStringDecimalType<ui64>(number); };

	constexpr String floatToStringDecimal(const f32 number) noexcept { return floatToStringType<f32>(number); };
	constexpr String floatToStringDecimal(const f64 number) noexcept { return floatToStringType<f64>(number); };

	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const i8 number) noexcept { intToStringDecimalType<DynStringContainer, i8>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const i16 number) noexcept { intToStringDecimalType<DynStringContainer, i16>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const i32 number) noexcept { intToStringDecimalType<DynStringContainer, i32>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const i64 number) noexcept { intToStringDecimalType<DynStringContainer, i64>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const ui8 number) noexcept { intToStringDecimalType<DynStringContainer, ui8>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const ui16 number) noexcept { intToStringDecimalType<DynStringContainer, ui16>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const ui32 number) noexcept { intToStringDecimalType<DynStringContainer, ui32>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringDecimal(DynStringContainer& output, const ui64 number) noexcept { intToStringDecimalType<DynStringContainer, ui64>(output, number); };

	constexpr String intToStringHexadecimal(const i8 number) noexcept { return intToStringHexadecimalType<i8>(number); };
	constexpr String intToStringHexadecimal(const i16 number) noexcept { return intToStringHexadecimalType<i16>(number); };
	constexpr String intToStringHexadecimal(const i32 number) noexcept { return intToStringHexadecimalType<i32>(number); };
	constexpr String intToStringHexadecimal(const i64 number) noexcept { return intToStringHexadecimalType<i64>(number); };
	constexpr String intToStringHexadecimal(const ui8 number) noexcept { return intToStringHexadecimalType<ui8>(number); };
	constexpr String intToStringHexadecimal(const ui16 number) noexcept { return intToStringHexadecimalType<ui16>(number); };
	constexpr String intToStringHexadecimal(const ui32 number) noexcept { return intToStringHexadecimalType<ui32>(number); };
	constexpr String intToStringHexadecimal(const ui64 number) noexcept { return intToStringHexadecimalType<ui64>(number); };

	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const i8 number) noexcept { intToStringHexadecimalType<DynStringContainer, i8>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const i16 number) noexcept { intToStringHexadecimalType<DynStringContainer, i16>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const i32 number) noexcept { intToStringHexadecimalType<DynStringContainer, i32>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const i64 number) noexcept { intToStringHexadecimalType<DynStringContainer, i64>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const ui8 number) noexcept { intToStringHexadecimalType<DynStringContainer, ui8>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const ui16 number) noexcept { intToStringHexadecimalType<DynStringContainer, ui16>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const ui32 number) noexcept { intToStringHexadecimalType<DynStringContainer, ui32>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringHexadecimal(DynStringContainer& output, const ui64 number) noexcept { intToStringHexadecimalType<DynStringContainer, ui64>(output, number); };

	constexpr String intToStringBinary(const i8 number) noexcept { return intToStringBinaryType<i8>(number); };
	constexpr String intToStringBinary(const i16 number) noexcept { return intToStringBinaryType<i16>(number); };
	constexpr String intToStringBinary(const i32 number) noexcept { return intToStringBinaryType<i32>(number); };
	constexpr String intToStringBinary(const i64 number) noexcept { return intToStringBinaryType<i64>(number); };
	constexpr String intToStringBinary(const ui8 number) noexcept { return intToStringBinaryType<ui8>(number); };
	constexpr String intToStringBinary(const ui16 number) noexcept { return intToStringBinaryType<ui16>(number); };
	constexpr String intToStringBinary(const ui32 number) noexcept { return intToStringBinaryType<ui32>(number); };
	constexpr String intToStringBinary(const ui64 number) noexcept { return intToStringBinaryType<ui64>(number); };

	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const i8 number) noexcept { intToStringBinaryType<DynStringContainer, i8>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const i16 number) noexcept { intToStringBinaryType<DynStringContainer, i16>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const i32 number) noexcept { intToStringBinaryType<DynStringContainer, i32>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const i64 number) noexcept { intToStringBinaryType<DynStringContainer, i64>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const ui8 number) noexcept { intToStringBinaryType<DynStringContainer, ui8>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const ui16 number) noexcept { intToStringBinaryType<DynStringContainer, ui16>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const ui32 number) noexcept { intToStringBinaryType<DynStringContainer, ui32>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void intToStringBinary(DynStringContainer& output, const ui64 number) noexcept { intToStringBinaryType<DynStringContainer, ui64>(output, number); };

	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void floatToString(DynStringContainer& output, const f32 number) noexcept { floatToStringType<DynStringContainer, f32>(output, number); };
	template<class DynStringContainer>
		requires(IsConvertDynStringContainer<DynStringContainer>)
	constexpr void floatToString(DynStringContainer& output, const f64 number) noexcept { floatToStringType<DynStringContainer, f64>(output, number); };
}