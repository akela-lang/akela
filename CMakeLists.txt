cmake_minimum_required(VERSION 3.16)
project(akela)

# AKELA
set(AKELA_DIR ${CMAKE_SOURCE_DIR})
set(AKELA_BINARY ${AKELA_DIR}/build/akela)
set(AKELA_A${AKELA_BINARY}/akela.a)
set(EXAMPLE_ADDITION ${CMAKE_SOURCE_DIR}/examples/addition.akela)
set(STD_DIR ${CMAKE_SOURCE_DIR}/std)
set(AKELA_MATH ${STD_DIR}/math.akela)
configure_file(${EXAMPLE_ADDITION} ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${AKELA_MATH} ${CMAKE_BINARY_DIR}/bin COPYONLY)

include_directories(.)

# LLVM
set(llvm_build_dir release)
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
endif()

# directories
add_subdirectory(zinc)
add_subdirectory(zinc-test)
add_subdirectory(akela)
add_subdirectory(akela-llvm)
add_subdirectory(akela-llvm-test)
add_subdirectory(akela-test)
add_subdirectory(akela-bin)