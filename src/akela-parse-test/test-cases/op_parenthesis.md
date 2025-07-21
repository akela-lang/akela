# Test Suite
operator precedence

```cent
TestSuite {
}
```

## Test
paren add

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
const speed: Int32 = 105
(speed + 1)
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
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "105"
      .type = type0
    }
  }
  Ast::Parenthesis {
    .type = type0
    Ast::Plus {
      .type = type0
      Ast::Id {
        .value = "speed"
        .type = type0
      }
      Ast::Number {
        .value = "1"
        .type = type0
      }
    }
  }
}
```

## Test
paren add 2

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
const speed: Int32 = 105
(speed) + 1
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
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "105"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Parenthesis {
      .type = type0
      Ast::Id {
        .value = "speed"
        .type = type0
      }
    }
    Ast::Number {
      .value = "1"
      .type = type0
    }
  }
}
```

## Test
paren add 3

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
const speed: Int32 = 105
speed + (1)
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
      .value = "speed"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Number {
      .value = "105"
      .type = type0
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Id {
      .value = "speed"
      .type = type0
    }
    Ast::Parenthesis {
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
paren add add

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
1 + (2 + 3)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Plus {
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Parenthesis {
      .type = type0
      Ast::Plus {
        .type = type0
        Ast::Number {
          .value = "2"
          .type = type0
        }
        Ast::Number {
          .value = "3"
          .type = type0
        }
      }
    }
  }
}
```

## Test
paren add add 2

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
(1 + 2) + 3
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Plus {
    .type = type0
    Ast::Parenthesis {
      .type = type0
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
    Ast::Number {
      .value = "3"
      .type = type0
    }
  }
}
```

## Test
paren mult

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
(5 * 2)
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
    Ast::Mult {
      .type = type0
      Ast::Number {
        .value = "5"
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
paren mult mult

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
1 * (2 * 3)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Mult {
    .type = type0
    Ast::Number {
      .value = "1"
      .type = type0
    }
    Ast::Parenthesis {
      .type = type0
      Ast::Mult {
        .type = type0
        Ast::Number {
          .value = "2"
          .type = type0
        }
        Ast::Number {
          .value = "3"
          .type = type0
        }
      }
    }
  }
}
```

## Test
paren mult mult 2

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
(1 * 2) * 3
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Mult {
    .type = type0
    Ast::Parenthesis {
      .type = type0
      Ast::Mult {
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
    Ast::Number {
      .value = "3"
      .type = type0
    }
  }
}
```