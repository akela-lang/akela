set(files main.c)
add_executable(test ${files})
target_link_libraries(test
        zinc-unit
        lava-unit
        json-unit
        dataframe-unit
        coverage-unit
        cobble-unit
        centipede-unit
        akela-unit
        akela-llvm-unit
        akela-parse-test
        akela-run-test
)
set_target_properties(test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set(akela_parse_test_dir ${CMAKE_SOURCE_DIR}/src/akela-parse-test/test-cases)
set(akela_run_test_dir ${CMAKE_SOURCE_DIR}/src/akela-run-test/test-cases)
set(test_config_path ${CMAKE_BINARY_DIR}/bin/test-config.cent)
set(output [[
Test {
    .akela_parse_test = "${akela_parse_test_dir}"
    .akela_run_test = "${akela_run_test_dir}"
}
]])
string(CONFIGURE "${output}" output2)
FILE(WRITE ${test_config_path} ${output2})
