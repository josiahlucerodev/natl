function(natl_Standard_Test NatlTestName TestDir)

#EXE
add_executable(${NatlTestName} ${TestDir}/main.cpp)

#OPTION
exe_emscripten_setup(${NatlTestName} TRUE)

#OPTIONS
set_property(TARGET ${NatlTestName} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${NatlTestName} PROPERTY CXX_STANDARD_REQUIRED On)

if(OPALET_PROFILE)
	ENABLE_PROFILING(${NatlTestName})
endif()

if(OPALET_WARNINGS)
	ENABLE_WARNINGS(${NatlTestName} TRUE)
endif()

#LINK
target_link_libraries(${NatlTestName} 
	PRIVATE 
	natl)

target_include_directories(${NatlTestName}  
	PUBLIC 
	"$CACHE{NATL_INCLUDE_DIR}"
	$<BUILD_INTERFACE:$CACHE{NATL_INCLUDE_DIR}>)

#OUTPUT
SET_STANDARD_OUTPUT(${NatlTestName})

endfunction()
