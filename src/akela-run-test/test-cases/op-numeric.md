# Test Suite
numeric operators

```cent
TestSuite {
}
```

## Test
add

```akela
1 + 40
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  ret i32 41
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 41
  }
}
```

## Test
add id and number

```akela
const a: Int32 = 4
a + 61
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 4, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %addtmp = add i32 %0, 61
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 65
  }
}
```

## Test
sub

```akela
15-2
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  ret i32 13
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 13
  }
}
```

## Test
sub 2

```akela
const a: Int32 = 10
a - 2
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 10, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %subtmp = sub i32 %0, 2
  ret i32 %subtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 8
  }
}
```