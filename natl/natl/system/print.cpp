//@interface
#include "print.h"

//@begin_non_modules
//own
#include "../util/compilerDependent.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <unistd.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
//@end_non_modules
 
namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	Bool print(const Ascii* string, const Size size) noexcept {		
		HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (outputHandle == INVALID_HANDLE_VALUE) {
			return false;
		}

		DWORD mode;
		DWORD charactersWritten;
		if (GetConsoleMode(outputHandle, &mode)) {
			if (!WriteConsoleA(outputHandle, string, static_cast<DWORD>(size), &charactersWritten, NULL)) {
				return false;
			}
		} else {
			WriteFile(outputHandle, string, static_cast<DWORD>(size), &charactersWritten, NULL);
		}

		return true;
	}

	Bool println(const Ascii* string, const Size size) noexcept {
		if (!print(string, size)) {
			return false;
		}
		return print("\n", 1);
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