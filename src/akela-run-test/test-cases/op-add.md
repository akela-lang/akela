# Test Suite
operator - add integers
```cent
TestSuite {
  .mute = false
  .solo = false
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

/abort/

/printf/

/exit/

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

/abort/

/printf/

/exit/

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

/abort/

/printf/

/exit/

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

/abort/

/printf/

/exit/

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

/abort/

/printf/

/exit/

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