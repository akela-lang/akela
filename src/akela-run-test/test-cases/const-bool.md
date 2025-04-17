# Test Suite
Const declaration and const use of Bool

```cent
  TestSuite {
    .solo = false
    .mute = false
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

/abort/

/printf/

/exit/

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

/abort/

/printf/

/exit/

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
