# Test Suite
boolean operator

```cent
TestSuite {
}
```

## Test
and

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
const a: Bool = true
const b: Bool = false
a && b
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "b"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
  Ast::And {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type0
    }
    Ast::Id {
      .value = "b"
      .type = type0
    }
  }
}
```

## Test
or

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
const a: Bool = true
const b: Bool = false
a || b
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "b"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
  Ast::Or {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type0
    }
    Ast::Id {
      .value = "b"
      .type = type0
    }
  }
}
```

## Test
or or

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
true || true || false
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Or {
    .type = type0
    Ast::Or {
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
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
}
```

## Test
boolean (error expected term)

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
true &&
```

```cent
Errors {
  Error {
    .message = "expected term after && or ||"
    .line = 2
    .col = 1
  }
}
```

## Test
boolean (error left no value)

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
foo() && true
```

```cent
Errors {
  Error {
    .message = "left-side operand of boolean operator has no type"
    .line = 2
    .col = 1
  }
}
```

## Test
boolean (error right no value)

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
true && foo()
```

```cent
Errors {
  Error {
    .message = "operand of boolean operator has no type"
    .line = 2
    .col = 9
  }
}
```

## Test
boolean (error left not boolean)

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
1 && true
```

```cent
Errors {
  Error {
    .message = "left-side expression of boolean operator is not boolean"
    .line = 1
    .col = 1
  }
}
```

## Test
boolean (error right not boolean)

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
true && 1
```

```cent
Errors {
  Error {
    .message = "expression of boolean operator is not boolean"
    .line = 1
    .col = 9
  }
}
```

## Test
boolean operator newline

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
true &&
false
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::And {
    .type = type0
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
    Ast::Boolean {
      .value = "false"
      .type = type0
    }
  }
}
```

## Test
id not

```cent
Test {
}
```

```akela
const a: Bool = true
!a
```

```cent
use lib::base::*
const type0 = Type::Boolean {
  .name = "Bool"
}
Ast::Stmts {
  .type = type0
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Boolean {
      .value = "true"
      .type = type0
    }
  }
  Ast::Not {
    .type = type0
    Ast::Id {
      .value = "a"
      .type = type0
    }
  }
}
```

## Test
not literal

```cent
Test {
}
```

```akela
!true
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
not (error not value)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo() end
!foo()
```

```cent
Errors {
  Error {
    .message = "! operator used on parse_factor with no value"
    .line = 2
    .col = 1
  }
}
```

## Test
not (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
!
```

```cent
Errors {
  Error {
    .message = "expected parse_factor after !"
    .line = 2
    .col = 1
  }
}
```

## Test
not (error not an expression)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo() end
!foo()
```

```cent
Error {
  .message = "! operator used on parse_factor with no value"
  .line = 2
  .col = 1
}
```

## Test
not (error not boolean)

```cent
Test {
  .has_error = true
}
```

```akela
!1
```

```cent
Error {
  .message = "not operator used on non-boolean"
  .line = 1
  .col = 1
}
```
