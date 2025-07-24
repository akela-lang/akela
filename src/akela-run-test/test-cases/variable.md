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

## Test
var assign multiple 1

```akela
var a: Int32
var b: Int32
var c: Int32
a = b = c = 44
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 44, ptr %c, align 4
  store i32 44, ptr %b, align 4
  store i32 44, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 44
  }
}
```

## Test
var assign multiple 2

```akela
var a: Int32
var b: Int32
var c: Int32
a = b = c = 44
b
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 44, ptr %c, align 4
  store i32 44, ptr %b, align 4
  store i32 44, ptr %a, align 4
  %0 = load i32, ptr %b, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 44
  }
}
```

## Test
var assign multiple 3

```akela
var a: Int32
var b: Int32
var c: Int32
a = b = c = 44
c
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 44, ptr %c, align 4
  store i32 44, ptr %b, align 4
  store i32 44, ptr %a, align 4
  %0 = load i32, ptr %c, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 44
  }
}
```

## Test
var assign multiple 4

```akela
var a: Int32
var b: Int32
var c: Int32
a = b = c = 44
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 44, ptr %c, align 4
  store i32 44, ptr %b, align 4
  store i32 44, ptr %a, align 4
  ret i32 44
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 44
  }
}
```

## Test
copy variable

```akela
const a: Int32 = 50
const b: Int32 = a
a + b
```

```llvm
/; ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 50, ptr %a, align 4
  %b = alloca i32, align 4
  %0 = load i32, ptr %a, align 4
  store i32 %0, ptr %b, align 4
  %1 = load i32, ptr %a, align 4
  %2 = load i32, ptr %b, align 4
  %addtmp = add i32 %1, %2
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 100
  }
}
```

## Test
const Int64

```akela
const a: Int64 = 2
a
```

```llvm
/; ModuleID/
/source_filename/
/target datalayout/

define i64 @__top_level() {
entry:
  %a = alloca i64, align 8
  store i64 2, ptr %a, align 8
  %0 = load i64, ptr %a, align 8
  ret i64 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int64
    .value = 2
  }
}
```

## Test
const Int32

```akela
const a: Int32 = 2
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define void @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 2, ptr %a, align 4
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
Int32 to Int64

```akela
const a: Int32 = 1
const b: Int64 = a
b
```

```c
#include <stdint.h>
#include <stdio.h>
uint64_t top_level()
{
    uint32_t a = 1;
    uint64_t b = a;
    return b;
}
int main() {
    printf("%ld\n", top_level());
    return 0;
}
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i64 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i64, align 8
  %0 = load i32, ptr %a, align 4
  %1 = zext i32 %0 to i64
  store i64 %1, ptr %b, align 8
  %2 = load i64, ptr %b, align 8
  ret i64 %2
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int64
    .value = 1
  }
}
```

## Test
assign scalar id

```akela
const a: Int32 = 11
var b: Int32
b = a
b
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 11, ptr %a, align 4
  %b = alloca i32, align 4
  %0 = load i32, ptr %a, align 4
  store i32 %0, ptr %b, align 4
  %1 = load i32, ptr %b, align 4
  ret i32 %1
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 11
  }
}
```

## Test
multi assign 1

```akela
var a: Int32
var b: Int32
var c: Int32
var d: Int32
a, b, c, d = 1, 2, 3, 4
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 1, ptr %a, align 4
  store i32 2, ptr %b, align 4
  store i32 3, ptr %c, align 4
  store i32 4, ptr %d, align 4
  %0 = load i32, ptr %a, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 1
  }
}
```

## Test
multi assign 2

```akela
var a: Int32
var b: Int32
var c: Int32
var d: Int32
a, b, c, d = 1, 2, 3, 4
b
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 1, ptr %a, align 4
  store i32 2, ptr %b, align 4
  store i32 3, ptr %c, align 4
  store i32 4, ptr %d, align 4
  %0 = load i32, ptr %b, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 2
  }
}
```

## Test
multi assign 3

```akela
var a: Int32
var b: Int32
var c: Int32
var d: Int32
a, b, c, d = 1, 2, 3, 4
c
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 1, ptr %a, align 4
  store i32 2, ptr %b, align 4
  store i32 3, ptr %c, align 4
  store i32 4, ptr %d, align 4
  %0 = load i32, ptr %c, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 3
  }
}
```