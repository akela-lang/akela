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
literal positive, literal positive

```akela
1 + 10
```

```llvm
/ModuleID/
/source_filename/
/target/

define i32 @__top_level() {
entry:
  ret i32 11
}
```

```cent
use lib::base::*
Test {
  .mute = false
  .solo = false
  .snapshot = false
  Field {
    .type = Type::Int32
    .value = 11
  }  
}
```

## Test
literal positive, literal negative

```akela
3 + -2
```

```llvm
/ModuleID/
/source_filename/
/target/

define i32 @__top_level() {
entry:
  ret i32 1
}
```

```cent
use lib::base::*
Test {
  .mute = false
  .solo = false
  .snapshot = false
  Field {
    .type = Type::Int32
    .value = 1
  }  
}
```

## Test
literal negative, literal positive

```akela
-3 + 2
```

```llvm
/ModuleID/
/source_filename/
/target/

define i32 @__top_level() {
entry:
  ret i32 -1
}
```

```cent
use lib::base::*
Test {
  .mute = false
  .solo = false
  .snapshot = false
  Field {
    .type = Type::Int32
    .value = -1
  }  
}
```

## Test
literal negative, literal negative

```akela
-5 + -20
```

```llvm
/ModuleID/
/source_filename/
/target/

define i32 @__top_level() {
entry:
  ret i32 -25
}
```

```cent
use lib::base::*
Test {
  .mute = false
  .solo = false
  .snapshot = false
  Field {
    .type = Type::Int32
    .value = -25
  }  
}
```

## Test
literal real, literal real

```akela
1.5 + 23.0
```

```llvm
/ModuleID/
/source_filename/
/target/

define double @__top_level() {
entry:
  ret double 2.450000e+01
}
```

```cent
use lib::base::*
Test {
  .mute = false
  .solo = false
  .snapshot = false
  Field {
    .type = Type::Real64
    .value = 24.5
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