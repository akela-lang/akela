#!/bin/sh
coverage_dir="data/coverage"
build_dir="cmake-build-coverage"
root_dir="../.."
rm -rf $build_dir &&
    cmake -B $build_dir -DCOVERAGE=On -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C $build_dir &&
    for name in zinc-test akela-test akela-llvm-test dataframe-test cobble-test json-test coverage-test centipede-test
    do
        $build_dir/bin/$name
    done &&
    rm -rf $coverage_dir &&
    mkdir -p $coverage_dir &&
    cd $coverage_dir &&
    for name in zinc zinc-test akela akela-test akela-llvm akela-llvm-test dataframe dataframe-test cobble cobble-test json json-test coverage coverage-test centipede centipede-test
    do
        mkdir -p $name &&
        cd $name &&
        gcov $root_dir/../$build_dir/src/$name/CMakeFiles/$name.dir/*.gcda &&
        cd ..
    done &&
    cd akela-llvm &&
    for name in `ls | grep -v '^akela_llvm'`
    do
        rm $name
    done