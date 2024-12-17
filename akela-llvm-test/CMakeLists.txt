# test
set(test_files
        test.c
        test_code_gen_tools.c
        test_akela_llvm_literal.c
        test_akela_llvm_variable.c
        test_akela_llvm_operator.c
        test_akela_llvm_stmts.c
        test_akela_llvm_function.c
        test_akela_llvm_array.c
        test_akela_llvm_struct.c
)
add_executable(akela-llvm-test ${test_files})
target_link_libraries(akela-llvm-test akela-llvm akela zinc ${coverage_lib} ${llvm_libs})

# function_pointer
add_executable(function_pointer function_pointer.cpp)
target_link_libraries(function_pointer ${llvm_libs} ${coverage_lib})

add_executable(test_abort test_abort.c)
target_link_libraries(test_abort ${coverage_lib})

set_target_properties(akela-llvm-test function_pointer test_abort
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)