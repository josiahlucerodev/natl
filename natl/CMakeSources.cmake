set(HEADER_FILES
natl/container.h
natl/error.h
natl/functional.h
natl/iterators.h
natl/partitioner.h
natl/pointer.h
natl/repeat.h
natl/test.h
natl/timer.h
natl/typeTraits.h
natl/uninitialized.h
natl/uninitializedArray.h
natl/uninitializedValue.h
natl/view.h
)

set(SOURCE_FILES
natl/timer.cpp
)

foreach(target_header IN LISTS HEADER_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target_header}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/natl/${target_header}>
	)
endforeach()

foreach(target_source IN LISTS SOURCE_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${target_source}>
	)
endforeach()

target_precompile_headers(natl PUBLIC natl/pch.h)