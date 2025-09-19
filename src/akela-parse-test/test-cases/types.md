# Test Suite
types

```cent
TestSuite {
}
```

## Test
(error missing declaration)

```cent
Test {
  .has_error = true
}
```

```akela
x + 1
```

```cent
Error {
  .message = "variable not declared: x"
  .line = 1
  .col = 1
}
```

## Test
(error missing declaration 2)

```cent
Test {
  .has_error = true
}
```

```akela
foo() + 1
```

```cent
Error {
  .message = "variable not declared: foo"
  .line = 1
  .col = 1
}
```

## Test
(error missing declaration 3)

```cent
Test {
  .has_error = true
}
```

```akela
x = fn() end
```

```cent
Error {
  .message = "variable not declared: x"
  .line = 1
  .col = 1
}
```

## Test
(error type not defined)

```cent
Test {
  .has_error = true
}
```

```akela
const x: SuperInt
x + 1
```

```cent
Error {
  .message = "type not defined: SuperInt"
  .line = 1
  .col = 10
}
```

## Test
function dseq comma (error)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo(a: Int32,) end
```

```cent
Error {
  .message = "expected declaration after comma"
  .line = 1
  .col = 17
}
```

## Test
declaration (expected colon)

```cent
Test {
  .has_error = true
}
```

```akela
const a
```

```cent
Error {
  .message = "expected colon after variable(s)"
  .line = 2
  .col = 1
}
```

## Test
(error expected type)

```cent
Test {
  .has_error = true
}
```

```akela
const a:
```

```cent
Error {
  .message = "expected type identifier or fn"
  .line = 2
  .col = 1
}
```

## Test
(error type not defined)

```cent
Test {
  .has_error = true
}
```

```akela
const a: Foo
```

```cent
Error {
  .message = "type not defined: Foo"
  .line = 1
  .col = 10
}
```

## Test
(error not a type)

```cent
Test {
  .has_error = true
}
```

```akela
const foo: Int32
const a: foo
```

```cent
Error {
  .message = "identifier is not a type: foo"
  .line = 2
  .col = 10
}
```

## Test
(error return type)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo()->Int32 true end
```

```cent
Error {
  .message = "returned type does not match function return type"
  .line = 1
  .col = 11
}
```

## Test
function call (error param)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo(a: Int32) true end; foo(true)
```

```cent
Error {
  .message = "parameter and arguments types do not match"
  .line = 1
  .col = 32
}
```

## Test
function (error param no value)

```cent
Test {
  .has_error = true
}
```

```akela
fn foo(a: Int32) true end; foo(foo(1))
```

```cent
Error {
  .message = "argument expression has no value"
  .line = 1
  .col = 32
}
```

## Test
newline types

```cent
Test {
}
```

```akela
const a
:
Int32
```

```cent
use lib::base::*
const type0 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Const {
    .id = Ast::Id {
      .value = "a"
    }
    .type_node = Ast::Type {
      .type = type0
    }
  }
}
```