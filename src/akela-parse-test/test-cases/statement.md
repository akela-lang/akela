# Test Suite
statements

```cent
TestSuite {
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
}
```

## Test
multiple statements

```cent
Test {
}
```


```akela
const i: Int32 = 0
const x: Int32 = 20
i + 1
x * 1
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
      .value = "i"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "20"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "i"
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Mult {
    .type = type0
    Ast::Id {
      .value = "x"
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
multiple statements 2

```cent
Test {
}
```

```akela
const x: Int32 = 0
x + 1
5 - 4
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
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "x"
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Minus {
    .type = type0
    Ast::Number {
      .value = "5"
      .type = type0
    }
    Ast::Number {
      .value = "4"
      .type = type0
    }
  }
}
```

## Test
multiple statements 3

```cent
Test {
}
```

```akela
const x: Int32 = 0
const y: Int32 = 1
const z: Int32 = 2
x+1
5-4
y+z
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
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "y"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "z"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "x"
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Minus {
    .type = type0
    Ast::Number {
      .value = "5"
      .type = type0
    }
    Ast::Number {
      .value = "4"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "y"
      .type = type0
    }
    Ast::Id {
      .value = "z"
      .type = type0
    }
  }
}
```

## Test
multiple statements 4

```cent
Test {
}
```

```akela
const x: Int32 = 0
const y: Int32 = 1
const z: Int32 = 2
x+1
5-4
y+z
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
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "y"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "z"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "x"
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::Minus {
    .type = type0
    Ast::Number {
      .value = "5"
      .type = type0
    }
    Ast::Number {
      .value = "4"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "y"
      .type = type0
    }
    Ast::Id {
      .value = "z"
      .type = type0
    }
  }
}
```

## Test
if

```cent
Test {
}
```

```akela
if true 1 else 2 end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::If {
    .type = type0
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Number {
          .value = "1"
          .type = type0
        }
      }
    }
    Ast::DefaultBranch {
      .type = type0
      Ast::Stmts {
        .type = type0
        Ast::Number {
          .value = "2"
          .type = type0
        }
      }
    }
  }
}
```

## Test
statement types

```cent
Test {
}
```

```akela
1
true
"hello"
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
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type2 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Number {
    .value = "1"
    .type = type1
  }
  Ast::Boolean {
    .value = "true"
    .type = type2
  }
  Ast::String {
    .value = "hello"
    .type = type0
  }
}
```

## Test
if 2

```cent
Test {
}
```

```akela
if true
  const x: Int32 = 0
  const y: Int32 = 1
  10 + 20
  x * y
end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  Ast::If {
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Const {
          Ast::Id {
            .value = "x"
          }
          Ast::Type {
            .type = type0
          }
          Ast::Number {
            .value = "0"
            .type = type0
          }
        }
        Ast::Const {
          Ast::Id {
            .value = "y"
          }
          Ast::Type {
            .type = type0
          }
          Ast::Number {
            .value = "1"
            .type = type0
          }
        }
        Ast::Plus {
          .type = type0
          Ast::Number {
            .value = "10"
            .type = type0
          }
          Ast::Number {
            .value = "20"
            .type = type0
          }
        }
        Ast::Mult {
          .type = type0
          Ast::Id {
            .value = "x"
            .type = type0
          }
          Ast::Id {
            .value = "y"
            .type = type0
          }
        }
      }
    }
  }
}
```

## Test
if elseif

```cent
Test {
}
```

```akela
const x: Int32 = 0
const y: Int32 = 1
if true
    10 + 20
    x * y
elseif true
    1
    2
end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  Ast::Const {
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "y"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::If {
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Plus {
          .type = type0
          Ast::Number {
            .value = "10"
            .type = type0
          }
          Ast::Number {
            .value = "20"
            .type = type0
          }
        }
        Ast::Mult {
          .type = type0
          Ast::Id {
            .value = "x"
            .type = type0
          }
          Ast::Id {
            .value = "y"
            .type = type0
          }
        }
      }
    }
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type1
      }
      Ast::Stmts {
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
  }
}
```

## Test
if elseif elseif

```cent
Test {
}
```

```akela
const x: Int32 = 0
const y: Int32 = 1
if true
  10 + 20
  x * y
elseif true
  1
  2
elseif true
  x
  y
end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  Ast::Const {
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "y"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::If {
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Plus {
          .type = type0
          Ast::Number {
            .value = "10"
            .type = type0
          }
          Ast::Number {
            .value = "20"
            .type = type0
          }
        }
        Ast::Mult {
          .type = type0
          Ast::Id {
            .value = "x"
            .type = type0
          }
          Ast::Id {
            .value = "y"
            .type = type0
          }
        }
      }
    }
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type1
      }
      Ast::Stmts {
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
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "true"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Id {
          .value = "x"
          .type = type0
        }
        Ast::Id {
          .value = "y"
          .type = type0
        }
      }
    }
  }
}
```

## Test
if else

```cent
Test {
}
```

```akela
const x: Int32 = 0
const y: Int32 = 1
if false
    10
else
    x
    y
end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "y"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::If {
    .type = type0
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "false"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Number {
          .value = "10"
          .type = type0
        }
      }
    }
    Ast::DefaultBranch {
      .type = type0
      Ast::Stmts {
        .type = type0
        Ast::Id {
          .value = "x"
          .type = type0
        }
        Ast::Id {
          .value = "y"
          .type = type0
        }
      }
    }
  }
}
```

## Test
if elseif else

```cent
Test {
}
```

```akela
const x: Int32 = 0
const y: Int32 = 1
if false
    10
elseif false
    20
else
    x
    y
end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "y"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
  Ast::If {
    .type = type0
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "false"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Number {
          .value = "10"
          .type = type0
        }
      }
    }
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "false"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Number {
          .value = "20"
          .type = type0
        }
      }
    }
    Ast::DefaultBranch {
      .type = type0
      Ast::Stmts {
        .type = type0
        Ast::Id {
          .value = "x"
          .type = type0
        }
        Ast::Id {
          .value = "y"
          .type = type0
        }
      }
    }
  }
}
```

## Test
if (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
if end
```

```cent
Error {
  .message = "expected condition after if"
  .line = 1057
  .col = 4
}
```

## Test
if (error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
if true
```

```cent
Error {
  .message = "expected end"
  .line = 1079
  .col = 1
}
```

## Test
elseif (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
if true elseif end
```

```cent
Error {
  .message = "expected condition after elseif"
  .line = 1099
  .col = 16
}
```

## Test
while

```cent
Test {
}
```

```akela
while true 1 end
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::While {
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
    Ast::Stmts {
      .type = type1
      Ast::Number {
        .value = "1"
        .type = type1
      }
    }
  }
}
```

## Test
while (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
while end
```

```cent
Error {
  .message = "expected expression after while"
  .line = 1158
  .col = 7
}
```

## Test
while (error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
while true
```

```cent
Error {
  .message = "expected end"
  .line = 1180
  .col = 1
}
```

## Test
for range

```cent
Test {
}
```

```akela
for i: Int32 = 0:10
    1
end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::ForRange {
    Ast::Declaration {
      Ast::Id {
        .value = "i"
      }
      Ast::Type {
        .type = type0
      }
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
    Ast::Number {
      .value = "10"
      .type = type0
    }
    Ast::Stmts {
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
for iteration

```cent
Test {
}
```

```akela
const list: [10]Int32 = [1,2,3,4,5,6,7,8,9,10]
for i: Int32 in list
    i
end
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = false
  .dim = 10
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Const {
    Ast::Id {
      .value = "list"
    }
    Ast::Type {
      .type = type0
    }
    Ast::ArrayLiteral {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type1
      }
      Ast::Number {
        .value = "2"
        .type = type1
      }
      Ast::Number {
        .value = "3"
        .type = type1
      }
      Ast::Number {
        .value = "4"
        .type = type1
      }
      Ast::Number {
        .value = "5"
        .type = type1
      }
      Ast::Number {
        .value = "6"
        .type = type1
      }
      Ast::Number {
        .value = "7"
        .type = type1
      }
      Ast::Number {
        .value = "8"
        .type = type1
      }
      Ast::Number {
        .value = "9"
        .type = type1
      }
      Ast::Number {
        .value = "10"
        .type = type1
      }
    }
  }
  Ast::ForIteration {
    Ast::Declaration {
      Ast::Id {
        .value = "i"
      }
      Ast::Type {
        .type = type1
      }
    }
    Ast::Id {
      .value = "list"
      .type = type0
    }
    Ast::Stmts {
      .type = type1
      Ast::Id {
        .value = "i"
        .type = type1
      }
    }
  }
}
```

## Test
for iteration (error list no value)

```cent
Test {
  .has_error = true
}
```

```akela
fn list()
end
for i: Int32 in list()
end
```

```cent
Error {
  .message = "iteration expression has no value"
  .line = 1356
  .col = 17
}
```

## Test
function iteration (error not an array or slice)

```cent
Test {
  .has_error = true
}
```

```akela
const list: Int32
for i: Int32 in list
end
```

```cent
Error {
  .message = "iteration expression is not an array or slice"
  .line = 1379
  .col = 17
}
```

## Test
for range (error no value)

```cent
Test {
  .has_error = true
}
```

```akela
const list: [10]Bool = [false, true, true, false, true, true, false, true, true, false]
for i: Int32 in list
end
```

```cent
Error {
  .message = "cannot cast list element"
  .line = 1402
  .col = 17
}
```

## Test
for (error expected = or in)

```cent
Test {
  .has_error = true
}
```

```akela
const list: [10]Int32
for i: Int32
end
```

```cent
Error {
  .message = "expected '=' or 'in' after for element declaration"
  .line = 1426
  .col = 1
}
```

## Test
for (error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
const list: [10]Int32
for i: Int32 = 1:10
```

```cent
Error {
  .message = "expected end"
  .line = 1449
  .col = 1
}
```

## Test
for (error expected range start)

```cent
Test {
  .has_error = true
}
```

```akela
const list: [2]Int32 = [0.1]
for i: Int32 =
```

```cent
Error {
  .message = "expected range start"
  .line = 1471
  .col = 1
}
```

## Test
for range (error expected colon)

```cent
Test {
  .has_error = true
}
```

```akela
const list: [10]Int32
for i: Int32 = 1
```

```cent
Error {
  .message = "expected colon"
  .line = 1493
  .col = 1
}
```

## Test
for range (error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
const list: [10]Int32
for i: Int32 = 1:
```

```cent
Error {
  .message = "expected range end"
  .line = 1515
  .col = 1
}
```

## Test
for range (error start no value)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo() end
for i: Int32 = foo():10
end
```

```cent
Error {
  .message = "start range expression has no value"
  .line = 1536
  .col = 16
}
```

## Test
for range (error start not numeric)

```cent
Test {
  .has_error = true
}
```

```akela
for i: Int32 = true:10 end
```

```cent
Error {
  .message = "start range expression is not numeric"
  .line = 1558
  .col = 16
}
```

## Test
for range (error end no value)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo() end
for i: Int32 = 1:foo()
end
```

```cent
Error {
  .message = "end range expression has no value"
  .line = 1580
  .col = 18
}
```

## Test
for range (error end not numeric)

```cent
Test {
  .has_error = true
}
```

```akela
for i: Int32 = 1:true
end
```

```cent
Error {
  .message = "end range expression is not numeric"
  .line = 1602
  .col = 18
}
```

## Test
for iteration (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
const list: [10]Int32
for i: Int32 in:
```

```cent
Error {
  .message = "expected for iteration expression"
  .line = 1625
  .col = 16
}
```

## Test
for range newline

```cent
Test {
}
```

```akela
for
i
:
Int32
=
0
:
10
    i
end
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::ForRange {
    Ast::Declaration {
      Ast::Id {
        .value = "i"
      }
      Ast::Type {
        .type = type0
      }
    }
    Ast::Number {
      .value = "0"
      .type = type0
    }
    Ast::Number {
      .value = "10"
      .type = type0
    }
    Ast::Stmts {
      .type = type0
      Ast::Id {
        .value = "i"
        .type = type0
      }
    }
  }
}
```

## Test
for iteration newline

```cent
Test {
}
```

```akela
const v: [10]Int32 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
for
x
: Int32
in
v
    x
end
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = false
  .dim = 10
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Const {
    Ast::Id {
      .value = "v"
    }
    Ast::Type {
      .type = type0
    }
    Ast::ArrayLiteral {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type1
      }
      Ast::Number {
        .value = "2"
        .type = type1
      }
      Ast::Number {
        .value = "3"
        .type = type1
      }
      Ast::Number {
        .value = "4"
        .type = type1
      }
      Ast::Number {
        .value = "5"
        .type = type1
      }
      Ast::Number {
        .value = "6"
        .type = type1
      }
      Ast::Number {
        .value = "7"
        .type = type1
      }
      Ast::Number {
        .value = "8"
        .type = type1
      }
      Ast::Number {
        .value = "9"
        .type = type1
      }
      Ast::Number {
        .value = "10"
        .type = type1
      }
    }
  }
  Ast::ForIteration {
    Ast::Declaration {
      Ast::Id {
        .value = "x"
      }
      Ast::Type {
        .type = type1
      }
    }
    Ast::Id {
      .value = "v"
      .type = type0
    }
    Ast::Stmts {
      .type = type1
      Ast::Id {
        .value = "x"
        .type = type1
      }
    }
  }
}
```