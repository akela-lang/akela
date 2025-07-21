# Test Suite
factor

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