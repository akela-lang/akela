# test
set(test_files
        test.c
        test_akela_llvm.c
        test_code_gen_tools.c
        test_akela_llvm_literal.c
)
add_executable(akela-llvm-test ${test_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(akela-llvm-test akela-llvm akela zinc gcov ${llvm_libs})
else()
    target_link_libraries(akela-llvm-test akela-llvm akela zinc ${llvm_libs})
endif()

# function_pointer
add_executable(function_pointer function_pointer.cpp)
if(${COVERAGE} MATCHES On)
    target_link_libraries(function_pointer ${llvm_libs} gcov)
else()
    target_link_libraries(function_pointer ${llvm_libs})
endif()

set_target_properties(akela-llvm-test function_pointer
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
