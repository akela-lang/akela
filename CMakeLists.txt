cmake_minimum_required(VERSION 3.20)
project(akela LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)         # Use C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensure the compiler strictly uses C++20
set(CMAKE_CXX_EXTENSIONS OFF)       # Disable compiler-specific extensions (optional)

# LLVM
find_package(LLVM 21.0.0 REQUIRED CONFIG PATHS ../llvm-project/build-release)
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

include_directories(src)

set(AKELA_DIR ${CMAKE_SOURCE_DIR})
set(STD_DIR ${CMAKE_SOURCE_DIR}/std)
set(AKELA_MATH ${STD_DIR}/math.ake)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#configure_file(${AKELA_MATH} ${CMAKE_BINARY_DIR})
#configure_file(${AKELA_MATH} ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE} COPYONLY)
# Set the runtime output directory (this handles multi-config automatically)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copy math.ake at build time
add_custom_command(
        OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/math.ake
        COMMAND ${CMAKE_COMMAND} -E copy ${AKELA_MATH} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/math.ake
        DEPENDS ${AKELA_MATH}
)

# Ensure it runs by attaching to a dummy target
add_custom_target(copy_math ALL DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/math.ake)

add_subdirectory(src/zinc)
add_subdirectory(src/zinc-unit)

add_subdirectory(src/akela)
add_subdirectory(src/akela-unit)
add_subdirectory(src/akela-parse-test)

add_subdirectory(src/akela-bin)
add_subdirectory(src/akela-run-test)
add_subdirectory(src/akela-llvm)
add_subdirectory(src/akela-llvm-unit)

add_subdirectory(src/cobble)
add_subdirectory(src/cobble-unit)

add_subdirectory(src/dataframe)
add_subdirectory(src/dataframe-unit)

add_subdirectory(src/json)
add_subdirectory(src/json-unit)

add_subdirectory(src/coverage)
add_subdirectory(src/coverage-bin)
add_subdirectory(src/coverage-unit)

add_subdirectory(src/centipede)
add_subdirectory(src/centipede-unit)

add_subdirectory(src/worm)
add_subdirectory(src/worm-unit)

include(akela.cmake)