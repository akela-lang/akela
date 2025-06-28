# Test Suite
Define constant and use

```cent
use lib::base::*
TestSuite {
  .solo = false
  .mute = false
}
```

## Test
Integer constant

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
const a: Int32 = 105
a
```

```cent
use lib::base::*
Ast::Stmts {
  .tu = TypeDef::Integer {
    .name = "Int32"
    .bit_count = 32
  }
  Ast::Const {
    Ast::LetLseq {
      Ast::Id {
        .value = "a"
      }
    }
    Ast::Type {
      .tu = TypeDef::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    Ast::LetRseq {
      Ast::Number {
        .value = "105"
        .tu = TypeDef::Integer {
          .name = "Int32"
          .bit_count = 32
        }
      }
    }
  }
  Ast::Id {
    .value = "a"
    .tu = TypeDef::Integer {
      .name = "Int32"
      .bit_count = 32
    }
  }
}
```

## Test
Double constant

```cent
use lib::base::*
Test {
  .mute = false
  .solo = true
  .snapshot = false
  .has_error = false
}
```

```akela
const a: Real64 = 1.5
a
```

```cent
use lib::base::*
Ast::Stmts {
  .tu = TypeDef::Real {
    .name = "Real64"
    .bit_count = 64
  }
  Ast::Const {
    Ast::LetLseq {
      Ast::Id {
        .value = "a"
      }
    }
    Ast::Type {
      .tu = TypeDef::Real {
        .name = "Real64"
        .bit_count = 64
      }
    }
    Ast::LetRseq {
      Ast::Number {
        .value = "1.5"
        .tu = TypeDef::Real {
          .name = "Real64"
          .bit_count = 64
        }
      }
    }
  }
  Ast::Id {
    .value = "a"
    .tu = TypeDef::Real {
      .name = "Real64"
      .bit_count = 64
    }
  }
}
```
