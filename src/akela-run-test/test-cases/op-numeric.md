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