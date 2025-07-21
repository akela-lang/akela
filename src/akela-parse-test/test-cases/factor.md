# Test Suite
factor

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
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
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
    Ast::Id {
      .value = "x"
    }
    Ast::Type {
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
newline not

```cent
Test {
}
```

```akela
!
true
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Not {
    .type = type0
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
}
```

## Test
newline sign

```cent
Test {
}
```

```akela
-
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

## Test
array element const

```cent
Test {
}
```

```akela
const a: [4 const]Int32 = [1, 2, 3, 4]
a[0]
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Array {
  .is_const = true
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
array element const (error)

```cent
Test {
  .has_error = true
}
```

```akela
const a: [4 const]Int32 = [1, 2, 3, 4]
a[0] = 10
```

```cent
Error {
  .message = "immutable variable changed in assignment"
  .line = 2
  .col = 1
}
```