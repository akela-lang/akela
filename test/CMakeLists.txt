set(alba_dir ../alba)
set(test_files
    ${alba_dir}/token.c
    ${alba_dir}/scan.c
    ${alba_dir}/dag.c
    ${alba_dir}/parse_stmts.c
    "${alba_dir}/parse_expr.c"
    ${alba_dir}/parse_factor.c
    ${alba_dir}/parse.c
    ${alba_dir}/parse_tools.c
    "${alba_dir}/source.c"
    "${alba_dir}/input.c"
    ${alba_dir}/os_win.c
    ${alba_dir}/uconv.c
    ${alba_dir}/parse_types.c
    ${alba_dir}/uconv.c
    ${alba_dir}/lookahead_translator.c
    ${alba_dir}/lookahead_char.c
    ${alba_dir}/hash.c
    ${alba_dir}/symbol_table.c
    assert_compiler.c
    "test.c"
    test_dag.c
    test_parse.c
    test_parse_expr.c
    test_parse_stmts.c
    test_scan.c
    test_input.c
    test_token.c
    test_lookahead_translator.c
    test_lookahead_char.c
    test_hash.c
    test_symbol_table.c
)

if(WIN32)
    include_directories("../external/icu4c/include" "..")
elseif(UNIX)
    include_directories("../external/icu/usr/local/include" "..")
endif()

add_executable(test ${test_files})

if(WIN32)

target_link_libraries(test icudt icuin icuio icutest icutu icuuc icudata icutest zinc)
file(GLOB icu_dll "${CMAKE_SOURCE_DIR}/external/icu4c/bin64/*.dll")
file(COPY ${icu_dll} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
        TARGET test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ZINC_DLL}
                ${CMAKE_CURRENT_BINARY_DIR}/zinc.dll
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

