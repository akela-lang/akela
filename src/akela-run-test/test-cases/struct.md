# Test Suite
struct

```cent
TestSuite {
}
```

## Test
assign fields 1

```akela
struct Point
    x: Real64
    y: Real64
end
var a: Point
a.x = 1.0
a.y = 2.0
a.x
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Point.0 = type { double, double }
%Point.1 = type { double, double }
%Point.2 = type { double, double }
%Point.3 = type { double, double }

define double @__top_level() {
entry:
  %a = alloca %Point.0, align 8
  %0 = getelementptr inbounds nuw %Point.1, ptr %a, i32 0, i32 0
  store double 1.000000e+00, ptr %0, align 8
  %1 = getelementptr inbounds nuw %Point.2, ptr %a, i32 0, i32 1
  store double 2.000000e+00, ptr %1, align 8
  %2 = getelementptr inbounds nuw %Point.3, ptr %a, i32 0, i32 0
  %x = load double, ptr %2, align 8
  ret double %x
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 1.0
  }
}
```

## Test
assign fields 2

```akela
struct Point
    x: Real64
    y: Real64
end
var a: Point
a.x = 1.0
a.y = 2.0
a.y
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Point.0 = type { double, double }
%Point.1 = type { double, double }
%Point.2 = type { double, double }
%Point.3 = type { double, double }

define double @__top_level() {
entry:
  %a = alloca %Point.0, align 8
  %0 = getelementptr inbounds nuw %Point.1, ptr %a, i32 0, i32 0
  store double 1.000000e+00, ptr %0, align 8
  %1 = getelementptr inbounds nuw %Point.2, ptr %a, i32 0, i32 1
  store double 2.000000e+00, ptr %1, align 8
  %2 = getelementptr inbounds nuw %Point.3, ptr %a, i32 0, i32 1
  %y = load double, ptr %2, align 8
  ret double %y
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 2.0
  }
}
```

## Test
Const assign 1

```akela
struct Point
    x: Real64
    y: Real64
end
const a: Point = Point
    x: 1.0
    y: 2.0
end
a.x
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Point.0 = type { double, double }
%Point.1 = type { double, double }
%Point.2 = type { double, double }

define double @__top_level() {
entry:
  %a = alloca %Point.0, align 8
  %x = getelementptr inbounds nuw %Point.1, ptr %a, i32 0, i32 0
  store double 1.000000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.1, ptr %a, i32 0, i32 1
  store double 2.000000e+00, ptr %y, align 8
  %0 = getelementptr inbounds nuw %Point.2, ptr %a, i32 0, i32 0
  %x1 = load double, ptr %0, align 8
  ret double %x1
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 1.0
  }
}
```

## Test
Const assign 2

```akela
struct Point
    x: Real64
    y: Real64
end
const a: Point = Point
    x: 1.0
    y: 2.0
end
a.y
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Point.0 = type { double, double }
%Point.1 = type { double, double }
%Point.2 = type { double, double }

define double @__top_level() {
entry:
  %a = alloca %Point.0, align 8
  %x = getelementptr inbounds nuw %Point.1, ptr %a, i32 0, i32 0
  store double 1.000000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.1, ptr %a, i32 0, i32 1
  store double 2.000000e+00, ptr %y, align 8
  %0 = getelementptr inbounds nuw %Point.2, ptr %a, i32 0, i32 1
  %y1 = load double, ptr %0, align 8
  ret double %y1
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 2.0
  }
}
```

## Test
const array copy Nat8

```akela
const a: [4 const]Nat8 = [1,2,3,4]
const b: [4 const]Nat8 = a
b[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [4 x i8], align 1
  %arrayelementtmp = getelementptr inbounds [4 x i8], ptr %a, i64 0, i64 0
  store i8 1, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i8, ptr %arrayelementtmp, i64 1
  store i8 2, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i8, ptr %arrayelementtmp1, i64 1
  store i8 3, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i8, ptr %arrayelementtmp2, i64 1
  store i8 4, ptr %arrayelementtmp3, align 1
  %b = alloca [4 x i8], align 1
  %lhstmp = getelementptr inbounds i8, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i8, ptr %a, i64 0
  %rhs = load i8, ptr %rhstmp, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp4 = getelementptr inbounds i8, ptr %b, i64 1
  %rhstmp5 = getelementptr inbounds i8, ptr %a, i64 1
  %rhs6 = load i8, ptr %rhstmp5, align 1
  store i8 %rhs6, ptr %lhstmp4, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %b, i64 2
  %rhstmp8 = getelementptr inbounds i8, ptr %a, i64 2
  %rhs9 = load i8, ptr %rhstmp8, align 1
  store i8 %rhs9, ptr %lhstmp7, align 1
  %lhstmp10 = getelementptr inbounds i8, ptr %b, i64 3
  %rhstmp11 = getelementptr inbounds i8, ptr %a, i64 3
  %rhs12 = load i8, ptr %rhstmp11, align 1
  store i8 %rhs12, ptr %lhstmp10, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %b, i64 0
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 1
  }
}
```

## Test
struct Nat8

```akela
struct Foo
    x: Nat8
end
var foo: Foo
foo.x = 5
foo.x
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { i8 }
%Foo.1 = type { i8 }
%Foo.2 = type { i8 }

define i8 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %0 = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  store i8 5, ptr %0, align 1
  %1 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  %x = load i8, ptr %1, align 1
  ret i8 %x
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 5
  }
}
```

## Test
struct array

```akela
struct Foo
    x: [4]Nat8
end
const foo: Foo = Foo
    x: [1,2,3,4]
end
foo.x[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { [4 x i8] }
%Foo.1 = type { [4 x i8] }
%Foo.2 = type { [4 x i8] }

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %x = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  %arrayliteraltmp = alloca [4 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [4 x i32], ptr %arrayliteraltmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %lhstmp = getelementptr inbounds i32, ptr %x, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %arrayliteraltmp, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp4 = getelementptr inbounds i32, ptr %x, i64 1
  %rhstmp5 = getelementptr inbounds i32, ptr %arrayliteraltmp, i64 1
  %rhs6 = load i32, ptr %rhstmp5, align 4
  store i32 %rhs6, ptr %lhstmp4, align 4
  %lhstmp7 = getelementptr inbounds i32, ptr %x, i64 2
  %rhstmp8 = getelementptr inbounds i32, ptr %arrayliteraltmp, i64 2
  %rhs9 = load i32, ptr %rhstmp8, align 4
  store i32 %rhs9, ptr %lhstmp7, align 4
  %lhstmp10 = getelementptr inbounds i32, ptr %x, i64 3
  %rhstmp11 = getelementptr inbounds i32, ptr %arrayliteraltmp, i64 3
  %rhs12 = load i32, ptr %rhstmp11, align 4
  store i32 %rhs12, ptr %lhstmp10, align 4
  %0 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %0, i64 0
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 1
  }
}
```

## Test
struct array copy Int32 to Nat8

```akela
const a: [4]Int32 = [1,2,3,4]
struct Foo
    x: [4]Nat8
end
const foo: Foo = Foo
    x: a
end
foo.x[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { [4 x i8] }
%Foo.1 = type { [4 x i8] }
%Foo.2 = type { [4 x i8] }

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [4 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [4 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %foo = alloca %Foo.0, align 8
  %x = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  %lhstmp = getelementptr inbounds i32, ptr %x, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %a, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp4 = getelementptr inbounds i32, ptr %x, i64 1
  %rhstmp5 = getelementptr inbounds i32, ptr %a, i64 1
  %rhs6 = load i32, ptr %rhstmp5, align 4
  store i32 %rhs6, ptr %lhstmp4, align 4
  %lhstmp7 = getelementptr inbounds i32, ptr %x, i64 2
  %rhstmp8 = getelementptr inbounds i32, ptr %a, i64 2
  %rhs9 = load i32, ptr %rhstmp8, align 4
  store i32 %rhs9, ptr %lhstmp7, align 4
  %lhstmp10 = getelementptr inbounds i32, ptr %x, i64 3
  %rhstmp11 = getelementptr inbounds i32, ptr %a, i64 3
  %rhs12 = load i32, ptr %rhstmp11, align 4
  store i32 %rhs12, ptr %lhstmp10, align 4
  %0 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %0, i64 0
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 1
  }
}
```

## Test
struct array assign 1

```akela
struct Foo
    x: [4]Int32
end
var foo: Foo
foo.x[0] = 1
foo.x[1] = 2
foo.x[2] = 3
foo.x[3] = 4
foo.x[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { [4 x i32] }
%Foo.1 = type { [4 x i32] }
%Foo.2 = type { [4 x i32] }
%Foo.3 = type { [4 x i32] }
%Foo.4 = type { [4 x i32] }
%Foo.5 = type { [4 x i32] }

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %0 = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %0, i64 0
  store i32 1, ptr %subscripttmp, align 4
  %1 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %1, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  %2 = getelementptr inbounds nuw %Foo.3, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %2, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  %3 = getelementptr inbounds nuw %Foo.4, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %3, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  %4 = getelementptr inbounds nuw %Foo.5, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %4, i64 0
  %elementtmp = load i32, ptr %subscripttmp12, align 4
  ret i32 %elementtmp
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
struct array assign 2

```akela
struct Foo
    x: [4]Int32
end
var foo: Foo
foo.x[0] = 1
foo.x[1] = 2
foo.x[2] = 3
foo.x[3] = 4
foo.x[1]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { [4 x i32] }
%Foo.1 = type { [4 x i32] }
%Foo.2 = type { [4 x i32] }
%Foo.3 = type { [4 x i32] }
%Foo.4 = type { [4 x i32] }
%Foo.5 = type { [4 x i32] }

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %0 = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %0, i64 0
  store i32 1, ptr %subscripttmp, align 4
  %1 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %1, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  %2 = getelementptr inbounds nuw %Foo.3, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %2, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  %3 = getelementptr inbounds nuw %Foo.4, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %3, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  %4 = getelementptr inbounds nuw %Foo.5, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %4, i64 1
  %elementtmp = load i32, ptr %subscripttmp12, align 4
  ret i32 %elementtmp
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
struct array assign 3

```akela
struct Foo
    x: [4]Int32
end
var foo: Foo
foo.x[0] = 1
foo.x[1] = 2
foo.x[2] = 3
foo.x[3] = 4
foo.x[2]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { [4 x i32] }
%Foo.1 = type { [4 x i32] }
%Foo.2 = type { [4 x i32] }
%Foo.3 = type { [4 x i32] }
%Foo.4 = type { [4 x i32] }
%Foo.5 = type { [4 x i32] }

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %0 = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %0, i64 0
  store i32 1, ptr %subscripttmp, align 4
  %1 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %1, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  %2 = getelementptr inbounds nuw %Foo.3, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %2, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  %3 = getelementptr inbounds nuw %Foo.4, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %3, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  %4 = getelementptr inbounds nuw %Foo.5, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %4, i64 2
  %elementtmp = load i32, ptr %subscripttmp12, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 3
  }
}
```

## Test
struct array assign 4

```akela
struct Foo
    x: [4]Int32
end
var foo: Foo
foo.x[0] = 1
foo.x[1] = 2
foo.x[2] = 3
foo.x[3] = 4
foo.x[3]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { [4 x i32] }
%Foo.1 = type { [4 x i32] }
%Foo.2 = type { [4 x i32] }
%Foo.3 = type { [4 x i32] }
%Foo.4 = type { [4 x i32] }
%Foo.5 = type { [4 x i32] }

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %0 = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %0, i64 0
  store i32 1, ptr %subscripttmp, align 4
  %1 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %1, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  %2 = getelementptr inbounds nuw %Foo.3, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %2, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  %3 = getelementptr inbounds nuw %Foo.4, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %3, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  %4 = getelementptr inbounds nuw %Foo.5, ptr %foo, i32 0, i32 0
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %4, i64 3
  %elementtmp = load i32, ptr %subscripttmp12, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 4
  }
}
```

## Test
Person first name

```akela
struct Person
    first_name: [100 const]Nat8
    last_name: [100 const]Nat8
    age: Nat32
end
const p: Person = Person
    first_name: "John"
    last_name: "Smith"
    age: 35
end
p.first_name[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Person.0 = type { [100 x i8], [100 x i8], i32 }
%Person.1 = type { [100 x i8], [100 x i8], i32 }
%Person.2 = type { [100 x i8], [100 x i8], i32 }

@.str = private unnamed_addr constant [5 x i8] c"John\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"Smith\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %p = alloca %Person.0, align 8
  %first_name = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 0
  %lhstmp = getelementptr inbounds i8, ptr %first_name, i64 0
  %rhs = load i8, ptr @.str, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp1 = getelementptr inbounds i8, ptr %first_name, i64 1
  %rhs2 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 1), align 1
  store i8 %rhs2, ptr %lhstmp1, align 1
  %lhstmp3 = getelementptr inbounds i8, ptr %first_name, i64 2
  %rhs4 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 2), align 1
  store i8 %rhs4, ptr %lhstmp3, align 1
  %lhstmp5 = getelementptr inbounds i8, ptr %first_name, i64 3
  %rhs6 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 3), align 1
  store i8 %rhs6, ptr %lhstmp5, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %first_name, i64 4
  %rhs8 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 4), align 1
  store i8 %rhs8, ptr %lhstmp7, align 1
  %last_name = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 1
  %lhstmp9 = getelementptr inbounds i8, ptr %last_name, i64 0
  %rhs10 = load i8, ptr @.str.1, align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  %lhstmp11 = getelementptr inbounds i8, ptr %last_name, i64 1
  %rhs12 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 1), align 1
  store i8 %rhs12, ptr %lhstmp11, align 1
  %lhstmp13 = getelementptr inbounds i8, ptr %last_name, i64 2
  %rhs14 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 2), align 1
  store i8 %rhs14, ptr %lhstmp13, align 1
  %lhstmp15 = getelementptr inbounds i8, ptr %last_name, i64 3
  %rhs16 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 3), align 1
  store i8 %rhs16, ptr %lhstmp15, align 1
  %lhstmp17 = getelementptr inbounds i8, ptr %last_name, i64 4
  %rhs18 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 4), align 1
  store i8 %rhs18, ptr %lhstmp17, align 1
  %lhstmp19 = getelementptr inbounds i8, ptr %last_name, i64 5
  %rhs20 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 5), align 1
  store i8 %rhs20, ptr %lhstmp19, align 1
  %age = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 2
  store i32 35, ptr %age, align 4
  %0 = getelementptr inbounds nuw %Person.2, ptr %p, i32 0, i32 0
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %0, i64 0
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 74
  }
}
```

## Test
Person, last name

```akela
struct Person
    first_name: [100 const]Nat8
    last_name: [100 const]Nat8
    age: Nat32
end
const p: Person = Person
    first_name: "John"
    last_name: "Smith"
    age: 35
end
p.last_name[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Person.0 = type { [100 x i8], [100 x i8], i32 }
%Person.1 = type { [100 x i8], [100 x i8], i32 }
%Person.2 = type { [100 x i8], [100 x i8], i32 }

@.str = private unnamed_addr constant [5 x i8] c"John\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"Smith\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %p = alloca %Person.0, align 8
  %first_name = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 0
  %lhstmp = getelementptr inbounds i8, ptr %first_name, i64 0
  %rhs = load i8, ptr @.str, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp1 = getelementptr inbounds i8, ptr %first_name, i64 1
  %rhs2 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 1), align 1
  store i8 %rhs2, ptr %lhstmp1, align 1
  %lhstmp3 = getelementptr inbounds i8, ptr %first_name, i64 2
  %rhs4 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 2), align 1
  store i8 %rhs4, ptr %lhstmp3, align 1
  %lhstmp5 = getelementptr inbounds i8, ptr %first_name, i64 3
  %rhs6 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 3), align 1
  store i8 %rhs6, ptr %lhstmp5, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %first_name, i64 4
  %rhs8 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 4), align 1
  store i8 %rhs8, ptr %lhstmp7, align 1
  %last_name = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 1
  %lhstmp9 = getelementptr inbounds i8, ptr %last_name, i64 0
  %rhs10 = load i8, ptr @.str.1, align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  %lhstmp11 = getelementptr inbounds i8, ptr %last_name, i64 1
  %rhs12 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 1), align 1
  store i8 %rhs12, ptr %lhstmp11, align 1
  %lhstmp13 = getelementptr inbounds i8, ptr %last_name, i64 2
  %rhs14 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 2), align 1
  store i8 %rhs14, ptr %lhstmp13, align 1
  %lhstmp15 = getelementptr inbounds i8, ptr %last_name, i64 3
  %rhs16 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 3), align 1
  store i8 %rhs16, ptr %lhstmp15, align 1
  %lhstmp17 = getelementptr inbounds i8, ptr %last_name, i64 4
  %rhs18 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 4), align 1
  store i8 %rhs18, ptr %lhstmp17, align 1
  %lhstmp19 = getelementptr inbounds i8, ptr %last_name, i64 5
  %rhs20 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 5), align 1
  store i8 %rhs20, ptr %lhstmp19, align 1
  %age = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 2
  store i32 35, ptr %age, align 4
  %0 = getelementptr inbounds nuw %Person.2, ptr %p, i32 0, i32 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %0, i64 0
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 83
  }
}
```

## Test
Person, age

```akela
struct Person
    first_name: [100 const]Nat8
    last_name: [100 const]Nat8
    age: Nat32
end
const p: Person = Person
    first_name: "John"
    last_name: "Smith"
    age: 35
end
p.age
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Person.0 = type { [100 x i8], [100 x i8], i32 }
%Person.1 = type { [100 x i8], [100 x i8], i32 }
%Person.2 = type { [100 x i8], [100 x i8], i32 }

@.str = private unnamed_addr constant [5 x i8] c"John\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"Smith\00", align 1

define i32 @__top_level() {
entry:
  %p = alloca %Person.0, align 8
  %first_name = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 0
  %lhstmp = getelementptr inbounds i8, ptr %first_name, i64 0
  %rhs = load i8, ptr @.str, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp1 = getelementptr inbounds i8, ptr %first_name, i64 1
  %rhs2 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 1), align 1
  store i8 %rhs2, ptr %lhstmp1, align 1
  %lhstmp3 = getelementptr inbounds i8, ptr %first_name, i64 2
  %rhs4 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 2), align 1
  store i8 %rhs4, ptr %lhstmp3, align 1
  %lhstmp5 = getelementptr inbounds i8, ptr %first_name, i64 3
  %rhs6 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 3), align 1
  store i8 %rhs6, ptr %lhstmp5, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %first_name, i64 4
  %rhs8 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 4), align 1
  store i8 %rhs8, ptr %lhstmp7, align 1
  %last_name = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 1
  %lhstmp9 = getelementptr inbounds i8, ptr %last_name, i64 0
  %rhs10 = load i8, ptr @.str.1, align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  %lhstmp11 = getelementptr inbounds i8, ptr %last_name, i64 1
  %rhs12 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 1), align 1
  store i8 %rhs12, ptr %lhstmp11, align 1
  %lhstmp13 = getelementptr inbounds i8, ptr %last_name, i64 2
  %rhs14 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 2), align 1
  store i8 %rhs14, ptr %lhstmp13, align 1
  %lhstmp15 = getelementptr inbounds i8, ptr %last_name, i64 3
  %rhs16 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 3), align 1
  store i8 %rhs16, ptr %lhstmp15, align 1
  %lhstmp17 = getelementptr inbounds i8, ptr %last_name, i64 4
  %rhs18 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 4), align 1
  store i8 %rhs18, ptr %lhstmp17, align 1
  %lhstmp19 = getelementptr inbounds i8, ptr %last_name, i64 5
  %rhs20 = load i8, ptr getelementptr inbounds (i8, ptr @.str.1, i64 5), align 1
  store i8 %rhs20, ptr %lhstmp19, align 1
  %age = getelementptr inbounds nuw %Person.1, ptr %p, i32 0, i32 2
  store i32 35, ptr %age, align 4
  %0 = getelementptr inbounds nuw %Person.2, ptr %p, i32 0, i32 2
  %age21 = load i32, ptr %0, align 4
  ret i32 %age21
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 35
  }
}
```

## Test
struct struct 1

```akela
struct Point
    x: Real64
    y: Real64
end
struct Line
    p0: Point
    p1: Point
end
const line: Line = Line
    p0: Point
        x: 1.5
        y: 2.5
    end
    p1: Point
        x: 3.0
        y: 4.0
    end
end
line.p0.x
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Line.4 = type { %Point.2, %Point.3 }
%Point.2 = type { double, double }
%Point.3 = type { double, double }
%Line.7 = type { %Point.5, %Point.6 }
%Point.5 = type { double, double }
%Point.6 = type { double, double }
%Point.8 = type { double, double }
%Point.9 = type { double, double }
%Line.12 = type { %Point.10, %Point.11 }
%Point.10 = type { double, double }
%Point.11 = type { double, double }
%Point.13 = type { double, double }

define double @__top_level() {
entry:
  %line = alloca %Line.4, align 8
  %p0 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 0
  %x = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 0
  store double 1.500000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 1
  store double 2.500000e+00, ptr %y, align 8
  %p1 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 1
  %x1 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 0
  store double 3.000000e+00, ptr %x1, align 8
  %y2 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 1
  store double 4.000000e+00, ptr %y2, align 8
  %0 = getelementptr inbounds nuw %Line.12, ptr %line, i32 0, i32 0
  %1 = getelementptr inbounds nuw %Point.13, ptr %0, i32 0, i32 0
  %x3 = load double, ptr %1, align 8
  ret double %x3
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 1.5
  }
}
```

## Test
struct struct 2

```akela
struct Point
    x: Real64
    y: Real64
end
struct Line
    p0: Point
    p1: Point
end
const line: Line = Line
    p0: Point
        x: 1.5
        y: 2.5
    end
    p1: Point
        x: 3.0
        y: 4.0
    end
end
line.p0.y
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Line.4 = type { %Point.2, %Point.3 }
%Point.2 = type { double, double }
%Point.3 = type { double, double }
%Line.7 = type { %Point.5, %Point.6 }
%Point.5 = type { double, double }
%Point.6 = type { double, double }
%Point.8 = type { double, double }
%Point.9 = type { double, double }
%Line.12 = type { %Point.10, %Point.11 }
%Point.10 = type { double, double }
%Point.11 = type { double, double }
%Point.13 = type { double, double }

define double @__top_level() {
entry:
  %line = alloca %Line.4, align 8
  %p0 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 0
  %x = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 0
  store double 1.500000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 1
  store double 2.500000e+00, ptr %y, align 8
  %p1 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 1
  %x1 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 0
  store double 3.000000e+00, ptr %x1, align 8
  %y2 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 1
  store double 4.000000e+00, ptr %y2, align 8
  %0 = getelementptr inbounds nuw %Line.12, ptr %line, i32 0, i32 0
  %1 = getelementptr inbounds nuw %Point.13, ptr %0, i32 0, i32 1
  %y3 = load double, ptr %1, align 8
  ret double %y3
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 2.5
  }
}
```

## Test
struct struct 3

```akela
struct Point
    x: Real64
    y: Real64
end
struct Line
    p0: Point
    p1: Point
end
const line: Line = Line
    p0: Point
        x: 1.5
        y: 2.5
    end
    p1: Point
        x: 3.0
        y: 4.0
    end
end
line.p1.x
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Line.4 = type { %Point.2, %Point.3 }
%Point.2 = type { double, double }
%Point.3 = type { double, double }
%Line.7 = type { %Point.5, %Point.6 }
%Point.5 = type { double, double }
%Point.6 = type { double, double }
%Point.8 = type { double, double }
%Point.9 = type { double, double }
%Line.12 = type { %Point.10, %Point.11 }
%Point.10 = type { double, double }
%Point.11 = type { double, double }
%Point.13 = type { double, double }

define double @__top_level() {
entry:
  %line = alloca %Line.4, align 8
  %p0 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 0
  %x = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 0
  store double 1.500000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 1
  store double 2.500000e+00, ptr %y, align 8
  %p1 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 1
  %x1 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 0
  store double 3.000000e+00, ptr %x1, align 8
  %y2 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 1
  store double 4.000000e+00, ptr %y2, align 8
  %0 = getelementptr inbounds nuw %Line.12, ptr %line, i32 0, i32 1
  %1 = getelementptr inbounds nuw %Point.13, ptr %0, i32 0, i32 0
  %x3 = load double, ptr %1, align 8
  ret double %x3
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 3.0
  }
}
```

## Test
struct struct 4

```akela
struct Point
    x: Real64
    y: Real64
end
struct Line
    p0: Point
    p1: Point
end
const line: Line = Line
    p0: Point
        x: 1.5
        y: 2.5
    end
    p1: Point
        x: 3.0
        y: 4.0
    end
end
line.p1.y
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Line.4 = type { %Point.2, %Point.3 }
%Point.2 = type { double, double }
%Point.3 = type { double, double }
%Line.7 = type { %Point.5, %Point.6 }
%Point.5 = type { double, double }
%Point.6 = type { double, double }
%Point.8 = type { double, double }
%Point.9 = type { double, double }
%Line.12 = type { %Point.10, %Point.11 }
%Point.10 = type { double, double }
%Point.11 = type { double, double }
%Point.13 = type { double, double }

define double @__top_level() {
entry:
  %line = alloca %Line.4, align 8
  %p0 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 0
  %x = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 0
  store double 1.500000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 1
  store double 2.500000e+00, ptr %y, align 8
  %p1 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 1
  %x1 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 0
  store double 3.000000e+00, ptr %x1, align 8
  %y2 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 1
  store double 4.000000e+00, ptr %y2, align 8
  %0 = getelementptr inbounds nuw %Line.12, ptr %line, i32 0, i32 1
  %1 = getelementptr inbounds nuw %Point.13, ptr %0, i32 0, i32 1
  %y3 = load double, ptr %1, align 8
  ret double %y3
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 4.0
  }
}
```

## Test
struct assign

```akela
struct Point
    x: Real64
    y: Real64
end
var p: Point = Point
    x: 1.5
    y: 2.5
end
p.x = 1.6
p.x
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Point.0 = type { double, double }
%Point.1 = type { double, double }
%Point.2 = type { double, double }
%Point.3 = type { double, double }

define double @__top_level() {
entry:
  %p = alloca %Point.0, align 8
  %x = getelementptr inbounds nuw %Point.1, ptr %p, i32 0, i32 0
  store double 1.500000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.1, ptr %p, i32 0, i32 1
  store double 2.500000e+00, ptr %y, align 8
  %0 = getelementptr inbounds nuw %Point.2, ptr %p, i32 0, i32 0
  store double 1.600000e+00, ptr %0, align 8
  %1 = getelementptr inbounds nuw %Point.3, ptr %p, i32 0, i32 0
  %x1 = load double, ptr %1, align 8
  ret double %x1
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 1.6
  }
}
```

## Test
struct struct assign field

```akela
struct Point
    x: Real64
    y: Real64
end
struct Line
    p0: Point
    p1: Point
end
var line: Line = Line
    p0: Point
        x: 1.5
        y: 2.5
    end
    p1: Point
        x: 3.0
        y: 4.0
    end
end
line.p0.x = 1.6
line.p0.x
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Line.4 = type { %Point.2, %Point.3 }
%Point.2 = type { double, double }
%Point.3 = type { double, double }
%Line.7 = type { %Point.5, %Point.6 }
%Point.5 = type { double, double }
%Point.6 = type { double, double }
%Point.8 = type { double, double }
%Point.9 = type { double, double }
%Line.12 = type { %Point.10, %Point.11 }
%Point.10 = type { double, double }
%Point.11 = type { double, double }
%Point.13 = type { double, double }
%Line.16 = type { %Point.14, %Point.15 }
%Point.14 = type { double, double }
%Point.15 = type { double, double }
%Point.17 = type { double, double }

define double @__top_level() {
entry:
  %line = alloca %Line.4, align 8
  %p0 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 0
  %x = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 0
  store double 1.500000e+00, ptr %x, align 8
  %y = getelementptr inbounds nuw %Point.8, ptr %p0, i32 0, i32 1
  store double 2.500000e+00, ptr %y, align 8
  %p1 = getelementptr inbounds nuw %Line.7, ptr %line, i32 0, i32 1
  %x1 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 0
  store double 3.000000e+00, ptr %x1, align 8
  %y2 = getelementptr inbounds nuw %Point.9, ptr %p1, i32 0, i32 1
  store double 4.000000e+00, ptr %y2, align 8
  %0 = getelementptr inbounds nuw %Line.12, ptr %line, i32 0, i32 0
  %1 = getelementptr inbounds nuw %Point.13, ptr %0, i32 0, i32 0
  store double 1.600000e+00, ptr %1, align 8
  %2 = getelementptr inbounds nuw %Line.16, ptr %line, i32 0, i32 0
  %3 = getelementptr inbounds nuw %Point.17, ptr %2, i32 0, i32 0
  %x3 = load double, ptr %3, align 8
  ret double %x3
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Real64
    .value = 1.6
  }
}
```

## Test
struct function 1

```akela
struct Foo
    compute: fn(Int32)->Int32
end
const foo: Foo = Foo
    compute: fn (x: Int32)->Int32
        x + 1
    end
end
foo.compute(77)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { ptr }
%Foo.1 = type { ptr }
%Foo.2 = type { ptr }

define i32 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %compute = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  store ptr @__anonymous_function_0, ptr %compute, align 8
  %0 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  %compute1 = load ptr, ptr %0, align 8
  %1 = call i32 %compute1(i32 77)
  ret i32 %1
}

define i32 @__anonymous_function_0(i32 %0) {
body:
  %x = alloca i32, align 4
  store i32 %0, ptr %x, align 4
  %1 = load i32, ptr %x, align 4
  %addtmp = add i32 %1, 1
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 78
  }
}
```

## Test
struct function 2

```akela
struct Foo
    compute: fn(Int32)->Int32
end
const foo: Foo = Foo
    compute: fn (x: Int32)->Int32
        x + 2
    end
end
foo.compute(77)
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

%Foo.0 = type { ptr }
%Foo.1 = type { ptr }
%Foo.2 = type { ptr }

define i32 @__top_level() {
entry:
  %foo = alloca %Foo.0, align 8
  %compute = getelementptr inbounds nuw %Foo.1, ptr %foo, i32 0, i32 0
  store ptr @__anonymous_function_0, ptr %compute, align 8
  %0 = getelementptr inbounds nuw %Foo.2, ptr %foo, i32 0, i32 0
  %compute1 = load ptr, ptr %0, align 8
  %1 = call i32 %compute1(i32 77)
  ret i32 %1
}

define i32 @__anonymous_function_0(i32 %0) {
body:
  %x = alloca i32, align 4
  store i32 %0, ptr %x, align 4
  %1 = load i32, ptr %x, align 4
  %addtmp = add i32 %1, 2
  ret i32 %addtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 79
  }
}
```