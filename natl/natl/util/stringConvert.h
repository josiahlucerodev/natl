#pragma once 

//own
#include "basicTypes.h"
#include "string.h"
#include "limits.h"
#include "../util/characterTest.h"
#include "../container/stringView.h"
#include "../container/string.h"
#include "../fundamental/expect.h"
#include "../math/satruatedArithmetic.h"

//interface
namespace natl {
	struct NumericConvertError {
	public:
		enum struct Flag {
			none = 0,
			noCharacters,
			valueExceedsUpperLimits,
			valueExceedsLowerLimits,
			unknownCharacter,
			expectedNumericCharacter,
		};

	public:
		Flag flag;
		Size index;

	public:
		constexpr NumericConvertError() noexcept {};
		constexpr NumericConvertError(const Flag& flagIn, const Size& indexIn) noexcept 
			:  flag(flagIn), index(indexIn) {};
		constexpr NumericConvertError(const Flag& flagIn) noexcept 
			: flag(flagIn), index(0) {};
	};

	constexpr ConstStringView convertErrorToString(const NumericConvertError::Flag convertError) {
		switch (convertError) {
		case NumericConvertError::Flag::none:
			return "none";
		case NumericConvertError::Flag::noCharacters:
			return "noCharacters";
		case NumericConvertError::Flag::valueExceedsUpperLimits:
			return "valueExceedsUpperLimits";
		case NumericConvertError::Flag::valueExceedsLowerLimits:
			return "valueExceedsLowerLimits";
		case NumericConvertError::Flag::unknownCharacter:
			return "unknownCharacter";
		case NumericConvertError::Flag::expectedNumericCharacter:
			return "expectedNumericCharacter";
		default:
			unreachable();
		}
	}

	template<typename Integer>
	constexpr Integer convertDecimalCharacterToNumber(const Ascii& character) noexcept {
		return static_cast<Integer>(character) - Integer(48);
	}
	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr natl::Expect<Integer, NumericConvertError> stringDecimalToInt(const ConstStringView& string) noexcept {
		using decayed_integer = Decay<Integer>;
		
		if (string.size() == 0) {
			return unexpected(NumericConvertError(NumericConvertError::Flag::noCharacters));
		}

		Size index = 0;
		Bool isNegative = false;

		if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
			if (string[index] == '-') {
				isNegative = true;
				index += 1;
			} else if (string[index] == '+') {
				isNegative = false;
				index += 1;
			}
		}

		decayed_integer value = 0;
		for (; index < string.size(); index++) {
			const Ascii character = string[index];
			if (character == ',') {
				continue;
			}

			if (!isDigit(character)) {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::expectedNumericCharacter, index));
			}
			decayed_integer characterValue = convertDecimalCharacterToNumber<decayed_integer>(character);
			if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
				characterValue = characterValue > 0 && isNegative ? -characterValue : characterValue;
			}

			if (natl::math::mulOverflow(value, decayed_integer(10), value) 
				|| natl::math::addOverflow(value, static_cast<decayed_integer>(characterValue), value)) {
				if (isNegative) {
					return unexpected(NumericConvertError(
						NumericConvertError::Flag::valueExceedsLowerLimits, index));
				} else {
					return unexpected(NumericConvertError(
						NumericConvertError::Flag::valueExceedsUpperLimits, index));
				}
			}
		}

		return value;
	}

	template<typename Integer>
	constexpr Integer convertHexadecimalCharacterToNumber(const Ascii& character) noexcept {
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
		requires(IsBuiltInIntegerC<Integer>)
	constexpr natl::Expect<Integer, NumericConvertError> stringHexadecimalToInt(const ConstStringView& string) noexcept {
		using decayed_integer = Decay<Integer>;
		if (string.size() == 0) {
			return unexpected(NumericConvertError(NumericConvertError::Flag::noCharacters));
		}

		Size index = 0;
		Bool isNegative = false;

		if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
			if (string[index] == '-') {
				isNegative = true;
				index += 1;
			} else if (string[index] == '+') {
				isNegative = false;
				index += 1;
			}
		}

		if (string.size() - index >= 2 && string[index] == '0' 
			&& (string[index + 1] == 'x' || string[index + 1] == 'X')) {
			index += 2;
		}

		ui64 value = 0;
		Size numCharacters = 0;
		for (; index < string.size(); index++) {
			const Ascii character = string[index];
			if (not isHexadecimalDigit(character)) {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::expectedNumericCharacter, index));
			}
			numCharacters++;
			if (numCharacters > (sizeof(Integer) * 2)) {
				if (isNegative) {
					return unexpected(NumericConvertError(
						NumericConvertError::Flag::valueExceedsLowerLimits, index));
				} else {
					return unexpected(NumericConvertError(
						NumericConvertError::Flag::valueExceedsUpperLimits, index));
				}
			}
			const decayed_integer characterValue = convertHexadecimalCharacterToNumber<decayed_integer>(character);
			value = (value << 4) + characterValue;
		}

		if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
			if (isNegative && value > static_cast<ui64>(Limits<Integer>::min())) {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::valueExceedsLowerLimits, index));
			} else if (value > static_cast<ui64>(Limits<Integer>::max())) {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::valueExceedsUpperLimits, index));
			}
		}


		if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
			if (isNegative && value == static_cast<ui64>(Limits<decayed_integer>::max()) + 1) {
				return Limits<decayed_integer>::min();
			}
			return isNegative ? -decayed_integer(value) : decayed_integer(value);
		} else {
			return static_cast<decayed_integer>(value);
		}
	}

	template<typename Integer>
	constexpr Integer convertBinaryCharacterToNumber(const Ascii& character) noexcept {
		switch (character) {
		case '0': return 0; case '1': return 1;
		default: return 0;
		}
	}

	template<typename Integer>
		requires(IsBuiltInIntegerC<Integer>)
	constexpr Expect<Integer, NumericConvertError> stringBinaryToInt(const ConstStringView& string) noexcept {
		using decayed_integer = Decay<Integer>;
		if (string.size() == 0) {
			return unexpected(NumericConvertError(NumericConvertError::Flag::noCharacters));
		}

		Size index = 0;
		Bool isNegative = false;

		if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
			if (string[index] == '-') {
				isNegative = true;
				index += 1;
			} else if (string[index] == '+') {
				isNegative = false;
				index += 1;
			}
		}

		if (string.size() - index >= 2 && string[index] == '0'
			&& (string[index + 1] == 'b' || string[index + 1] == 'B')) {
			index += 2;
		}

		ui64 value = 0;
		Size numCharacters = 0;
		for (; index < string.size(); index++) {
			const Ascii character = string[index];
			if (not isBinaryDigit(character)) {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::expectedNumericCharacter, index));
			}
			numCharacters++;
			if (numCharacters > (sizeof(Integer) * 8)) {
				if (isNegative) {
					return unexpected(NumericConvertError(
						NumericConvertError::Flag::valueExceedsLowerLimits, index));
				} else {
					return unexpected(NumericConvertError(
						NumericConvertError::Flag::valueExceedsUpperLimits, index));
				}
			}
			const ui64 characterValue = convertBinaryCharacterToNumber<decayed_integer>(character);
			value = (value << 1) + characterValue;
		}

		if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
			if (isNegative && value > static_cast<ui64>(Limits<decayed_integer>::min())) {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::valueExceedsLowerLimits, index));
			} else if (value > static_cast<ui64>(Limits<decayed_integer>::max())) {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::valueExceedsUpperLimits, index));
			}
		}

		if constexpr (IsBuiltInSignedIntegerC<decayed_integer>) {
			if (isNegative && value == static_cast<ui64>(Limits<decayed_integer>::max()) + 1) {
				return Limits<decayed_integer>::min();
			}
			return isNegative ? -decayed_integer(value) : decayed_integer(value);
		} else {
			return static_cast<decayed_integer>(value);
		}
	}

	template<typename Float>
		requires(IsBuiltInFloatingPointC<Float>)
	constexpr Expect<Float, NumericConvertError> stringDecimalToFloat(const ConstStringView& string) noexcept {
		if (string.size() == 0) {
			return unexpected(NumericConvertError(NumericConvertError::Flag::noCharacters));
		}

		Bool isNegative = false;
		Size index = 0;

		if (string[index] == '-') {
			isNegative = true;
			++index;
		}
		else if (string[index] == '+') {
			isNegative = false;
			++index;
		}

		Float result = 0.0f; 
		Bool decimalFound = false;
		Float decimalMul = 10.0f;
		Size numberOfCharacters = 0;

		for (; index < string.size(); ++index) {
			const Ascii valueCharacter = string[index];
			numberOfCharacters++;

			if (isDigit(valueCharacter)) {
				const ui64 number = convertDecimalCharacterToNumber<ui64>(valueCharacter);
				if (decimalFound) {
					result = result + (static_cast<Float>(number) / decimalMul);
					decimalMul *= 10.0f;
				} else {
					constexpr Size maxNumOfCharacter = IsSameC<f32, Decay<Float>> ? 38 : 308;
					if (numberOfCharacters > maxNumOfCharacter) {
						if (isNegative) {
							return unexpected(NumericConvertError(
								NumericConvertError::Flag::valueExceedsLowerLimits, index));
						} else {
							return unexpected(NumericConvertError(
								NumericConvertError::Flag::valueExceedsUpperLimits, index));
						}
					}
					result = result * 10.0f + static_cast<Float>(number);
				}
			} else if (valueCharacter == '.') {
				if (decimalFound) {
					return unexpected(NumericConvertError(
						NumericConvertError::Flag::unknownCharacter, index));
				}
				decimalFound = true;
				numberOfCharacters = 0;
			} else {
				return unexpected(NumericConvertError(
					NumericConvertError::Flag::expectedNumericCharacter, index));
				return 0;
			}
		}

		return isNegative ? -result : result;
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
			output.pushBack('0');
			return;
		}

		Bool isNegative = false;

		if constexpr (IsBuiltInSignedIntegerC<Integer>) {
			if (number < 0) {
				isNegative = true;
				number = -number; 
			}
		}

		Size count = 0;
		while (number > 0) {
			const char digitChar = '0' + static_cast<char>(number % 10); 
			output.pushBack(digitChar);
			number /= 10; 
			count += 1;
		}

		if constexpr (IsBuiltInSignedIntegerC<Integer>) {
			if (isNegative) {
				output.pushBack('-');
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
				output.pushBack('0');
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
			output.pushBack('0');
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
			output.pushBack('-');
		}

		const i64 integerPart = static_cast<i64>(number);
		intToStringDecimal<DynStringContainer, i64>(output, integerPart);
		output.pushBack('.');
		Float fractionalPart = number - static_cast<Float>(static_cast<i64>(number));
		if (fractionalPart == Float(0.0) || (fractionalPart - Float(0.000001)) / fractionalPart <= Float(0.0)) {
			output.pushBack('0');
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
			output.pushBack(digitCharacter);
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