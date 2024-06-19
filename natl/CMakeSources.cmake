set(HEADER_FILES
natl/container/any.h
natl/container/array.h
natl/container/arrayView.h
natl/container/batchPool.h
natl/container/bitArray.h
natl/container/colony.h
natl/container/container.h
natl/container/dynArray.h
natl/container/flatHashMap.h
natl/container/functional.h
natl/container/functionCache.h
natl/container/heapArray.h
natl/container/partitioner.h
natl/container/priorityQueue.h
natl/container/smallDynArray.h
natl/container/string.h
natl/container/stringView.h
natl/container/variant.h
natl/container/view.h
natl/container.h
natl/fundamental/expect.h
natl/fundamental/option.h
natl/fundamental/pair.h
natl/fundamental/strongType.h
natl/fundamental/tuple.h
natl/fundamental.h
natl/math/basicFloatOperations.h
natl/math/constants.h
natl/math/exponential.h
natl/math/hyperbolic.h
natl/math/power.h
natl/math/satruatedArithmetic.h
natl/math/standard.h
natl/math/trigonometric.h
natl/math.h
natl/natl.h
natl/processing/format.h
natl/processing/generator.h
natl/processing/serialization.h
natl/processing.h
natl/simd/cmaskOperations.h
natl/simd/math/basicFloatOperations.h
natl/simd/math.h
natl/simd/operations.h
natl/simd/simdBase.h
natl/simd/simdClass.h
natl/simd/simdOf.h
natl/simd/standardArch.h
natl/simd.h
natl/sync/atomic.h
natl/sync/mutex.h
natl/sync/smartThread.h
natl/sync/synchronizedValue.h
natl/sync/thread.h
natl/sync.h
natl/system/filesystem.h
natl/system/print.h
natl/system/printColor.h
natl/system/printFormatted.h
natl/system/timer.h
natl/system.h
natl/units/base.h
natl/units/categoryConversion.h
natl/units/freedom.h
natl/units/imperial.h
natl/units/metric.h
natl/units/standard.h
natl/units.h
natl/util/algorithm.h
natl/util/allocator.h
natl/util/assert.h
natl/util/basicTypes.h
natl/util/bits.h
natl/util/characterTest.h
natl/util/commonHashs.h
natl/util/compare.h
natl/util/compilerDependent.h
natl/util/coroutines.h
natl/util/dataMovement.h
natl/util/error.h
natl/util/hash.h
natl/util/iteration.h
natl/util/iterators.h
natl/util/lexicographicalCompare.h
natl/util/limits.h
natl/util/memory.h
natl/util/numerics.h
natl/util/pointer.h
natl/util/stringConvert.h
natl/util/stringLiteral.h
natl/util/stringOperations.h
natl/util/test.h
natl/util/typeInfo.h
natl/util/typePack.h
natl/util/typeTraits.h
natl/util/uninitialized.h
natl/util/utility.h
natl/util.h
)

set(SOURCE_FILES
natl/sync/mutex.cpp
natl/sync/thread.cpp
natl/system/filesystem.cpp
natl/system/print.cpp
natl/system/printColor.cpp
natl/system/timer.cpp
natl/util/allocater.cpp
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