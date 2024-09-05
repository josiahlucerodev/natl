#pragma once 

//own
#include "basicTypes.h"

//interface 
namespace natl {
	constexpr Bool isDigit(const Ascii character) noexcept {
		return character >= '0' && character <= '9';
	}
	constexpr Bool isDecimalDigit(const Ascii character) noexcept {
		return isDigit(character);
	}
	constexpr Bool isHexadecimalDigit(const Ascii character) noexcept {
		return isDigit(character) || (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F');
	}
	constexpr Bool isBinaryDigit(const char character) noexcept {
		switch (character) {
		case '0': case '1':
			return true;
		default:
			return false;
		}
	}
	constexpr Bool isLowercase(const Ascii character) noexcept {
		return character >= 'a' && character <= 'z';
	}
	constexpr Bool isUppercase(const Ascii character) noexcept {
		return character >= 'A' && character <= 'Z';
	}
	constexpr Bool isAlphabetic(const Ascii character) noexcept {
		return isLowercase(character) || isUppercase(character);
	}
	constexpr Bool isAlphanumeric(const Ascii character) noexcept {
		return isLowercase(character) || isUppercase(character) || isDigit(character);
	}
	constexpr Bool isSpace(const Ascii character) noexcept {
		switch (character) {
		case ' ':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case '\v':
			return true;
		default:
			return false;
		}
	}
	constexpr Bool isBlank(const Ascii character) noexcept {
		switch (character) {
		case ' ':
		case '\t':
			return true;
		default:
			return false;
		}
	}
	constexpr Bool isAlphabeticSpace(const Ascii character) noexcept {
		return character == ' ';
	}
	constexpr Bool isPunctuation(const Ascii character) noexcept {
		switch (character) {
		case '!':
		case '"':
		case '#':
		case '$':
		case '%':
		case '&':
		case '\'':
		case '(':
		case ')':
		case '*':
		case '+':
		case ',':
		case '-':
		case '.':
		case '/':
		case ':':
		case ';':
		case '<':
		case '=':
		case '>':
		case '?':
		case '@':
		case '[':
		case '\\':
		case ']':
		case '^':
		case '_':
		case '`':
		case '{':
		case '|':
		case '}':
		case '~':
			return true;
		default:
			return false;
		}
	}

	constexpr Ascii toLowercase(const Ascii character) noexcept {
		return isUppercase(character) ? character + ('a' - 'A') : character;
	}
	constexpr Ascii toUppercase(const Ascii character) noexcept {
		return isLowercase(character) ? character - ('a' - 'A') : character;
	}
}