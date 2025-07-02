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
Ast::Stmts {
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
  Ast::Plus {
    .type = Type::Integer {
      .name = "Int32"
      .bit_count = 32
    }
    Ast::Sign {
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
      Ast::Minus {
      }
      Ast::Number {
        .value = "5"
        .type = Type::Integer {
          .name = "Int32"
          .bit_count = 32
        }
      }
    }
    Ast::Sign {
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
      Ast::Minus {
      }
      Ast::Number {
        .value = "20"
        .type = Type::Integer {
          .name = "Int32"
          .bit_count = 32
        }
      }
    }
  }
}
```