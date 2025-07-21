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
      .line = 2
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
      .line = 2
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
      .line = 1
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
      .line = 2
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
      .line = 1
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

## Test
sub positive

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
const speed: Int32 = 100
speed - +1
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
      .value = "100"
      .type = type0
    }
  }
  Ast::Minus {
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
sub negative

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
const speed: Int32 = 100
speed - -1
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
      .value = "100"
      .type = type0
    }
  }
  Ast::Minus {
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
mult

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
5 * 2
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
    .type = type0
    Ast::Number {
      .value = "5"
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
mult (error expected term)

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
5 *
```

```cent
Errors {
  Error {
    .message = "expected term after operator"
    .line = 2
    .col = 1
  }
}
```

## Test
mult (error left no value)

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
foo() * 1
```

```cent
Errors {
  Error {
    .message = "multiplication operand has no value"
    .line = 2
    .col = 1
  }
}
```

## Test
mult (error left not numeric)

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
true * 1
```

```cent
Errors {
  Error {
    .message = "multiplication on non-numeric operand"
    .line = 1
    .col = 1
  }
}
```

## Test
mult (error right no value)

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
1 * foo()
```

```cent
Errors {
  Error {
    .message = "multiplication operand has no value"
    .line = 2
    .col = 5
  }
}
```

## Test
mult (error right not numeric)

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
1 * true
```

```cent
Errors {
  Error {
    .message = "multiplication on non-numeric operand"
    .line = 1
    .col = 5
  }
}
```

## Test
mult positive

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
const speed: Int32 = 50
speed * +1
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
      .value = "50"
      .type = type0
    }
  }
  Ast::Mult {
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
mult negative

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
const speed: Int32 = 50
speed * -1
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
      .value = "50"
      .type = type0
    }
  }
  Ast::Mult {
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
divide

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
52 / 2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Divide {
    .type = type0
    Ast::Number {
      .value = "52"
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
add add

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
1 + 2 + 3
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
    Ast::Plus {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
    }
    Ast::Number {
      .value = "3"
      .type = type0
    }
  }
}
```

## Test
mult mult

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
1 * 2 * 3
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
    .type = type0
    Ast::Mult {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
    }
    Ast::Number {
      .value = "3"
      .type = type0
    }
  }
}
```

## Test
add mult

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
5 + 3 * 2
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
    Ast::Number {
      .value = "5"
      .type = type0
    }
    Ast::Mult {
      .type = type0
      Ast::Number {
        .value = "3"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
    }
  }
}
```

## Test
mult add

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
4 * 3 + 2
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
    Ast::Mult {
      .type = type0
      Ast::Number {
        .value = "4"
        .type = type0
      }
      Ast::Number {
        .value = "3"
        .type = type0
      }
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
power

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
5 ^ 2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Power {
    .type = type0
    Ast::Number {
      .value = "5"
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
power (error expected term)

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
5 ^
```

```cent
Errors {
  Error {
    .message = "expected term after caret"
    .line = 2
    .col = 1
  }
}
```

## Test
power (error left no value)

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
5 ^ foo()
```

```cent
Errors {
  Error {
    .message = "power operand has no value"
    .line = 2
    .col = 5
  }
}
```

## Test
power (error left not numeric)

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
true ^ 2
```

```cent
Errors {
  Error {
    .message = "power on non-numeric operand"
    .line = 1
    .col = 1
  }
}
```

## Test
power (error right no value)

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
5 ^ foo()
```

```cent
Errors {
  Error {
    .message = "power operand has no value"
    .line = 2
    .col = 5
  }
}
```

## Test
power (error right not numeric)

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
5 ^ true
```

```cent
Errors {
  Error {
    .message = "power on non-numeric operand"
    .line = 1
    .col = 5
  }
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
    .type = type0
    Ast::Parenthesis {
      .type = type0
      Ast::Id {
        .value = "speed"
        .type = type0
      }
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
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
    .type = type0
    Ast::Id {
      .value = "speed"
      .type = type0
    }
    Ast::Parenthesis {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type0
      }
    }
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
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Parenthesis {
      .type = type0
      Ast::Plus {
        .type = type0
        Ast::Number {
          .value = "2"
          .type = type0
        }
        Ast::Number {
          .value = "3"
          .type = type0
        }
      }
    }
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
    .type = type0
    Ast::Parenthesis {
      .type = type0
      Ast::Plus {
        .type = type0
        Ast::Number {
          .value = "1"
          .type = type0
        }
        Ast::Number {
          .value = "2"
          .type = type0
        }
      }
    }
    Ast::Number {
      .value = "3"
      .type = type0
    }
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
      .type = type0
      Ast::Number {
        .value = "5"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
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
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Parenthesis {
      .type = type0
      Ast::Mult {
        .type = type0
        Ast::Number {
          .value = "2"
          .type = type0
        }
        Ast::Number {
          .value = "3"
          .type = type0
        }
      }
    }
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
    .type = type0
    Ast::Parenthesis {
      .type = type0
      Ast::Mult {
        .type = type0
        Ast::Number {
          .value = "1"
          .type = type0
        }
        Ast::Number {
          .value = "2"
          .type = type0
        }
      }
    }
    Ast::Number {
      .value = "3"
      .type = type0
    }
  }
}
```

## Test
comparison

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
var count: Int32 = 5
count == 10
count != 11.1
count <= 12
count >= 13
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
  Ast::Var {
    Ast::Id {
      .value = "count"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "5"
      .type = type0
    }
  }
  Ast::Equality {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "10"
      .type = type0
    }
  }
  Ast::NotEqual {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "11.1"
      .type = type1
    }
  }
  Ast::LessThanOrEqual {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "12"
      .type = type0
    }
  }
  Ast::GreaterThanOrEqual {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "13"
      .type = type0
    }
  }
}
```

## Test
comparison identity

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
true == true
true != true
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Equality {
    .type = type0
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
  Ast::NotEqual {
    .type = type0
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
}
```

## Test
comparison (error no term)

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
100 <
```

```cent
Errors {
  Error {
    .message = "expected term after comparison operator"
    .line = 2
    .col = 1
  }
}
```

## Test
comparison (error left no value)

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
foo() < 100
```

```cent
Errors {
  Error {
    .message = "operand has no value"
    .line = 2
    .col = 1
  }
}
```

## Test
comparison (error right no value)

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
100 < foo()
```

```cent
Errors {
  Error {
    .message = "operand has no value"
    .line = 2
    .col = 7
  }
}
```

## Test
comparison (error left not numeric)

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
true < 100
```

```cent
Errors {
  Error {
    .message = "comparison operand is not numeric"
    .line = 1
    .col = 1
  }
}
```

## Test
comparison (error right not numeric)

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
100 < true
```

```cent
Errors {
  Error {
    .message = "comparison operand is not numeric"
    .line = 1
    .col = 7
  }
}
```

## Test
and

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
const a: Bool = true
const b: Bool = false
a && b
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
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
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "b"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
  Ast::And {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type0
    }
    Ast::Id {
      .value = "b"
      .type = type0
    }
  }
}
```

## Test
or

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
const a: Bool = true
const b: Bool = false
a || b
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
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
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "b"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
  Ast::Or {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type0
    }
    Ast::Id {
      .value = "b"
      .type = type0
    }
  }
}
```

## Test
or or

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
true || true || false
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Or {
    .type = type0
    Ast::Or {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type0
      }
      Ast::Boolean {
        .value = "true"
        .type = type0
      }
    }
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
}
```

## Test
boolean (error expected term)

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
true &&
```

```cent
Errors {
  Error {
    .message = "expected term after && or ||"
    .line = 2
    .col = 1
  }
}
```

## Test
boolean (error left no value)

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
foo() && true
```

```cent
Errors {
  Error {
    .message = "left-side operand of boolean operator has no type"
    .line = 2
    .col = 1
  }
}
```

## Test
boolean (error right no value)

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
true && foo()
```

```cent
Errors {
  Error {
    .message = "operand of boolean operator has no type"
    .line = 2
    .col = 9
  }
}
```

## Test
boolean (error left not boolean)

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
1 && true
```

```cent
Errors {
  Error {
    .message = "left-side expression of boolean operator is not boolean"
    .line = 1
    .col = 1
  }
}
```

## Test
boolean (error right not boolean)

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
true && 1
```

```cent
Errors {
  Error {
    .message = "expression of boolean operator is not boolean"
    .line = 1
    .col = 9
  }
}
```

## Test
array subscript

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
const a: [5]Int32 = [0, 1, 2, 3, 4]
a[0]
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Array {
  .is_const = false
  .dim = 5
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type2 = Type::Natural {
  .name = "Nat64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type1
    }
    Ast::ArrayLiteral {
      .type = type1
      Ast::Number {
        .value = "0"
        .type = type0
      }
      Ast::Number {
        .value = "1"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
      Ast::Number {
        .value = "3"
        .type = type0
      }
      Ast::Number {
        .value = "4"
        .type = type0
      }
    }
  }
  Ast::ArraySubscript {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type1
    }
    Ast::Number {
      .value = "0"
      .type = type2
    }
  }
}
```

## Test
array subscript 2

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
const a: [3][3]Int32 = [
  [0, 1, 2],
  [3, 4, 5],
  [6, 7, 8]
]
a[1][1]
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Array {
  .is_const = false
  .dim = 3
  .type = Type::Array {
    .is_const = false
    .dim = 3
    .type = Type::Integer {
      .name = "Int32"
      .bit_count = 32
    }
  }
}
const type2 = Type::Array {
  .is_const = false
  .dim = 3
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type3 = Type::Natural {
  .name = "Nat64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type1
    }
    Ast::ArrayLiteral {
      .type = type1
      Ast::ArrayLiteral {
        .type = type2
        Ast::Number {
          .value = "0"
          .type = type0
        }
        Ast::Number {
          .value = "1"
          .type = type0
        }
        Ast::Number {
          .value = "2"
          .type = type0
        }
      }
      Ast::ArrayLiteral {
        .type = type2
        Ast::Number {
          .value = "3"
          .type = type0
        }
        Ast::Number {
          .value = "4"
          .type = type0
        }
        Ast::Number {
          .value = "5"
          .type = type0
        }
      }
      Ast::ArrayLiteral {
        .type = type2
        Ast::Number {
          .value = "6"
          .type = type0
        }
        Ast::Number {
          .value = "7"
          .type = type0
        }
        Ast::Number {
          .value = "8"
          .type = type0
        }
      }
    }
  }
  Ast::ArraySubscript {
    .type = type0
    Ast::ArraySubscript {
      .type = type2
      Ast::Id {
        .value = "a"
        .type = type1
      }
      Ast::Number {
        .value = "1"
        .type = type3
      }
    }
    Ast::Number {
      .value = "1"
      .type = type3
    }
  }
}
```

## Test
array subscript 3

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
const a: [4]Int32 = [0, 1, 2, 3]
const b: Nat64 = 1
a[b]
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Array {
  .is_const = false
  .dim = 4
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type2 = Type::Natural {
  .name = "Nat64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type1
    }
    Ast::ArrayLiteral {
      .type = type1
      Ast::Number {
        .value = "0"
        .type = type0
      }
      Ast::Number {
        .value = "1"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
      Ast::Number {
        .value = "3"
        .type = type0
      }
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "b"
    }
    Ast::Type {
      .type = type2
    }
    Ast::Number {
      .value = "1"
      .type = type2
    }
  }
  Ast::ArraySubscript {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type1
    }
    Ast::Id {
      .value = "b"
      .type = type2
    }
  }
}
```

## Test
array subscript 4

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
const x: [2][3][4 const]Int32 =
  [
    [
      [1, 2, 3, 4],
      [5, 6, 7, 8],
      [9, 10, 11, 12]
    ],
    [
      [13, 14, 15, 16],
      [17, 18, 19, 20],
      [21, 22, 23, 24]
    ]
  ]
x[0][1][2]
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Array {
  .is_const = false
  .dim = 2
  .type = Type::Array {
    .is_const = false
    .dim = 3
    .type = Type::Array {
      .is_const = true
      .dim = 4
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
  }
}
const type2 = Type::Array {
  .is_const = false
  .dim = 3
  .type = Type::Array {
    .is_const = true
    .dim = 4
    .type = Type::Integer {
      .name = "Int32"
      .bit_count = 32
    }
  }
}
const type3 = Type::Array {
  .is_const = true
  .dim = 4
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type4 = Type::Natural {
  .name = "Nat64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
      .type = type1
    }
    Ast::ArrayLiteral {
      .type = type1
      Ast::ArrayLiteral {
        .type = type2
        Ast::ArrayLiteral {
          .type = type3
          Ast::Number {
            .value = "1"
            .type = type0
          }
          Ast::Number {
            .value = "2"
            .type = type0
          }
          Ast::Number {
            .value = "3"
            .type = type0
          }
          Ast::Number {
            .value = "4"
            .type = type0
          }
        }
        Ast::ArrayLiteral {
          .type = type3
          Ast::Number {
            .value = "5"
            .type = type0
          }
          Ast::Number {
            .value = "6"
            .type = type0
          }
          Ast::Number {
            .value = "7"
            .type = type0
          }
          Ast::Number {
            .value = "8"
            .type = type0
          }
        }
        Ast::ArrayLiteral {
          .type = type3
          Ast::Number {
            .value = "9"
            .type = type0
          }
          Ast::Number {
            .value = "10"
            .type = type0
          }
          Ast::Number {
            .value = "11"
            .type = type0
          }
          Ast::Number {
            .value = "12"
            .type = type0
          }
        }
      }
      Ast::ArrayLiteral {
        .type = type2
        Ast::ArrayLiteral {
          .type = type3
          Ast::Number {
            .value = "13"
            .type = type0
          }
          Ast::Number {
            .value = "14"
            .type = type0
          }
          Ast::Number {
            .value = "15"
            .type = type0
          }
          Ast::Number {
            .value = "16"
            .type = type0
          }
        }
        Ast::ArrayLiteral {
          .type = type3
          Ast::Number {
            .value = "17"
            .type = type0
          }
          Ast::Number {
            .value = "18"
            .type = type0
          }
          Ast::Number {
            .value = "19"
            .type = type0
          }
          Ast::Number {
            .value = "20"
            .type = type0
          }
        }
        Ast::ArrayLiteral {
          .type = type3
          Ast::Number {
            .value = "21"
            .type = type0
          }
          Ast::Number {
            .value = "22"
            .type = type0
          }
          Ast::Number {
            .value = "23"
            .type = type0
          }
          Ast::Number {
            .value = "24"
            .type = type0
          }
        }
      }
    }
  }
  Ast::ArraySubscript {
    .type = type0
    Ast::ArraySubscript {
      .type = type3
      Ast::ArraySubscript {
        .type = type2
        Ast::Id {
          .value = "x"
          .type = type1
        }
        Ast::Number {
          .value = "0"
          .type = type4
        }
      }
      Ast::Number {
        .value = "1"
        .type = type4
      }
    }
    Ast::Number {
      .value = "2"
      .type = type4
    }
  }
}
```

## Test
array subscript 4

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
fn foo() end;
foo()[1]
```

```cent
Errors {
  Error {
    .message = "expression has subscript but has no value"
    .line = 2
    .col = 1
  }
}
```

## Test
array subscript (error not array)

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
const a: Int32 = 0
a[1]
```

```cent
Errors {
  Error {
    .message = "expression has subscript but is not an array or slice"
    .line = 2
    .col = 1
  }
}
```

## Test
array subscript (error expected right square bracket)

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
const a: [10]Int32
a[1
```

```cent
Errors {
  Error {
    .message = "expected right-square-bracket"
    .line = 3
    .col = 1
  }
}
```

## Test
assign string

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
var a: [6 const]Nat8 = "hello"
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = true
  .dim = 6
  .type = Type::Natural {
    .name = "Nat8"
    .bit_count = 8
  }
}
Ast::Stmts {
  Ast::Var {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
    Ast::String {
      .value = "hello"
      .type = type0
    }
  }
}
```

## Test
assign multiple

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
var a: Int32
var b: Int32
var c: Int32
a = b = c = 0
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
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
  }
  Ast::Var {
    Ast::Id {
      .value = "b"
    }
    Ast::Type {
      .type = type0
    }
  }
  Ast::Var {
    Ast::Id {
      .value = "c"
    }
    Ast::Type {
      .type = type0
    }
  }
  Ast::Assign {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type0
    }
    Ast::Id {
      .value = "b"
      .type = type0
    }
    Ast::Id {
      .value = "c"
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
}
```

## Test
assignment (error eseq count)

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
var a: Int32
var b: Int32
var c: Int32
a, b, c = 1, 2
```

```cent
Errors {
  Error {
    .message = "assignment sequence counts do not match"
    .line = 4
    .col = 11
  }
}
```

## Test
assignment (error no rhs)

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
const a: Bool =
```

```cent
Errors {
  Error {
    .message = "expected expression"
    .line = 2
    .col = 1
  }
}
```

## Test
assign (error no value right)

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
const a: [10]Nat8 = foo()
```

```cent
Errors {
  Error {
    .message = "cannot assign with operand that has no value"
    .line = 2
    .col = 21
  }
}
```

## Test
assign (error not compatible)

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
const x: Int32 = true
```

```cent
Errors {
  Error {
    .message = "values in assignment are not compatible"
    .line = 1
    .col = 18
  }
}
```

## Test
assign (error l-value)

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
true = true
```

```cent
Errors {
  Error {
    .message = "invalid lvalue"
    .line = 1
    .col = 1
  }
}
```

## Test
assign newline

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
var a: Int32
a =
1
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
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
  }
  Ast::Assign {
    .type = type0
    Ast::Id {
      .value = "a"
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
boolean operator newline

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
true &&
false
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::And {
    .type = type0
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
}
```

## Test
comparison operator newline

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
1 <
2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::LessThan {
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
add newline

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
1 +
2
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
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
mult newline

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
1 *
2
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
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
power newline

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
1 ^
2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Power {
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
subscript newline

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
const a: [3]Int32 = [100,200,300]
a[
0
]
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Array {
  .is_const = false
  .dim = 3
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type2 = Type::Natural {
  .name = "Nat64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type1
    }
    Ast::ArrayLiteral {
      .type = type1
      Ast::Number {
        .value = "100"
        .type = type0
      }
      Ast::Number {
        .value = "200"
        .type = type0
      }
      Ast::Number {
        .value = "300"
        .type = type0
      }
    }
  }
  Ast::ArraySubscript {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type1
    }
    Ast::Number {
      .value = "0"
      .type = type2
    }
  }
}
```

## Test
call newline

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
fn foo(a: Int32, b: Int32)->Int32 a+b end
foo(
1,
2
)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Function {
  .name = "foo"
  .input = Input {
    TypeParam::Regular {
      .name = "a"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "b"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
  }
  .output = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
Ast::Stmts {
  .type = type0
  Ast::Function {
    .type = type1
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
        Ast::Declaration {
          Ast::Id {
            .value = "a"
          }
          Ast::Type {
            .type = type0
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "b"
          }
          Ast::Type {
            .type = type0
          }
        }
      }
      Ast::Dret {
        Ast::Type {
          .type = type0
        }
      }
    }
    Ast::Stmts {
      .type = type0
      Ast::Plus {
        .type = type0
        Ast::Id {
          .value = "a"
          .type = type0
        }
        Ast::Id {
          .value = "b"
          .type = type0
        }
      }
    }
  }
  Ast::Call {
    .type = type0
    Ast::Id {
      .value = "foo"
      .type = type1
    }
    Ast::Cseq {
      Ast::Number {
        .value = "1"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
    }
  }
}
```

## Test
assign multi

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
var a: Int32
var b: Int32
var c: Int32
a,b,c = 1,2,3
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Var {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
  }
  Ast::Var {
    Ast::Id {
      .value = "b"
    }
    Ast::Type {
      .type = type0
    }
  }
  Ast::Var {
    Ast::Id {
      .value = "c"
    }
    Ast::Type {
      .type = type0
    }
  }
  Ast::Assign {
    Ast::Eseq {
      Ast::Id {
        .value = "a"
        .type = type0
      }
      Ast::Id {
        .value = "b"
        .type = type0
      }
      Ast::Id {
        .value = "c"
        .type = type0
      }
    }
    Ast::Eseq {
      Ast::Number {
        .value = "1"
        .type = type0
      }
      Ast::Number {
        .value = "2"
        .type = type0
      }
      Ast::Number {
        .value = "3"
        .type = type0
      }
    }
  }
}
```

## Test
assign (error l-value)

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
1 = 1
```

```cent
Errors {
  Error {
    .message = "invalid lvalue"
    .line = 1
    .col = 1
  }
}
```

## Test
assign (error eseq l-value)

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
1,2,3 = 1,2,3
```

```cent
Errors {
  Error {
    .message = "invalid lvalue"
    .line = 1
    .col = 1
  }
}
```

## Test
array string return

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
fn foo(a: Int32, b: Int32, c: Int32)->[6 const]Nat8
  "hello"
end
foo(1, 2, 3)
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = true
  .dim = 6
  .type = Type::Natural {
    .name = "Nat8"
    .bit_count = 8
  }
}
const type1 = Type::Function {
  .name = "foo"
  .input = Input {
    TypeParam::Regular {
      .name = "a"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "b"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "c"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
  }
  .output = Type::Array {
    .is_const = true
    .dim = 6
    .type = Type::Natural {
      .name = "Nat8"
      .bit_count = 8
    }
  }
}
const type2 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Function {
    .type = type1
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
        Ast::Declaration {
          Ast::Id {
            .value = "a"
          }
          Ast::Type {
            .type = type2
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "b"
          }
          Ast::Type {
            .type = type2
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "c"
          }
          Ast::Type {
            .type = type2
          }
        }
      }
      Ast::Dret {
        Ast::Type {
          .type = type0
        }
      }
    }
    Ast::Stmts {
      .type = type0
      Ast::String {
        .value = "hello"
        .type = type0
      }
    }
  }
  Ast::Call {
    .type = type0
    Ast::Id {
      .value = "foo"
      .type = type1
    }
    Ast::Cseq {
      Ast::Number {
        .value = "1"
        .type = type2
      }
      Ast::Number {
        .value = "2"
        .type = type2
      }
      Ast::Number {
        .value = "3"
        .type = type2
      }
    }
  }
}
```