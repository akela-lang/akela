# library
set(lib_files
        cg.cpp
        tools.cpp
        literal.cpp
        function.cpp
        variable.cpp
        stmts.cpp
        operator.cpp
        jit.cpp
        struct.cpp
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