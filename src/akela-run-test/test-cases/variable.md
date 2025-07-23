# Test Suite
variable

```cent
TestSuite {
}
```

## Test
const void

```akela
const a: Int32
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define void @__top_level() {
entry:
  %a = alloca i32, align 4
  ret void
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Void
  }
}
```

## Test
const void 2

```akela
const a: Int32 = 1
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define void @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  ret void
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Void
  }
}
```

## Test
const int

```akela
const a: Int32 = 31
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 31, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 31
  }
}
```

## Test
var assign

```akela
var a: Int32 = 30
a = 40
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 30, ptr %a, align 4
  store i32 40, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 40
  }
}
```