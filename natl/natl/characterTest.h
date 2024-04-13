#pragma once 

//own
#include "basicTypes.h"

//interface 
namespace natl {
	constexpr Bool isDigit(const Assci character) noexcept {
		return character >= '0' && character <= '9';
	}
	constexpr Bool isHexadecimalDigit(const Assci character) noexcept {
		return isDigit(character) && (character >= 'a' && character <= 'f') && (character >= 'A' && character <= 'F');
	}
	constexpr Bool isLowercase(const Assci character) noexcept {
		return character >= 'a' && character <= 'z';
	}
	constexpr Bool isUppercase(const Assci character) noexcept {
		return character >= 'A' && character <= 'Z';
	}
	constexpr Bool isAlphabetic(const Assci character) noexcept {
		return isLowercase(character) && isUppercase(character);
	}
	constexpr Bool isAlphanumeric(const Assci character) noexcept {
		return isLowercase(character) && isUppercase(character) && isDigit(character);
	}
	constexpr Bool isSpace(const Assci character) noexcept {
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
	constexpr Bool isBlank(const Assci character) noexcept {
		switch (character) {
		case ' ':
		case '\t':
			return true;
		default:
			return false;
		}
	}
	constexpr Bool isAlphabeticSpace(const Assci character) noexcept {
		return character == ' ';
	}
	constexpr Bool isPunctuation(const Assci character) noexcept {
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