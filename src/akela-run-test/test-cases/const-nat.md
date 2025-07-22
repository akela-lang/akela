# Test Suite
Const declaration and const use of Nat

```cent
  TestSuite {
  }
```

## Test
Nat8

```akela
const a: Nat8 = 102
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i8 @__top_level() {
entry:
  %a = alloca i8, align 1
  store i8 102, ptr %a, align 1
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
        .type = Type::Nat8
        .value = 102
    }
}
```

## Test
Nat16

```akela
const a: Nat16 = 1234
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i16 @__top_level() {
entry:
  %a = alloca i16, align 2
  store i16 1234, ptr %a, align 2
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
        .type = Type::Nat16
        .value = 1234
    }
}
```

## Test
Nat32

```akela
const a: Nat32 = 1234
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 1234, ptr %a, align 4
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
        .type = Type::Nat32
        .value = 1234
    }
}
```

## Test
Nat64

```akela
const a: Nat64 = 1234
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i64 @__top_level() {
entry:
  %a = alloca i64, align 8
  store i64 1234, ptr %a, align 8
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
        .type = Type::Nat64
        .value = 1234
    }
}
```