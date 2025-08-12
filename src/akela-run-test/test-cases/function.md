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
return Int32, no call

```akela
fn foo(a: Int32, b: Int32, c: Int32)->Int32
    a + b + c
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
    .type = Type::Void
  }
}
```

## Test
return Int32, call

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

## Test
anonymous function

```akela
const foo: fn (Int32, Int32, Int32)->Int32 = fn (a: Int32, b: Int32, c: Int32)->Int32
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
  %foo = alloca ptr, align 8
  store ptr @__anonymous_function_0, ptr %foo, align 8
  %0 = load ptr, ptr %foo, align 8
  %1 = call i32 %0(i32 1, i32 2, i32 3)
  ret i32 %1
}

define i32 @__anonymous_function_0(i32 %0, i32 %1, i32 %2) {
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

## Test
return Bool, true and true

```akela
fn foo(a: Bool, b: Bool)->Bool
    a && b
end
foo(true, true)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i1 @__top_level() {
entry:
  %0 = call i1 @foo(i1 true, i1 true)
  ret i1 %0
}

define i1 @foo(i1 %0, i1 %1) {
body:
  %a = alloca i1, align 1
  store i1 %0, ptr %a, align 1
  %b = alloca i1, align 1
  store i1 %1, ptr %b, align 1
  %2 = load i1, ptr %a, align 1
  br i1 %2, label %and.rhs, label %and.end

and.rhs:                                          ; preds = %body
  %3 = load i1, ptr %b, align 1
  br label %and.end

and.end:                                          ; preds = %and.rhs, %body
  %and.tmp = phi i1 [ false, %body ], [ %3, %and.rhs ]
  ret i1 %and.tmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Bool
    .value = true
  }
}
```

## Test
return Bool, false and true

```akela
fn foo(a: Bool, b: Bool)->Bool
    a && b
end
foo(false, true)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i1 @__top_level() {
entry:
  %0 = call i1 @foo(i1 false, i1 true)
  ret i1 %0
}

define i1 @foo(i1 %0, i1 %1) {
body:
  %a = alloca i1, align 1
  store i1 %0, ptr %a, align 1
  %b = alloca i1, align 1
  store i1 %1, ptr %b, align 1
  %2 = load i1, ptr %a, align 1
  br i1 %2, label %and.rhs, label %and.end

and.rhs:                                          ; preds = %body
  %3 = load i1, ptr %b, align 1
  br label %and.end

and.end:                                          ; preds = %and.rhs, %body
  %and.tmp = phi i1 [ false, %body ], [ %3, %and.rhs ]
  ret i1 %and.tmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Bool
    .value = false
  }
}
```

## Test
return Bool, true and false

```akela
fn foo(a: Bool, b: Bool)->Bool
    a && b
end
foo(true, false)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i1 @__top_level() {
entry:
  %0 = call i1 @foo(i1 true, i1 false)
  ret i1 %0
}

define i1 @foo(i1 %0, i1 %1) {
body:
  %a = alloca i1, align 1
  store i1 %0, ptr %a, align 1
  %b = alloca i1, align 1
  store i1 %1, ptr %b, align 1
  %2 = load i1, ptr %a, align 1
  br i1 %2, label %and.rhs, label %and.end

and.rhs:                                          ; preds = %body
  %3 = load i1, ptr %b, align 1
  br label %and.end

and.end:                                          ; preds = %and.rhs, %body
  %and.tmp = phi i1 [ false, %body ], [ %3, %and.rhs ]
  ret i1 %and.tmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Bool
    .value = false
  }
}
```

## Test
return Bool, false and false

```akela
fn foo(a: Bool, b: Bool)->Bool
    a && b
end
foo(false, false)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i1 @__top_level() {
entry:
  %0 = call i1 @foo(i1 false, i1 false)
  ret i1 %0
}

define i1 @foo(i1 %0, i1 %1) {
body:
  %a = alloca i1, align 1
  store i1 %0, ptr %a, align 1
  %b = alloca i1, align 1
  store i1 %1, ptr %b, align 1
  %2 = load i1, ptr %a, align 1
  br i1 %2, label %and.rhs, label %and.end

and.rhs:                                          ; preds = %body
  %3 = load i1, ptr %b, align 1
  br label %and.end

and.end:                                          ; preds = %and.rhs, %body
  %and.tmp = phi i1 [ false, %body ], [ %3, %and.rhs ]
  ret i1 %and.tmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Bool
    .value = false
  }
}
```

## Test
function return array

```akela
fn foo(a: Int32, b: Int32, c: Int32)->[6 const]Nat8
    "hello"
end
foo(1, 2, 3)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [6 x i8] c"hello\00", align 1

define ptr @__top_level() {
entry:
  %0 = call ptr @foo(i32 1, i32 2, i32 3)
  ret ptr %0
}

define ptr @foo(i32 %0, i32 %1, i32 %2) {
body:
  %a = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 %1, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 %2, ptr %c, align 4
  ret ptr @.str
}
```

```cent
use lib::base::*
Test {
  .mute = true
  Field {
    .type = Type::Nat64
    .value = 103
  }
}
```