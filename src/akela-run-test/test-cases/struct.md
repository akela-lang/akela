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

## Test
const array copy

```akela
const a: [100 const]Nat8 = "John"
const b: [100 const]Nat8 = a
b[0]
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
  %b = alloca [100 x i8], align 1
  %lhstmp199 = getelementptr inbounds i8, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i8, ptr %a, i64 0
  %rhs200 = load i8, ptr %rhstmp, align 1
  store i8 %rhs200, ptr %lhstmp199, align 1
  %lhstmp201 = getelementptr inbounds i8, ptr %b, i64 1
  %rhstmp202 = getelementptr inbounds i8, ptr %a, i64 1
  %rhs203 = load i8, ptr %rhstmp202, align 1
  store i8 %rhs203, ptr %lhstmp201, align 1
  %lhstmp204 = getelementptr inbounds i8, ptr %b, i64 2
  %rhstmp205 = getelementptr inbounds i8, ptr %a, i64 2
  %rhs206 = load i8, ptr %rhstmp205, align 1
  store i8 %rhs206, ptr %lhstmp204, align 1
  %lhstmp207 = getelementptr inbounds i8, ptr %b, i64 3
  %rhstmp208 = getelementptr inbounds i8, ptr %a, i64 3
  %rhs209 = load i8, ptr %rhstmp208, align 1
  store i8 %rhs209, ptr %lhstmp207, align 1
  %lhstmp210 = getelementptr inbounds i8, ptr %b, i64 4
  %rhstmp211 = getelementptr inbounds i8, ptr %a, i64 4
  %rhs212 = load i8, ptr %rhstmp211, align 1
  store i8 %rhs212, ptr %lhstmp210, align 1
  %lhstmp213 = getelementptr inbounds i8, ptr %b, i64 5
  %rhstmp214 = getelementptr inbounds i8, ptr %a, i64 5
  %rhs215 = load i8, ptr %rhstmp214, align 1
  store i8 %rhs215, ptr %lhstmp213, align 1
  %lhstmp216 = getelementptr inbounds i8, ptr %b, i64 6
  %rhstmp217 = getelementptr inbounds i8, ptr %a, i64 6
  %rhs218 = load i8, ptr %rhstmp217, align 1
  store i8 %rhs218, ptr %lhstmp216, align 1
  %lhstmp219 = getelementptr inbounds i8, ptr %b, i64 7
  %rhstmp220 = getelementptr inbounds i8, ptr %a, i64 7
  %rhs221 = load i8, ptr %rhstmp220, align 1
  store i8 %rhs221, ptr %lhstmp219, align 1
  %lhstmp222 = getelementptr inbounds i8, ptr %b, i64 8
  %rhstmp223 = getelementptr inbounds i8, ptr %a, i64 8
  %rhs224 = load i8, ptr %rhstmp223, align 1
  store i8 %rhs224, ptr %lhstmp222, align 1
  %lhstmp225 = getelementptr inbounds i8, ptr %b, i64 9
  %rhstmp226 = getelementptr inbounds i8, ptr %a, i64 9
  %rhs227 = load i8, ptr %rhstmp226, align 1
  store i8 %rhs227, ptr %lhstmp225, align 1
  %lhstmp228 = getelementptr inbounds i8, ptr %b, i64 10
  %rhstmp229 = getelementptr inbounds i8, ptr %a, i64 10
  %rhs230 = load i8, ptr %rhstmp229, align 1
  store i8 %rhs230, ptr %lhstmp228, align 1
  %lhstmp231 = getelementptr inbounds i8, ptr %b, i64 11
  %rhstmp232 = getelementptr inbounds i8, ptr %a, i64 11
  %rhs233 = load i8, ptr %rhstmp232, align 1
  store i8 %rhs233, ptr %lhstmp231, align 1
  %lhstmp234 = getelementptr inbounds i8, ptr %b, i64 12
  %rhstmp235 = getelementptr inbounds i8, ptr %a, i64 12
  %rhs236 = load i8, ptr %rhstmp235, align 1
  store i8 %rhs236, ptr %lhstmp234, align 1
  %lhstmp237 = getelementptr inbounds i8, ptr %b, i64 13
  %rhstmp238 = getelementptr inbounds i8, ptr %a, i64 13
  %rhs239 = load i8, ptr %rhstmp238, align 1
  store i8 %rhs239, ptr %lhstmp237, align 1
  %lhstmp240 = getelementptr inbounds i8, ptr %b, i64 14
  %rhstmp241 = getelementptr inbounds i8, ptr %a, i64 14
  %rhs242 = load i8, ptr %rhstmp241, align 1
  store i8 %rhs242, ptr %lhstmp240, align 1
  %lhstmp243 = getelementptr inbounds i8, ptr %b, i64 15
  %rhstmp244 = getelementptr inbounds i8, ptr %a, i64 15
  %rhs245 = load i8, ptr %rhstmp244, align 1
  store i8 %rhs245, ptr %lhstmp243, align 1
  %lhstmp246 = getelementptr inbounds i8, ptr %b, i64 16
  %rhstmp247 = getelementptr inbounds i8, ptr %a, i64 16
  %rhs248 = load i8, ptr %rhstmp247, align 1
  store i8 %rhs248, ptr %lhstmp246, align 1
  %lhstmp249 = getelementptr inbounds i8, ptr %b, i64 17
  %rhstmp250 = getelementptr inbounds i8, ptr %a, i64 17
  %rhs251 = load i8, ptr %rhstmp250, align 1
  store i8 %rhs251, ptr %lhstmp249, align 1
  %lhstmp252 = getelementptr inbounds i8, ptr %b, i64 18
  %rhstmp253 = getelementptr inbounds i8, ptr %a, i64 18
  %rhs254 = load i8, ptr %rhstmp253, align 1
  store i8 %rhs254, ptr %lhstmp252, align 1
  %lhstmp255 = getelementptr inbounds i8, ptr %b, i64 19
  %rhstmp256 = getelementptr inbounds i8, ptr %a, i64 19
  %rhs257 = load i8, ptr %rhstmp256, align 1
  store i8 %rhs257, ptr %lhstmp255, align 1
  %lhstmp258 = getelementptr inbounds i8, ptr %b, i64 20
  %rhstmp259 = getelementptr inbounds i8, ptr %a, i64 20
  %rhs260 = load i8, ptr %rhstmp259, align 1
  store i8 %rhs260, ptr %lhstmp258, align 1
  %lhstmp261 = getelementptr inbounds i8, ptr %b, i64 21
  %rhstmp262 = getelementptr inbounds i8, ptr %a, i64 21
  %rhs263 = load i8, ptr %rhstmp262, align 1
  store i8 %rhs263, ptr %lhstmp261, align 1
  %lhstmp264 = getelementptr inbounds i8, ptr %b, i64 22
  %rhstmp265 = getelementptr inbounds i8, ptr %a, i64 22
  %rhs266 = load i8, ptr %rhstmp265, align 1
  store i8 %rhs266, ptr %lhstmp264, align 1
  %lhstmp267 = getelementptr inbounds i8, ptr %b, i64 23
  %rhstmp268 = getelementptr inbounds i8, ptr %a, i64 23
  %rhs269 = load i8, ptr %rhstmp268, align 1
  store i8 %rhs269, ptr %lhstmp267, align 1
  %lhstmp270 = getelementptr inbounds i8, ptr %b, i64 24
  %rhstmp271 = getelementptr inbounds i8, ptr %a, i64 24
  %rhs272 = load i8, ptr %rhstmp271, align 1
  store i8 %rhs272, ptr %lhstmp270, align 1
  %lhstmp273 = getelementptr inbounds i8, ptr %b, i64 25
  %rhstmp274 = getelementptr inbounds i8, ptr %a, i64 25
  %rhs275 = load i8, ptr %rhstmp274, align 1
  store i8 %rhs275, ptr %lhstmp273, align 1
  %lhstmp276 = getelementptr inbounds i8, ptr %b, i64 26
  %rhstmp277 = getelementptr inbounds i8, ptr %a, i64 26
  %rhs278 = load i8, ptr %rhstmp277, align 1
  store i8 %rhs278, ptr %lhstmp276, align 1
  %lhstmp279 = getelementptr inbounds i8, ptr %b, i64 27
  %rhstmp280 = getelementptr inbounds i8, ptr %a, i64 27
  %rhs281 = load i8, ptr %rhstmp280, align 1
  store i8 %rhs281, ptr %lhstmp279, align 1
  %lhstmp282 = getelementptr inbounds i8, ptr %b, i64 28
  %rhstmp283 = getelementptr inbounds i8, ptr %a, i64 28
  %rhs284 = load i8, ptr %rhstmp283, align 1
  store i8 %rhs284, ptr %lhstmp282, align 1
  %lhstmp285 = getelementptr inbounds i8, ptr %b, i64 29
  %rhstmp286 = getelementptr inbounds i8, ptr %a, i64 29
  %rhs287 = load i8, ptr %rhstmp286, align 1
  store i8 %rhs287, ptr %lhstmp285, align 1
  %lhstmp288 = getelementptr inbounds i8, ptr %b, i64 30
  %rhstmp289 = getelementptr inbounds i8, ptr %a, i64 30
  %rhs290 = load i8, ptr %rhstmp289, align 1
  store i8 %rhs290, ptr %lhstmp288, align 1
  %lhstmp291 = getelementptr inbounds i8, ptr %b, i64 31
  %rhstmp292 = getelementptr inbounds i8, ptr %a, i64 31
  %rhs293 = load i8, ptr %rhstmp292, align 1
  store i8 %rhs293, ptr %lhstmp291, align 1
  %lhstmp294 = getelementptr inbounds i8, ptr %b, i64 32
  %rhstmp295 = getelementptr inbounds i8, ptr %a, i64 32
  %rhs296 = load i8, ptr %rhstmp295, align 1
  store i8 %rhs296, ptr %lhstmp294, align 1
  %lhstmp297 = getelementptr inbounds i8, ptr %b, i64 33
  %rhstmp298 = getelementptr inbounds i8, ptr %a, i64 33
  %rhs299 = load i8, ptr %rhstmp298, align 1
  store i8 %rhs299, ptr %lhstmp297, align 1
  %lhstmp300 = getelementptr inbounds i8, ptr %b, i64 34
  %rhstmp301 = getelementptr inbounds i8, ptr %a, i64 34
  %rhs302 = load i8, ptr %rhstmp301, align 1
  store i8 %rhs302, ptr %lhstmp300, align 1
  %lhstmp303 = getelementptr inbounds i8, ptr %b, i64 35
  %rhstmp304 = getelementptr inbounds i8, ptr %a, i64 35
  %rhs305 = load i8, ptr %rhstmp304, align 1
  store i8 %rhs305, ptr %lhstmp303, align 1
  %lhstmp306 = getelementptr inbounds i8, ptr %b, i64 36
  %rhstmp307 = getelementptr inbounds i8, ptr %a, i64 36
  %rhs308 = load i8, ptr %rhstmp307, align 1
  store i8 %rhs308, ptr %lhstmp306, align 1
  %lhstmp309 = getelementptr inbounds i8, ptr %b, i64 37
  %rhstmp310 = getelementptr inbounds i8, ptr %a, i64 37
  %rhs311 = load i8, ptr %rhstmp310, align 1
  store i8 %rhs311, ptr %lhstmp309, align 1
  %lhstmp312 = getelementptr inbounds i8, ptr %b, i64 38
  %rhstmp313 = getelementptr inbounds i8, ptr %a, i64 38
  %rhs314 = load i8, ptr %rhstmp313, align 1
  store i8 %rhs314, ptr %lhstmp312, align 1
  %lhstmp315 = getelementptr inbounds i8, ptr %b, i64 39
  %rhstmp316 = getelementptr inbounds i8, ptr %a, i64 39
  %rhs317 = load i8, ptr %rhstmp316, align 1
  store i8 %rhs317, ptr %lhstmp315, align 1
  %lhstmp318 = getelementptr inbounds i8, ptr %b, i64 40
  %rhstmp319 = getelementptr inbounds i8, ptr %a, i64 40
  %rhs320 = load i8, ptr %rhstmp319, align 1
  store i8 %rhs320, ptr %lhstmp318, align 1
  %lhstmp321 = getelementptr inbounds i8, ptr %b, i64 41
  %rhstmp322 = getelementptr inbounds i8, ptr %a, i64 41
  %rhs323 = load i8, ptr %rhstmp322, align 1
  store i8 %rhs323, ptr %lhstmp321, align 1
  %lhstmp324 = getelementptr inbounds i8, ptr %b, i64 42
  %rhstmp325 = getelementptr inbounds i8, ptr %a, i64 42
  %rhs326 = load i8, ptr %rhstmp325, align 1
  store i8 %rhs326, ptr %lhstmp324, align 1
  %lhstmp327 = getelementptr inbounds i8, ptr %b, i64 43
  %rhstmp328 = getelementptr inbounds i8, ptr %a, i64 43
  %rhs329 = load i8, ptr %rhstmp328, align 1
  store i8 %rhs329, ptr %lhstmp327, align 1
  %lhstmp330 = getelementptr inbounds i8, ptr %b, i64 44
  %rhstmp331 = getelementptr inbounds i8, ptr %a, i64 44
  %rhs332 = load i8, ptr %rhstmp331, align 1
  store i8 %rhs332, ptr %lhstmp330, align 1
  %lhstmp333 = getelementptr inbounds i8, ptr %b, i64 45
  %rhstmp334 = getelementptr inbounds i8, ptr %a, i64 45
  %rhs335 = load i8, ptr %rhstmp334, align 1
  store i8 %rhs335, ptr %lhstmp333, align 1
  %lhstmp336 = getelementptr inbounds i8, ptr %b, i64 46
  %rhstmp337 = getelementptr inbounds i8, ptr %a, i64 46
  %rhs338 = load i8, ptr %rhstmp337, align 1
  store i8 %rhs338, ptr %lhstmp336, align 1
  %lhstmp339 = getelementptr inbounds i8, ptr %b, i64 47
  %rhstmp340 = getelementptr inbounds i8, ptr %a, i64 47
  %rhs341 = load i8, ptr %rhstmp340, align 1
  store i8 %rhs341, ptr %lhstmp339, align 1
  %lhstmp342 = getelementptr inbounds i8, ptr %b, i64 48
  %rhstmp343 = getelementptr inbounds i8, ptr %a, i64 48
  %rhs344 = load i8, ptr %rhstmp343, align 1
  store i8 %rhs344, ptr %lhstmp342, align 1
  %lhstmp345 = getelementptr inbounds i8, ptr %b, i64 49
  %rhstmp346 = getelementptr inbounds i8, ptr %a, i64 49
  %rhs347 = load i8, ptr %rhstmp346, align 1
  store i8 %rhs347, ptr %lhstmp345, align 1
  %lhstmp348 = getelementptr inbounds i8, ptr %b, i64 50
  %rhstmp349 = getelementptr inbounds i8, ptr %a, i64 50
  %rhs350 = load i8, ptr %rhstmp349, align 1
  store i8 %rhs350, ptr %lhstmp348, align 1
  %lhstmp351 = getelementptr inbounds i8, ptr %b, i64 51
  %rhstmp352 = getelementptr inbounds i8, ptr %a, i64 51
  %rhs353 = load i8, ptr %rhstmp352, align 1
  store i8 %rhs353, ptr %lhstmp351, align 1
  %lhstmp354 = getelementptr inbounds i8, ptr %b, i64 52
  %rhstmp355 = getelementptr inbounds i8, ptr %a, i64 52
  %rhs356 = load i8, ptr %rhstmp355, align 1
  store i8 %rhs356, ptr %lhstmp354, align 1
  %lhstmp357 = getelementptr inbounds i8, ptr %b, i64 53
  %rhstmp358 = getelementptr inbounds i8, ptr %a, i64 53
  %rhs359 = load i8, ptr %rhstmp358, align 1
  store i8 %rhs359, ptr %lhstmp357, align 1
  %lhstmp360 = getelementptr inbounds i8, ptr %b, i64 54
  %rhstmp361 = getelementptr inbounds i8, ptr %a, i64 54
  %rhs362 = load i8, ptr %rhstmp361, align 1
  store i8 %rhs362, ptr %lhstmp360, align 1
  %lhstmp363 = getelementptr inbounds i8, ptr %b, i64 55
  %rhstmp364 = getelementptr inbounds i8, ptr %a, i64 55
  %rhs365 = load i8, ptr %rhstmp364, align 1
  store i8 %rhs365, ptr %lhstmp363, align 1
  %lhstmp366 = getelementptr inbounds i8, ptr %b, i64 56
  %rhstmp367 = getelementptr inbounds i8, ptr %a, i64 56
  %rhs368 = load i8, ptr %rhstmp367, align 1
  store i8 %rhs368, ptr %lhstmp366, align 1
  %lhstmp369 = getelementptr inbounds i8, ptr %b, i64 57
  %rhstmp370 = getelementptr inbounds i8, ptr %a, i64 57
  %rhs371 = load i8, ptr %rhstmp370, align 1
  store i8 %rhs371, ptr %lhstmp369, align 1
  %lhstmp372 = getelementptr inbounds i8, ptr %b, i64 58
  %rhstmp373 = getelementptr inbounds i8, ptr %a, i64 58
  %rhs374 = load i8, ptr %rhstmp373, align 1
  store i8 %rhs374, ptr %lhstmp372, align 1
  %lhstmp375 = getelementptr inbounds i8, ptr %b, i64 59
  %rhstmp376 = getelementptr inbounds i8, ptr %a, i64 59
  %rhs377 = load i8, ptr %rhstmp376, align 1
  store i8 %rhs377, ptr %lhstmp375, align 1
  %lhstmp378 = getelementptr inbounds i8, ptr %b, i64 60
  %rhstmp379 = getelementptr inbounds i8, ptr %a, i64 60
  %rhs380 = load i8, ptr %rhstmp379, align 1
  store i8 %rhs380, ptr %lhstmp378, align 1
  %lhstmp381 = getelementptr inbounds i8, ptr %b, i64 61
  %rhstmp382 = getelementptr inbounds i8, ptr %a, i64 61
  %rhs383 = load i8, ptr %rhstmp382, align 1
  store i8 %rhs383, ptr %lhstmp381, align 1
  %lhstmp384 = getelementptr inbounds i8, ptr %b, i64 62
  %rhstmp385 = getelementptr inbounds i8, ptr %a, i64 62
  %rhs386 = load i8, ptr %rhstmp385, align 1
  store i8 %rhs386, ptr %lhstmp384, align 1
  %lhstmp387 = getelementptr inbounds i8, ptr %b, i64 63
  %rhstmp388 = getelementptr inbounds i8, ptr %a, i64 63
  %rhs389 = load i8, ptr %rhstmp388, align 1
  store i8 %rhs389, ptr %lhstmp387, align 1
  %lhstmp390 = getelementptr inbounds i8, ptr %b, i64 64
  %rhstmp391 = getelementptr inbounds i8, ptr %a, i64 64
  %rhs392 = load i8, ptr %rhstmp391, align 1
  store i8 %rhs392, ptr %lhstmp390, align 1
  %lhstmp393 = getelementptr inbounds i8, ptr %b, i64 65
  %rhstmp394 = getelementptr inbounds i8, ptr %a, i64 65
  %rhs395 = load i8, ptr %rhstmp394, align 1
  store i8 %rhs395, ptr %lhstmp393, align 1
  %lhstmp396 = getelementptr inbounds i8, ptr %b, i64 66
  %rhstmp397 = getelementptr inbounds i8, ptr %a, i64 66
  %rhs398 = load i8, ptr %rhstmp397, align 1
  store i8 %rhs398, ptr %lhstmp396, align 1
  %lhstmp399 = getelementptr inbounds i8, ptr %b, i64 67
  %rhstmp400 = getelementptr inbounds i8, ptr %a, i64 67
  %rhs401 = load i8, ptr %rhstmp400, align 1
  store i8 %rhs401, ptr %lhstmp399, align 1
  %lhstmp402 = getelementptr inbounds i8, ptr %b, i64 68
  %rhstmp403 = getelementptr inbounds i8, ptr %a, i64 68
  %rhs404 = load i8, ptr %rhstmp403, align 1
  store i8 %rhs404, ptr %lhstmp402, align 1
  %lhstmp405 = getelementptr inbounds i8, ptr %b, i64 69
  %rhstmp406 = getelementptr inbounds i8, ptr %a, i64 69
  %rhs407 = load i8, ptr %rhstmp406, align 1
  store i8 %rhs407, ptr %lhstmp405, align 1
  %lhstmp408 = getelementptr inbounds i8, ptr %b, i64 70
  %rhstmp409 = getelementptr inbounds i8, ptr %a, i64 70
  %rhs410 = load i8, ptr %rhstmp409, align 1
  store i8 %rhs410, ptr %lhstmp408, align 1
  %lhstmp411 = getelementptr inbounds i8, ptr %b, i64 71
  %rhstmp412 = getelementptr inbounds i8, ptr %a, i64 71
  %rhs413 = load i8, ptr %rhstmp412, align 1
  store i8 %rhs413, ptr %lhstmp411, align 1
  %lhstmp414 = getelementptr inbounds i8, ptr %b, i64 72
  %rhstmp415 = getelementptr inbounds i8, ptr %a, i64 72
  %rhs416 = load i8, ptr %rhstmp415, align 1
  store i8 %rhs416, ptr %lhstmp414, align 1
  %lhstmp417 = getelementptr inbounds i8, ptr %b, i64 73
  %rhstmp418 = getelementptr inbounds i8, ptr %a, i64 73
  %rhs419 = load i8, ptr %rhstmp418, align 1
  store i8 %rhs419, ptr %lhstmp417, align 1
  %lhstmp420 = getelementptr inbounds i8, ptr %b, i64 74
  %rhstmp421 = getelementptr inbounds i8, ptr %a, i64 74
  %rhs422 = load i8, ptr %rhstmp421, align 1
  store i8 %rhs422, ptr %lhstmp420, align 1
  %lhstmp423 = getelementptr inbounds i8, ptr %b, i64 75
  %rhstmp424 = getelementptr inbounds i8, ptr %a, i64 75
  %rhs425 = load i8, ptr %rhstmp424, align 1
  store i8 %rhs425, ptr %lhstmp423, align 1
  %lhstmp426 = getelementptr inbounds i8, ptr %b, i64 76
  %rhstmp427 = getelementptr inbounds i8, ptr %a, i64 76
  %rhs428 = load i8, ptr %rhstmp427, align 1
  store i8 %rhs428, ptr %lhstmp426, align 1
  %lhstmp429 = getelementptr inbounds i8, ptr %b, i64 77
  %rhstmp430 = getelementptr inbounds i8, ptr %a, i64 77
  %rhs431 = load i8, ptr %rhstmp430, align 1
  store i8 %rhs431, ptr %lhstmp429, align 1
  %lhstmp432 = getelementptr inbounds i8, ptr %b, i64 78
  %rhstmp433 = getelementptr inbounds i8, ptr %a, i64 78
  %rhs434 = load i8, ptr %rhstmp433, align 1
  store i8 %rhs434, ptr %lhstmp432, align 1
  %lhstmp435 = getelementptr inbounds i8, ptr %b, i64 79
  %rhstmp436 = getelementptr inbounds i8, ptr %a, i64 79
  %rhs437 = load i8, ptr %rhstmp436, align 1
  store i8 %rhs437, ptr %lhstmp435, align 1
  %lhstmp438 = getelementptr inbounds i8, ptr %b, i64 80
  %rhstmp439 = getelementptr inbounds i8, ptr %a, i64 80
  %rhs440 = load i8, ptr %rhstmp439, align 1
  store i8 %rhs440, ptr %lhstmp438, align 1
  %lhstmp441 = getelementptr inbounds i8, ptr %b, i64 81
  %rhstmp442 = getelementptr inbounds i8, ptr %a, i64 81
  %rhs443 = load i8, ptr %rhstmp442, align 1
  store i8 %rhs443, ptr %lhstmp441, align 1
  %lhstmp444 = getelementptr inbounds i8, ptr %b, i64 82
  %rhstmp445 = getelementptr inbounds i8, ptr %a, i64 82
  %rhs446 = load i8, ptr %rhstmp445, align 1
  store i8 %rhs446, ptr %lhstmp444, align 1
  %lhstmp447 = getelementptr inbounds i8, ptr %b, i64 83
  %rhstmp448 = getelementptr inbounds i8, ptr %a, i64 83
  %rhs449 = load i8, ptr %rhstmp448, align 1
  store i8 %rhs449, ptr %lhstmp447, align 1
  %lhstmp450 = getelementptr inbounds i8, ptr %b, i64 84
  %rhstmp451 = getelementptr inbounds i8, ptr %a, i64 84
  %rhs452 = load i8, ptr %rhstmp451, align 1
  store i8 %rhs452, ptr %lhstmp450, align 1
  %lhstmp453 = getelementptr inbounds i8, ptr %b, i64 85
  %rhstmp454 = getelementptr inbounds i8, ptr %a, i64 85
  %rhs455 = load i8, ptr %rhstmp454, align 1
  store i8 %rhs455, ptr %lhstmp453, align 1
  %lhstmp456 = getelementptr inbounds i8, ptr %b, i64 86
  %rhstmp457 = getelementptr inbounds i8, ptr %a, i64 86
  %rhs458 = load i8, ptr %rhstmp457, align 1
  store i8 %rhs458, ptr %lhstmp456, align 1
  %lhstmp459 = getelementptr inbounds i8, ptr %b, i64 87
  %rhstmp460 = getelementptr inbounds i8, ptr %a, i64 87
  %rhs461 = load i8, ptr %rhstmp460, align 1
  store i8 %rhs461, ptr %lhstmp459, align 1
  %lhstmp462 = getelementptr inbounds i8, ptr %b, i64 88
  %rhstmp463 = getelementptr inbounds i8, ptr %a, i64 88
  %rhs464 = load i8, ptr %rhstmp463, align 1
  store i8 %rhs464, ptr %lhstmp462, align 1
  %lhstmp465 = getelementptr inbounds i8, ptr %b, i64 89
  %rhstmp466 = getelementptr inbounds i8, ptr %a, i64 89
  %rhs467 = load i8, ptr %rhstmp466, align 1
  store i8 %rhs467, ptr %lhstmp465, align 1
  %lhstmp468 = getelementptr inbounds i8, ptr %b, i64 90
  %rhstmp469 = getelementptr inbounds i8, ptr %a, i64 90
  %rhs470 = load i8, ptr %rhstmp469, align 1
  store i8 %rhs470, ptr %lhstmp468, align 1
  %lhstmp471 = getelementptr inbounds i8, ptr %b, i64 91
  %rhstmp472 = getelementptr inbounds i8, ptr %a, i64 91
  %rhs473 = load i8, ptr %rhstmp472, align 1
  store i8 %rhs473, ptr %lhstmp471, align 1
  %lhstmp474 = getelementptr inbounds i8, ptr %b, i64 92
  %rhstmp475 = getelementptr inbounds i8, ptr %a, i64 92
  %rhs476 = load i8, ptr %rhstmp475, align 1
  store i8 %rhs476, ptr %lhstmp474, align 1
  %lhstmp477 = getelementptr inbounds i8, ptr %b, i64 93
  %rhstmp478 = getelementptr inbounds i8, ptr %a, i64 93
  %rhs479 = load i8, ptr %rhstmp478, align 1
  store i8 %rhs479, ptr %lhstmp477, align 1
  %lhstmp480 = getelementptr inbounds i8, ptr %b, i64 94
  %rhstmp481 = getelementptr inbounds i8, ptr %a, i64 94
  %rhs482 = load i8, ptr %rhstmp481, align 1
  store i8 %rhs482, ptr %lhstmp480, align 1
  %lhstmp483 = getelementptr inbounds i8, ptr %b, i64 95
  %rhstmp484 = getelementptr inbounds i8, ptr %a, i64 95
  %rhs485 = load i8, ptr %rhstmp484, align 1
  store i8 %rhs485, ptr %lhstmp483, align 1
  %lhstmp486 = getelementptr inbounds i8, ptr %b, i64 96
  %rhstmp487 = getelementptr inbounds i8, ptr %a, i64 96
  %rhs488 = load i8, ptr %rhstmp487, align 1
  store i8 %rhs488, ptr %lhstmp486, align 1
  %lhstmp489 = getelementptr inbounds i8, ptr %b, i64 97
  %rhstmp490 = getelementptr inbounds i8, ptr %a, i64 97
  %rhs491 = load i8, ptr %rhstmp490, align 1
  store i8 %rhs491, ptr %lhstmp489, align 1
  %lhstmp492 = getelementptr inbounds i8, ptr %b, i64 98
  %rhstmp493 = getelementptr inbounds i8, ptr %a, i64 98
  %rhs494 = load i8, ptr %rhstmp493, align 1
  store i8 %rhs494, ptr %lhstmp492, align 1
  %lhstmp495 = getelementptr inbounds i8, ptr %b, i64 99
  %rhstmp496 = getelementptr inbounds i8, ptr %a, i64 99
  %rhs497 = load i8, ptr %rhstmp496, align 1
  store i8 %rhs497, ptr %lhstmp495, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str.1)
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
    .value = 74
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