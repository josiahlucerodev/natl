//interface
#include "filesystem.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM


//implementation 
namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	Option<Size> getFileSize(const FileHandle& file) noexcept {
		if (file.isHandleNotOpen()) { return OptionEmpty(); }

		LARGE_INTEGER newFilePointer;
		const Bool successfulGetFileSize = GetFileSizeEx(file.nativeHandle(), &newFilePointer);

		if (successfulGetFileSize == true) {
			return newFilePointer.QuadPart;
		} else {
			return OptionEmpty();
		}
	}

	FileHandle openFile(const Ascii* filePath, const FileOpenMode openMode) noexcept {
		DWORD desiredAccess;
		DWORD shareMode;
		DWORD creationDisposition;

		switch (openMode) {
		case FileOpenMode::readStart:
			desiredAccess = GENERIC_READ;
			shareMode = FILE_SHARE_READ;
			creationDisposition = OPEN_EXISTING;
			break;
		case FileOpenMode::writeDestroy:
			desiredAccess = GENERIC_WRITE;
			shareMode = 0;
			creationDisposition = CREATE_ALWAYS;
			break;
		case FileOpenMode::readWriteStart:
			desiredAccess = (GENERIC_READ | GENERIC_WRITE);
			shareMode = 0;
			creationDisposition = OPEN_ALWAYS;
			break;
		case FileOpenMode::readWriteDestory:
			desiredAccess = (GENERIC_READ | GENERIC_WRITE);
			shareMode = 0;
			creationDisposition = CREATE_ALWAYS;
			break;
		default:
			return{};
		}

		SECURITY_ATTRIBUTES securityAttributes;
		securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		securityAttributes.lpSecurityDescriptor = NULL;
		securityAttributes.bInheritHandle = TRUE;

		HANDLE fileHandle = CreateFileA(filePath, desiredAccess, shareMode, &securityAttributes, creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

		if (fileHandle == INVALID_HANDLE_VALUE) {
			return {};
		} else {
			return FileHandle(fileHandle);
		}
	}
	Bool closeFile(FileHandle& file) noexcept {
		if (file.isHandleNotOpen()) { return false; }
		const Bool successfulCloseFile = CloseHandle(file.nativeHandle());
		file.setHandleNull();
		return successfulCloseFile;
	}

	Option<Size> readFile(const FileHandle& file, const FileOffset offset, const FileCount count, Byte* dst) noexcept {
		if (file.isHandleNotOpen() || dst == nullptr) { return {}; }

		LARGE_INTEGER  distanceToMove;
		distanceToMove.QuadPart = offset.value();
		LARGE_INTEGER newFilePointer;
		const Bool successfulMoveFilePointer = SetFilePointerEx(file.nativeHandle(), distanceToMove, &newFilePointer, FILE_BEGIN);

		if (successfulMoveFilePointer == false) {
			return {};
		}

		DWORD numberOfBytesRead = 0;
		const Bool successfulFileRead = ReadFile(file.nativeHandle(), dst, static_cast<DWORD>(count.value()), &numberOfBytesRead, NULL);

		if (successfulFileRead == false && numberOfBytesRead == 0) {
			return {};
		}

		return static_cast<Size>(numberOfBytesRead);
	}

	Option<Size> writeFile(const FileHandle& file, const FileOffset offset, const FileCount count, const Byte* src) noexcept {
		if (file.isHandleNotOpen() || src == nullptr) { return {}; }

		LARGE_INTEGER  distanceToMove;
		distanceToMove.QuadPart = offset.value();
		LARGE_INTEGER newFilePointer;
		const Bool successfulMoveFilePointer = SetFilePointerEx(file.nativeHandle(), distanceToMove, &newFilePointer, FILE_BEGIN);
		if (successfulMoveFilePointer == false) {
			return {};
		}

		DWORD bytesWritten;
		const Bool successfulWriteFile = WriteFile(file.nativeHandle(), src, static_cast<DWORD>(count.value()), &bytesWritten, NULL);
		if (successfulWriteFile == false && bytesWritten == 0) {
			return {};
		}

		return static_cast<Size>(bytesWritten);
	}

	Bool doesFileExist(const Ascii* fileName) noexcept {
		DWORD fileAttributes = GetFileAttributesA(fileName);
		return (fileAttributes != INVALID_FILE_ATTRIBUTES);
	}

	namespace impl {
		inline Bool doesFileHaveAttributes(const Ascii* fileName, const DWORD attributes) noexcept {
			DWORD fileAttributes = GetFileAttributesA(fileName);
			return (fileAttributes != INVALID_FILE_ATTRIBUTES) && ((fileAttributes & attributes) != 0);
		}
		inline Bool doesFileNotHaveAttributes(const Ascii* fileName, const DWORD attributes) noexcept {
			DWORD fileAttributes = GetFileAttributesA(fileName);
			return (fileAttributes != INVALID_FILE_ATTRIBUTES) && ((fileAttributes & attributes) == 0);
		}
	}

	inline Bool isBlockFile(const Ascii* fileName) noexcept {
		return impl::doesFileHaveAttributes(fileName, FILE_ATTRIBUTE_DEVICE);
	}
	inline Bool isDirectory(const Ascii* fileName) noexcept {
		return impl::doesFileHaveAttributes(fileName, FILE_ATTRIBUTE_DIRECTORY);
	}
	inline Bool isFifo(const Ascii* fileName) noexcept {
		HANDLE namedPipe = CreateNamedPipeA(fileName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 0, 0, 0, NULL);
		if (namedPipe != INVALID_HANDLE_VALUE) {
			CloseHandle(namedPipe);
			return true;
		} else {
			return false;
		}
	}
	Bool isRegularFile(const Ascii* fileName) noexcept {
		return impl::doesFileNotHaveAttributes(fileName, FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_VIRTUAL);
	}
	Bool isSymbolicLink(const Ascii* fileName) noexcept {
		return impl::doesFileHaveAttributes(fileName, FILE_ATTRIBUTE_REPARSE_POINT);
	}

	FileType getFileType(const Ascii* fileName) noexcept {
		DWORD fileAttributes = GetFileAttributesA(fileName);

		if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
			return FileType::notFound;
		}

		if ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			return FileType::directory;
		}

		if ((fileAttributes & FILE_ATTRIBUTE_DEVICE)) {
			return FileType::block;
		}

		if ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
			return FileType::symbolicLink;
		}

		HANDLE namedPipe = CreateNamedPipeA(fileName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 0, 0, 0, NULL);
		if (namedPipe != INVALID_HANDLE_VALUE) {
			CloseHandle(namedPipe);
			return FileType::fifo;
		}

		return FileType::unknown;
	}

	Option<Size> getWorkingDirectorySize() noexcept {
		DWORD workingDirectorySize = GetCurrentDirectoryA(0, NULL);
		if (workingDirectorySize == 0) {
			return OptionEmpty{};
		}
		return static_cast<Size>(workingDirectorySize);
	}
	Bool getWorkingDirectory(Ascii* dst, const Size dstSize) noexcept {
		DWORD result = GetCurrentDirectoryA(static_cast<DWORD>(dstSize), dst);
		return result != 0;
	}

#endif // NATL_WINDOWS_PLATFORM

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)

	using UnixNativeFileHandle = GenericInt;
	constexpr inline UnixNativeFileHandle unixFileOpFailValue = -1;

	Option<Size> getFileSize(const FileHandle& file) noexcept {
		struct stat64 fileStat;
		if (fstat64(file.nativeHandle(), &fileStat) == unixFileOpFailValue) {
			return false;
		} else {
			return static_cast<Size>(fileStat.st_size);
		}
	}

	FileHandle openFile(const Ascii* filePath, const FileOpenMode openMode) noexcept {
		GenericInt fileOpenFlags;

		switch (openMode) {
		case FileOpenMode::readStart:
			fileOpenFlags = O_RDONLY;
			break;
		case FileOpenMode::writeDestroy:
			fileOpenFlags = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case FileOpenMode::readWriteStart:
			fileOpenFlags = O_RDWR | O_CREAT;
			break;
		case FileOpenMode::readWriteDestory:
			fileOpenFlags = O_RDWR | O_CREAT | O_TRUNC;
			break;
		default:
			return{};
		}

		const GenericInt fileOpenResult = open64(filePath, fileOpenFlags);
		if (fileOpenResult == unixFileOpFailValue) {
			return false;
		} else {
			return fileOpenResult;
		}
	}

	Bool closeFile(FileHandle& file) noexcept {
		if (file.isHandleNotOpen()) { return false; }
		const GenericInt closeFileResult = close(file.nativeHandle());
		file.setHandleNull();
		return closeFileResult != unixFileOpFailValue;
	}

	Option<Size> readFile(const FileHandle& file, const FileOffset offset, const FileCount count, Byte* dst) noexcept {
		if (file.isHandleNotOpen() || dst == nullptr) { return false; }

		const i64 fileLseekResult = lseek64(file.nativeHandle(), static_cast<long int>(offset.value()), SEEK_SET);
		if (fileLseekResult == unixFileOpFailValue) {
			return {};
		}

		const i64 fileReadResult = read(file.nativeHandle(), reinterpret_cast<void*>(dst), count.value());
		if (fileReadResult == unixFileOpFailValue) {
			return {};
		}

		return static_cast<Size>(fileReadResult);
	}

	Option<Size> writeFile(const FileHandle& file, const FileOffset offset, const FileCount count, const Byte* src) noexcept {
		if (file.isHandleNotOpen() || src == nullptr) { return false; }

		const i64 fileLseekResult = lseek64(file.nativeHandle(), static_cast<long int>(offset.value()), SEEK_SET);
		if (fileLseekResult == unixFileOpFailValue) {
			return false;
		}

		const i64 fileWriteResult = write(file.nativeHandle(), reinterpret_cast<const void*>(src), count.value());
		if (fileWriteResult == unixFileOpFailValue) {
			return false;
		}

		return static_cast<Size>(fileWriteResult);
	}

	Option<Size> getWorkingDirectorySize() noexcept {
		Ascii tempWorkingDirectoryPathStorage[PATH_MAX];
		Ascii* workingDirectoryPath = getcwd(tempWorkingDirectoryPathStorage, PATH_MAX);

		if (workingDirectoryPath == nullptr) {
			return OptionEmpty{};
		}

		return cstringLength(workingDirectoryPath);
	}

	Bool getWorkingDirectory(Ascii* dst, const Size dstSize) noexcept {
		Ascii* workingDirectoryPath = getcwd(dst, dstSize);
		return workingDirectoryPath != nullptr;
	}

	Bool doesFileExist(const Ascii* fileName) noexcept {
		struct stat64 fileStat;
		return stat64(fileName, &fileStat) != unixFileOpFailValue;
	}

	Bool isBlockFile(const Ascii* fileName) noexcept {
		struct stat64 fileStat;
		if (stat64(fileName, &fileStat) == unixFileOpFailValue) {
			return false;
		}
		return S_ISBLK(fileStat.st_mode);
	}
	Bool isDirectory(const Ascii* fileName) noexcept {
		struct stat64 fileStat;
		if (stat64(fileName, &fileStat) == unixFileOpFailValue) {
			return false;
		}
		return S_ISDIR(fileStat.st_mode);
	}
	Bool isFifo(const Ascii* fileName) noexcept {
		struct stat64 fileStat;
		if (stat64(fileName, &fileStat) == unixFileOpFailValue) {
			return false;
		}
		return S_ISFIFO(fileStat.st_mode);
	}
	Bool isRegularFile(const Ascii* fileName) noexcept {
		struct stat64 fileStat;
		if (stat64(fileName, &fileStat) == unixFileOpFailValue) {
			return false;
		}
		return S_ISREG(fileStat.st_mode);
	}
	Bool isSymbolicLink(const Ascii* fileName) noexcept {
		struct stat64 fileStat;
		if (stat64(fileName, &fileStat) == unixFileOpFailValue) {
			return false;
		}
		return S_ISLNK(fileStat.st_mode);
	}

	FileType getFileType(const Ascii* fileName) noexcept {
		struct stat64 fileStat;
		if (stat64(fileName, &fileStat) == unixFileOpFailValue) {
			return FileType::notFound;
		}
		if (S_ISREG(fileStat.st_mode)) {
			return FileType::regular;
		}
		if (S_ISDIR(fileStat.st_mode)) {
			return FileType::directory;
		}
		if (S_ISBLK(fileStat.st_mode)) {
			return FileType::block;
		}
		if (S_ISLNK(fileStat.st_mode)) {
			return FileType::symbolicLink;
		}
		if (S_ISFIFO(fileStat.st_mode)) {
			return FileType::fifo;
		}
		return FileType::unknown;
	}

#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
}