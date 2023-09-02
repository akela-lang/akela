cmake_minimum_required(VERSION 3.16)
project(akela)

set(ICU_DIR ${CMAKE_SOURCE_DIR}/../icu/source/install)
set(ICU_LIB_DIR ${ICU_DIR}/lib)

add_library(icudata STATIC IMPORTED)
set_target_properties(icudata PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicudata.a)

add_library(icui18n STATIC IMPORTED)
set_target_properties(icui18n PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicui18n.a)

add_library(icuio STATIC IMPORTED)
set_target_properties(icuio PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicuio.a)

add_library(icutu STATIC IMPORTED)
set_target_properties(icutu PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicutu.a)

add_library(icuuc STATIC IMPORTED)
set_target_properties(icuuc PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicuuc.a)

include_directories("${CMAKE_SOURCE_DIR}/../icu/source/install/include")

set(AKELA_DIR ${CMAKE_SOURCE_DIR})
set(AKELA_BINARY ${AKELA_DIR}/build/akela)
set(AKELA_SO ${AKELA_BINARY}/akela.so)
set(EXAMPLE_ADDITION ${CMAKE_SOURCE_DIR}/examples/addition.akela)

set(ZINC_DIR ${CMAKE_SOURCE_DIR}/../zinc)
set(ZINC_BINARY ${ZINC_DIR}/build/zinc)
set(ZINC_A ${ZINC_BINARY}/libzinc.a)

add_library(zinc STATIC IMPORTED)
set_target_properties(zinc PROPERTIES
    IMPORTED_LOCATION ${ZINC_A}
)

set(BASE_DIR ${CMAKE_SOURCE_DIR}/base)
set(AKELA_MATH ${BASE_DIR}/math.akela)

include_directories(${ZINC_DIR} .)

add_subdirectory(akela)
add_subdirectory(test)
