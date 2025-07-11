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

## Test
multiple inputs

```cent
Test {
}
```

```akela
fn foo(x: Int32, y: Int32)
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
    TypeParam::Regular {
      .name = "y"
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
        Ast::Declaration {
          Ast::Id {
            .value = "y"
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

## Test
three inputs

```cent
Test {
}
```

```akela
fn foo(x: Int32, y: Int32, z: Int32)->Int32
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
    TypeParam::Regular {
      .name = "y"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "z"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
  }
  .output = Type::Integer {
    .name = "Int32"
    .bit_count = 32
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
        Ast::Declaration {
          Ast::Id {
            .value = "y"
          }
          Ast::Type {
            .type = type1
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "z"
          }
          Ast::Type {
            .type = type1
          }
        }
      }
      Ast::Dret {
        Ast::Type {
          .type = type1
        }
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

## Test
return type (error)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo(x: Int32)->Int32
    true    
end
```

```cent
Error {
  .message = "returned type does not match function return type"
  .line = 399
  .col = 19
}
```