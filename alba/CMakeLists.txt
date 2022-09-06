
set(lib_files
    scan.c
    token.c
    ast.c
    parse_stmts.c
    "parse_expr.c"
    parse_factor.c
    "source.c"
    parse.c
    parse_tools.c
    "input.c"
    os_win.c
    uconv.c
    parse_types.c
    lookahead_char.c
    lookahead_translator.c
    hash.c
    symbol_table.c
    type_def.c
    unit_test_compiler.c
    comp_unit.c
    comp_table.c
)

set(bin_files
    alba.c
)

# library
add_definitions(-DALBA_EXPORT)
add_library(alba SHARED ${lib_files})
target_link_libraries(alba icudt icuin icuio icutest icutu icuuc zinc)

# executable
add_executable(alba-bin ${bin_files})
target_link_libraries(alba-bin alba icudt icuin icuio icutest icutu icuuc zinc)
SET_TARGET_PROPERTIES(alba-bin
  PROPERTIES OUTPUT_NAME alba)

add_custom_command(
        TARGET alba-bin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ZINC_DLL}
                ${ALBA_DLL}
                ${ICU_UC_DLL}
                ${ICU_DT_DLL}
                ${EXAMPLE_ADDITION}
                ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
        TARGET alba POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ZINC_DLL}
                ${ALBA_DLL}
                ${ICU_UC_DLL}
                ${ICU_DT_DLL}
                ${EXAMPLE_ADDITION}
                ${CMAKE_BINARY_DIR}/test
)



if(WIN32)

elseif(UNIX)

target_link_libraries(alba-lib icuio icutest icutu icuuc icudata icutest zinc)

endif()
