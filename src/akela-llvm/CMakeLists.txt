# library
set(lib_files
        akela_llvm.cpp
        akela_llvm_tools.cpp
        akela_llvm_literal.cpp
        akela_llvm_function.cpp
        akela_llvm_variable.cpp
        akela_llvm_stmts.cpp
        akela_llvm_operator.cpp
        akela_llvm_jit.cpp
        akela_llvm_struct.cpp
)

add_library(akela-llvm STATIC ${lib_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(akela-llvm zinc gcov)
else()
    target_link_libraries(akela-llvm zinc)
endif()

set_target_properties(akela-llvm
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)