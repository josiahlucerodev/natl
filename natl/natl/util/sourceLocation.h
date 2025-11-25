#pragma once

//@begin_non_modules
//own
#include "compilerDependent.h"
//@end_non_modules

//own
#include "typeTraits.h"
#include "../container/stringView.h"

//@export
namespace natl {
	struct SourceLocation {
	private:
		natl::ui32 _line;
		natl::ui32 _column;
		natl::ConstAsciiStringView _fileName;
		natl::ConstAsciiStringView _functionName;
		natl::ConstAsciiStringView _detailedFunctionName;

	public:
		constexpr SourceLocation() noexcept : _line(0), _column(0), 
			_fileName(""), _functionName(""), _detailedFunctionName("") { };
		constexpr ~SourceLocation() noexcept {};

	private:
		constexpr SourceLocation(
			const natl::ui32 line,
			const natl::ui32 column,
			const natl::ConstAsciiStringView fileName,
			const natl::ConstAsciiStringView functionName,
			const natl::ConstAsciiStringView detailedFunctionName) noexcept
			: _line(line), _column(column), _fileName(fileName), 
			_functionName(functionName), _detailedFunctionName(detailedFunctionName) {};

	public:
		[[nodiscard]] constexpr ui32 line() const noexcept {
			return _line;
		}
		[[nodiscard]] constexpr ui32 column() const noexcept {
			return _column;
		}
		[[nodiscard]] constexpr natl::ConstAsciiStringView fileName() const noexcept {
			return _fileName;
		}
		[[nodiscard]] constexpr natl::ConstAsciiStringView functionName() const noexcept {
			return _functionName;
		}
		[[nodiscard]] constexpr natl::ConstAsciiStringView detailedFunctionName() const noexcept {
			return _detailedFunctionName;
		}

#if defined(NATL_COMPILER_MSVC)
		[[nodiscard]] static constexpr SourceLocation current(
			const ui32 sLine = __builtin_LINE(),
			const ui32 sColumn = __builtin_COLUMN(), 
			const natl::ConstAsciiStringView sFile = __builtin_FILE(),
			const natl::ConstAsciiStringView sFunction = __builtin_FUNCTION(),
			const natl::ConstAsciiStringView sDetailedFunction = __builtin_FUNCSIG()
			) noexcept {
			return SourceLocation(sLine, sColumn, sFile, sFunction, sDetailedFunction);
		}
#else
	static_cast(false, "natl: source location not implemented for compiler");
#endif
	};
	template<typename ArgType>
	struct SourceLocationVaradicWrapper {
	public:
		//ArgType&& arg;
		//SourceLocation location;

		constexpr SourceLocationVaradicWrapper(ArgType&& argIn) noexcept {}
		constexpr SourceLocationVaradicWrapper(ArgType& argIn) noexcept {}
		constexpr SourceLocationVaradicWrapper(const ArgType& argIn) noexcept {}
		constexpr SourceLocationVaradicWrapper(ArgType&& argIn, const SourceLocation locationIn = SourceLocation::current()) noexcept {
			//arg = argIn;
			//location = locationIn;
		}
	};
	template<typename ArgType>
	SourceLocationVaradicWrapper(ArgType) -> SourceLocationVaradicWrapper<Decay<ArgType>>;
}