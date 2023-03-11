#LIBRARY
add_library(natl)

#SOURCES
include(CMakeSources.cmake)

#OUTPUT
set_target_properties(natl
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#OPTIONS
if(NATL_MODULES) 
	message("natl modules")
	if(MSVC)
		target_compile_options(natl PUBLIC /EHsc /nologo /MTd /std:c++latest)
	else()
		target_compile_features(natl PUBLIC cxx_std_23)
	endif()
else()
	message("natl headers")
	target_compile_features(natl PUBLIC cxx_std_17)
endif()

if(NATL_PROFILE) 
	if(MSVC)
		message("natl profile")
		target_compile_options(natl PRIVATE /Zi)
		target_link_options(natl PRIVATE /DEBUG /OPT:REF /OPT:ICF /PROFILE)
	endif()
endif()

if(NATL_COMPILER_WARNINGS) 
	message("natl compiler warnings")
	target_compile_options(natl PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
	)
endif()