#VERSION
cmake_minimum_required (VERSION 3.9)

#PROJECT 
project(natl)

#NACMAKE
include(externals/nacmake/externals_nacmake.cmake)

CHECK_IF_IS_ROOT(NATL_TRANSPILER_IS_ROOT)

#PROJECT LIBRARY
add_subdirectory(natl)

#TEST
if(NATL_TRANSPILER_IS_ROOT)
	message("natl tests")
	add_subdirectory(tests)
endif()