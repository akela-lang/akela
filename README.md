# Akela
Akela is a programming language geared toward finance.

## Build
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
    ninja -C cmake-build-debug

## Run Tests
    cmake-build-debug/test/test

## Valgrind
    valgrind cmake-build-debug/test/test

    valgrind --leak-check=full --num-callers=25 cmake-build-debug/test/test