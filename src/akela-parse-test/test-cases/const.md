# Test Suite
Define constant and use

```cent
use lib::base::*
TestSuite {
  .solo = false
  .mute = false
}
```

## Test
Integer constant

```cent
use lib::base::*
Test {
  .solo = false
  .mute = false
  .has_error = false
}
```

```akela
const a: Int32 = 105
a
```

```cent
use lib::base::*
Ast::Stmts {
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .tu = TypeUse {
        .td = Int32
      }
    }
    Ast::Number {
      .value = "105"
    }
  }
  Ast::Id {
    .value = "a"
  }
  .tu = TypeUse {
    .td = Int32
  }
}
```