# Test Suite
loop

```cent
TestSuite {
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
    .cond = Ast::Boolean {
      .value = "true"
      .type = type0
    }
    .body = Ast::Stmts {
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
  .line = 1
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
  .line = 2
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
    .dec = Ast::Declaration {
      Ast::Id {
        .value = "i"
      }
      Ast::Type {
        .type = type0
      }
    }
    .start = Ast::Number {
      .value = "0"
      .type = type0
    }
    .end = Ast::Number {
      .value = "10"
      .type = type0
    }
    .body = Ast::Stmts {
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
  .line = 3
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
  .line = 2
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
  .line = 2
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
  .line = 3
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
  .line = 3
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
  .line = 3
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
  .line = 3
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
  .line = 3
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
  .line = 2
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
  .line = 1
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
  .line = 2
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
  .line = 1
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
  .line = 2
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
    .dec = Ast::Declaration {
      Ast::Id {
        .value = "i"
      }
      Ast::Type {
        .type = type0
      }
    }
    .start = Ast::Number {
      .value = "0"
      .type = type0
    }
    .end = Ast::Number {
      .value = "10"
      .type = type0
    }
    .body = Ast::Stmts {
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
