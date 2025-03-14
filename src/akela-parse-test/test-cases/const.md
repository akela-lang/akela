# Test Suite
Define constant and use
```
use lib::base::*
TestSuite {
  .solo = false
  .mute = false
  .headers = headers
}
```

## Test
Declare and use an integer
```
use lib::base::*;
Test {
  .solo = false
  .mute = false
  .has_error = false
}
```

### Source
```
const a: Int32 = 105
a
```

### Ast
```
use lib::base::*
Ast::Stmts {
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .tu = Type_use {
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
}
```