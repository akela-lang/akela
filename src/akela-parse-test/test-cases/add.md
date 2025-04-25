# Test Suite
Addition

## Test
Addition boolean

```cent
Test {
  .mute = false
  .solo = false
  .snapshot = false
  .has_error = true
}
```

```akela
5 + true
```

```cent
Errors {
  Error {
    .message = "addition on non-numeric operand"
    .line = 17
    .col = 5
  }
}
```