# LLVM Details

## Compile Clang
To output LLVM IR from C code you may want to build clang.
```
cmake -S llvm -B build-release -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_ENABLE_PROJECTS=clang -G Ninja
ninja -C build-release check-llvm -j 8
```