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
  .tu = TypeUse::TypeDef {
    .td = TypeDef::Integer {
      .name = "Int32"
      .bit_count = 32
      .is_signed = true
    }
  }
  Ast::Const {
    Ast::LetLseq {
      Ast::Id {
        .value = "a"
      }
    }
    Ast::Type {
      .tu = TypeUse::TypeDef {
        .td = TypeDef::Integer {
          .name = "Int32"
          .bit_count = 32
          .is_signed = true
        }
      }
    }
    Ast::LetRseq {
      Ast::Number {
        .value = "105"
        .tu = TypeUse::TypeDef {
          .td = TypeDef::Integer {
            .name = "Int32"
            .bit_count = 32
            .is_signed = true
          }
        }
      }
    }
  }
  Ast::Id {
    .value = "a"
    .tu = TypeUse::TypeDef {
      .td = TypeDef::Integer {
        .name = "Int32"
        .bit_count = 32
        .is_signed = true
      }
    }
  }
}
```

## Test
Double constant

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
const a: Real64 = 1.5
a
```

```cent
use lib::base::*
Ast::Stmts {
  .tu = TypeUse::TypeDef {
    .td = TypeDef::Real {
      .name = "Real64"
      .bit_count = 64
      .is_signed = false
    }
  }
  Ast::Const {
    Ast::LetLseq {
      Ast::Id {
        .value = "a"
      }
    }
    Ast::Type {
      .tu = TypeUse::TypeDef {
        .td = TypeDef::Real {
          .name = "Real64"
          .bit_count = 64
          .is_signed = false
        }
      }
    }
    Ast::LetRseq {
      Ast::Number {
        .value = "1.5"
        .tu = TypeUse::TypeDef {
          .td = TypeDef::Real {
            .name = "Real64"
            .bit_count = 64
            .is_signed = false
          }
        }
      }
    }
  }
  Ast::Id {
    .value = "a"
    .tu = TypeUse::TypeDef {
      .td = TypeDef::Real {
        .name = "Real64"
        .bit_count = 64
        .is_signed = false
      }
    }
  }
}
```
