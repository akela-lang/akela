set(test_files
    unit_test_compiler.c
    "test.c"
    test_ast.c
    test_parse.c
    test_parse_factor.c
    test_parse_expr.c
    test_parse_stmts.c
    test_scan.c
    test_input.c
    test_token.c
    test_lookahead_translator.c
    test_lookahead_char.c
    test_hash.c
    test_symbol_table.c
    test_parse_types.c
    test_type.c
)

if(WIN32)
    include_directories("../external/icu4c/include" "..")
elseif(UNIX)
    include_directories("../external/icu/usr/local/include" "..")
endif()

add_executable(test ${test_files})

if(WIN32)

target_link_libraries(test alba-lib icudt icuin icuio icutest icutu icuuc zinc)

add_custom_command(
        TARGET test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ZINC_DLL}
                ${ALBA_DLL}
                ${ICU_UC_DLL}
                ${ICU_DT_DLL}
                ${EXAMPLE_ADDITION}
                ${CMAKE_CURRENT_BINARY_DIR}
)

elseif(UNIX)

target_link_libraries(test icuio icutest icutu icuuc icudata icutest zinc)

add_custom_command(
        TARGET test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ZINC_SO}
                $ENV{HOME}/lib/libzinc.so
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ICU_LIB_DIR}/*.so*
                $ENV{HOME}/lib
)

endif()

