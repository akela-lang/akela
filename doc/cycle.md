cycle
```cent
const foo = Foo {
  .@child_of(foo)
}
```

ok
```cent
const foo = Foo {
  const bar = Bar {
    .@child_of(foo)
  }
}
```

cycle
```cent
const foo = Foo {
  .@child_of(bar)
  const bar = Bar {
  }
  bar
}
```

cycle
```cent
const a = A {
  .@child_of(b)
}
const b = B {
  .@child_of(a)
}
```

cycle
```cent
const a = A {
  b
}
const b = B {
  a
}
```