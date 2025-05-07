set(files
        unit.c
        unit_lex.c
        unit_parse.c
)

add_library(lava-unit STATIC ${files})
target_link_libraries(lava-unit lava ${coverage_lib})
set_target_properties(lava-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)