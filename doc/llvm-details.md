# LLVM Details

## Compile Clang
To output LLVM IR from C code you may want to build clang (and lldb).
```
cmake -S llvm -B build-release -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DLLVM_ENABLE_PROJECTS="clang;lldb"
ninja -C build-release check-llvm
ninja -C build-release clang lldb
```

## Building LLVM on Windows
```
git clone https://github.com/llvm/llvm-project
cd llvm-project
cmake -S llvm -B build-release -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_ENABLE_PROJECTS=clang
```

Build the resulting solution in directory build-release with Visual Studio.