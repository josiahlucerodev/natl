#VERSION
cmake_minimum_required (VERSION 3.9)

#PROJECT 
project(natl)

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

#IS ROOT
CHECK_IF_IS_ROOT(NATL_IS_ROOT)

#PROJECT LIBRARY
add_subdirectory(natl)

#TEST
if(NATL_IS_ROOT)
	message("natl: tests")
	add_subdirectory(tests)
endif()