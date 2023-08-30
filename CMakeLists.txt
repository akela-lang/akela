cmake_minimum_required(VERSION 3.16)
project(alba)

if(WIN32)

set(ICU_INCLUDE ${CMAKE_SOURCE_DIR}/external/icu4c/include)

set(ICU_BIN ${CMAKE_SOURCE_DIR}/external/icu4c/bin64)
set(ICU_DT_DLL ${ICU_BIN}/icudt70.dll)
set(ICU_IN_DLL ${ICU_BIN}/icuin70.dll)
set(ICU_IO_DLL ${ICU_BIN}/icuio70.dll)
set(ICU_TU_DLL ${ICU_BIN}/icutu70.dll)
set(ICU_UC_DLL ${ICU_BIN}/icuuc70.dll)

set(ICU_LIB ${CMAKE_SOURCE_DIR}/external/icu4c/lib64/)
set(ICU_DT_LIB ${ICU_LIB}/icudt.lib)
set(ICU_IN_LIB ${ICU_LIB}/icuin.lib)
set(ICU_IO_LIB ${ICU_LIB}/icuio.lib)
set(ICU_TEST_LIB ${ICU_LIB}/icutest.lib)
set(ICU_TU_LIB ${ICU_LIB}/icutu.lib)
set(ICU_UC_LIB ${ICU_LIB}/icuuc.lib)

add_library(icudt STATIC IMPORTED)
set_target_properties(icudt PROPERTIES IMPORTED_LOCATION ${ICU_DT_LIB})
add_library(icuin STATIC IMPORTED)
set_target_properties(icuin PROPERTIES IMPORTED_LOCATION ${ICU_IN_LIB})
add_library(icuio STATIC IMPORTED)
set_target_properties(icuio PROPERTIES IMPORTED_LOCATION ${ICU_IO_LIB})
add_library(icutest STATIC IMPORTED)
set_target_properties(icutest PROPERTIES IMPORTED_LOCATION ${ICU_TEST_LIB})
add_library(icutu STATIC IMPORTED)
set_target_properties(icutu PROPERTIES IMPORTED_LOCATION ${ICU_TU_LIB})
add_library(icuuc STATIC IMPORTED)
set_target_properties(icuuc PROPERTIES IMPORTED_LOCATION ${ICU_UC_LIB})

include_directories(${ICU_INCLUDE})

elseif(UNIX)


set(ICU_DIR ${CMAKE_SOURCE_DIR}/external/icu/usr/local)
set(ICU_LIB_DIR ${ICU_DIR}/lib)

add_library(icudata STATIC IMPORTED)
set_target_properties(icudata PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicudata.so)

add_library(icui18n STATIC IMPORTED)
set_target_properties(icui18n PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicui18n.so)

add_library(icuio STATIC IMPORTED)
set_target_properties(icuio PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicuio.so)

add_library(icutu STATIC IMPORTED)
set_target_properties(icutu PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicutu.so)

add_library(icuuc STATIC IMPORTED)
set_target_properties(icuuc PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicuuc.so)

add_library(icutest STATIC IMPORTED)
set_target_properties(icutest PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicutest.so)

include_directories("external/icu/usr/local/include")

endif()

if(WIN32)

set(EXAMPLE_DIR ${CMAKE_SOURCE_DIR}/examples)
set(EXAMPLE_ADDITION ${EXAMPLE_DIR}/addition.akela)

set(ZINC_DIR ${CMAKE_SOURCE_DIR}/../zinc)
set(ZINC_BINARY ${ZINC_DIR}/out/build/x64-Debug/zinc)
set(ZINC_LIB ${ZINC_BINARY}/zinc.lib)
set(ZINC_DLL ${ZINC_BINARY}/zinc.dll)

set(ALBA_DIR ${CMAKE_SOURCE_DIR})
set(ALBA_BINARY ${ALBA_DIR}/out/build/x64-Debug/alba)
set(ALBA_LIB ${ALBA_BINARY}/alba.lib)
set(ALBA_DLL ${ALBA_BINARY}/alba.dll)

add_library(zinc SHARED IMPORTED)
set_target_properties(zinc PROPERTIES
    IMPORTED_LOCATION ${ZINC_DLL}
    IMPORTED_IMPLIB ${ZINC_LIB}
)

elseif(UNIX)

set(ALBA_DIR ${CMAKE_SOURCE_DIR})
set(ALBA_BINARY ${ALBA_DIR}/build/alba)
set(ALBA_SO ${ALBA_BINARY}/alba.so)

set(ZINC_DIR ${CMAKE_SOURCE_DIR}/../zinc)
set(ZINC_BINARY ${ZINC_DIR}/build/zinc)
set(ZINC_SO ${ZINC_BINARY}/libzinc.so)

add_library(zinc SHARED IMPORTED)
set_target_properties(zinc PROPERTIES
    IMPORTED_LOCATION ${ZINC_SO}
)

endif()

# math.akela
set(BASE_DIR ${CMAKE_SOURCE_DIR}/base)
set(ALBA_MATH ${BASE_DIR}/math.akela)

include_directories(${ZINC_DIR} .)

add_subdirectory(akela)
add_subdirectory(test)
