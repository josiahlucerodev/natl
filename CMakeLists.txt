#VERSION
cmake_minimum_required (VERSION 3.28)

#PROJECT 
project(natl)

option(ENABLE_CODE_COVERAGE "Enable coverage reporting" OFF)

#DEPENDENCIES
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR) #IS ROOT
	#NACTEMPLATE
	include(externals/nactemplate/externals_nactemplate.cmake)
else()
	#NACTEMPLATE
	if(NOT TARGET nactemplate)
		message(FATAL_ERROR "natl: requires nactemplate")
	endif()
endif()

SET(NATL_NO_MODULES TRUE)

#IS ROOT
CHECK_IF_IS_ROOT(NATL_IS_ROOT)

#PROJECT LIBRARY
add_subdirectory(natl)

#TEST
if(NATL_IS_ROOT)
	message("natl: tests")
	enable_testing()
	include(CTest)
	configure_parallel_testing()
	add_subdirectory(tests)
endif()