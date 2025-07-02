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
  .snapshot = false
  .has_error = false
}
```

```akela
const a: Int32 = 105
a
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "105"
      .type = type0
    }
  }
  Ast::Id {
    .value = "a"
    .type = type0
  }
}
```

## Test
Double constant

```cent
use lib::base::*
Test {
  .mute = false
  .solo = false
  .snapshot = false
  .has_error = false
}
```

```akela
const a: Real64 = 1.5
a
```

```cent
use lib::base::*
Ast::Stmts {
  .type = Type::Real {
    .name = "Real64"
    .bit_count = 64
  }
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = Type::Real {
        .name = "Real64"
        .bit_count = 64
      }
    }
    Ast::Number {
      .value = "1.5"
      .type = Type::Real {
        .name = "Real64"
        .bit_count = 64
      }
    }
  }
  Ast::Id {
    .value = "a"
    .type = Type::Real {
      .name = "Real64"
      .bit_count = 64
    }
  }
}
```
