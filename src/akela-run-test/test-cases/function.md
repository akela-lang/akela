# Test Suite
function

```cent
TestSuite {
}
```

## Test
3 args, no return

```akela
fn foo(a: Int32, b: Int32, c: Int32)
end
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define ptr @__top_level() {
entry:
  ret ptr @foo
}

define void @foo(i32 %0, i32 %1, i32 %2) {
body:
  %a = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 %1, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 %2, ptr %c, align 4
  ret void
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Void
  }
} 
```

## Test
return Int32

```akela
fn foo(a: Int32, b: Int32, c: Int32)->Int32
    a + b + c
end
foo(1, 2, 3)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i32 @__top_level() {
entry:
  %0 = call i32 @foo(i32 1, i32 2, i32 3)
  ret i32 %0
}

define i32 @foo(i32 %0, i32 %1, i32 %2) {
body:
  %a = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 %1, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 %2, ptr %c, align 4
  %3 = load i32, ptr %a, align 4
  %4 = load i32, ptr %b, align 4
  %addtmp = add i32 %3, %4
  %5 = load i32, ptr %c, align 4
  %addtmp1 = add i32 %addtmp, %5
  ret i32 %addtmp1
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 6
  }
}
```