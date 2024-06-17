//interface
#include "print.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <unistd.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM


//implementation 
namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	Bool print(const Ascii* string, const Size size) noexcept {
		HANDLE consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (consoleOutputHandle == INVALID_HANDLE_VALUE) {
			return false;
		}

		DWORD charactersWritten;
		if (!WriteConsoleA(consoleOutputHandle, string, static_cast<DWORD>(size), &charactersWritten, NULL)) {
			return false;
		}

		return true;
	}

	Bool println(const Ascii* string, const Size size) noexcept {
		HANDLE consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (consoleOutputHandle == INVALID_HANDLE_VALUE) {
			return false;
		}

		DWORD charactersWritten;
		if (!WriteConsoleA(consoleOutputHandle, string, static_cast<DWORD>(size), &charactersWritten, NULL)) {
			return false;
		}
		const Ascii newLineCharacter = '\n';
		if (!WriteConsoleA(consoleOutputHandle, &newLineCharacter, 1, &charactersWritten, NULL)) {
			return false;
		}

		return true;
	}

	InputReturn input(Ascii* dst, const Size dstSize) noexcept {
		HANDLE consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);
		if (consoleInputHandle == INVALID_HANDLE_VALUE) {
			return InputReturn{ false, 0 };
		}

		DWORD charactersRead{ 0 };
		if (ReadConsoleA(consoleInputHandle, static_cast<LPVOID>(dst), static_cast<DWORD>(dstSize - 1), &charactersRead, NULL) == 0) {
			return InputReturn{ false, 0 };
		}

		if (charactersRead != 0) {
			dst[charactersRead - 1] = '\0';
		}

		return InputReturn{ true, charactersRead };
	}
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	Bool print(const Ascii* string, const Size size) noexcept {
		SSize bytes_written = static_cast<SSize>(write(STDOUT_FILENO, string, static_cast<size_t>(size)));
		if (bytes_written == -1) {
			return false;
		}
		return true;
	}

	Bool println(const Ascii* string, const Size size) noexcept {
		bool successful = print(string, size);
		const Ascii newLineCharacter = '\n';
		successful = successful && print(&newLineCharacter);
		return successful;
	}

	InputReturn input(Ascii* dst, const Size dstSize) noexcept {
		SSize bytesRead = static_cast<SSize>(read(STDIN_FILENO, dst, static_cast<size_t>(dstSize - 1)));
		if (bytesRead == -1) {
			return InputReturn{ false, 0 };
		}
		dst[bytesRead] = '\0'; 
		return InputReturn{ false, static_cast<Size>(bytesRead) };
	}
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
}