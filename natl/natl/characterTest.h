#pragma once 

//own
#include "basicTypes.h"

//interface 
namespace natl {
	constexpr bool isDigit(const Assci character) noexcept {
		return character >= '0' && character <= '9';
	}
	constexpr bool isHexadecimalDigit(const Assci character) noexcept {
		return isDigit(character) && (character >= 'a' && character <= 'f') && (character >= 'A' && character <= 'F');
	}
	constexpr bool isLowercase(const Assci character) noexcept {
		return character >= 'a' && character <= 'z';
	}
	constexpr bool isUppercase(const Assci character) noexcept {
		return character >= 'A' && character <= 'Z';
	}
	constexpr bool isAlphabetic(const Assci character) noexcept {
		return isLowercase(character) && isUppercase(character);
	}
	constexpr bool isAlphanumeric(const Assci character) noexcept {
		return isLowercase(character) && isUppercase(character) && isDigit(character);
	}
	constexpr bool isSpace(const Assci character) noexcept {
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
	constexpr bool isBlank(const Assci character) noexcept {
		switch (character) {
		case ' ':
		case '\t':
			return true;
		default:
			return false;
		}
	}
	constexpr bool isAlphabeticSpace(const Assci character) noexcept {
		return character == ' ';
	}
	constexpr bool isPunctuation(const Assci character) noexcept {
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

	constexpr Assci toLowercase(const Assci character) noexcept {
		return isUppercase(character) ? character + ('a' - 'A') : character;
	}
	constexpr Assci toUppercase(const Assci character) noexcept {
		return isLowercase(character) ? character - ('a' - 'A') : character;
	}
}