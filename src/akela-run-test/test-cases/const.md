# Test Suite
Const declaration and const use

```cent
  TestSuite {
    .solo = false
    .mute = false
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

declare void @abort()

declare void @printf(ptr, ...)

declare void @exit(i32)

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