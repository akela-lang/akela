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