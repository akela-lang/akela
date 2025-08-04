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