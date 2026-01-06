#TESTS
set(RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

function(natl_add_test TestName TestFile)
    get_filename_component(TestFileDir "${TestFile}" DIRECTORY)    
    set(TEST_ARGS "--timeout" "5" "--resource-path" "${CMAKE_CURRENT_SOURCE_DIR}/${TestFileDir}")
    # -- STD --

    #EXE
	add_executable(${TestName} ${TestFile})
	exe_emscripten_setup(${TestName} TRUE)
    nact_add_test(${TestName} ${TEST_ARGS})  

	#OPTION
	set_target_properties(${TestName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")
	set_target_properties(${TestName} PROPERTIES CXX_STANDARD 20)
	set_target_properties(${TestName} PROPERTIES CXX_STANDARD_REQUIRED On)


	if(NACTEMPLATE_PROFILE)
		ENABLE_PROFILING(${TestName})
	endif()

	if(NACTEMPLATE_WARNINGS)
		ENABLE_WARNINGS(${TestName} TRUE)
	endif()
    
    SET_STANDARD_OUTPUT_SUB(${TestName} "tests/${TestFileDir}")

	#LINK
	target_link_libraries(${TestName}  
		natl)

    target_include_directories(${TestName}  
        PUBLIC 
        "$CACHE{NATL_INCLUDE_DIR}"
        $<BUILD_INTERFACE:$CACHE{NATL_INCLUDE_DIR}>)

    #SOURCES
    foreach(target_header IN LISTS ARGN)
		target_sources(natl 
			PRIVATE
				$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target_header}>
				$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${target_header}>)
	endforeach()

    # -- MODULES --
    if(NOT NACTEMPLATE_NO_MODULES AND NOT NATL_NO_MODULES)
        #EXE
        set(TestNameModules "${TestName}_Modules")
        add_executable(${TestNameModules} "modules/tests/${TestFile}m")
        exe_emscripten_setup(${TestNameModules} TRUE)
        nact_add_test(${TestNameModules} ${TEST_ARGS}) 

        #OPTION
        set_target_properties(${TestNameModules} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test/modules")
        set_target_properties(${TestNameModules} PROPERTIES CXX_STANDARD 20)
        set_target_properties(${TestNameModules} PROPERTIES CXX_STANDARD_REQUIRED On)
        set_target_properties(${TestNameModules} PROPERTIES CXX_MODULE_STD 1)

        if(NACTEMPLATE_PROFILE)
            ENABLE_PROFILING(${TestNameModules})
        endif()

        if(NACTEMPLATE_WARNINGS)
            ENABLE_WARNINGS(${TestNameModules} TRUE)
        endif()
        
        SET_STANDARD_OUTPUT_SUB(${TestNameModules} "tests/modules/${TestFileDir}")

        #LINK
	    target_link_libraries(${TestNameModules}  
		    natlModules)

        target_include_directories(${TestNameModules}  
            PUBLIC 
            "$CACHE{NATL_MODULES_INCLUDE_DIR}"
            $<BUILD_INTERFACE:$CACHE{NATL_MODULES_INCLUDE_DIR}>)

        #SOURCES
        foreach(arg IN LISTS ARGN)
            get_filename_component(argName ${arg} NAME_WE)
            get_filename_component(argDir ${arg} DIRECTORY)
            set(moduleArg "modules/tests/${argDir}/${argName}.ixx")

            target_sources(${TestNameModules} 
		        PUBLIC
			        FILE_SET CXX_MODULES FILES		
				    ${moduleArg})
        endforeach()
    endif()
endfunction()

#container
NATL_ADD_TEST(NatlAnyUT container/any_utest.cpp)
NATL_ADD_TEST(NatlArrayUT container/array_utest.cpp)
NATL_ADD_TEST(NatlBatchPoolUT container/batchPool_utest.cpp)
NATL_ADD_TEST(NatlColonyUT container/colony_utest.cpp)
NATL_ADD_TEST(NatlDynArrayUT container/dynArray_utest.cpp)
NATL_ADD_TEST(NatlFlatHashMapUT container/flatHashMap_utest.cpp container/hashMapTests.h)
NATL_ADD_TEST(NatlFlatHashSetUT container/flatHashSet_utest.cpp container/hashSetTests.h)
NATL_ADD_TEST(NatlFunctionalUT container/functional_utest.cpp)
NATL_ADD_TEST(NatlHeapArrayUT container/heapArray_utest.cpp)
NATL_ADD_TEST(NatlPersistentHashMapUT container/persistentHashMap_utest.cpp container/hashMapTests.h)
NATL_ADD_TEST(NatlPersistentHashSetUT container/persistentHashSet_utest.cpp container/hashSetTests.h)
NATL_ADD_TEST(NatlPriorityQueueUT container/priorityQueue_utest.cpp)
NATL_ADD_TEST(NatlStringUT container/string_utest.cpp)
NATL_ADD_TEST(NatlVariantUT container/variant_utest.cpp)

#math
NATL_ADD_TEST(NatlMathUT math/math_utest.cpp)

#processing
NATL_ADD_TEST(NatlFormatUT processing/format_utest.cpp)
NATL_ADD_TEST(NatlGeneratorUT processing/generator_utest.cpp)

#simd
NATL_ADD_TEST(NatlSimdUT simd/simd_utest.cpp)

#sync
NATL_ADD_TEST(NatlAtomicUT sync/atomic_utest.cpp)
NATL_ADD_TEST(NatlSynchronizedValueUT sync/synchronizedValue_utest.cpp)

#system
NATL_ADD_TEST(NatlFilesystemUT system/filesystem_utest.cpp)
NATL_ADD_TEST(NatlPrintUT system/print_utest.cpp)
NATL_ADD_TEST(NatlThreadUT system/thread_utest.cpp)
NATL_ADD_TEST(NatlTimerUT system/timer_utest.cpp)

#units
NATL_ADD_TEST(NatlUnitsUT units/units_utest.cpp)

#util
NATL_ADD_TEST(NatlAlgorithmUT util/algorithm_utest.cpp)
NATL_ADD_TEST(NatlAllocPartsUT util/allocParts_utest.cpp)
NATL_ADD_TEST(NatlBasicTypesUT util/basicTypes_utest.cpp)
NATL_ADD_TEST(NatlBitsUT util/bits_utest.cpp)
NATL_ADD_TEST(NatlCastUT util/cast_utest.cpp)
NATL_ADD_TEST(NatlDataMovementUT util/dataMovement_utest.cpp)
NATL_ADD_TEST(NatlEnumUT util/enum_utest.cpp)
NATL_ADD_TEST(NatlExecutionSessionUT util/executionSession_utest.cpp)
NATL_ADD_TEST(NatlExpectUT util/expect_utest.cpp)
NATL_ADD_TEST(NatlFunctionCacheUT util/functionCache_utest.cpp)
NATL_ADD_TEST(NatlIterationUT util/iteration_utest.cpp)
NATL_ADD_TEST(NatlLimitsUT util/limits_utest.cpp)
NATL_ADD_TEST(NatlMemoryUT util/memory_utest.cpp)
NATL_ADD_TEST(NatlMixinUT util/mixin_utest.cpp)
NATL_ADD_TEST(NatlOptionUT util/option_utest.cpp)
NATL_ADD_TEST(NatlPointerUT util/pointer_utest.cpp)
NATL_ADD_TEST(NatlReflectionUT util/reflection_utest.cpp)
NATL_ADD_TEST(NatlStatefulUT util/stateful_utest.cpp)
NATL_ADD_TEST(NatlStringConvertUT util/stringConvert_utest.cpp)
NATL_ADD_TEST(NatlTestUT util/test_utest.cpp)
NATL_ADD_TEST(NatlTupleUT util/tuple_utest.cpp)
NATL_ADD_TEST(NatlTypeInfoUT util/typeInfo_utest.cpp)
NATL_ADD_TEST(NatlTypePackUT util/typePack_utest.cpp)