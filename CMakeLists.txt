cmake_minimum_required(VERSION 3.20)
project(alba)
set(source_files
    src/main.c
    src/scan.c
    src/result.c
    src/ustring.c
    src/memory.c
    src/defer.c
    src/dag.c
)
include_directories("external/icu4c/include")
add_executable(alba ${source_files})

add_library(icudt STATIC IMPORTED)
set_target_properties(icudt PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/icudt.lib")
add_library(icuin STATIC IMPORTED)
set_target_properties(icuin PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/icuin.lib")
add_library(icuio STATIC IMPORTED)
set_target_properties(icuio PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/icuio.lib")
add_library(icutest STATIC IMPORTED)
set_target_properties(icutest PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/icutest.lib")
add_library(icutu STATIC IMPORTED)
set_target_properties(icutu PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/icutu.lib")
add_library(icuuc STATIC IMPORTED)
set_target_properties(icuuc PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/icuuc.lib")
target_link_libraries(alba icudt icuin icuio icutest icutu icuuc)

file(GLOB icu_lib "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/*.lib")
file(GLOB icu_exp "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/*.exp")
file(GLOB icu_pdb "${CMAKE_SOURCE_DIR}/external/icu4c/lib64/*.pdb")
file(GLOB icu_dll "${CMAKE_SOURCE_DIR}/external/icu4c/bin64/*.dll")
file(COPY ${icu_lib} DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY ${icu_exp} DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY ${icu_pdb} DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY ${icu_dll} DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/examples/addition.alba" DESTINATION "${CMAKE_BINARY_DIR}")
set_target_properties(alba PROPERTIES
    VS_DEBUGGER_COMMAND "${BINARY_DIR}/alba"
    VS_DEBUGGER_COMMAND_ARGUMENTS "file1"
)