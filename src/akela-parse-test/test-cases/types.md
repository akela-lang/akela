# Test Suite
types

```cent
TestSuite {
}
```

## Test
(error missing declaration)

```cent
Test {
  .has_error = true
}
```

```akela
x + 1
```

```cent
Error {
  .message = "variable not declared: x"
  .line = 19
  .col = 1
}
```

## Test
(error missing declaration 2)

```cent
Test {
  .has_error = true
}
```

```akela
foo() + 1
```

```cent
Error {
  .message = "variable not declared: foo"
  .line = 40
  .col = 1
}
```

## Test
(error missing declaration 3)

```cent
Test {
  .has_error = true
}
```

```akela
x = fn() end
```

```cent
Error {
  .message = "variable not declared: x"
  .line = 61
  .col = 1
}
```

## Test
(error type not defined)

```cent
Test {
  .has_error = true
}
```

```akela
const x: SuperInt
x + 1
```

```cent
Error {
  .message = "type not defined: SuperInt"
  .line = 82
  .col = 10
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
function dseq comma (error)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo(a: Int32,) end
```

```cent
Error {
  .message = "expected declaration after comma"
  .line = 202
  .col = 17
}
```

## Test
declaration (expected colon)

```cent
Test {
  .has_error = true
}
```

```akela
const a
```

```cent
Error {
  .message = "expected colon after variable(s)"
  .line = 224
  .col = 1
}
```

## Test
(error expected type)

```cent
Test {
  .has_error = true
}
```

```akela
const a:
```

```cent
Error {
  .message = "expected type identifier or fn"
  .line = 245
  .col = 1
}
```