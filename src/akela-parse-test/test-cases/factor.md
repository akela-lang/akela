# Test Suite
factor

## Test
number integer

```cent
Test {
}
```

```akela
32
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Number {
    .value = "32"
    .type = type0
  }
}
```

## Test
number real

```cent
Test {
}
```

```akela
5.0e0
```

```cent
use lib::base::*
const type0 = Type::Real {
  .name = "Real64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Number {
    .value = "5.0e0"
    .type = type0
  }
}
```

## Test
string

```cent
Test {
}
```

```akela
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
Ast::Stmts {
  .type = type0
  Ast::String {
    .value = "hello"
    .type = type0
  }
}
```

## Test
boolean true

```cent
Test {
}
```

```akela
true
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Boolean {
    .value = "true"
    .type = type0
  }
}
```