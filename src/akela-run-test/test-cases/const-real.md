# Test Suite
Const declaration and const use of Nat

```cent
  TestSuite {
    .solo = false
    .mute = false
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
