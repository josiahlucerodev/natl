#VERSION
cmake_minimum_required (VERSION 3.8)

#PROJECT 
project(natl)

#OPTIONS
option(NATL_TESTS "enable tests" ON)
option(NATL_PROFILE "enable profile" ON)
option(NATL_COMPILER_WARNINGS "disable compiler warnings" ON)
option(NATL_EMSCRIPTEN "natl emscripten" ON)
option(NATL_HTML_OUTPUT "natl html output" ON)
option(NATL_JS_OUTPUT "natl js output" ON)

set(NATL_TESTS ON)
set(NATL_PROFILE OFF)
set(NATL_COMPILER_WARNINGS OFF)
set(NATL_EMSCRIPTEN OFF)
set(NATL_HTML_OUTPUT OFF)

#OPTIONS 
set(NATL_CXX_COMPILER_FLAGS "")
set(NATL_CXX_LINK_FLAGS "")

if(NATL_COMPILER_WARNINGS) 
	message("natl compiler warnings")
	if(MSVC)
		list(APPEND NATL_CXX_COMPILER_FLAGS "/W4" "/WX")
	else()
		list(APPEND NATL_CXX_COMPILER_FLAGS "-Wall" "-Wextra" "-Wpedantic" "-Werror")
	endif()
endif()

if(NATL_PROFILE) 
	message("natl profile")
	if(MSVC)
		list(APPEND NATL_CXX_COMPILER_FLAGS "/Zi")
		list(APPEND NATL_CXX_LINK_FLAGS "/DEBUG" "/OPT:REF" "/OPT:ICF" "/PROFILE")
	endif()
endif()

if(NATL_EMSCRIPTEN)
	list(APPEND NATL_CXX_COMPILER_FLAGS "-std=gnu++20")
elseif(MSVC)
	list(APPEND NATL_CXX_COMPILER_FLAGS "/EHsc" "/nologo" "/MTd" "/std:c++20")
else()
	list(APPEND NATL_CXX_COMPILER_FLAGS "std=c++20")
endif()

#COMPILE DEFINITIONS
set(NATL_CXX_COMPILE_DEFINITIONS "")
#list(APPEND NATL_CXX_COMPILE_DEFINITIONS "NATL_HEADERS")
if(NATL_EMSCRIPTEN)
	list(APPEND NATL_CXX_COMPILE_DEFINITIONS "NATL_EMSCRIPTEN")
endif()

#PROJECT LIBRARY
add_subdirectory(natl)

#TEST
if(NATL_TESTS)
	message("natl tests")
	add_subdirectory(tests)
endif()

unset(NATL_TESTS CACHE)
unset(NATL_PROFILE CACHE)
unset(OPALET_MODULES CACHE)
unset(NATL_COMPILE_WARNING CACHE)
