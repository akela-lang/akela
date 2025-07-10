# Test Suite
function

```cent
TestSuite {
}
```

## Test
function no inputs no outputs

```cent
Test {
}
```

```akela
fn foo()
  const x: Int32 = 10
  x+1
  5+4
end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .name = "foo"
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Function {
    .type = type0
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
      }
      Ast::Dret {
      }
    }
    Ast::Stmts {
      .type = type1
      Ast::Const {
        Ast::Id {
          .value = "x"
        }
        Ast::Type {
          .type = type1
        }
        Ast::Number {
          .value = "10"
          .type = type1
        }
      }
      Ast::Plus {
        .type = type1
        Ast::Id {
          .value = "x"
          .type = type1
        }
        Ast::Number {
          .value = "1"
          .type = type1
        }
      }
      Ast::Plus {
        .type = type1
        Ast::Number {
          .value = "5"
          .type = type1
        }
        Ast::Number {
          .value = "4"
          .type = type1
        }
      }
    }
  }
}
```

## Test
input

```cent
Test {
}
```

```akela
fn foo(x: Int32)
  x+1
  5+4
end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .name = "foo"
  .input = Input {
    TypeParam::Regular {
      .name = "x"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
  }
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  .type = type0
  Ast::Function {
    .type = type0
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
        Ast::Declaration {
          Ast::Id {
            .value = "x"
          }
          Ast::Type {
            .type = type1
          }
        }
      }
      Ast::Dret {
      }
    }
    Ast::Stmts {
      .type = type1
      Ast::Plus {
        .type = type1
        Ast::Id {
          .value = "x"
          .type = type1
        }
        Ast::Number {
          .value = "1"
          .type = type1
        }
      }
      Ast::Plus {
        .type = type1
        Ast::Number {
          .value = "5"
          .type = type1
        }
        Ast::Number {
          .value = "4"
          .type = type1
        }
      }
    }
  }
}
```