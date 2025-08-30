# Test Suite
operator parenthesis

```cent
TestSuite {
}
```

## Test
paren add

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
const speed: Int32 = 105
(speed + 1)
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
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "105"
      .type = type0
    }
  }
  Ast::Parenthesis {
    .type = type0
    Ast::Plus {
      .left = Ast::Id {
        .value = "speed"
        .type = type0
      }
      .right = Ast::Number {
        .value = "1"
        .type = type0
      }
      .type = type0
    }
  }
}
```

## Test
paren add 2

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
const speed: Int32 = 105
(speed) + 1
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
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "105"
      .type = type0
    }
  }
  Ast::Plus {
    .left = Ast::Parenthesis {
      .type = type0
      Ast::Id {
        .value = "speed"
        .type = type0
      }
    }
    .right = Ast::Number {
      .value = "1"
      .type = type0
    }
    .type = type0
  }
}
```

## Test
paren add 3

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
const speed: Int32 = 105
speed + (1)
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
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "105"
      .type = type0
    }
  }
  Ast::Plus {
    .left = Ast::Id {
      .value = "speed"
      .type = type0
    }
    .right = Ast::Parenthesis {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type0
      }
    }
    .type = type0
  }
}
```

## Test
paren add add

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
1 + (2 + 3)
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
    .left = Ast::Number {
      .value = "1"
      .type = type0
    }
    .right = Ast::Parenthesis {
      .type = type0
      Ast::Plus {
        .left = Ast::Number {
          .value = "2"
          .type = type0
        }
        .right = Ast::Number {
          .value = "3"
          .type = type0
        }
        .type = type0
      }
    }
    .type = type0
  }
}
```

## Test
paren add add 2

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
(1 + 2) + 3
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
    .left = Ast::Parenthesis {
      .type = type0
      Ast::Plus {
        .left = Ast::Number {
          .value = "1"
          .type = type0
        }
        .right = Ast::Number {
          .value = "2"
          .type = type0
        }
        .type = type0
      }
    }
    .right = Ast::Number {
      .value = "3"
      .type = type0
    }
    .type = type0
  }
}
```

## Test
paren mult

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
(5 * 2)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Parenthesis {
    .type = type0
    Ast::Mult {
      .left = Ast::Number {
        .value = "5"
        .type = type0
      }
      .right = Ast::Number {
        .value = "2"
        .type = type0
      }
      .type = type0
    }
  }
}
```

## Test
paren mult mult

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
1 * (2 * 3)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Mult {
    .left = Ast::Number {
      .value = "1"
      .type = type0
    }
    .right = Ast::Parenthesis {
      .type = type0
      Ast::Mult {
        .left = Ast::Number {
          .value = "2"
          .type = type0
        }
        .right = Ast::Number {
          .value = "3"
          .type = type0
        }
        .type = type0
      }
    }
    .type = type0
  }
}
```

## Test
paren mult mult 2

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
(1 * 2) * 3
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Mult {
    .left = Ast::Parenthesis {
      .type = type0
      Ast::Mult {
        .left = Ast::Number {
          .value = "1"
          .type = type0
        }
        .right = Ast::Number {
          .value = "2"
          .type = type0
        }
        .type = type0
      }
    }
    .right = Ast::Number {
      .value = "3"
      .type = type0
    }
    .type = type0
  }
}
```

## Test
paren number

```cent
Test {
}
```

```akela
(32)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Parenthesis {
    .type = type0
    Ast::Number {
      .value = "32"
      .type = type0
    }
  }
}
```

## Test
paren (error no expression)

```cent
Test {
  .has_error = true
}
```

```akela
()
```

```cent
Errors {
  Error {
    .message = "empty parenthesis"
    .line = 1
    .col = 2
  }
}
```

## Test
paren (error expected right paren)

```cent
Test {
  .has_error = true
}
```

```akela
(1
```

```cent
Errors {
  Error {
    .message = "expected right parenthesis"
    .line = 2
    .col = 1
  }
}
```

## Test
paren (error no value)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo() end
(foo())
```

```cent
Errors {
  Error {
    .message = "parenthesis on expression that has no value"
    .line = 2
    .col = 2
  }
}
```

## Test
newline parenthesis

```cent
Test {
}
```

```akela
(
200
)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Parenthesis {
    .type = type0
    Ast::Number {
      .value = "200"
      .type = type0
    }
  }
}
```
