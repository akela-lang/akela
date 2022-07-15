cmake_minimum_required(VERSION 3.20)
project(alba)

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

set(ZINC_DIR ${CMAKE_SOURCE_DIR}/../zinc)
set(ZINC_BINARY ${ZINC_DIR}/out/build/x64-Debug/zinc)
set(ZINC_LIB ${ZINC_BINARY}/zinc.lib)
set(ZINC_DLL ${ZINC_BINARY}/zinc.dll)

add_library(zinc SHARED IMPORTED)
set_target_properties(zinc PROPERTIES
    IMPORTED_LOCATION ${ZINC_DLL}
    IMPORTED_IMPLIB ${ZINC_LIB}
)

include_directories(${ZINC_DIR} .)

add_subdirectory(alba)
add_subdirectory(test)
