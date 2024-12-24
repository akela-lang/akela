# Akela
Akela is a general-purpose programming language with the aim of being
capable of standalone and embeddable execution,
suitable for numerical computation and finance,
and capable of robust software construction.

## Subprojects
* Akela - The Akela compiler; a compiler written in C
* Akela-LLVM - The Akela LLVM IR code generation library
* Zinc - A no-dependency library for basic C programming tools 
* Cobble - Regular expression library used for parsing coverage files
* Dataframe - For doing vector math and writing and reading vectors to and from CSV files
* JSON - A JSON parser and stringify library
* Centipede - The Centipede DAG description language parser; is a file format for compiler ASTs
and will be used for testing AST output of the Akela parser

The Cobble, Dataframe, JSON, and Centipede projects very useful for the
automated testing of the Akela Compiler, but will also be useful comparing
how good the Akela language will be at writing compilers compared to the C language.

The Akela-LLVM subproject depends on LLVM.

The Cobble (Regex), JSON, and Centipede subprojects depend on
the [ICU library](https://icu.unicode.org/) which can be installed on most OSs.

## LLVM
    cd ..
    git clone https://github.com/llvm/llvm-project
    cd llvm-project
    git checkout tags/llvmorg-19.1.6
	cmake -S llvm -B build-release -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -G Ninja
	ninja -C build-release check-llvm -j 8
    cd ../akela

## Build
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
    ninja -C cmake-build-debug

## Run Tests
    cmake-build-debug/bin/zinc-test
    cmake-build-debug/bin/akela-test
    cmake-build-debug/bin/akela-llvm-test
    cmake-build-debug/bin/dataframe-test
    cmake-build-debug/bin/cobble-test
    cmake-build-debug/bin/json-test
    cmake-build-debug/bin/coverage-test
    cmake-build-debug/bin/centipede-test

## Run JIT on file
    cmake-build-debug/bin/akela examples/addition.akela

## Valgrind
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/zinc-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/akela-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/akela-llvm-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/dataframe-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/cobble-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/json-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/coverage-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/centipede-test

## Coverage
    ./generate-coverage.sh
    ninja -C cmake-build-debug
    cmake-build-debug/bin/coverage data/coverage