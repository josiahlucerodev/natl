#pragma once

//std
#include <compare>
#include <coroutine>

//compiler 
#if defined(__EMSCRIPTEN__)
#define NATL_COMPILER_EMSCRIPTEN
#elif defined(__GNUC__)
#define NATL_COMPILER_GCC
#elif defined(__clang__)
#define NATL_COMPILER_CLANG
#elif defined(_MSC_VER)
#define NATL_COMPILER_MSVC
#endif 

//force inline 
#if defined(NATL_COMPILER_EMSCRIPTEN) 
#define NATL_FORCE_INLINE __attribute__((always_inline)) inline
#elif defined(NATL_COMPILER_CLANG)
#define NATL_FORCE_INLINE [[clang::always_inline]] inline
#elif defined(NATL_COMPILER_GCC)
#define NATL_FORCE_INLINE [[gnu::always_inline]] inline
#elif defined(NATL_COMPILER_MSVC)
#define NATL_FORCE_INLINE __forceinline
#else
static_cast(false, "natl: force inline for compiler not implemented");
#endif

//in debug 
//NATL_IN_DEBUG
#ifdef _DEBUG
#define NATL_IN_DEBUG
#endif //  _DEBUG

#ifndef NATL_IN_DEBUG
#ifdef NDEBUG
#define NATL_IN_DEBUG
#endif // NDEBUG
#endif // !NATL_IN_DEBUG


#ifndef NATL_IN_DEBUG
#ifdef _LIBCPP___DEBUG
#define NATL_IN_DEBUG
#include <emscripten.h>
#endif // _LIBCPP___DEBUG
#endif // !NATL_IN_DEBUG

//bit 
#if defined(NATL_COMPILER_EMSCRIPTEN) ||  defined(NATL_COMPILER_CLANG) || defined(NATL_COMPILER_GCC)
#if defined(__LP64__) || defined(_LP64)
#define NATL_64BIT
#else
#define NATL_32BIT
#endif
#endif //  NATL_COMPILER_EMSCRIPTEN || NATL_COMPILER_CLANG || NATL_COMPILER_GCC


#if !(defined(NATL_64BIT) || defined(NATL_32BIT))
#ifdef NATL_COMPILER_MSVC
#if defined(_WIN64)
#define NATL_64BIT
#else
#define NATL_32BIT
#endif
#endif //NATL_COMPILER_MSVC
#endif // ! NATL_64BIT || NATL_32BIT

//platform
#if defined(NATL_COMPILER_EMSCRIPTEN)
#define NATL_WEB_PLATFORM
#elif defined(__unix__)
#define NATL_UNIX_PLATFORM
#elif defined(_WIN32) || defined(WIN32)
#define NATL_WINDOWS_PLATFORM
#else 
static_assert(false, "natl: platform type not supported");
#endif

//architecture

#if defined(NATL_COMPILER_EMSCRIPTEN) 

#define NATL_ARCHITECTURE_WASM

#elif defined(NATL_COMPILER_CLANG)

static_cast(false, "natl: architecture for compiler not implemented");
#elif defined(NATL_COMPILER_GCC)

#ifdef __x86_64__ 
#define NATL_ARCHITECTURE_X86_64
#else 
static_assert(false, "natl: unknown architecture");
#endif

#elif defined(NATL_COMPILER_MSVC)

#ifdef _M_X64 
#define NATL_ARCHITECTURE_X86_64
#else 
static_assert(false, "natl: unknown architecture");
#endif

#else
static_cast(false, "natl: architecture for compiler not implemented");
#endif

//counter 
#if defined(NATL_COMPILER_EMSCRIPTEN) 
#define NATL_COUNTER __COUNTER__
#elif defined(NATL_COMPILER_CLANG)
#define NATL_COUNTER __COUNTER__
#elif defined(NATL_COMPILER_GCC)
#define NATL_COUNTER __COUNTER__
#elif defined(NATL_COMPILER_MSVC)
#define NATL_COUNTER __COUNTER__
#else
static_cast(false, "natl: counter for compiler not implemented");
#endif

//warnings 
#if defined(NATL_COMPILER_EMSCRIPTEN) 
#define NATL_DISABLE_WARNINGS \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wall\"")
#define NATL_ENABLE_WARNINGS _Pragma("GCC diagnostic pop")
#elif defined(NATL_COMPILER_CLANG)
#define NATL_DISABLE_WARNINGS \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wall\"")
#define NATL_ENABLE_WARNINGS _Pragma("GCC diagnostic pop")
#elif defined(NATL_COMPILER_GCC)
#define NATL_DISABLE_WARNINGS \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wall\"")
#define NATL_ENABLE_WARNINGS _Pragma("GCC diagnostic pop")
#elif defined(NATL_COMPILER_MSVC)
#define NATL_DISABLE_WARNINGS __pragma(warning(push, 0))
#define NATL_ENABLE_WARNINGS __pragma(warning(pop))
#else
static_cast(false, "natl: disable warnings not implmeented");
static_cast(false, "natl: enable warnings not implmeented");
#endif

#if __cplusplus >= 199711L
#define  NATL_PCH_CXX_98 
#endif

#if __cplusplus >= 201103L
#define  NATL_PCH_CXX_11 
#endif

#if __cplusplus >= 201703L
#define  NATL_PCH_CXX_17 
#endif

#if __cplusplus >= 201402L
#define  NATL_PCH_CXX_14 
#endif

#if __cplusplus >= 202002L
#define  NATL_PCH_CXX_20 
#endif

#if __cplusplus >= 1000000L
#define  NATL_PCH_CXX_23 
#endif

//reflection
#define NATL_ENABLE_PRIVATE_STRUCT_REFLECTION \
	template<natl::Size, natl::Size> \
	friend struct natl::impl::GetStructMemberAggregate; \
	template<natl::Size> \
	friend struct natl::impl::GetStructMembers; \
	template<typename> \
	friend struct natl::StructMemberCountV;

//enum
#define NATL_ADD_BIT_FLAG_OPERATIONS(EnumName) \
constexpr inline EnumName operator|(const EnumName lhs, const EnumName rhs) noexcept { \
	using underlying_type = natl::UnderlyingType<EnumName>; \
	return static_cast<EnumName>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs)); \
} \
constexpr inline EnumName operator&(const EnumName lhs, const EnumName rhs) noexcept { \
	using underlying_type = natl::UnderlyingType<EnumName>; \
	return static_cast<EnumName>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs)); \
} 
