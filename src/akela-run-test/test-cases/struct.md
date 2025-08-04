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