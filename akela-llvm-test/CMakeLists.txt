# test
set(test_files
        test.c
        test_code_gen.c
        test_code_gen_tools.c
)
add_executable(akela-llvm-test ${test_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(akela-llvm-test akela-llvm akela zinc gcov ${llvm_libs})
else()
    target_link_libraries(akela-llvm-test akela-llvm akela zinc ${llvm_libs})
endif()

set_target_properties(akela-llvm-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
