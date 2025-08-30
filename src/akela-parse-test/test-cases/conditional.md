# Test Suite
conditional

```cent
TestSuite {
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
          .left = Ast::Number {
            .value = "10"
            .type = type0
          }
          .right = Ast::Number {
            .value = "20"
            .type = type0
          }
          .type = type0
        }
        Ast::Mult {
          .left = Ast::Id {
            .value = "x"
            .type = type0
          }
          .right = Ast::Id {
            .value = "y"
            .type = type0
          }
          .type = type0
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
          .left = Ast::Number {
            .value = "10"
            .type = type0
          }
          .right = Ast::Number {
            .value = "20"
            .type = type0
          }
          .type = type0
        }
        Ast::Mult {
          .left = Ast::Id {
            .value = "x"
            .type = type0
          }
          .right = Ast::Id {
            .value = "y"
            .type = type0
          }
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
          .left = Ast::Number {
            .value = "10"
            .type = type0
          }
          .right = Ast::Number {
            .value = "20"
            .type = type0
          }
          .type = type0
        }
        Ast::Mult {
          .left = Ast::Id {
            .value = "x"
            .type = type0
          }
          .right = Ast::Id {
            .value = "y"
            .type = type0
          }
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

## Test
if elseif else

```cent
Test {
}
```

```akela
const x: Int32 = 0
const y: Int32 = 1
if false
    10
elseif false
    20
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
    Ast::ConditionalBranch {
      .type = type0
      Ast::Boolean {
        .value = "false"
        .type = type1
      }
      Ast::Stmts {
        .type = type0
        Ast::Number {
          .value = "20"
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

## Test
if (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
if end
```

```cent
Error {
  .message = "expected condition after if"
  .line = 1
  .col = 4
}
```

## Test
if (error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
if true
```

```cent
Error {
  .message = "expected end"
  .line = 2
  .col = 1
}
```

## Test
elseif (error expected expression)

```cent
Test {
  .has_error = true
}
```

```akela
if true elseif end
```

```cent
Error {
  .message = "expected condition after elseif"
  .line = 1
  .col = 16
}
```
