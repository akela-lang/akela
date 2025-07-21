# Test Suite
array

```cent
TestSuite {
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

## Test
array

```cent
Test {
}
```

```akela
const a: [10]Int32 = [0,1,2,3,4,5,6,7,8,9]
a[1]
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Array {
  .is_const = false
  .dim = 10
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
      Ast::Number {
        .value = "9"
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
      .value = "1"
      .type = type2
    }
  }
}
```

## Test
array literal integer

```cent
Test {
}
```

```akela
[1,2,3]
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = false
  .dim = 3
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
  .type = type0
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
  }
}
```

## Test
array literal real

```cent
Test {
}
```

```akela
[1.0,2.5,3.2]
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = false
  .dim = 3
  .type = Type::Real {
    .name = "Real64"
    .bit_count = 64
  }
}
const type1 = Type::Real {
  .name = "Real64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::ArrayLiteral {
    .type = type0
    Ast::Number {
      .value = "1.0"
      .type = type1
    }
    Ast::Number {
      .value = "2.5"
      .type = type1
    }
    Ast::Number {
      .value = "3.2"
      .type = type1
    }
  }
}
```

## Test
array literal numeric

```cent
Test {
}
```

```akela
[1, 2.5, 3]
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = false
  .dim = 3
  .type = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type2 = Type::Real {
  .name = "Real64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::ArrayLiteral {
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type1
    }
    Ast::Number {
      .value = "2.5"
      .type = type2
    }
    Ast::Number {
      .value = "3"
      .type = type1
    }
  }
}
```

## Test
array literal mixed (error)

```cent
Test {
  .has_error = true
}
```

```akela
[1,true,3]
```

```cent
Errors {
  Error {
    .message = "array elements not the same type"
    .line = 1
    .col = 2
  }
}
```

## Test
array literal bool

```cent
Test {
}
```

```akela
[false,true,false]
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = false
  .dim = 3
  .type = Type::Boolean {
    .name = "Bool"
  }
}
const type1 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::ArrayLiteral {
    .type = type0
    Ast::Boolean {
      .value = "false"
      .type = type1
    }
    Ast::Boolean {
      .value = "true"
      .type = type1
    }
    Ast::Boolean {
      .value = "false"
      .type = type1
    }
  }
}
```

## Test
array literal empty error

```cent
Test {
  .has_error = true
}
```

```akela
[]
```

```cent
Errors {
  Error {
    .message = "array literal has no elements"
    .line = 1
    .col = 2
  }
}
```

## Test
array literal (error right square bracket)

```cent
Test {
  .has_error = true
}
```

```akela
[1,2
```

```cent
Errors {
  Error {
    .message = "expected right square bracket"
    .line = 2
    .col = 1
  }
}
```

## Test
array literal (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
[1,]
```

```cent
Errors {
  Error {
    .message = "expected expr after comma"
    .line = 1
    .col = 4
  }
}
```

## Test
newline array literal

```cent
Test {
}
```

```akela
[
1,
2,
3
]
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = false
  .dim = 3
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
  .type = type0
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
  }
}
```

