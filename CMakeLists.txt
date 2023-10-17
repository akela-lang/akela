cmake_minimum_required(VERSION 3.16)
project(akela)

# ICU
set(ICU_DIR ${CMAKE_SOURCE_DIR}/../icu/install)
set(ICU_LIB_DIR ${ICU_DIR}/lib)
add_library(icui18n STATIC IMPORTED)
set_target_properties(icui18n PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicui18n.a)
add_library(icuio STATIC IMPORTED)
set_target_properties(icuio PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicuio.a)
add_library(icutu STATIC IMPORTED)
set_target_properties(icutu PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicutu.a)
add_library(icuuc STATIC IMPORTED)
add_library(icudata STATIC IMPORTED)
set_target_properties(icudata PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicudata.a)
set_target_properties(icuuc PROPERTIES IMPORTED_LOCATION ${ICU_DIR}/lib/libicuuc.a)
include_directories("${CMAKE_SOURCE_DIR}/../icu/install/include")

# AKELA
set(AKELA_DIR ${CMAKE_SOURCE_DIR})
set(AKELA_BINARY ${AKELA_DIR}/build/akela)
set(AKELA_A${AKELA_BINARY}/akela.a)
set(EXAMPLE_ADDITION ${CMAKE_SOURCE_DIR}/examples/addition.akela)

set(ZINC_DIR ${CMAKE_SOURCE_DIR}/../zinc)
set(ZINC_BINARY ${ZINC_DIR}/cmake-build-debug/zinc)
set(ZINC_A ${ZINC_BINARY}/libzinc.a)

set(BASE_DIR ${CMAKE_SOURCE_DIR}/base)
set(AKELA_MATH ${BASE_DIR}/math.akela)

# ZINC
add_library(zinc STATIC IMPORTED)
set_target_properties(zinc PROPERTIES
    IMPORTED_LOCATION ${ZINC_A}
)

include_directories(${ZINC_DIR} .)

# LLVM
set(llvm_build_dir release)
find_package(LLVM 16 REQUIRED CONFIG PATHS ${CMAKE_SOURCE_DIR}/../llvm-project/${llvm_build_dir})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})
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

# directories
add_subdirectory(akela)
add_subdirectory(test)
