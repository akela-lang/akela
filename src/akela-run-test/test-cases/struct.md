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
const array

```akela
const a: [6 const]Nat8 = "hello"
a[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [6 x i8], align 1
  %lhstmp = getelementptr inbounds i8, ptr %a, i64 0
  %rhs = load i8, ptr @.str, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp1 = getelementptr inbounds i8, ptr %a, i64 1
  %rhs2 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 1), align 1
  store i8 %rhs2, ptr %lhstmp1, align 1
  %lhstmp3 = getelementptr inbounds i8, ptr %a, i64 2
  %rhs4 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 2), align 1
  store i8 %rhs4, ptr %lhstmp3, align 1
  %lhstmp5 = getelementptr inbounds i8, ptr %a, i64 3
  %rhs6 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 3), align 1
  store i8 %rhs6, ptr %lhstmp5, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %a, i64 4
  %rhs8 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 4), align 1
  store i8 %rhs8, ptr %lhstmp7, align 1
  %lhstmp9 = getelementptr inbounds i8, ptr %a, i64 5
  %rhs10 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 5), align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 0
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 104
  }
}
```

## Test
const array 2

```akela
const a: [6 const]Nat8 = "John"
a[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [5 x i8] c"John\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [6 x i8], align 1
  %lhstmp = getelementptr inbounds i8, ptr %a, i64 0
  %rhs = load i8, ptr @.str, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp1 = getelementptr inbounds i8, ptr %a, i64 1
  %rhs2 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 1), align 1
  store i8 %rhs2, ptr %lhstmp1, align 1
  %lhstmp3 = getelementptr inbounds i8, ptr %a, i64 2
  %rhs4 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 2), align 1
  store i8 %rhs4, ptr %lhstmp3, align 1
  %lhstmp5 = getelementptr inbounds i8, ptr %a, i64 3
  %rhs6 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 3), align 1
  store i8 %rhs6, ptr %lhstmp5, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %a, i64 4
  %rhs8 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 4), align 1
  store i8 %rhs8, ptr %lhstmp7, align 1
  %lhstmp9 = getelementptr inbounds i8, ptr %a, i64 5
  %rhs10 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 5), align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 0
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
array const smaller to bigger

```akela
const a: [100 const]Nat8 = "John"
a[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [5 x i8] c"John\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [100 x i8], align 1
  %lhstmp = getelementptr inbounds i8, ptr %a, i64 0
  %rhs = load i8, ptr @.str, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp1 = getelementptr inbounds i8, ptr %a, i64 1
  %rhs2 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 1), align 1
  store i8 %rhs2, ptr %lhstmp1, align 1
  %lhstmp3 = getelementptr inbounds i8, ptr %a, i64 2
  %rhs4 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 2), align 1
  store i8 %rhs4, ptr %lhstmp3, align 1
  %lhstmp5 = getelementptr inbounds i8, ptr %a, i64 3
  %rhs6 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 3), align 1
  store i8 %rhs6, ptr %lhstmp5, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %a, i64 4
  %rhs8 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 4), align 1
  store i8 %rhs8, ptr %lhstmp7, align 1
  %lhstmp9 = getelementptr inbounds i8, ptr %a, i64 5
  %rhs10 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 5), align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  %lhstmp11 = getelementptr inbounds i8, ptr %a, i64 6
  %rhs12 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 6), align 1
  store i8 %rhs12, ptr %lhstmp11, align 1
  %lhstmp13 = getelementptr inbounds i8, ptr %a, i64 7
  %rhs14 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 7), align 1
  store i8 %rhs14, ptr %lhstmp13, align 1
  %lhstmp15 = getelementptr inbounds i8, ptr %a, i64 8
  %rhs16 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 8), align 1
  store i8 %rhs16, ptr %lhstmp15, align 1
  %lhstmp17 = getelementptr inbounds i8, ptr %a, i64 9
  %rhs18 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 9), align 1
  store i8 %rhs18, ptr %lhstmp17, align 1
  %lhstmp19 = getelementptr inbounds i8, ptr %a, i64 10
  %rhs20 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 10), align 1
  store i8 %rhs20, ptr %lhstmp19, align 1
  %lhstmp21 = getelementptr inbounds i8, ptr %a, i64 11
  %rhs22 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 11), align 1
  store i8 %rhs22, ptr %lhstmp21, align 1
  %lhstmp23 = getelementptr inbounds i8, ptr %a, i64 12
  %rhs24 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 12), align 1
  store i8 %rhs24, ptr %lhstmp23, align 1
  %lhstmp25 = getelementptr inbounds i8, ptr %a, i64 13
  %rhs26 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 13), align 1
  store i8 %rhs26, ptr %lhstmp25, align 1
  %lhstmp27 = getelementptr inbounds i8, ptr %a, i64 14
  %rhs28 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 14), align 1
  store i8 %rhs28, ptr %lhstmp27, align 1
  %lhstmp29 = getelementptr inbounds i8, ptr %a, i64 15
  %rhs30 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 15), align 1
  store i8 %rhs30, ptr %lhstmp29, align 1
  %lhstmp31 = getelementptr inbounds i8, ptr %a, i64 16
  %rhs32 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 16), align 1
  store i8 %rhs32, ptr %lhstmp31, align 1
  %lhstmp33 = getelementptr inbounds i8, ptr %a, i64 17
  %rhs34 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 17), align 1
  store i8 %rhs34, ptr %lhstmp33, align 1
  %lhstmp35 = getelementptr inbounds i8, ptr %a, i64 18
  %rhs36 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 18), align 1
  store i8 %rhs36, ptr %lhstmp35, align 1
  %lhstmp37 = getelementptr inbounds i8, ptr %a, i64 19
  %rhs38 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 19), align 1
  store i8 %rhs38, ptr %lhstmp37, align 1
  %lhstmp39 = getelementptr inbounds i8, ptr %a, i64 20
  %rhs40 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 20), align 1
  store i8 %rhs40, ptr %lhstmp39, align 1
  %lhstmp41 = getelementptr inbounds i8, ptr %a, i64 21
  %rhs42 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 21), align 1
  store i8 %rhs42, ptr %lhstmp41, align 1
  %lhstmp43 = getelementptr inbounds i8, ptr %a, i64 22
  %rhs44 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 22), align 1
  store i8 %rhs44, ptr %lhstmp43, align 1
  %lhstmp45 = getelementptr inbounds i8, ptr %a, i64 23
  %rhs46 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 23), align 1
  store i8 %rhs46, ptr %lhstmp45, align 1
  %lhstmp47 = getelementptr inbounds i8, ptr %a, i64 24
  %rhs48 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 24), align 1
  store i8 %rhs48, ptr %lhstmp47, align 1
  %lhstmp49 = getelementptr inbounds i8, ptr %a, i64 25
  %rhs50 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 25), align 1
  store i8 %rhs50, ptr %lhstmp49, align 1
  %lhstmp51 = getelementptr inbounds i8, ptr %a, i64 26
  %rhs52 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 26), align 1
  store i8 %rhs52, ptr %lhstmp51, align 1
  %lhstmp53 = getelementptr inbounds i8, ptr %a, i64 27
  %rhs54 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 27), align 1
  store i8 %rhs54, ptr %lhstmp53, align 1
  %lhstmp55 = getelementptr inbounds i8, ptr %a, i64 28
  %rhs56 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 28), align 1
  store i8 %rhs56, ptr %lhstmp55, align 1
  %lhstmp57 = getelementptr inbounds i8, ptr %a, i64 29
  %rhs58 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 29), align 1
  store i8 %rhs58, ptr %lhstmp57, align 1
  %lhstmp59 = getelementptr inbounds i8, ptr %a, i64 30
  %rhs60 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 30), align 1
  store i8 %rhs60, ptr %lhstmp59, align 1
  %lhstmp61 = getelementptr inbounds i8, ptr %a, i64 31
  %rhs62 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 31), align 1
  store i8 %rhs62, ptr %lhstmp61, align 1
  %lhstmp63 = getelementptr inbounds i8, ptr %a, i64 32
  %rhs64 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 32), align 1
  store i8 %rhs64, ptr %lhstmp63, align 1
  %lhstmp65 = getelementptr inbounds i8, ptr %a, i64 33
  %rhs66 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 33), align 1
  store i8 %rhs66, ptr %lhstmp65, align 1
  %lhstmp67 = getelementptr inbounds i8, ptr %a, i64 34
  %rhs68 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 34), align 1
  store i8 %rhs68, ptr %lhstmp67, align 1
  %lhstmp69 = getelementptr inbounds i8, ptr %a, i64 35
  %rhs70 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 35), align 1
  store i8 %rhs70, ptr %lhstmp69, align 1
  %lhstmp71 = getelementptr inbounds i8, ptr %a, i64 36
  %rhs72 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 36), align 1
  store i8 %rhs72, ptr %lhstmp71, align 1
  %lhstmp73 = getelementptr inbounds i8, ptr %a, i64 37
  %rhs74 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 37), align 1
  store i8 %rhs74, ptr %lhstmp73, align 1
  %lhstmp75 = getelementptr inbounds i8, ptr %a, i64 38
  %rhs76 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 38), align 1
  store i8 %rhs76, ptr %lhstmp75, align 1
  %lhstmp77 = getelementptr inbounds i8, ptr %a, i64 39
  %rhs78 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 39), align 1
  store i8 %rhs78, ptr %lhstmp77, align 1
  %lhstmp79 = getelementptr inbounds i8, ptr %a, i64 40
  %rhs80 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 40), align 1
  store i8 %rhs80, ptr %lhstmp79, align 1
  %lhstmp81 = getelementptr inbounds i8, ptr %a, i64 41
  %rhs82 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 41), align 1
  store i8 %rhs82, ptr %lhstmp81, align 1
  %lhstmp83 = getelementptr inbounds i8, ptr %a, i64 42
  %rhs84 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 42), align 1
  store i8 %rhs84, ptr %lhstmp83, align 1
  %lhstmp85 = getelementptr inbounds i8, ptr %a, i64 43
  %rhs86 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 43), align 1
  store i8 %rhs86, ptr %lhstmp85, align 1
  %lhstmp87 = getelementptr inbounds i8, ptr %a, i64 44
  %rhs88 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 44), align 1
  store i8 %rhs88, ptr %lhstmp87, align 1
  %lhstmp89 = getelementptr inbounds i8, ptr %a, i64 45
  %rhs90 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 45), align 1
  store i8 %rhs90, ptr %lhstmp89, align 1
  %lhstmp91 = getelementptr inbounds i8, ptr %a, i64 46
  %rhs92 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 46), align 1
  store i8 %rhs92, ptr %lhstmp91, align 1
  %lhstmp93 = getelementptr inbounds i8, ptr %a, i64 47
  %rhs94 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 47), align 1
  store i8 %rhs94, ptr %lhstmp93, align 1
  %lhstmp95 = getelementptr inbounds i8, ptr %a, i64 48
  %rhs96 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 48), align 1
  store i8 %rhs96, ptr %lhstmp95, align 1
  %lhstmp97 = getelementptr inbounds i8, ptr %a, i64 49
  %rhs98 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 49), align 1
  store i8 %rhs98, ptr %lhstmp97, align 1
  %lhstmp99 = getelementptr inbounds i8, ptr %a, i64 50
  %rhs100 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 50), align 1
  store i8 %rhs100, ptr %lhstmp99, align 1
  %lhstmp101 = getelementptr inbounds i8, ptr %a, i64 51
  %rhs102 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 51), align 1
  store i8 %rhs102, ptr %lhstmp101, align 1
  %lhstmp103 = getelementptr inbounds i8, ptr %a, i64 52
  %rhs104 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 52), align 1
  store i8 %rhs104, ptr %lhstmp103, align 1
  %lhstmp105 = getelementptr inbounds i8, ptr %a, i64 53
  %rhs106 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 53), align 1
  store i8 %rhs106, ptr %lhstmp105, align 1
  %lhstmp107 = getelementptr inbounds i8, ptr %a, i64 54
  %rhs108 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 54), align 1
  store i8 %rhs108, ptr %lhstmp107, align 1
  %lhstmp109 = getelementptr inbounds i8, ptr %a, i64 55
  %rhs110 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 55), align 1
  store i8 %rhs110, ptr %lhstmp109, align 1
  %lhstmp111 = getelementptr inbounds i8, ptr %a, i64 56
  %rhs112 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 56), align 1
  store i8 %rhs112, ptr %lhstmp111, align 1
  %lhstmp113 = getelementptr inbounds i8, ptr %a, i64 57
  %rhs114 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 57), align 1
  store i8 %rhs114, ptr %lhstmp113, align 1
  %lhstmp115 = getelementptr inbounds i8, ptr %a, i64 58
  %rhs116 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 58), align 1
  store i8 %rhs116, ptr %lhstmp115, align 1
  %lhstmp117 = getelementptr inbounds i8, ptr %a, i64 59
  %rhs118 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 59), align 1
  store i8 %rhs118, ptr %lhstmp117, align 1
  %lhstmp119 = getelementptr inbounds i8, ptr %a, i64 60
  %rhs120 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 60), align 1
  store i8 %rhs120, ptr %lhstmp119, align 1
  %lhstmp121 = getelementptr inbounds i8, ptr %a, i64 61
  %rhs122 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 61), align 1
  store i8 %rhs122, ptr %lhstmp121, align 1
  %lhstmp123 = getelementptr inbounds i8, ptr %a, i64 62
  %rhs124 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 62), align 1
  store i8 %rhs124, ptr %lhstmp123, align 1
  %lhstmp125 = getelementptr inbounds i8, ptr %a, i64 63
  %rhs126 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 63), align 1
  store i8 %rhs126, ptr %lhstmp125, align 1
  %lhstmp127 = getelementptr inbounds i8, ptr %a, i64 64
  %rhs128 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 64), align 1
  store i8 %rhs128, ptr %lhstmp127, align 1
  %lhstmp129 = getelementptr inbounds i8, ptr %a, i64 65
  %rhs130 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 65), align 1
  store i8 %rhs130, ptr %lhstmp129, align 1
  %lhstmp131 = getelementptr inbounds i8, ptr %a, i64 66
  %rhs132 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 66), align 1
  store i8 %rhs132, ptr %lhstmp131, align 1
  %lhstmp133 = getelementptr inbounds i8, ptr %a, i64 67
  %rhs134 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 67), align 1
  store i8 %rhs134, ptr %lhstmp133, align 1
  %lhstmp135 = getelementptr inbounds i8, ptr %a, i64 68
  %rhs136 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 68), align 1
  store i8 %rhs136, ptr %lhstmp135, align 1
  %lhstmp137 = getelementptr inbounds i8, ptr %a, i64 69
  %rhs138 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 69), align 1
  store i8 %rhs138, ptr %lhstmp137, align 1
  %lhstmp139 = getelementptr inbounds i8, ptr %a, i64 70
  %rhs140 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 70), align 1
  store i8 %rhs140, ptr %lhstmp139, align 1
  %lhstmp141 = getelementptr inbounds i8, ptr %a, i64 71
  %rhs142 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 71), align 1
  store i8 %rhs142, ptr %lhstmp141, align 1
  %lhstmp143 = getelementptr inbounds i8, ptr %a, i64 72
  %rhs144 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 72), align 1
  store i8 %rhs144, ptr %lhstmp143, align 1
  %lhstmp145 = getelementptr inbounds i8, ptr %a, i64 73
  %rhs146 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 73), align 1
  store i8 %rhs146, ptr %lhstmp145, align 1
  %lhstmp147 = getelementptr inbounds i8, ptr %a, i64 74
  %rhs148 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 74), align 1
  store i8 %rhs148, ptr %lhstmp147, align 1
  %lhstmp149 = getelementptr inbounds i8, ptr %a, i64 75
  %rhs150 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 75), align 1
  store i8 %rhs150, ptr %lhstmp149, align 1
  %lhstmp151 = getelementptr inbounds i8, ptr %a, i64 76
  %rhs152 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 76), align 1
  store i8 %rhs152, ptr %lhstmp151, align 1
  %lhstmp153 = getelementptr inbounds i8, ptr %a, i64 77
  %rhs154 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 77), align 1
  store i8 %rhs154, ptr %lhstmp153, align 1
  %lhstmp155 = getelementptr inbounds i8, ptr %a, i64 78
  %rhs156 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 78), align 1
  store i8 %rhs156, ptr %lhstmp155, align 1
  %lhstmp157 = getelementptr inbounds i8, ptr %a, i64 79
  %rhs158 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 79), align 1
  store i8 %rhs158, ptr %lhstmp157, align 1
  %lhstmp159 = getelementptr inbounds i8, ptr %a, i64 80
  %rhs160 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 80), align 1
  store i8 %rhs160, ptr %lhstmp159, align 1
  %lhstmp161 = getelementptr inbounds i8, ptr %a, i64 81
  %rhs162 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 81), align 1
  store i8 %rhs162, ptr %lhstmp161, align 1
  %lhstmp163 = getelementptr inbounds i8, ptr %a, i64 82
  %rhs164 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 82), align 1
  store i8 %rhs164, ptr %lhstmp163, align 1
  %lhstmp165 = getelementptr inbounds i8, ptr %a, i64 83
  %rhs166 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 83), align 1
  store i8 %rhs166, ptr %lhstmp165, align 1
  %lhstmp167 = getelementptr inbounds i8, ptr %a, i64 84
  %rhs168 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 84), align 1
  store i8 %rhs168, ptr %lhstmp167, align 1
  %lhstmp169 = getelementptr inbounds i8, ptr %a, i64 85
  %rhs170 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 85), align 1
  store i8 %rhs170, ptr %lhstmp169, align 1
  %lhstmp171 = getelementptr inbounds i8, ptr %a, i64 86
  %rhs172 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 86), align 1
  store i8 %rhs172, ptr %lhstmp171, align 1
  %lhstmp173 = getelementptr inbounds i8, ptr %a, i64 87
  %rhs174 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 87), align 1
  store i8 %rhs174, ptr %lhstmp173, align 1
  %lhstmp175 = getelementptr inbounds i8, ptr %a, i64 88
  %rhs176 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 88), align 1
  store i8 %rhs176, ptr %lhstmp175, align 1
  %lhstmp177 = getelementptr inbounds i8, ptr %a, i64 89
  %rhs178 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 89), align 1
  store i8 %rhs178, ptr %lhstmp177, align 1
  %lhstmp179 = getelementptr inbounds i8, ptr %a, i64 90
  %rhs180 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 90), align 1
  store i8 %rhs180, ptr %lhstmp179, align 1
  %lhstmp181 = getelementptr inbounds i8, ptr %a, i64 91
  %rhs182 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 91), align 1
  store i8 %rhs182, ptr %lhstmp181, align 1
  %lhstmp183 = getelementptr inbounds i8, ptr %a, i64 92
  %rhs184 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 92), align 1
  store i8 %rhs184, ptr %lhstmp183, align 1
  %lhstmp185 = getelementptr inbounds i8, ptr %a, i64 93
  %rhs186 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 93), align 1
  store i8 %rhs186, ptr %lhstmp185, align 1
  %lhstmp187 = getelementptr inbounds i8, ptr %a, i64 94
  %rhs188 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 94), align 1
  store i8 %rhs188, ptr %lhstmp187, align 1
  %lhstmp189 = getelementptr inbounds i8, ptr %a, i64 95
  %rhs190 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 95), align 1
  store i8 %rhs190, ptr %lhstmp189, align 1
  %lhstmp191 = getelementptr inbounds i8, ptr %a, i64 96
  %rhs192 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 96), align 1
  store i8 %rhs192, ptr %lhstmp191, align 1
  %lhstmp193 = getelementptr inbounds i8, ptr %a, i64 97
  %rhs194 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 97), align 1
  store i8 %rhs194, ptr %lhstmp193, align 1
  %lhstmp195 = getelementptr inbounds i8, ptr %a, i64 98
  %rhs196 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 98), align 1
  store i8 %rhs196, ptr %lhstmp195, align 1
  %lhstmp197 = getelementptr inbounds i8, ptr %a, i64 99
  %rhs198 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 99), align 1
  store i8 %rhs198, ptr %lhstmp197, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 0
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
const array different sizes 2

```akela
const a: [100 const]Nat8 = "John"
a[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [5 x i8] c"John\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [100 x i8], align 1
  %lhstmp = getelementptr inbounds i8, ptr %a, i64 0
  %rhs = load i8, ptr @.str, align 1
  store i8 %rhs, ptr %lhstmp, align 1
  %lhstmp1 = getelementptr inbounds i8, ptr %a, i64 1
  %rhs2 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 1), align 1
  store i8 %rhs2, ptr %lhstmp1, align 1
  %lhstmp3 = getelementptr inbounds i8, ptr %a, i64 2
  %rhs4 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 2), align 1
  store i8 %rhs4, ptr %lhstmp3, align 1
  %lhstmp5 = getelementptr inbounds i8, ptr %a, i64 3
  %rhs6 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 3), align 1
  store i8 %rhs6, ptr %lhstmp5, align 1
  %lhstmp7 = getelementptr inbounds i8, ptr %a, i64 4
  %rhs8 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 4), align 1
  store i8 %rhs8, ptr %lhstmp7, align 1
  %lhstmp9 = getelementptr inbounds i8, ptr %a, i64 5
  %rhs10 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 5), align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  %lhstmp11 = getelementptr inbounds i8, ptr %a, i64 6
  %rhs12 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 6), align 1
  store i8 %rhs12, ptr %lhstmp11, align 1
  %lhstmp13 = getelementptr inbounds i8, ptr %a, i64 7
  %rhs14 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 7), align 1
  store i8 %rhs14, ptr %lhstmp13, align 1
  %lhstmp15 = getelementptr inbounds i8, ptr %a, i64 8
  %rhs16 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 8), align 1
  store i8 %rhs16, ptr %lhstmp15, align 1
  %lhstmp17 = getelementptr inbounds i8, ptr %a, i64 9
  %rhs18 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 9), align 1
  store i8 %rhs18, ptr %lhstmp17, align 1
  %lhstmp19 = getelementptr inbounds i8, ptr %a, i64 10
  %rhs20 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 10), align 1
  store i8 %rhs20, ptr %lhstmp19, align 1
  %lhstmp21 = getelementptr inbounds i8, ptr %a, i64 11
  %rhs22 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 11), align 1
  store i8 %rhs22, ptr %lhstmp21, align 1
  %lhstmp23 = getelementptr inbounds i8, ptr %a, i64 12
  %rhs24 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 12), align 1
  store i8 %rhs24, ptr %lhstmp23, align 1
  %lhstmp25 = getelementptr inbounds i8, ptr %a, i64 13
  %rhs26 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 13), align 1
  store i8 %rhs26, ptr %lhstmp25, align 1
  %lhstmp27 = getelementptr inbounds i8, ptr %a, i64 14
  %rhs28 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 14), align 1
  store i8 %rhs28, ptr %lhstmp27, align 1
  %lhstmp29 = getelementptr inbounds i8, ptr %a, i64 15
  %rhs30 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 15), align 1
  store i8 %rhs30, ptr %lhstmp29, align 1
  %lhstmp31 = getelementptr inbounds i8, ptr %a, i64 16
  %rhs32 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 16), align 1
  store i8 %rhs32, ptr %lhstmp31, align 1
  %lhstmp33 = getelementptr inbounds i8, ptr %a, i64 17
  %rhs34 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 17), align 1
  store i8 %rhs34, ptr %lhstmp33, align 1
  %lhstmp35 = getelementptr inbounds i8, ptr %a, i64 18
  %rhs36 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 18), align 1
  store i8 %rhs36, ptr %lhstmp35, align 1
  %lhstmp37 = getelementptr inbounds i8, ptr %a, i64 19
  %rhs38 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 19), align 1
  store i8 %rhs38, ptr %lhstmp37, align 1
  %lhstmp39 = getelementptr inbounds i8, ptr %a, i64 20
  %rhs40 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 20), align 1
  store i8 %rhs40, ptr %lhstmp39, align 1
  %lhstmp41 = getelementptr inbounds i8, ptr %a, i64 21
  %rhs42 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 21), align 1
  store i8 %rhs42, ptr %lhstmp41, align 1
  %lhstmp43 = getelementptr inbounds i8, ptr %a, i64 22
  %rhs44 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 22), align 1
  store i8 %rhs44, ptr %lhstmp43, align 1
  %lhstmp45 = getelementptr inbounds i8, ptr %a, i64 23
  %rhs46 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 23), align 1
  store i8 %rhs46, ptr %lhstmp45, align 1
  %lhstmp47 = getelementptr inbounds i8, ptr %a, i64 24
  %rhs48 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 24), align 1
  store i8 %rhs48, ptr %lhstmp47, align 1
  %lhstmp49 = getelementptr inbounds i8, ptr %a, i64 25
  %rhs50 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 25), align 1
  store i8 %rhs50, ptr %lhstmp49, align 1
  %lhstmp51 = getelementptr inbounds i8, ptr %a, i64 26
  %rhs52 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 26), align 1
  store i8 %rhs52, ptr %lhstmp51, align 1
  %lhstmp53 = getelementptr inbounds i8, ptr %a, i64 27
  %rhs54 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 27), align 1
  store i8 %rhs54, ptr %lhstmp53, align 1
  %lhstmp55 = getelementptr inbounds i8, ptr %a, i64 28
  %rhs56 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 28), align 1
  store i8 %rhs56, ptr %lhstmp55, align 1
  %lhstmp57 = getelementptr inbounds i8, ptr %a, i64 29
  %rhs58 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 29), align 1
  store i8 %rhs58, ptr %lhstmp57, align 1
  %lhstmp59 = getelementptr inbounds i8, ptr %a, i64 30
  %rhs60 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 30), align 1
  store i8 %rhs60, ptr %lhstmp59, align 1
  %lhstmp61 = getelementptr inbounds i8, ptr %a, i64 31
  %rhs62 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 31), align 1
  store i8 %rhs62, ptr %lhstmp61, align 1
  %lhstmp63 = getelementptr inbounds i8, ptr %a, i64 32
  %rhs64 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 32), align 1
  store i8 %rhs64, ptr %lhstmp63, align 1
  %lhstmp65 = getelementptr inbounds i8, ptr %a, i64 33
  %rhs66 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 33), align 1
  store i8 %rhs66, ptr %lhstmp65, align 1
  %lhstmp67 = getelementptr inbounds i8, ptr %a, i64 34
  %rhs68 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 34), align 1
  store i8 %rhs68, ptr %lhstmp67, align 1
  %lhstmp69 = getelementptr inbounds i8, ptr %a, i64 35
  %rhs70 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 35), align 1
  store i8 %rhs70, ptr %lhstmp69, align 1
  %lhstmp71 = getelementptr inbounds i8, ptr %a, i64 36
  %rhs72 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 36), align 1
  store i8 %rhs72, ptr %lhstmp71, align 1
  %lhstmp73 = getelementptr inbounds i8, ptr %a, i64 37
  %rhs74 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 37), align 1
  store i8 %rhs74, ptr %lhstmp73, align 1
  %lhstmp75 = getelementptr inbounds i8, ptr %a, i64 38
  %rhs76 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 38), align 1
  store i8 %rhs76, ptr %lhstmp75, align 1
  %lhstmp77 = getelementptr inbounds i8, ptr %a, i64 39
  %rhs78 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 39), align 1
  store i8 %rhs78, ptr %lhstmp77, align 1
  %lhstmp79 = getelementptr inbounds i8, ptr %a, i64 40
  %rhs80 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 40), align 1
  store i8 %rhs80, ptr %lhstmp79, align 1
  %lhstmp81 = getelementptr inbounds i8, ptr %a, i64 41
  %rhs82 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 41), align 1
  store i8 %rhs82, ptr %lhstmp81, align 1
  %lhstmp83 = getelementptr inbounds i8, ptr %a, i64 42
  %rhs84 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 42), align 1
  store i8 %rhs84, ptr %lhstmp83, align 1
  %lhstmp85 = getelementptr inbounds i8, ptr %a, i64 43
  %rhs86 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 43), align 1
  store i8 %rhs86, ptr %lhstmp85, align 1
  %lhstmp87 = getelementptr inbounds i8, ptr %a, i64 44
  %rhs88 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 44), align 1
  store i8 %rhs88, ptr %lhstmp87, align 1
  %lhstmp89 = getelementptr inbounds i8, ptr %a, i64 45
  %rhs90 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 45), align 1
  store i8 %rhs90, ptr %lhstmp89, align 1
  %lhstmp91 = getelementptr inbounds i8, ptr %a, i64 46
  %rhs92 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 46), align 1
  store i8 %rhs92, ptr %lhstmp91, align 1
  %lhstmp93 = getelementptr inbounds i8, ptr %a, i64 47
  %rhs94 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 47), align 1
  store i8 %rhs94, ptr %lhstmp93, align 1
  %lhstmp95 = getelementptr inbounds i8, ptr %a, i64 48
  %rhs96 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 48), align 1
  store i8 %rhs96, ptr %lhstmp95, align 1
  %lhstmp97 = getelementptr inbounds i8, ptr %a, i64 49
  %rhs98 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 49), align 1
  store i8 %rhs98, ptr %lhstmp97, align 1
  %lhstmp99 = getelementptr inbounds i8, ptr %a, i64 50
  %rhs100 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 50), align 1
  store i8 %rhs100, ptr %lhstmp99, align 1
  %lhstmp101 = getelementptr inbounds i8, ptr %a, i64 51
  %rhs102 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 51), align 1
  store i8 %rhs102, ptr %lhstmp101, align 1
  %lhstmp103 = getelementptr inbounds i8, ptr %a, i64 52
  %rhs104 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 52), align 1
  store i8 %rhs104, ptr %lhstmp103, align 1
  %lhstmp105 = getelementptr inbounds i8, ptr %a, i64 53
  %rhs106 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 53), align 1
  store i8 %rhs106, ptr %lhstmp105, align 1
  %lhstmp107 = getelementptr inbounds i8, ptr %a, i64 54
  %rhs108 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 54), align 1
  store i8 %rhs108, ptr %lhstmp107, align 1
  %lhstmp109 = getelementptr inbounds i8, ptr %a, i64 55
  %rhs110 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 55), align 1
  store i8 %rhs110, ptr %lhstmp109, align 1
  %lhstmp111 = getelementptr inbounds i8, ptr %a, i64 56
  %rhs112 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 56), align 1
  store i8 %rhs112, ptr %lhstmp111, align 1
  %lhstmp113 = getelementptr inbounds i8, ptr %a, i64 57
  %rhs114 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 57), align 1
  store i8 %rhs114, ptr %lhstmp113, align 1
  %lhstmp115 = getelementptr inbounds i8, ptr %a, i64 58
  %rhs116 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 58), align 1
  store i8 %rhs116, ptr %lhstmp115, align 1
  %lhstmp117 = getelementptr inbounds i8, ptr %a, i64 59
  %rhs118 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 59), align 1
  store i8 %rhs118, ptr %lhstmp117, align 1
  %lhstmp119 = getelementptr inbounds i8, ptr %a, i64 60
  %rhs120 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 60), align 1
  store i8 %rhs120, ptr %lhstmp119, align 1
  %lhstmp121 = getelementptr inbounds i8, ptr %a, i64 61
  %rhs122 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 61), align 1
  store i8 %rhs122, ptr %lhstmp121, align 1
  %lhstmp123 = getelementptr inbounds i8, ptr %a, i64 62
  %rhs124 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 62), align 1
  store i8 %rhs124, ptr %lhstmp123, align 1
  %lhstmp125 = getelementptr inbounds i8, ptr %a, i64 63
  %rhs126 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 63), align 1
  store i8 %rhs126, ptr %lhstmp125, align 1
  %lhstmp127 = getelementptr inbounds i8, ptr %a, i64 64
  %rhs128 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 64), align 1
  store i8 %rhs128, ptr %lhstmp127, align 1
  %lhstmp129 = getelementptr inbounds i8, ptr %a, i64 65
  %rhs130 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 65), align 1
  store i8 %rhs130, ptr %lhstmp129, align 1
  %lhstmp131 = getelementptr inbounds i8, ptr %a, i64 66
  %rhs132 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 66), align 1
  store i8 %rhs132, ptr %lhstmp131, align 1
  %lhstmp133 = getelementptr inbounds i8, ptr %a, i64 67
  %rhs134 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 67), align 1
  store i8 %rhs134, ptr %lhstmp133, align 1
  %lhstmp135 = getelementptr inbounds i8, ptr %a, i64 68
  %rhs136 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 68), align 1
  store i8 %rhs136, ptr %lhstmp135, align 1
  %lhstmp137 = getelementptr inbounds i8, ptr %a, i64 69
  %rhs138 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 69), align 1
  store i8 %rhs138, ptr %lhstmp137, align 1
  %lhstmp139 = getelementptr inbounds i8, ptr %a, i64 70
  %rhs140 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 70), align 1
  store i8 %rhs140, ptr %lhstmp139, align 1
  %lhstmp141 = getelementptr inbounds i8, ptr %a, i64 71
  %rhs142 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 71), align 1
  store i8 %rhs142, ptr %lhstmp141, align 1
  %lhstmp143 = getelementptr inbounds i8, ptr %a, i64 72
  %rhs144 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 72), align 1
  store i8 %rhs144, ptr %lhstmp143, align 1
  %lhstmp145 = getelementptr inbounds i8, ptr %a, i64 73
  %rhs146 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 73), align 1
  store i8 %rhs146, ptr %lhstmp145, align 1
  %lhstmp147 = getelementptr inbounds i8, ptr %a, i64 74
  %rhs148 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 74), align 1
  store i8 %rhs148, ptr %lhstmp147, align 1
  %lhstmp149 = getelementptr inbounds i8, ptr %a, i64 75
  %rhs150 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 75), align 1
  store i8 %rhs150, ptr %lhstmp149, align 1
  %lhstmp151 = getelementptr inbounds i8, ptr %a, i64 76
  %rhs152 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 76), align 1
  store i8 %rhs152, ptr %lhstmp151, align 1
  %lhstmp153 = getelementptr inbounds i8, ptr %a, i64 77
  %rhs154 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 77), align 1
  store i8 %rhs154, ptr %lhstmp153, align 1
  %lhstmp155 = getelementptr inbounds i8, ptr %a, i64 78
  %rhs156 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 78), align 1
  store i8 %rhs156, ptr %lhstmp155, align 1
  %lhstmp157 = getelementptr inbounds i8, ptr %a, i64 79
  %rhs158 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 79), align 1
  store i8 %rhs158, ptr %lhstmp157, align 1
  %lhstmp159 = getelementptr inbounds i8, ptr %a, i64 80
  %rhs160 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 80), align 1
  store i8 %rhs160, ptr %lhstmp159, align 1
  %lhstmp161 = getelementptr inbounds i8, ptr %a, i64 81
  %rhs162 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 81), align 1
  store i8 %rhs162, ptr %lhstmp161, align 1
  %lhstmp163 = getelementptr inbounds i8, ptr %a, i64 82
  %rhs164 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 82), align 1
  store i8 %rhs164, ptr %lhstmp163, align 1
  %lhstmp165 = getelementptr inbounds i8, ptr %a, i64 83
  %rhs166 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 83), align 1
  store i8 %rhs166, ptr %lhstmp165, align 1
  %lhstmp167 = getelementptr inbounds i8, ptr %a, i64 84
  %rhs168 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 84), align 1
  store i8 %rhs168, ptr %lhstmp167, align 1
  %lhstmp169 = getelementptr inbounds i8, ptr %a, i64 85
  %rhs170 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 85), align 1
  store i8 %rhs170, ptr %lhstmp169, align 1
  %lhstmp171 = getelementptr inbounds i8, ptr %a, i64 86
  %rhs172 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 86), align 1
  store i8 %rhs172, ptr %lhstmp171, align 1
  %lhstmp173 = getelementptr inbounds i8, ptr %a, i64 87
  %rhs174 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 87), align 1
  store i8 %rhs174, ptr %lhstmp173, align 1
  %lhstmp175 = getelementptr inbounds i8, ptr %a, i64 88
  %rhs176 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 88), align 1
  store i8 %rhs176, ptr %lhstmp175, align 1
  %lhstmp177 = getelementptr inbounds i8, ptr %a, i64 89
  %rhs178 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 89), align 1
  store i8 %rhs178, ptr %lhstmp177, align 1
  %lhstmp179 = getelementptr inbounds i8, ptr %a, i64 90
  %rhs180 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 90), align 1
  store i8 %rhs180, ptr %lhstmp179, align 1
  %lhstmp181 = getelementptr inbounds i8, ptr %a, i64 91
  %rhs182 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 91), align 1
  store i8 %rhs182, ptr %lhstmp181, align 1
  %lhstmp183 = getelementptr inbounds i8, ptr %a, i64 92
  %rhs184 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 92), align 1
  store i8 %rhs184, ptr %lhstmp183, align 1
  %lhstmp185 = getelementptr inbounds i8, ptr %a, i64 93
  %rhs186 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 93), align 1
  store i8 %rhs186, ptr %lhstmp185, align 1
  %lhstmp187 = getelementptr inbounds i8, ptr %a, i64 94
  %rhs188 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 94), align 1
  store i8 %rhs188, ptr %lhstmp187, align 1
  %lhstmp189 = getelementptr inbounds i8, ptr %a, i64 95
  %rhs190 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 95), align 1
  store i8 %rhs190, ptr %lhstmp189, align 1
  %lhstmp191 = getelementptr inbounds i8, ptr %a, i64 96
  %rhs192 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 96), align 1
  store i8 %rhs192, ptr %lhstmp191, align 1
  %lhstmp193 = getelementptr inbounds i8, ptr %a, i64 97
  %rhs194 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 97), align 1
  store i8 %rhs194, ptr %lhstmp193, align 1
  %lhstmp195 = getelementptr inbounds i8, ptr %a, i64 98
  %rhs196 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 98), align 1
  store i8 %rhs196, ptr %lhstmp195, align 1
  %lhstmp197 = getelementptr inbounds i8, ptr %a, i64 99
  %rhs198 = load i8, ptr getelementptr inbounds (i8, ptr @.str, i64 99), align 1
  store i8 %rhs198, ptr %lhstmp197, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 0
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