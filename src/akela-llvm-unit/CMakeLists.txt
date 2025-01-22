# test
set(akela_llvm_unit_files
        test.c
        test_cg_tools.c
        test_literal.c
        test_variable.c
        test_operator.c
        test_stmts.c
        test_function.c
        test_array.c
        test_struct.c
)
add_executable(akela-llvm-unit ${akela_llvm_unit_files})
target_link_libraries(akela-llvm-unit akela-llvm akela zinc ${coverage_lib} ${llvm_libs})

# function_pointer
add_executable(function_pointer function_pointer.cpp)
target_link_libraries(function_pointer ${llvm_libs} ${coverage_lib})

add_executable(test_abort test_abort.c)
target_link_libraries(test_abort ${coverage_lib})

set_target_properties(akela-llvm-unit function_pointer test_abort
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)