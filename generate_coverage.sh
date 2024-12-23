#!/bin/sh
coverage_dir="data/coverage"
root_dir="../.."
rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCOVERAGE=On -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C cmake-build-debug &&
    for name in zinc-test akela-test akela-llvm-test dataframe-test cobble-test json-test coverage-test centipede-test
    do
        cmake-build-debug/bin/$name
    done &&
    rm -rf $coverage_dir &&
    mkdir -p $coverage_dir &&
    cd $coverage_dir &&
    for name in zinc zinc-test akela akela-test akela-llvm akela-llvm-test dataframe dataframe-test cobble cobble-test json json-test coverage coverage-test centipede centipede-test
    do
        mkdir -p $name &&
        cd $name &&
        gcov $root_dir/../cmake-build-debug/$name/CMakeFiles/$name.dir/*.gcda &&
        cd ..
    done &&
    cd akela-llvm &&
    for name in `ls | grep -v '^akela_llvm'`
    do
        rm $name
    done &&
    cd .. &&
    cd $root_dir &&
    rm -rf cmake-build-debug &&
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja