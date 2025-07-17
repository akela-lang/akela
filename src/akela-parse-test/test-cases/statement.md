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