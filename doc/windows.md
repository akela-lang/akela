# Windows

## Dependencies

Get ICU.

## msys
```
export CC=/ucrt64/bin/gcc
export CXX=/ucrt64/bin/g++

cmake -B build-msys2 -G "Unix Makefiles" -DCMAKE_C_COMPILER=/ucrt64/bin/gcc -DCMAKE_CXX_COMPILER=/ucrt64/bin/g++ -DICU_LIB_DIR="../icu-windows/lib64" -DICU_LIB_TYPE=STATIC -DICU_INCLUDE_DIR="../icu-windows/include"
```

## Visual Studio
```
cmake -B build -G "Visual Studio 17 2022" -DICU_LIB_DIR="../icu-windows/lib64" -DICU_LIB_TYPE=STATIC -DICU_INCLUDE_DIR="../icu-windows/include"
```