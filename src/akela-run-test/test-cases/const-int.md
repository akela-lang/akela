# Test Suite
Const declaration and const use of Int

```cent
  TestSuite {
  }
```

## Test
Int8

```akela
const a: Int8 = -104
a
```

```llvm
/; ModuleID/
/source_filename/
/target datalayout/

define i8 @__top_level() {
entry:
  %a = alloca i8, align 1
  store i8 -104, ptr %a, align 1
  %0 = load i8, ptr %a, align 1
  ret i8 %0
}
```

```cent
use lib::base::*;
Test {
  .solo = false
  .mute = false
  .snapshot = false
  Field {
    .type = Type::Int8
    .value = -104
  }
}
```

## Test
Int16

```akela
const a: Int16 = -104
a
```

```llvm
/; ModuleID/
/source_filename/
/target datalayout/

define i16 @__top_level() {
entry:
  %a = alloca i16, align 2
  store i16 -104, ptr %a, align 2
  %0 = load i16, ptr %a, align 2
  ret i16 %0
}
```

```cent
use lib::base::*;
Test {
  .solo = false
  .mute = false
  .snapshot = false
  Field {
    .type = Type::Int16
    .value = -104
  }
}
```

## Test
Int32

```akela
const a: Int32 = -104
a
```

```llvm
/; ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 -104, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  ret i32 %0
}
```

```cent
use lib::base::*;
Test {
  .solo = false
  .mute = false
  .snapshot = false
  Field {
    .type = Type::Int32
    .value = -104
  }
}
```

## Test
Int64

```akela
const a: Int64 = 1401
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i64 @__top_level() {
entry:
  %a = alloca i64, align 8
  store i64 1401, ptr %a, align 8
  %0 = load i64, ptr %a, align 8
  ret i64 %0
}
```

```cent
use lib::base::*;
Test {
    .solo = false
    .mute = false
    .snapshot = false
    Field {
        .type = Type::Int64
        .value = 1401
    }
}
```