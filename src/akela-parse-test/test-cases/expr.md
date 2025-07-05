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
    .line = 595
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
    .line = 622
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
    .line = 649
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
    .line = 677
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
    .line = 704
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
    .line = 1108
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
    .line = 1135
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
    .line = 1162
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
    .line = 1190
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
    .line = 1217
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
    .line = 1790
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
    .line = 1817
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
    .line = 1845
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
    .line = 1872
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
    .line = 1899
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
    .line = 2100
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
    .line = 2127
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
    .line = 2155
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
    .line = 2182
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
    .line = 2209
    .col = 9
  }
}
```

## Test
array declare

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
array declare

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