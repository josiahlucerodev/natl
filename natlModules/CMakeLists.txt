#LIBRARY
add_library(natlModules)

#FLAGS
foreach(NATL_CXX_COMPILER_FLAG IN LISTS NATL_CXX_COMPILER_FLAGS)
    target_compile_options(natl PUBLIC "${NATL_CXX_COMPILER_FLAG}")
endforeach()

foreach(NATL_CXX_LINK_FLAG IN LISTS NATL_CXX_LINK_FLAGS)
    target_link_options(natl PUBLIC "${NATL_CXX_LINK_FLAG}")
endforeach()

#SOURCES
include(CMakeSources.cmake)

#OUTPUT
set_target_properties(natl
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)