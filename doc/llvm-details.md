# LLVM Details

## Compile Clang
To output LLVM IR from C code you may want to build clang.
```
cmake -S llvm -B build-release -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DLLVM_ENABLE_PROJECTS=clang
ninja -C build-release check-llvm
ninja -C build-release clang
```