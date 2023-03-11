#VERSION
cmake_minimum_required (VERSION 3.8)

#OPTIONS
option(NATL_TESTS "enable tests" ON)
option(NATL_PROFILE "enable profile" ON)
option(NATL_MODULES "disable modules" OFF)
option(NATL_COMPILER_WARNINGS "disable compiler warnings" ON)

set(NATL_TESTS ON)
set(NATL_TESTS ON)
set(NATL_MODULES ON)
set(NATL_COMPILER_WARNINGS OFF)

#PROJECT 
project(natl)

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
