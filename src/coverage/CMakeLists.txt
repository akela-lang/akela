set(coverage_files
        data.c
        parse.c
)
add_library(coverage ${coverage_files})
target_link_libraries(coverage cobble zinc)
set_target_properties(coverage
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)