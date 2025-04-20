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
  .snapshot = true
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
  Ast::Const {
    Ast::LetLseq {
      Ast::Id {
        .value = "a"
      }
    }
    Ast::Type {
      .tu = TypeUse {
        .td = Int32
      }
    }
    Ast::LetRseq {
      Ast::Number {
        .value = "105"
        .tu = TypeUse {
          .td = Int32
        }
      }
    }
  }
  Ast::Id {
    .value = "a"
    .tu = TypeUse {
      .td = Int32
    }
  }
  .tu = TypeUse {
    .td = Int32
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
  Ast::Const {
    Ast::LetLseq {
      Ast::Id {
        .value = "a"
      }
    }
    Ast::Type {
      .tu = TypeUse {
        .td = Real64
      }
    }
    Ast::LetRseq {
      Ast::Number {
        .value = "1.5"
        .tu = TypeUse {
          .td = Real64
        }
      }
    }
  }
  Ast::Id {
    .value = "a"
    .tu = TypeUse {
      .td = Real64
    }
  }
  .tu = TypeUse {
    .td = Real64
  }
}
```