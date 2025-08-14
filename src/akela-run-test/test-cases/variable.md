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

## Test
multi assign 4

```akela
var a: Int32
var b: Int32
var c: Int32
var d: Int32
a, b, c, d = 1, 2, 3, 4
d
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
  %0 = load i32, ptr %d, align 4
  ret i32 %0
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 4
  }
}
```

## Test
false

```akela
const a: Bool = false
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i1 @__top_level() {
entry:
  %a = alloca i1, align 1
  store i1 false, ptr %a, align 1
  %0 = load i1, ptr %a, align 1
  ret i1 %0
}
```

```cent
use lib::base::*;
Test {
    .solo = false
    .mute = false
    .snapshot = false
    Field {
        .type = Type::Bool
        .value = false
    }
}
```

## Test
true

```akela
const a: Bool = true
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i1 @__top_level() {
entry:
  %a = alloca i1, align 1
  store i1 true, ptr %a, align 1
  %0 = load i1, ptr %a, align 1
  ret i1 %0
}
```

```cent
use lib::base::*;
Test {
    .solo = false
    .mute = false
    .snapshot = false
    Field {
        .type = Type::Bool
        .value = true
    }
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

## Test
Real16

```akela
const a: Real16 = 1.54
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define half @__top_level() {
entry:
  %a = alloca half, align 2
  store half 0xH3E29, ptr %a, align 2
  %0 = load half, ptr %a, align 2
  ret half %0
}
```

```cent
use lib::base::*;
Test {
    .solo = false
    .mute = true
    .snapshot = false
    Field {
        .type = Type::Real16
        .value = "/^1\\.54\\d*$/"
    }
}
```

## Test
Real32

```akela
const a: Real32 = 1.54
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define float @__top_level() {
entry:
  %a = alloca float, align 4
  store float 0x3FF8A3D700000000, ptr %a, align 4
  %0 = load float, ptr %a, align 4
  ret float %0
}
```

```cent
use lib::base::*;
Test {
    .solo = false
    .mute = false
    .snapshot = false
    Field {
        .type = Type::Real32
        .value = 1.54
    }
}
```

## Test
Real64

```akela
const a: Real64 = 1.54
a
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define double @__top_level() {
entry:
  %a = alloca double, align 8
  store double 1.540000e+00, ptr %a, align 8
  %0 = load double, ptr %a, align 8
  ret double %0
}
```

```cent
use lib::base::*;
Test {
    .solo = false
    .mute = false
    .snapshot = false
    Field {
        .type = Type::Real64
        .value = 1.54
    }
}
```