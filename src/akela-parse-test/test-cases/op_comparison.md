# Test Suite
comparison operator

```cent
TestSuite {
}
```

## Test
comparison

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
var count: Int32 = 5
count == 10
count != 11.1
count <= 12
count >= 13
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Real {
  .name = "Real64"
  .bit_count = 64
}
Ast::Stmts {
  .type = type0
  Ast::Var {
    Ast::Id {
      .value = "count"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "5"
      .type = type0
    }
  }
  Ast::Equality {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "10"
      .type = type0
    }
  }
  Ast::NotEqual {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "11.1"
      .type = type1
    }
  }
  Ast::LessThanOrEqual {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "12"
      .type = type0
    }
  }
  Ast::GreaterThanOrEqual {
    .type = type0
    Ast::Id {
      .value = "count"
      .type = type0
    }
    Ast::Number {
      .value = "13"
      .type = type0
    }
  }
}
```

## Test
comparison identity

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
true == true
true != true
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Equality {
    .type = type0
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
  Ast::NotEqual {
    .type = type0
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
}
```

## Test
comparison (error no term)

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
100 <
```

```cent
Errors {
  Error {
    .message = "expected term after comparison operator"
    .line = 2
    .col = 1
  }
}
```

## Test
comparison (error left no value)

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
foo() < 100
```

```cent
Errors {
  Error {
    .message = "operand has no value"
    .line = 2
    .col = 1
  }
}
```

## Test
comparison (error right no value)

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
100 < foo()
```

```cent
Errors {
  Error {
    .message = "operand has no value"
    .line = 2
    .col = 7
  }
}
```

## Test
comparison (error left not numeric)

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
true < 100
```

```cent
Errors {
  Error {
    .message = "comparison operand is not numeric"
    .line = 1
    .col = 1
  }
}
```

## Test
comparison (error right not numeric)

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
100 < true
```

```cent
Errors {
  Error {
    .message = "comparison operand is not numeric"
    .line = 1
    .col = 7
  }
}
```

## Test
comparison operator newline

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
1 <
2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::LessThan {
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
```
