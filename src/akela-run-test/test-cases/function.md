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

## Test
extern

```akela
extern pow(x: Real64, y: Real64)->Real64
pow(5.0, 2.0)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define double @__top_level() {
entry:
  %0 = call double @pow(double 5.000000e+00, double 2.000000e+00)
  ret double %0
}

declare double @pow(double, double)
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 25.0
  }
}
```

## Test
function assign

```akela
fn foo()->Int32 1 end
var bar: fn ()->Int32
bar = foo
bar()
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i32 @__top_level() {
entry:
  %bar = alloca ptr, align 8
  store ptr @foo, ptr %bar, align 8
  %0 = load ptr, ptr %bar, align 8
  %1 = call i32 %0()
  ret i32 %1
}

define i32 @foo() {
body:
  ret i32 1
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 1
  }
}
```

## Test
function assign 2

```akela
fn foo()->Int32 1 end
fn bar()->Int32 2 end
foo = bar
foo()
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i32 @__top_level() {
entry:
  %bar = alloca ptr, align 8
  store ptr @bar, ptr %bar, align 8
  %0 = load ptr, ptr %bar, align 8
  %1 = call i32 %0()
  ret i32 %1
}

define i32 @foo() {
body:
  ret i32 1
}

define i32 @bar() {
body:
  ret i32 2
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 2
  }
}
```

## Test
const assign

```akela
fn foo(a: Int32)->Int32
    a + 1
end
const a: fn (Int32)->Int32 = foo
a(1)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i32 @__top_level() {
entry:
  %a = alloca ptr, align 8
  store ptr @foo, ptr %a, align 8
  %0 = load ptr, ptr %a, align 8
  %1 = call i32 %0(i32 1)
  ret i32 %1
}

define i32 @foo(i32 %0) {
body:
  %a = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %1 = load i32, ptr %a, align 4
  %addtmp = add i32 %1, 1
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 2
  }
}
```

## Test
function expression

```akela
fn foo(a: Int32)->Int32
  a + 1
end(1)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i32 @__top_level() {
entry:
  %0 = call i32 @foo(i32 1)
  ret i32 %0
}

define i32 @foo(i32 %0) {
body:
  %a = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %1 = load i32, ptr %a, align 4
  %addtmp = add i32 %1, 1
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 2
  }
}
```

## Test
anonymous expression

```akela
fn (a: Int32)->Int32
    a + 1
end(1)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define i32 @__top_level() {
entry:
  %0 = call i32 @__anonymous_function_0(i32 1)
  ret i32 %0
}

define i32 @__anonymous_function_0(i32 %0) {
body:
  %a = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %1 = load i32, ptr %a, align 4
  %addtmp = add i32 %1, 1
  ret i32 %addtmp
}
```


```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 2
  }
}
```

## Test
function call and array subscript 1

```akela
fn add_ten(array: [10]Int32, i: Nat64)->Int32
    array[i] + 10
end
const a: [10]Int32 = [100,200,300,400,500,600,700,800,900,1000]
add_ten(a, 0)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [10 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [10 x i32], ptr %a, i64 0, i64 0
  store i32 100, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 200, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 300, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 400, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 500, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 600, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 700, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 800, ptr %arrayelementtmp7, align 4
  %arrayelementtmp8 = getelementptr inbounds i32, ptr %arrayelementtmp7, i64 1
  store i32 900, ptr %arrayelementtmp8, align 4
  %arrayelementtmp9 = getelementptr inbounds i32, ptr %arrayelementtmp8, i64 1
  store i32 1000, ptr %arrayelementtmp9, align 4
  %0 = call i32 @add_ten(ptr %a, i64 0)
  ret i32 %0
}

define i32 @add_ten(ptr %0, i64 %1) {
body:
  %array = alloca ptr, align 8
  store ptr %0, ptr %array, align 8
  %i = alloca i64, align 8
  store i64 %1, ptr %i, align 8
  %2 = load ptr, ptr %array, align 8
  %3 = load i64, ptr %i, align 8
  %4 = icmp ult i64 %3, 10
  br i1 %4, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %body
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %body
  %subscripttmp = getelementptr inbounds i32, ptr %2, i64 %3
  %elementtmp = load i32, ptr %subscripttmp, align 4
  %addtmp = add i32 %elementtmp, 10
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 110
  }
}
```

## Test
function call and array subscript 2

```akela
fn add_ten(array: [10]Int32, i: Nat64)->Int32
    array[i] + 10
end
const a: [10]Int32 = [100,200,300,400,500,600,700,800,900,1000]
add_ten(a, 1)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [10 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [10 x i32], ptr %a, i64 0, i64 0
  store i32 100, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 200, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 300, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 400, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 500, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 600, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 700, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 800, ptr %arrayelementtmp7, align 4
  %arrayelementtmp8 = getelementptr inbounds i32, ptr %arrayelementtmp7, i64 1
  store i32 900, ptr %arrayelementtmp8, align 4
  %arrayelementtmp9 = getelementptr inbounds i32, ptr %arrayelementtmp8, i64 1
  store i32 1000, ptr %arrayelementtmp9, align 4
  %0 = call i32 @add_ten(ptr %a, i64 1)
  ret i32 %0
}

define i32 @add_ten(ptr %0, i64 %1) {
body:
  %array = alloca ptr, align 8
  store ptr %0, ptr %array, align 8
  %i = alloca i64, align 8
  store i64 %1, ptr %i, align 8
  %2 = load ptr, ptr %array, align 8
  %3 = load i64, ptr %i, align 8
  %4 = icmp ult i64 %3, 10
  br i1 %4, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %body
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %body
  %subscripttmp = getelementptr inbounds i32, ptr %2, i64 %3
  %elementtmp = load i32, ptr %subscripttmp, align 4
  %addtmp = add i32 %elementtmp, 10
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 210
  }
}
```

## Test
function call and array subscript 3

```akela
fn add_ten(array: [10]Int32, i: Nat64)->Int32
    array[i] + 10
end
const a: [10]Int32 = [100,200,300,400,500,600,700,800,900,1000]
add_ten(a, 2)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [10 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [10 x i32], ptr %a, i64 0, i64 0
  store i32 100, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 200, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 300, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 400, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 500, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 600, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 700, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 800, ptr %arrayelementtmp7, align 4
  %arrayelementtmp8 = getelementptr inbounds i32, ptr %arrayelementtmp7, i64 1
  store i32 900, ptr %arrayelementtmp8, align 4
  %arrayelementtmp9 = getelementptr inbounds i32, ptr %arrayelementtmp8, i64 1
  store i32 1000, ptr %arrayelementtmp9, align 4
  %0 = call i32 @add_ten(ptr %a, i64 2)
  ret i32 %0
}

define i32 @add_ten(ptr %0, i64 %1) {
body:
  %array = alloca ptr, align 8
  store ptr %0, ptr %array, align 8
  %i = alloca i64, align 8
  store i64 %1, ptr %i, align 8
  %2 = load ptr, ptr %array, align 8
  %3 = load i64, ptr %i, align 8
  %4 = icmp ult i64 %3, 10
  br i1 %4, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %body
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %body
  %subscripttmp = getelementptr inbounds i32, ptr %2, i64 %3
  %elementtmp = load i32, ptr %subscripttmp, align 4
  %addtmp = add i32 %elementtmp, 10
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 310
  }
}
```

## Test
struct param

```akela
struct Point
    x: Real64
    y: Real64
end
fn add_ten(p: Point)->Real64
    p.x + 10.0
end
const p: Point = Point
    x: 2.5
    y: 3.5
end
add_ten(p)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Point.3 = type { double, double }
%Point.4 = type { double, double }
%Point.2 = type { double, double }

define double @__top_level() {
entry:
  %p = alloca %Point.3, align 8
  %x = getelementptr inbounds nuw %Point.4, ptr %p, i32 0, i32 0
  store double 2.500000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.4, ptr %p, i32 0, i32 1
  store double 3.500000e+00, ptr %y, align 8
  %0 = call double @add_ten(ptr %p)
  ret double %0
}

define double @add_ten(ptr %0) {
body:
  %p = alloca ptr, align 8
  store ptr %0, ptr %p, align 8
  %1 = load ptr, ptr %p, align 8
  %2 = getelementptr inbounds nuw %Point.2, ptr %1, i32 0, i32 0
  %x = load double, ptr %2, align 8
  %addtmp = fadd double %x, 1.000000e+01
  ret double %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 12.5
  }
}
```