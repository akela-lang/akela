cmake_minimum_required(VERSION 3.20)
project(alba)
set(source_files
    src/main.c
    src/scan.c
    src/result.c
)
add_executable(alba ${source_files})
file(COPY "${CMAKE_SOURCE_DIR}/examples/addition.alba" DESTINATION "${CMAKE_BINARY_DIR}")
set_target_properties(alba PROPERTIES
    VS_DEBUGGER_COMMAND "${BINARY_DIR}/alba"
    VS_DEBUGGER_COMMAND_ARGUMENTS "file1"
)
