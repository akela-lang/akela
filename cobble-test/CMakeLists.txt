set(test_files
        test.c
        test_ast_node.c
        test_compile.c
        test_compile_tools.c
        test_match.c
        test_match_tools.c
)

add_executable(cobble-test ${test_files})
target_link_libraries(cobble-test cobble zinc ${coverage_lib})
set_target_properties(cobble-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)