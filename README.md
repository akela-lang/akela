# Akela
Akela is a general-purpose programming language with the aim to be capable of constructing robust
software, standalone or embeddable execution, and suitable for numerical computation and finance.

## Dependencies
* [Zinc](https://github.com/akela-lang/zinc)
* [LLVM](https://github.com/llvm/llvm-project)

## Build
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
    ninja -C cmake-build-debug

## Run Tests
    cmake-build-debug/test/test

## Valgrind
    valgrind --leak-check=full --num-callers=40 cmake-build-debug/test/test

## Coverage
    rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCOVERAGE=On -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C cmake-build-debug
    cmake-build-debug/test/test &&
    rm -rf coverage &&
    mkdir -p coverage &&
    cd coverage &&
    gcov ../cmake-build-debug/akela/CMakeFiles/akela.dir/*.gcda &&
    gcov ../cmake-build-debug/test/CMakeFiles/test.dir/*.gcda &&
    cd .. &&
    rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C cmake-build-debug
