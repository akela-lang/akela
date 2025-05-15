set(coverage_unit_files
        unit.c
        unit_data.c
        unit_parse.c
)
add_library(coverage-unit STATIC ${coverage_unit_files})
target_link_libraries(coverage-unit coverage zinc ${coverage_lib})
set_target_properties(coverage-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)