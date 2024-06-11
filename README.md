# Akela
Akela is a general-purpose programming language with the aim of supporting robust software
construction, standalone and embeddable execution, and to be suitable for numerical computation
and finance.

## LLVM
    cd ..
    git clone https://github.com/llvm/llvm-project
    cd llvm-project
	cmake -S llvm -B build-release -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -G Ninja
	ninja -C build-release check-llvm -j 8
    cd ../akela

## Build
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
    ninja -C cmake-build-debug

## Run Tests
    cmake-build-debug/bin/zinc-test
    cmake-build-debug/bin/akela-test
    cmake-build-debug/bin/akela-llvm-test

## Run JIT on file
    cmake-build-debug/bin/akela examples/addition.akela

## Valgrind
    valgrind --leak-check=full --num-callers=40 cmake-build-debug/bin/zinc-test
    valgrind --leak-check=full --num-callers=40 cmake-build-debug/bin/akela-test
    valgrind --leak-check=full --num-callers=40 cmake-build-debug/bin/akela-llvm-test

## Coverage
    rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCOVERAGE=On -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C cmake-build-debug &&
    cmake-build-debug/bin/zinc-test &&
    cmake-build-debug/bin/akela-test &&
    cmake-build-debug/bin/akela-llvm-test &&
    rm -rf coverage &&
    mkdir -p coverage &&
    cd coverage &&
    gcov ../cmake-build-debug/akela/CMakeFiles/akela.dir/*.gcda &&
    gcov ../cmake-build-debug/akela-test/CMakeFiles/akela-test.dir/*.gcda &&
    gcov ../cmake-build-debug/akela-llvm/CMakeFiles/akela-llvm.dir/*.gcda &&
    gcov ../cmake-build-debug/akela-llvm-test/CMakeFiles/akela-llvm-test.dir/*.gcda &&
    gcov ../cmake-build-debug/zinc/CMakeFiles/zinc.dir/*.gcda &&
    gcov ../cmake-build-debug/zinc-test/CMakeFiles/zinc-test.dir/*.gcda &&
    cd .. &&
    rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C cmake-build-debug

## CMake options to specify compiler
    -DCMAKE_C_COMPILER=clang
    -DCMAKE_CXX_COMPILER=clang++

## Optional LLVM targets
	ninja -C build-release clang -j 8
	ninja -C build-release Kaleidoscope -j 8