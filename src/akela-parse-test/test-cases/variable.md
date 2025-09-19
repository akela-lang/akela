# Test Suite
variable

```cent
TestSuite {
}
```

## Test
Integer

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
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
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
Natural

```cent
use lib::base::*
Test {
}
```

```akela
const a: Nat64 = 241
a
```

```cent
use lib::base::*
const type0 = Type::Natural {
  .name = "Nat64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "241"
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
Double

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
const type0 = Type::Real {
  .name = "Real64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1.5"
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
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::String {
      .value = "hello"
      .type = type0
    }
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
  Ast::Var {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
  }
  Ast::Assign {
    .left = Ast::Id {
      .value = "a"
      .type = type0
    }
    .right = Ast::Number {
      .value = "1"
      .type = type0
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
id

```cent
Test {
}
```

```akela
const x: Int64 = 1
x
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    .id = Ast::Id {
      .value = "x"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Id {
    .value = "x"
    .type = type0
  }
}
```

## Test
id 2

```cent
Test {
}
```

```akela
const _x10: Int64 = 1
_x10
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    .id = Ast::Id {
      .value = "_x10"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Id {
    .value = "_x10"
    .type = type0
  }
}
```

## Test
id 3

```cent
Test {
}
```

```akela
const a2: Int64 = 1
a2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    .id = Ast::Id {
      .value = "a2"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Id {
    .value = "a2"
    .type = type0
  }
}
```

## Test
id greek

```cent
Test {
}
```

```akela
const αβγ: Int64 = 1
αβγ
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    .id = Ast::Id {
      .value = "αβγ"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Id {
    .value = "αβγ"
    .type = type0
  }
}
```

## Test
id cyrillic (error)

```cent
Test {
  .has_error = true
}
```

```akela
const я: Int32
я
```
```cent
Errors {
  Error {
    .message = "Unrecognized character: я"
    .line = 1
    .col = 7
  }
}
```

## Test
newline after const

```cent
Test {
}
```

```akela
const
x
:
Int32 =
1
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Const {
    .id = Ast::Id {
      .value = "x"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
}
```

## Test
newline after var

```cent
Test {
}
```

```akela
var
x
:
Int32 =
1
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Var {
    .id = Ast::Id {
      .value = "x"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
}
```

## Test
assign

```cent
Test {
}
```

```akela
var a: Int32; a = 1
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Var {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
  }
  Ast::Assign {
    .left = Ast::Id {
      .value = "a"
      .type = type0
    }
    .right = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
}
```

## Test
assign 2

```cent
Test {
}
```

```akela
var a: Int32
a = 1 + 2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Var {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
  }
  Ast::Assign {
    .left = Ast::Id {
      .value = "a"
      .type = type0
    }
    .right = Ast::Plus {
      .type = type0
      .left = Ast::Number {
        .value = "1"
        .type = type0
      }
      .right = Ast::Number {
        .value = "2"
        .type = type0
      }
    }
  }
}
```

## Test
const

```cent
Test {
}
```

```akela
const a: Int32 = 0
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Const {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "0"
      .type = type0
    }
  }
}
```

## Test
const 2

```cent
Test {
}
```

```akela
const a: Int32 = 1
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Const {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "1"
      .type = type0
    }
  }
}
```

## Test
const (error expected declaration)

```cent
Test {
  .has_error = true
}
```

```akela
const
```

```cent
Error {
  .message = "expected id"
  .line = 2
  .col = 1
}
```

## Test
var

```cent
Test {
}
```

```akela
var x: Int32 = 10
x = 5
x
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
    .id = Ast::Id {
      .value = "x"
    }
    .type_node = Ast::Type {
      .type = type0
    }
    .expr = Ast::Number {
      .value = "10"
      .type = type0
    }
  }
  Ast::Assign {
    .left = Ast::Id {
      .value = "x"
      .type = type0
    }
    .right = Ast::Number {
      .value = "5"
      .type = type0
    }
  }
  Ast::Id {
    .value = "x"
    .type = type0
  }
}
```

## Test
const (error immutable changed)

```cent
Test {
  .has_error = true
}
```

```akela
const x: Int32 = 10
x = 5
x
```

```cent
Error {
  .message = "immutable variable changed in assignment"
  .line = 2
  .col = 1
}
```