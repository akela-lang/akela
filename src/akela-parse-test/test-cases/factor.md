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

## Test
boolean false

```cent
Test {
}
```

```akela
false
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Boolean {
    .value = "false"
    .type = type0
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
    Ast::Id {
      .value = "_x10"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
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
    Ast::Id {
      .value = "a2"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
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
    Ast::Id {
      .value = "αβγ"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
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
    .line = 311
    .col = 7
  }
}
```

## Test
sign negative

```cent
Test {
}
```

```akela
-30
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
      .value = "30"
      .type = type0
    }
  }
}
```

## Test
sign positive

```cent
Test {
}
```

```akela
+30
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
    Ast::Plus {
    }
    Ast::Number {
      .value = "30"
      .type = type0
    }
  }
}
```