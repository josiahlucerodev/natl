#EXE
add_executable(NatlPartitionerTest main.cpp)

#LINK
link_libraries(natl)

target_link_libraries(NatlPartitionerTest 
	PRIVATE 
		natl
)

#OPTIONS
if(NATL_MODULES) 
	if(MSVC)
		message("test module")
		target_compile_options(NatlPartitionerTest PUBLIC /EHsc /nologo /MTd /std:c++latest)
	else()
		target_compile_features(NatlPartitionerTest PUBLIC cxx_std_23)
	endif()
else()
	target_compile_features(NatlPartitionerTest PUBLIC cxx_std_17)
endif()

if(NATL_PROFILE) 
	if(MSVC)
		target_compile_options(NatlPartitionerTest PRIVATE /Zi)
		target_link_options(NatlPartitionerTest PRIVATE /DEBUG /OPT:REF /OPT:ICF /PROFILE)
	endif()
endif()

if(NATL_COMPILER_WARNINGS) 
	target_compile_options(NatlPartitionerTest PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
	)
endif()