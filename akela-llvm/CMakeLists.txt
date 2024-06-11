# library
set(lib_files
        akela_llvm.cpp
        akela_llvm_tools.cpp
        akela_llvm_literal.cpp
)

add_library(akela-llvm STATIC ${lib_files})
target_link_libraries(akela-llvm zinc)
set_target_properties(akela-llvm
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)