#!/bin/sh
coverage_dir="data/coverage"
build_dir="cmake-build-coverage"
root_dir="../.."
rm -rf $build_dir &&
    cmake -B $build_dir -DCOVERAGE=On -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
    ninja -C $build_dir &&
    for name in zinc-unit akela-unit akela-llvm-unit dataframe-unit cobble-unit json-unit coverage-unit centipede-unit
    do
        $build_dir/bin/$name
    done &&
    rm -rf $coverage_dir &&
    mkdir -p $coverage_dir &&
    cd $coverage_dir &&
    for name in zinc zinc-unit akela akela-unit akela-llvm akela-llvm-unit dataframe dataframe-unit cobble cobble-unit json json-unit coverage coverage-unit centipede centipede-unit
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