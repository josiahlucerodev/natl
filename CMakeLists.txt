#VERSION
cmake_minimum_required (VERSION 3.9)

#PROJECT 
project(natl)


#DEPENDENCIES
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR) #IS ROOT
	#NACMAKE
	include(externals/nacmake/externals_nacmake.cmake)
else()
	#NACMAKE
	if(not PROJECT_EXISTS nacmake)
		message(FATAL_ERROR "natl: requires nacmake")
	endif()
endif()

#IS ROOT
CHECK_IF_IS_ROOT(NATL_TRANSPILER_IS_ROOT)

#PROJECT LIBRARY
add_subdirectory(natl)

#TEST
if(NATL_TRANSPILER_IS_ROOT)
	message("natl: tests")
	add_subdirectory(tests)
endif()