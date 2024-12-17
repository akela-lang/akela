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
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja -DICU_LIB=ENABLE
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

## Coverage
    rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCOVERAGE=On -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C cmake-build-debug &&
    cmake-build-debug/bin/zinc-test &&
    cmake-build-debug/bin/akela-test &&
    cmake-build-debug/bin/akela-llvm-test &&
    cmake-build-debug/bin/dataframe-test &&
    cmake-build-debug/bin/cobble-test &&
    cmake-build-debug/bin/json-test &&
    rm -rf coverage &&
    mkdir -p coverage &&
    cd coverage &&
    gcov ../cmake-build-debug/akela/CMakeFiles/akela.dir/*.gcda &&
    gcov ../cmake-build-debug/akela-test/CMakeFiles/akela-test.dir/*.gcda &&
    gcov ../cmake-build-debug/akela-llvm/CMakeFiles/akela-llvm.dir/*.gcda &&
    gcov ../cmake-build-debug/akela-llvm-test/CMakeFiles/akela-llvm-test.dir/*.gcda &&
    gcov ../cmake-build-debug/zinc/CMakeFiles/zinc.dir/*.gcda &&
    gcov ../cmake-build-debug/zinc-test/CMakeFiles/zinc-test.dir/*.gcda &&
    gcov ../cmake-build-debug/cobble/CMakeFiles/cobble.dir/*.gcda &&
    gcov ../cmake-build-debug/cobble-test/CMakeFiles/cobble-test.dir/*.gcda &&
    gcov ../cmake-build-debug/dataframe/CMakeFiles/dataframe.dir/*.gcda &&
    gcov ../cmake-build-debug/dataframe-test/CMakeFiles/dataframe-test.dir/*.gcda &&
    gcov ../cmake-build-debug/json/CMakeFiles/json.dir/*.gcda &&
    gcov ../cmake-build-debug/json-test/CMakeFiles/json-test.dir/*.gcda &&
    cd .. &&
    rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C cmake-build-debug

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
