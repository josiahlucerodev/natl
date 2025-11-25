#pragma once 

//@begin_non_modules

//std
#include <type_traits>
#include <cstdlib>
#include <stdlib.h>

//own
#include "compilerDependent.h"

//@end_non_modules

//@export 
namespace natl {	
	template<typename ToType, typename FormType>
	ToType& punCast(FormType& from) noexcept {
		return *reinterpret_cast<ToType*>(&from);
	}

    enum struct ProgramPlatformType : int {
        unknownPlatform,
        unixPlatform, 
        windowsPlatform,
        webPlatform
    };

    consteval ProgramPlatformType getPlatformType() noexcept {
#if defined(NATL_UNIX_PLATFORM)
        return ProgramPlatformType::unixPlatform;
#elif defined(NATL_WINDOWS_PLATFORM)
        return ProgramPlatformType::windowsPlatform;
#elif defined(NATL_WEB_PLATFORM)
        return ProgramPlatformType::webPlatform;
#else
        static_cast("natl: get platform type not implemented");
#endif
    }

    consteval bool natlInDebug() noexcept {
#ifdef NATL_IN_DEBUG
        return true;
#else
        return false;
#endif
    }

	[[nodiscard]] constexpr bool isConstantEvaluated() noexcept {
		return std::is_constant_evaluated();
	}

    namespace impl {
        inline void CEEdebugBreak() noexcept;
    }

    NATL_FORCE_INLINE constexpr void constantEvaluatedError() noexcept {
        if (isConstantEvaluated()) {
            int* ptr = nullptr;
            *ptr = 0;
        } else {
            impl::CEEdebugBreak();
        }
    }


    NATL_FORCE_INLINE constexpr void debugBreak() noexcept {
#ifdef NATL_IN_DEBUG
        if (isConstantEvaluated()) {
            constantEvaluatedError();
        } else {


#if defined(NATL_COMPILER_EMSCRIPTEN) 
            emscripten_debugger();
#elif defined(NATL_COMPILER_CLANG)
            __builtin_debugtrap();
#elif defined(NATL_COMPILER_GCC)
            __builtin_trap();
#elif defined(NATL_COMPILER_MSVC)
            __debugbreak();
#else 
            static_cast(false, "natl: debug break not implemented");
#endif 

        }
#endif
    }

    namespace impl {
        inline void CEEdebugBreak() noexcept {
            debugBreak();
        }
    }

    [[noreturn]] NATL_FORCE_INLINE constexpr void terminate() noexcept {
        if (isConstantEvaluated()) {
            constantEvaluatedError();

#ifdef NATL_COMPILER_GCC
            abort();
#endif // NATL_COMPILER_GCC

        } else {
#ifdef NATL_IN_DEBUG
            debugBreak();
#endif // NATL_IN_DEBUG
            abort();
        }
    }

	[[noreturn]] inline void unreachable() noexcept {
        if constexpr (natlInDebug()) {
            debugBreak();
        }

#if defined(NATL_COMPILER_EMSCRIPTEN) 
        __builtin_unreachable();
#elif defined(NATL_COMPILER_CLANG)
        __builtin_unreachable();
#elif defined(NATL_COMPILER_GCC)
        __builtin_unreachable();
#elif defined(NATL_COMPILER_MSVC)
        __assume(false);
#else 
        static_cast(false, "natl: unreachable not implemented");
#endif 
	}

    consteval const char* platformTypeToString(const ProgramPlatformType platformType) noexcept {
        switch (platformType) {
        case ProgramPlatformType::unixPlatform:
            return "unix";
        case ProgramPlatformType::windowsPlatform:
            return "windows";
        case ProgramPlatformType::webPlatform:
            return "web";
        case ProgramPlatformType::unknownPlatform:
            return "unknown";
        default:
            unreachable();
        }
    }

    consteval const char* getPlatformName() noexcept {
        return platformTypeToString(getPlatformType());
    }

    [[noreturn]] inline void rexit(int exitCode) noexcept {
        std::exit(exitCode);
    }
}