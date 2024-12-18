cmake_minimum_required(VERSION 3.16)
project(akela)
set(CMAKE_CXX_STANDARD 17)

# AKELA
set(AKELA_DIR ${CMAKE_SOURCE_DIR})
set(STD_DIR ${CMAKE_SOURCE_DIR}/std)
set(AKELA_MATH ${STD_DIR}/math.akela)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
configure_file(${AKELA_MATH} ${CMAKE_BINARY_DIR}/bin COPYONLY)

include_directories(.)

# LLVM
set(llvm_build_dir build-release)
find_package(LLVM 19 REQUIRED CONFIG PATHS ${CMAKE_SOURCE_DIR}/../llvm-project/${llvm_build_dir})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_BUILD_MAIN_SRC_DIR}/examples/Kaleidoscope/include)
set(LLVM_LINK_COMPONENTS
        AllTargetsAsmParsers
        AllTargetsCodeGens
        AllTargetsDescs
        AllTargetsDisassemblers
        AllTargetsInfos
        Analysis
        BitReader
        Core
        ExecutionEngine
        InstCombine
        Interpreter
        MC
        MCDisassembler
        MCJIT
        Object
        OrcJIT
        RuntimeDyld
        ScalarOpts
        Support
        Target
        TransformUtils
        native
        nativecodegen
        AsmPrinter
        DebugInfoPDB
        IrReader
        LibDriver
        Linker
        LTO
        Option
        WindowsManifest
        WindowsDriver
        )
llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})

if(${COVERAGE} MATCHES On)
        add_compile_options(-Wall -fprofile-arcs -ftest-coverage)
        set(coverage_lib gcov)
endif()

if (${ICU_LIB_DIR})
        if (${ICU_LIB_TYPE} MATCHES SHARED)
                add_library(icudata SHARED IMPORTED)
                add_library(icui18n SHARED IMPORTED)
                add_library(icuio SHARED IMPORTED)
                add_library(icutu SHARED IMPORTED)
                add_library(icuuc SHARED IMPORTED)
                set(ICU_LIB_EXT "so")
        elseif(${ICU_LIB_TYPE} MATCHES STATIC)
                add_library(icudata STATIC IMPORTED)
                add_library(icui18n STATIC IMPORTED)
                add_library(icuio STATIC IMPORTED)
                add_library(icutu STATIC IMPORTED)
                add_library(icuuc STATIC IMPORTED)
                set(ICU_LIB_EXT "a")
        else()
                add_library(icudata STATIC IMPORTED)
                add_library(icui18n STATIC IMPORTED)
                add_library(icuio STATIC IMPORTED)
                add_library(icutu STATIC IMPORTED)
                add_library(icuuc STATIC IMPORTED)
                set(ICU_LIB_EXT "a")
        endif()

        set_target_properties(icu PROPERTIES
                IMPORTED_LOCATION ${ICU_LIB_DIR}/icudata.${ICU_LIB_EXT}
                IMPORTED_LOCATION ${ICU_LIB_DIR}/icui18n.${ICU_LIB_EXT}
                IMPORTED_LOCATION ${ICU_LIB_DIR}/icuio.${ICU_LIB_EXT}
                IMPORTED_LOCATION ${ICU_LIB_DIR}/icutu.${ICU_LIB_EXT}
                IMPORTED_LOCATION ${ICU_LIB_DIR}/icuuc.${ICU_LIB_EXT}
        )
endif()

if (${ICU_INCLUDE_DIR})
        include_directories(${ICU_INCLUDE_DIR})
endif()

# directories
add_subdirectory(zinc)
add_subdirectory(zinc-test)
add_subdirectory(akela)
add_subdirectory(akela-llvm)
add_subdirectory(akela-llvm-test)
add_subdirectory(akela-test)
add_subdirectory(akela-bin)
add_subdirectory(cobble)
add_subdirectory(cobble-test)
add_subdirectory(dataframe)
add_subdirectory(dataframe-test)
add_subdirectory(json)
add_subdirectory(json-test)
add_subdirectory(coverage)
add_subdirectory(coverage-bin)
add_subdirectory(coverage-test)