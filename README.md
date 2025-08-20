# Akela
Akela is a general-purpose programming language with the aim of being
capable of standalone and embeddable execution,
suitable for numerical computation and finance,
and capable of robust software construction.

Akela is influenced heavily by Julia, Rust, Zig, and Go. 
The Akela project wants to excel at finance.

## Status
The Akela project is a work in progress. Most of the work has been focused on fast
recursive decent parsing, type checking, and testing tools. Code generation has been
lightly worked on.

I am working transitioning the syntax tree to be a tagged union.
See the [todo](doc/todo.md) for what I will be working on in the near term.

## Project Goals
Many of the following goals are aspirational.
There is a rudimentary JIT compiler. There are a lot of automated tests.

* Statically typed language and type inference which we think helps
software engineers more than dynamically typed or gradually typed
* Fast compilation: handwritten lexer and parser in C
* An AOT compiler for standalone execution
* A JIT compiler for embeddable or interactive execution
* Hot reloading for a quick development / test cycle
* Meta programming: compile time expressions
* Object-Oriented programming: methods and static or runtime dispatch
* Option data types
* Memory management with allocators including an arena allocator, a reference counting allocator, and user-defined allocators
* Series / Vector data types for financial data
* Support for trading indicators, screeners, and backtesting

## Subprojects
* Akela - The Akela compiler which is written in C;
has a handwritten lexer and recursive decent parser
* Akela-LLVM - The Akela LLVM IR code generation library
* Zinc - A no-dependency library for basic C programming tools 
* Cobble - Regular expression library used for parsing file extensions, coverage data, and test data
* Dataframe - For doing vector math and writing and reading vectors to and from CSV files
* JSON - A JSON parser and stringify library
* Coverage - Generate and analyze code coverage of tests
* Centipede - The parser for Akela object notation which feels like a light-weight mix of JSON and XML.
It is used for configuration and to describe DAGs (syntax trees)
* Lava - A parser of a subset of markdown which is used for testing scripts (automated testing)
* Akela Parse Test - The automated parsing integration tests which are kept in markdown test scripts
* Akela Run Test - The automated LLVM and end-to-end tests which are kept in markdown test scripts
* Test - The test runner which runs all tests including unit, integration and end-to-end tests

The Cobble, Coverage, Dataframe, JSON, Centipede, and Lava subprojects are very useful for the
automated testing of the Akela compiler. When these subprojects are re-written in Akela
they will be useful as a comparison to the C versions.

The Akela-LLVM subproject depends on [LLVM](https://llvm.org/).

When testing code coverage, [gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) should be installed.

[Valgrind](https://valgrind.org/) is required when checking for memory errors.

## LLVM
```
cd ..
git clone https://github.com/llvm/llvm-project
cd llvm-project
git fetch --tags
git checkout llvmorg-21.1.0-rc1
cmake -S llvm -B build-release -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -G Ninja
ninja -C build-release check-llvm
cd ../akela
```

## Build Compiler
```
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
ninja -C cmake-build-debug
```

## Run Tests
```
cmake-build-debug/bin/test
```

## Run JIT on file
```
cmake-build-debug/bin/akela examples/addition.ake
```

## Valgrind
```
valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/test
valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/akela examples/addition.ake
```

## Code Coverage
```
./generate-coverage.sh
cmake-build-coverage/bin/coverage data/coverage
```
