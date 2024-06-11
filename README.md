# Akela
Akela is a general-purpose programming language with the aim of supporting robust software
construction, standalone and embeddable execution, and to be suitable for numerical computation
and finance.

## LLVM
    cd ..
    git clone https://github.com/llvm/llvm-project
    cd llvm-project
	cmake -S llvm -B release -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -G Ninja
	ninja -C release check-llvm -j 8
	ninja -C release clang -j 8
	ninja -C release Kaleidoscope -j 8
    cd ../akela

## Build
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
    ninja -C cmake-build-debug

## Run Tests
    cmake-build-debug/bin/zinc-test
    cmake-build-debug/bin/test

## Run JIT on file
    cmake-build-debug/bin/akela examples/addition.akela

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
