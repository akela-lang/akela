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

## Test
(error expected left parenthesis)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo
```

```cent
Error {
  .message = "expected left parenthesis"
  .line = 423
  .col = 1
}
```

## Test
(error expected right parenthesis)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo(
```

```cent
Error {
  .message = "expected right parenthesis"
  .line = 444
  .col = 1
}
```

## Test
(error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo()
```

```cent
Error {
  .message = "expected end"
  .line = 465
  .col = 1
}
```
## Test
return type does not match (error)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo()->Int32
    return true
end
```

```cent
Error {
  .message = "returned type does not match function return type"
  .line = 485
  .col = 5
}
```

## Test
return outside of function (error)

```cent
Test {
  .has_error = true
}
```

```akela
return true
```

```cent
Error {
  .message = "return statement outside of function"
  .line = 507
  .col = 1
}
```

## Test
newline

```cent
Test {
}
```

```akela
fn
foo
(
a: Int32,
b: Int32
)->
Int32 1 end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .name = "foo"
  .input = Input {
    TypeParam::Regular {
      .name = "a"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "b"
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
            .value = "a"
          }
          Ast::Type {
            .type = type1
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "b"
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
      Ast::Number {
        .value = "1"
        .type = type1
      }
    }
  }
}
```

## Test
return

```cent
Test {
}
```

```akela
fn foo()->Int32 return 1 end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .name = "foo"
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
      }
      Ast::Dret {
        Ast::Type {
          .type = type1
        }
      }
    }
    Ast::Stmts {
      .type = type1
      Ast::Return {
        .type = type1
        Ast::Number {
          .value = "1"
          .type = type1
        }
      }
    }
  }
}
```

## Test
return (error no value)

```cent
Test {
  .has_error = true
}
```

```akela
fn bar() end
fn foo()->Int32
  return bar()
end
```

```cent
Error {
  .message = "return expression has no value"
  .line = 675
  .col = 10
}
```

## Test
anonymous function

```cent
Test {
}
```

```akela
const a: fn(Int32, Int32, Int32) = fn (x: Int32, y: Int32, z: Int32)
  x + y + z
end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .input = Input {
    TypeParam::Regular {
      .name = ""
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = ""
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = ""
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
  }
}
const type1 = Type::Function {
  .name = "__anonymous_function_0"
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
}
const type2 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Const {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Function {
      .type = type1
      Ast::Prototype {
        Ast::Id {
          .value = "__anonymous_function_0"
        }
        Ast::Dseq {
          Ast::Declaration {
            Ast::Id {
              .value = "x"
            }
            Ast::Type {
              .type = type2
            }
          }
          Ast::Declaration {
            Ast::Id {
              .value = "y"
            }
            Ast::Type {
              .type = type2
            }
          }
          Ast::Declaration {
            Ast::Id {
              .value = "z"
            }
            Ast::Type {
              .type = type2
            }
          }
        }
        Ast::Dret {
        }
      }
      Ast::Stmts {
        .type = type2
        Ast::Plus {
          .type = type2
          Ast::Plus {
            .type = type2
            Ast::Id {
              .value = "x"
              .type = type2
            }
            Ast::Id {
              .value = "y"
              .type = type2
            }
          }
          Ast::Id {
            .value = "z"
            .type = type2
          }
        }
      }
    }
  }
}
```

## Test
anonymous function 2

```cent
Test {
}
```

```akela
var a: fn (Int32, Int32, Int32)->Int32 = fn(x: Int32, y: Int32, z: Int32)->Int32
  x + y + z
end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .input = Input {
    TypeParam::Regular {
      .name = ""
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = ""
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = ""
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
const type1 = Type::Function {
  .name = "__anonymous_function_0"
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
const type2 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Var {
    Ast::Id {
      .value = "a"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Function {
      .type = type1
      Ast::Prototype {
        Ast::Id {
          .value = "__anonymous_function_0"
        }
        Ast::Dseq {
          Ast::Declaration {
            Ast::Id {
              .value = "x"
            }
            Ast::Type {
              .type = type2
            }
          }
          Ast::Declaration {
            Ast::Id {
              .value = "y"
            }
            Ast::Type {
              .type = type2
            }
          }
          Ast::Declaration {
            Ast::Id {
              .value = "z"
            }
            Ast::Type {
              .type = type2
            }
          }
        }
        Ast::Dret {
          Ast::Type {
            .type = type2
          }
        }
      }
      Ast::Stmts {
        .type = type2
        Ast::Plus {
          .type = type2
          Ast::Plus {
            .type = type2
            Ast::Id {
              .value = "x"
              .type = type2
            }
            Ast::Id {
              .value = "y"
              .type = type2
            }
          }
          Ast::Id {
            .value = "z"
            .type = type2
          }
        }
      }
    }
  }
}
```

## Test
assignment (type error)

```cent
Test {
  .has_error = true
}
```

```akela
const a: fn(Bool) = fn(x: Int32) end
```

```cent
Error {
  .message = "values in assignment are not compatible"
  .line = 988
  .col = 21
}
```

## Test
return (error type does not match)

```cent
Test {
  .has_error = true
}
```

```akela
const f: fn()->Int32 = fn()->Int32 true end
```

```cent
Error {
  .message = "returned type does not match function return type"
  .line = 1009
  .col = 30
}
```

## Test
right paren (error)

```cent
Test {
  .has_error = true
}
```

```akela
fn (
```

```cent
Error {
  .message = "expected right parenthesis"
  .line = 1031
  .col = 1
}
```

## Test
(error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
fn ()
```

```cent
Error {
  .message = "expected end"
  .line = 1052
  .col = 1
}
```

## Test
function proto

```cent
Test {
}
```

```akela
var foo: fn (Int32)->Int32 = fn (a: Int32)->Int32
    a + 1
end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .input = Input {
    TypeParam::Regular {
      .name = ""
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
const type1 = Type::Function {
  .name = "__anonymous_function_0"
  .input = Input {
    TypeParam::Regular {
      .name = "a"
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
const type2 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Var {
    Ast::Id {
      .value = "foo"
    }
    Ast::Type {
      .type = type0
    }
    Ast::Function {
      .type = type1
      Ast::Prototype {
        Ast::Id {
          .value = "__anonymous_function_0"
        }
        Ast::Dseq {
          Ast::Declaration {
            Ast::Id {
              .value = "a"
            }
            Ast::Type {
              .type = type2
            }
          }
        }
        Ast::Dret {
          Ast::Type {
            .type = type2
          }
        }
      }
      Ast::Stmts {
        .type = type2
        Ast::Plus {
          .type = type2
          Ast::Id {
            .value = "a"
            .type = type2
          }
          Ast::Number {
            .value = "1"
            .type = type2
          }
        }
      }
    }
  }
}
```

## Test
type check (error not compatible)

```cent
Test {
  .has_error = true
}
```

```akela
const foo: fn (a: Bool)->Int32 = fn (a: Int32)->Int32
    a + 1
end
```

```cent
Error {
  .message = "values in assignment are not compatible"
  .line = 1172
  .col = 1
}
```

## Test
assign (error type)

```cent
Test {
  .has_error = true
}
```

```akela
const a: Int32 = true
```

```cent
Error {
  .message = "values in assignment are not compatible"
  .line = 1195
  .col = 18
}
```

## Test
call

```cent
Test {
}
```

```akela
fn foo() 1 end; foo()
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
      Ast::Number {
        .value = "1"
        .type = type1
      }
    }
  }
  Ast::Call {
    Ast::Id {
      .value = "foo"
      .type = type0
    }
    Ast::Cseq {
    }
  }
}
```

## Test
call return type 2

```cent
Test {
}
```

```akela
fn foo()->Int32 1 end
foo() + 2
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Function {
  .name = "foo"
  .output = Type::Integer {
    .name = "Int32"
    .bit_count = 32
  }
}
Ast::Stmts {
  .type = type0
  Ast::Function {
    .type = type1
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
      }
      Ast::Dret {
        Ast::Type {
          .type = type0
        }
      }
    }
    Ast::Stmts {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type0
      }
    }
  }
  Ast::Plus {
    .type = type0
    Ast::Call {
      .type = type0
      Ast::Id {
        .value = "foo"
        .type = type1
      }
      Ast::Cseq {
      }
    }
    Ast::Number {
      .value = "2"
      .type = type0
    }
  }
}
```

## Test
return (error 2)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo()->Bool true end
foo() + 2
```

```cent
Error {
  .message = "addition on non-numeric operand"
  .line = 1338
  .col = 1
}
```

## Test
call 2

```cent
Test {
}
```

```akela
fn foo(arg1: Int32) arg1 end
foo(2)
```

```cent
use lib::base::*
const type0 = Type::Function {
  .name = "foo"
  .input = Input {
    TypeParam::Regular {
      .name = "arg1"
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
  Ast::Function {
    .type = type0
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
        Ast::Declaration {
          Ast::Id {
            .value = "arg1"
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
      Ast::Id {
        .value = "arg1"
        .type = type1
      }
    }
  }
  Ast::Call {
    Ast::Id {
      .value = "foo"
      .type = type0
    }
    Ast::Cseq {
      Ast::Number {
        .value = "2"
        .type = type1
      }
    }
  }
}
```

## Test
call 3

```cent
Test {
}
```

```akela
fn foo(arg1: Int32, arg2: Int32)->Int32
  1
end
const x: Int32 = 1
const y: Int32 = 2
foo(x,y)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Function {
  .name = "foo"
  .input = Input {
    TypeParam::Regular {
      .name = "arg1"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "arg2"
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
Ast::Stmts {
  .type = type0
  Ast::Function {
    .type = type1
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
        Ast::Declaration {
          Ast::Id {
            .value = "arg1"
          }
          Ast::Type {
            .type = type0
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "arg2"
          }
          Ast::Type {
            .type = type0
          }
        }
      }
      Ast::Dret {
        Ast::Type {
          .type = type0
        }
      }
    }
    Ast::Stmts {
      .type = type0
      Ast::Number {
        .value = "1"
        .type = type0
      }
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
      .value = "1"
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
      .value = "2"
      .type = type0
    }
  }
  Ast::Call {
    .type = type0
    Ast::Id {
      .value = "foo"
      .type = type1
    }
    Ast::Cseq {
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
```

## Test
call 4

```cent
Test {
}
```

```akela
fn foo(arg0: Int32, arg1: Int32, arg2: Int32)->Int32
    100
end
const x: Int32 = 1
const y: Int32 = 2
foo(x, y, 1)
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type1 = Type::Function {
  .name = "foo"
  .input = Input {
    TypeParam::Regular {
      .name = "arg0"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "arg1"
      .type = Type::Integer {
        .name = "Int32"
        .bit_count = 32
      }
    }
    TypeParam::Regular {
      .name = "arg2"
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
Ast::Stmts {
  .type = type0
  Ast::Function {
    .type = type1
    Ast::Prototype {
      Ast::Id {
        .value = "foo"
      }
      Ast::Dseq {
        Ast::Declaration {
          Ast::Id {
            .value = "arg0"
          }
          Ast::Type {
            .type = type0
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "arg1"
          }
          Ast::Type {
            .type = type0
          }
        }
        Ast::Declaration {
          Ast::Id {
            .value = "arg2"
          }
          Ast::Type {
            .type = type0
          }
        }
      }
      Ast::Dret {
        Ast::Type {
          .type = type0
        }
      }
    }
    Ast::Stmts {
      .type = type0
      Ast::Number {
        .value = "100"
        .type = type0
      }
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
      .value = "1"
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
      .value = "2"
      .type = type0
    }
  }
  Ast::Call {
    .type = type0
    Ast::Id {
      .value = "foo"
      .type = type1
    }
    Ast::Cseq {
      Ast::Id {
        .value = "x"
        .type = type0
      }
      Ast::Id {
        .value = "y"
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