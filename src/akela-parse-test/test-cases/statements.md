# Test Suite
statements

```cent
TestSuite {
}
```

## Test
blank

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
```

```cent
use lib::base::*
Ast::Stmts {
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
    .left = Ast::Id {
      .value = "i"
      .type = type0
    }
    .right = Ast::Number {
      .value = "1"
      .type = type0
    }
    .type = type0
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
    .left = Ast::Id {
      .value = "x"
      .type = type0
    }
    .right = Ast::Number {
      .value = "1"
      .type = type0
    }
    .type = type0
  }
  Ast::Minus {
    .left = Ast::Number {
      .value = "5"
      .type = type0
    }
    .right = Ast::Number {
      .value = "4"
      .type = type0
    }
    .type = type0
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
    .left = Ast::Id {
      .value = "x"
      .type = type0
    }
    .right = Ast::Number {
      .value = "1"
      .type = type0
    }
    .type = type0
  }
  Ast::Minus {
    .left = Ast::Number {
      .value = "5"
      .type = type0
    }
    .right = Ast::Number {
      .value = "4"
      .type = type0
    }
    .type = type0
  }
  Ast::Plus {
    .left = Ast::Id {
      .value = "y"
      .type = type0
    }
    .right = Ast::Id {
      .value = "z"
      .type = type0
    }
    .type = type0
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
    .left = Ast::Id {
      .value = "x"
      .type = type0
    }
    .right = Ast::Number {
      .value = "1"
      .type = type0
    }
    .type = type0
  }
  Ast::Minus {
    .left = Ast::Number {
      .value = "5"
      .type = type0
    }
    .right = Ast::Number {
      .value = "4"
      .type = type0
    }
    .type = type0
  }
  Ast::Plus {
    .left = Ast::Id {
      .value = "y"
      .type = type0
    }
    .right = Ast::Id {
      .value = "z"
      .type = type0
    }
    .type = type0
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