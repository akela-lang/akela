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