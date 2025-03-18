set(files
        unit.c
        unit_lex.c
)

add_executable(lava-unit ${files})
target_link_libraries(lava-unit lava zinc ${coverage_lib})
set_target_properties(lava-unit
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)