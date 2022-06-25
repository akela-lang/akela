set(alba_dir ../alba)
set(test_files
    ${alba_dir}/token.c
    ${alba_dir}/scan.c
    ${alba_dir}/result.c
    ${alba_dir}/buffer.c
    ${alba_dir}/memory.c
    ${alba_dir}/dag.c
    ${alba_dir}/parse_stmts.c
    "${alba_dir}/parse_expr.c"
    ${alba_dir}/parse.c
    ${alba_dir}/parse_tools.c
    ${alba_dir}/allocator.c
    "${alba_dir}/input.c"
    ${alba_dir}/os_win.c
    ${alba_dir}/uconv.c
    ${alba_dir}/types.c
    assert.c
    "test.c"
    test_parse.c
    test_parse_expr.c
    test_parse_stmts.c
    test_scan.c
    test_input.c
    test_buffer.c
    test_allocator.c
)

include_directories("../external/icu4c/include" "..")
add_executable(test ${test_files})
target_link_libraries(test icudt icuin icuio icutest icutu icuuc)

file(GLOB icu_dll "${CMAKE_SOURCE_DIR}/external/icu4c/bin64/*.dll")
file(COPY ${icu_dll} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
