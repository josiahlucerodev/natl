function(natl_Standard_Test NatlTestName TestDir)

  #EXE
add_executable(${NatlTestName} ${TestDir}/main.cpp)

#OPTION
if(NATL_HTML_OUTPUT)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

#FLAGS
foreach(NATL_CXX_COMPILER_FLAG IN LISTS NATL_CXX_COMPILER_FLAGS)
    target_compile_options(${NatlTestName} PUBLIC "${NATL_CXX_COMPILER_FLAG}")
endforeach()

foreach(NATL_CXX_LINK_FLAG IN LISTS NATL_CXX_LINK_FLAGS)
    target_link_options(${NatlTestName} PUBLIC "${NATL_CXX_LINK_FLAG}")
endforeach()

#COMPILE DEFINITIONS
foreach(NATL_CXX_COMPILE_DEFINITION IN LISTS NATL_CXX_COMPILE_DEFINITIONS)
    target_compile_definitions(${NatlTestName} PRIVATE "${NATL_CXX_COMPILE_DEFINITION}")
endforeach()


#LINK
target_link_libraries(${NatlTestName} 
	PRIVATE 
		natl
)
include_directories(${NatlTestName} 
	PRIVATE 
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/natl>
)

#OUTPUT
set_target_properties(${NatlTestName}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

endfunction()
