# Test Suite
expression

```cent
use lib::base::*
TestSuite {
  .solo = false
  .mute = false
}
```

## Test
blank

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
```

```cent
use lib::base::*
Ast::Stmts {
}
```

## Test
add

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
var speed: Int32 = 100; speed + 1
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Var {
    Ast::Id {
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "100"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "speed"
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
}
```

## Test
add (error expected term)

```cent
use lib::base::*
Test {
  .solo = false
  .mute = false
  .snapshot = false
  .has_error = true
}
```

```akela
1 +
```

```cent
  Errors {
    Error {
      .message = "expected term after additive operator"
      .line = 100
      .col = 1
    }
  }
```

## Test
add (error left no value)

```cent
use lib::base::*
Test {
  .solo = false
  .mute = false
  .snapshot = false
  .has_error = true
}
```

```akela
fn foo() end
foo() + 1
```

```cent
  Errors {
    Error {
      .message = "addition operand has no value"
      .line = 127
      .col = 1
    }
  }
```

## Test
add (error left not numeric)

```cent
use lib::base::*
Test {
  .solo = false
  .mute = false
  .snapshot = false
  .has_error = true
}
```

```akela
true + 1
```

```cent
  Errors {
    Error {
      .message = "addition on non-numeric operand"
      .line = 154
      .col = 1
    }
  }
```

## Test
add (error right no value)

```cent
use lib::base::*
Test {
  .solo = false
  .mute = false
  .snapshot = false
  .has_error = true
}
```

```akela
fn foo() end
1 + foo()
```

```cent
  Errors {
    Error {
      .message = "addition operand has no value"
      .line = 182
      .col = 5
    }
  }
```

## Test
add (error right not numeric)

```cent
use lib::base::*
Test {
  .solo = false
  .mute = false
  .snapshot = false
  .has_error = true
}
```

```akela
1 + true
```

```cent
  Errors {
    Error {
      .message = "addition on non-numeric operand"
      .line = 209
      .col = 5
    }
  }
```

## Test
add mixed types

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
1 + 5.0
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Real {
  .name = "Real64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Plus {
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Number {
      .value = "5.0"
      .type = type1
    }
  }
}
```

## Test
add positive

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
var speed: Int32 = 100
speed + +1
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Var {
    Ast::Id {
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "100"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "speed"
      .type = type0
    }
    Ast::Sign {
      .type = type0
      Ast::Plus {
      }
      Ast::Number {
        .value = "1"
        .type = type0
      }
    }
  }
}
```

## Test
add negative

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
const speed: Int32
speed + -1
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
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "speed"
      .type = type0
    }
    Ast::Sign {
      .type = type0
      Ast::Minus {
      }
      Ast::Number {
        .value = "1"
        .type = type0
      }
    }
  }
}
```

## Test
sub

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
const delta: Int32 = 3
100 - delta
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
      .value = "delta"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "3"
      .type = type0
    }
  }
  Ast::Minus {
    .type = type0
    Ast::Number {
      .value = "100"
      .type = type0
    }
    Ast::Id {
      .value = "delta"
      .type = type0
    }
  }
}
```