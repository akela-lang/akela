# Test Suite
literal

```cent
TestSuite {
}
```

## Test
integer

```akela
1
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  ret i32 1
}
```

```cent
use lib::base::*;
Test {
  Field {
    .type = Type::Int32
    .value = 1
  }
}
```