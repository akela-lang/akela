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
  .line = 19
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
  .line = 40
  .col = 1
}
```