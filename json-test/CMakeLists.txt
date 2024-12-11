set(json_test_sources
        test.c
)
add_executable(json-test ${json_test_sources})
target_link_libraries(json-test json)