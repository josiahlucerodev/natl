#SOURCES
include(CMakeSources.cmake)


# -- NATL --

#LIBARY
set(NATL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH INTERNAL)
add_library(natl)

#OPTIONS
set_target_properties(natl PROPERTIES UNITY_BUILD ON)
set_target_properties(natl PROPERTIES CXX_STANDARD 20)

if(NACTEMPLATE_WARNINGS)
	ENABLE_WARNINGS(natl TRUE)
endif()

if(NACTEMPLATE_PROFILING)
	ENABLE_PROFILING(natl)
endif()

SET_STANDARD_OUTPUT(natl)

#SOURCES 
target_precompile_headers(natl PUBLIC natl/pch.h)

foreach(target_header IN LISTS CPP_HEADER_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target_header}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/natl/${target_header}>)
endforeach()

foreach(target_source IN LISTS CPP_SOURCE_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${target_source}>
	)
endforeach()

# -- NATL MODULES --
if(NOT NACTEMPLATE_NO_MODULES AND NOT NATL_NO_MODULES)
	message("natl: modules")
	#LIBARY
	set(NATL_MODULES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/modules" CACHE PATH INTERNAL)
	add_library(natlModules)
	
	#OPTIONS
	set_target_properties(natlModules PROPERTIES UNITY_BUILD ON)
	set_target_properties(natlModules PROPERTIES CXX_STANDARD 20)
	set_target_properties(${TestNameModules} PROPERTIES CXX_STANDARD_REQUIRED On)
	set_target_properties(natlModules PROPERTIES CXX_MODULE_STD 1)

	if(NACTEMPLATE_WARNINGS)
		ENABLE_WARNINGS(natlModules TRUE)
	endif()

	if(NACTEMPLATE_PROFILING)
		ENABLE_PROFILING(natlModules)
	endif()

	SET_STANDARD_OUTPUT(natlModules)

	#SOURCES 
	foreach(target_header IN LISTS CPP_MODULE_HEADER_FILES)
		target_sources(natl 
			PRIVATE
				$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target_header}>
				$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/natlModules/${target_header}>)
	endforeach()

	foreach(target_source IN LISTS CPP_MODULE_SOURCE_FILES)
		target_sources(natlModules 
			PRIVATE
				$<BUILD_INTERFACE:${target_source}>
		)
	endforeach()

	target_sources(natlModules 
		PUBLIC
			FILE_SET CXX_MODULES FILES		
				${CPP_MODULE_INTERFACE_FILES}
				#${CPP_MODULE_IMPLEMENTATION_FILES}
					
	)

	foreach(target_implementation IN LISTS CPP_MODULE_IMPLEMENTATION_FILES)
		set_source_files_properties(${target_implementation} 
			PROPERTIES CXX_MODULE_TYPE "implementation")
		target_sources(natlModules 
			PRIVATE
			$<BUILD_INTERFACE:${target_implementation}>
		)
	endforeach()
endif()