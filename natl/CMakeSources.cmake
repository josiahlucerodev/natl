set(MODULE_FILES
natl/partitioner.ixx
)

if(NATL_MODULES)
	foreach(natl_module IN LISTS MODULE_FILES)
	target_sources(natl 
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/${natl_module}
	)
	endforeach()

	foreach(natl_source IN LISTS MODULE_SOURCE_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${natl_source}>
	)
	endforeach()

	if(MSVC)
		target_sources(natl 
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/external/MSVC/std.compat.ixx
			${CMAKE_CURRENT_SOURCE_DIR}/external/MSVC/std.ixx
	)
	endif()

else()
	foreach(natl_header IN LISTS HEADER_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${natl_header}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${natl_header}>
	)
	endforeach()

	foreach(natl_source IN LISTS SOURCE_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${natl_source}>
	)
	endforeach()
endif()

target_sources(natl 
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/CMakeSources.cmake>
)