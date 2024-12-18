# Akela
Akela is a general-purpose programming language with the aim of being
capable of standalone and embeddable execution,
suitable for numerical computation and finance,
and capable of robust software construction.

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
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/zinc-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/akela-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/akela-llvm-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/dataframe-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/cobble-test
    valgrind --leak-check=full --num-callers=60 cmake-build-debug/bin/json-test

## Coverage
    ./generate-coverage.sh

## CMake options to specify compiler
    rm -r cmake-build-debug
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G Ninja
    ninja -C cmake-build-debug

## Optional LLVM targets
	ninja -C build-release clang -j 8
	ninja -C build-release Kaleidoscope -j 8

## LLVM Debug
    cmake -S llvm -B build-debug -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Debug -G Ninja
	ninja -C build-debug check-llvm -j 2
	ninja -C build-debug clang -j 2
	ninja -C build-debug Kaleidoscope -j 2
