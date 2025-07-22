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