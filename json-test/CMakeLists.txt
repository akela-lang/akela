set(json_test_sources
        test.c
        test_lex.c
        test_lex_tools.c
        test_dom.c
        test_parse.c
        test_parse_tools.c
        test_stringify.c
)
add_executable(json-test ${json_test_sources})
target_link_libraries(json-test json ${coverage_lib})