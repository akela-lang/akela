# akela-unit
set(akela_unit_files
        unit.c
        unit_ast.c
        unit_parse.c
        unit_lex_setup.c
        unit_lex.c
        unit_lex_number.c
        unit_lex_tools.c
        unit_symbol_table.c
        unit_type.c
        unit_comp_unit.c
        unit_comp_table.c
        unit_environment.c
        unit_type_list.c
        unit_type_slots.c
)
add_library(akela-unit STATIC ${akela_unit_files})
target_link_libraries(akela-unit akela zinc ${coverage_lib})

# event loop
set(event_loop_files event_loop.c)
add_executable(event_loop ${event_loop_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(event_loop zinc gcov)
else()
    target_link_libraries(event_loop zinc)
endif()

set_target_properties(akela-unit event_loop
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)