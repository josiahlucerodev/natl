set(HEADER_FILES
natl/algorithm.h
natl/allocator.h
natl/array.h
natl/arrayView.h
natl/basicTypes.h
natl/batchPool.h
natl/bitArray.h
natl/commonHashs.h
natl/container.h
natl/dataMovement.h
natl/dynArray.h
natl/error.h
natl/expect.h
natl/flatHashMap.h
natl/functional.h
natl/hash.h
natl/heapArray.h
natl/iterators.h
natl/lexicographicalCompare.h
natl/limits.h
natl/natl.h
natl/option.h
natl/pair.h
natl/partitioner.h
natl/peramaterPackOperations.h
natl/pointer.h
natl/repeat.h
natl/serialization.h
natl/smallDynArray.h
natl/string.h
natl/stringConvert.h
natl/stringLiteral.h
natl/stringView.h
natl/test.h
natl/timer.h
natl/typeTraits.h
natl/variant.h
natl/view.h
)

set(SOURCE_FILES
natl/allocator.cpp
)

foreach(target_header IN LISTS HEADER_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target_header}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/natl/${target_header}>
	 "natl/heapArray.h")
endforeach()

foreach(target_source IN LISTS SOURCE_FILES)
	target_sources(natl 
		PRIVATE
			$<BUILD_INTERFACE:${target_source}>
	)
endforeach()

target_precompile_headers(natl PUBLIC natl/pch.h)