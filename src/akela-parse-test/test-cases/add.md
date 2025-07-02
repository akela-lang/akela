# Test Suite
Addition

## Test
Addition boolean

```cent
Test {
  .mute = false
  .solo = false
  .snapshot = false
  .has_error = true
}
```

```akela
5 + true
```

```cent
Errors {
  Error {
    .message = "addition on non-numeric operand"
    .line = 17
    .col = 5
  }
}
```

## Test
literal negative, literal negative

```cent
use lib::base::*
Test {
  .mute = false
  .solo = false
  .snapshot = false
}
```

```akela
-5 + -20
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Plus {
    .type = type0
    Ast::Sign {
      .type = type0
      Ast::Minus {
      }
      Ast::Number {
        .value = "5"
        .type = type0
      }
    }
    Ast::Sign {
      .type = type0
      Ast::Minus {
      }
      Ast::Number {
        .value = "20"
        .type = type0
      }
    }
  }
}
```