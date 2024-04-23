# Akela
Akela is a general-purpose programming language geared toward numerical computation and finance.

## Dependencies
* (https://github.com/akela-lang/zinc)[Zinc]
* (https://releases.llvm.org/)[LLVM]

## Build
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
    ninja -C cmake-build-debug

## Run Tests
    cmake-build-debug/test/test

## Valgrind
    valgrind cmake-build-debug/test/test

    valgrind --leak-check=full --num-callers=40 cmake-build-debug/test/test