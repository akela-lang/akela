# Test Suite
array

```cent
TestSuite {
}
```

## Test
array const 1

```akela
const a: [4 const]Nat8 = [10,20,30,40]
a[0]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [4 x i8], align 1
  %arrayelementtmp = getelementptr inbounds [4 x i8], ptr %a, i64 0, i64 0
  store i8 10, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i8, ptr %arrayelementtmp, i64 1
  store i8 20, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i8, ptr %arrayelementtmp1, i64 1
  store i8 30, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i8, ptr %arrayelementtmp2, i64 1
  store i8 40, ptr %arrayelementtmp3, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
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
    .value = 10
  }
}
```

## Test
const string

```akela
"hello"
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [6 x i8] c"hello\00", align 1

define ptr @__top_level() {
entry:
  ret ptr @.str
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = "hello"
  }
}
```

## Test
const string variable

```akela
const a: [6 const]Nat8 = "hello"
a[0]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

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
array const 1

```akela
const a: [5 const]Nat8 = [1,2,3,4,0]
a[0]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [5 x i8], align 1
  %arrayelementtmp = getelementptr inbounds [5 x i8], ptr %a, i64 0, i64 0
  store i8 1, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i8, ptr %arrayelementtmp, i64 1
  store i8 2, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i8, ptr %arrayelementtmp1, i64 1
  store i8 3, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i8, ptr %arrayelementtmp2, i64 1
  store i8 4, ptr %arrayelementtmp3, align 1
  %arrayelementtmp4 = getelementptr inbounds i8, ptr %arrayelementtmp3, i64 1
  store i8 0, ptr %arrayelementtmp4, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
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
    .value = 1
  }
}
```

## Test
array const 2

```akela
const a: [5 const]Nat8 = [1,2,3,4,0]
a[1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [5 x i8], align 1
  %arrayelementtmp = getelementptr inbounds [5 x i8], ptr %a, i64 0, i64 0
  store i8 1, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i8, ptr %arrayelementtmp, i64 1
  store i8 2, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i8, ptr %arrayelementtmp1, i64 1
  store i8 3, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i8, ptr %arrayelementtmp2, i64 1
  store i8 4, ptr %arrayelementtmp3, align 1
  %arrayelementtmp4 = getelementptr inbounds i8, ptr %arrayelementtmp3, i64 1
  store i8 0, ptr %arrayelementtmp4, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 1
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 2
  }
}
```

## Test
array const 3

```akela
const a: [5 const]Nat8 = [1,2,3,4,0]
a[2]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [5 x i8], align 1
  %arrayelementtmp = getelementptr inbounds [5 x i8], ptr %a, i64 0, i64 0
  store i8 1, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i8, ptr %arrayelementtmp, i64 1
  store i8 2, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i8, ptr %arrayelementtmp1, i64 1
  store i8 3, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i8, ptr %arrayelementtmp2, i64 1
  store i8 4, ptr %arrayelementtmp3, align 1
  %arrayelementtmp4 = getelementptr inbounds i8, ptr %arrayelementtmp3, i64 1
  store i8 0, ptr %arrayelementtmp4, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 2
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 3
  }
}
```

## Test
array const 4

```akela
const a: [5 const]Nat8 = [1,2,3,4,0]
a[3]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [5 x i8], align 1
  %arrayelementtmp = getelementptr inbounds [5 x i8], ptr %a, i64 0, i64 0
  store i8 1, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i8, ptr %arrayelementtmp, i64 1
  store i8 2, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i8, ptr %arrayelementtmp1, i64 1
  store i8 3, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i8, ptr %arrayelementtmp2, i64 1
  store i8 4, ptr %arrayelementtmp3, align 1
  %arrayelementtmp4 = getelementptr inbounds i8, ptr %arrayelementtmp3, i64 1
  store i8 0, ptr %arrayelementtmp4, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 3
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 4
  }
}
```

## Test
array const 5

```akela
const a: [5 const]Nat8 = [1,2,3,4,0]
a[4]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i8 @__top_level() {
entry:
  %a = alloca [5 x i8], align 1
  %arrayelementtmp = getelementptr inbounds [5 x i8], ptr %a, i64 0, i64 0
  store i8 1, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i8, ptr %arrayelementtmp, i64 1
  store i8 2, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i8, ptr %arrayelementtmp1, i64 1
  store i8 3, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i8, ptr %arrayelementtmp2, i64 1
  store i8 4, ptr %arrayelementtmp3, align 1
  %arrayelementtmp4 = getelementptr inbounds i8, ptr %arrayelementtmp3, i64 1
  store i8 0, ptr %arrayelementtmp4, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i8, ptr %a, i64 4
  %elementtmp = load i8, ptr %subscripttmp, align 1
  ret i8 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 0
  }
}
```

## Test
array not const 1

```akela
const a: [5]Int32 = [1,2,3,4,5]
a[0]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [5 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 5, ptr %arrayelementtmp4, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 0
  %elementtmp = load i32, ptr %subscripttmp, align 4
  ret i32 %elementtmp
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
array not const 2

```akela
const a: [5]Int32 = [1,2,3,4,5]
a[1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [5 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 5, ptr %arrayelementtmp4, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 1
  %elementtmp = load i32, ptr %subscripttmp, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 2
  }
}
```

## Test
array not const 3

```akela
const a: [5]Int32 = [1,2,3,4,5]
a[2]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [5 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 5, ptr %arrayelementtmp4, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 2
  %elementtmp = load i32, ptr %subscripttmp, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 3
  }
}
```

## Test
array not const 4

```akela
const a: [5]Int32 = [1,2,3,4,5]
a[3]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [5 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 5, ptr %arrayelementtmp4, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 3
  %elementtmp = load i32, ptr %subscripttmp, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Nat8
    .value = 4
  }
}
```

## Test
array not const 5

```akela
const a: [5]Int32 = [1,2,3,4,5]
a[4]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [5 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 5, ptr %arrayelementtmp4, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 4
  %elementtmp = load i32, ptr %subscripttmp, align 4
  ret i32 %elementtmp
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
multidimensional literal 1

```akela
const a: [2][2]Int32 = [[1,2],[3,4]]
a[0][0]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [2 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [2 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp4 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp3, i64 0, i64 0
  store i32 3, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 4, ptr %arrayelementtmp5, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [2 x i32], ptr %a, i64 0
  br i1 true, label %continuetmp7, label %aborttmp6

aborttmp6:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp7

continuetmp7:                                     ; preds = %aborttmp6, %continuetmp
  %subscripttmp8 = getelementptr inbounds i32, ptr %subscripttmp, i64 0
  %elementtmp = load i32, ptr %subscripttmp8, align 4
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
multidimensional literal 2

```akela
const a: [2][2]Int32 = [[1,2],[3,4]]
a[0][1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [2 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [2 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp4 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp3, i64 0, i64 0
  store i32 3, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 4, ptr %arrayelementtmp5, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [2 x i32], ptr %a, i64 0
  br i1 true, label %continuetmp7, label %aborttmp6

aborttmp6:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp7

continuetmp7:                                     ; preds = %aborttmp6, %continuetmp
  %subscripttmp8 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %elementtmp = load i32, ptr %subscripttmp8, align 4
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
multidimensional literal 3

```akela
const a: [2][2]Int32 = [[1,2],[3,4]]
a[1][0]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [2 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [2 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp4 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp3, i64 0, i64 0
  store i32 3, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 4, ptr %arrayelementtmp5, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [2 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp7, label %aborttmp6

aborttmp6:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp7

continuetmp7:                                     ; preds = %aborttmp6, %continuetmp
  %subscripttmp8 = getelementptr inbounds i32, ptr %subscripttmp, i64 0
  %elementtmp = load i32, ptr %subscripttmp8, align 4
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
multidimensional literal 4

```akela
const a: [2][2]Int32 = [[1,2],[3,4]]
a[1][1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [2 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [2 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp4 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp3, i64 0, i64 0
  store i32 3, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 4, ptr %arrayelementtmp5, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [2 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp7, label %aborttmp6

aborttmp6:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp7

continuetmp7:                                     ; preds = %aborttmp6, %continuetmp
  %subscripttmp8 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %elementtmp = load i32, ptr %subscripttmp8, align 4
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
array multidimensional signed

```akela
const a: [2][2]Int32 = [[1,-2],[3,4]]
a[0][1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [2 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [2 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 -2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp4 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp3, i64 0, i64 0
  store i32 3, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 4, ptr %arrayelementtmp5, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [2 x i32], ptr %a, i64 0
  br i1 true, label %continuetmp7, label %aborttmp6

aborttmp6:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp7

continuetmp7:                                     ; preds = %aborttmp6, %continuetmp
  %subscripttmp8 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %elementtmp = load i32, ptr %subscripttmp8, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = -2
  }
}
```

## Test
array literal multidimensional Int32

```akela
const a: [2][2]Int32 = [[1,2],[3,4]]
a[0][1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [2 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [2 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp4 = getelementptr inbounds [2 x i32], ptr %arrayelementtmp3, i64 0, i64 0
  store i32 3, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 4, ptr %arrayelementtmp5, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [2 x i32], ptr %a, i64 0
  br i1 true, label %continuetmp7, label %aborttmp6

aborttmp6:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp7

continuetmp7:                                     ; preds = %aborttmp6, %continuetmp
  %subscripttmp8 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %elementtmp = load i32, ptr %subscripttmp8, align 4
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
array pointer

```akela
const a: [4]Int32 = [1,2,3,4]
const b: [4]Int32 = a
b[1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
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
  %b = alloca [4 x i32], align 4
  %lhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %a, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp4 = getelementptr inbounds i32, ptr %b, i64 1
  %rhstmp5 = getelementptr inbounds i32, ptr %a, i64 1
  %rhs6 = load i32, ptr %rhstmp5, align 4
  store i32 %rhs6, ptr %lhstmp4, align 4
  %lhstmp7 = getelementptr inbounds i32, ptr %b, i64 2
  %rhstmp8 = getelementptr inbounds i32, ptr %a, i64 2
  %rhs9 = load i32, ptr %rhstmp8, align 4
  store i32 %rhs9, ptr %lhstmp7, align 4
  %lhstmp10 = getelementptr inbounds i32, ptr %b, i64 3
  %rhstmp11 = getelementptr inbounds i32, ptr %a, i64 3
  %rhs12 = load i32, ptr %rhstmp11, align 4
  store i32 %rhs12, ptr %lhstmp10, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %b, i64 1
  %elementtmp = load i32, ptr %subscripttmp, align 4
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
boolean 1

```akela
const a: [4]Bool = [true,false,true,false]
a[0]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i1 @__top_level() {
entry:
  %a = alloca [4 x i1], align 1
  %arrayelementtmp = getelementptr inbounds [4 x i1], ptr %a, i64 0, i64 0
  store i1 true, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i1, ptr %arrayelementtmp, i64 1
  store i1 false, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i1, ptr %arrayelementtmp1, i64 1
  store i1 true, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i1, ptr %arrayelementtmp2, i64 1
  store i1 false, ptr %arrayelementtmp3, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i1, ptr %a, i64 0
  %elementtmp = load i1, ptr %subscripttmp, align 1
  ret i1 %elementtmp
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
boolean 2

```akela
const a: [4]Bool = [true,false,true,false]
a[1]
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i1 @__top_level() {
entry:
  %a = alloca [4 x i1], align 1
  %arrayelementtmp = getelementptr inbounds [4 x i1], ptr %a, i64 0, i64 0
  store i1 true, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i1, ptr %arrayelementtmp, i64 1
  store i1 false, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i1, ptr %arrayelementtmp1, i64 1
  store i1 true, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i1, ptr %arrayelementtmp2, i64 1
  store i1 false, ptr %arrayelementtmp3, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i1, ptr %a, i64 1
  %elementtmp = load i1, ptr %subscripttmp, align 1
  ret i1 %elementtmp
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
boolean 3

```akela
const a: [4]Bool = [true,false,true,false]
a[2]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i1 @__top_level() {
entry:
  %a = alloca [4 x i1], align 1
  %arrayelementtmp = getelementptr inbounds [4 x i1], ptr %a, i64 0, i64 0
  store i1 true, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i1, ptr %arrayelementtmp, i64 1
  store i1 false, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i1, ptr %arrayelementtmp1, i64 1
  store i1 true, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i1, ptr %arrayelementtmp2, i64 1
  store i1 false, ptr %arrayelementtmp3, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i1, ptr %a, i64 2
  %elementtmp = load i1, ptr %subscripttmp, align 1
  ret i1 %elementtmp
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
boolean 4

```akela
const a: [4]Bool = [true,false,true,false]
a[3]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i1 @__top_level() {
entry:
  %a = alloca [4 x i1], align 1
  %arrayelementtmp = getelementptr inbounds [4 x i1], ptr %a, i64 0, i64 0
  store i1 true, ptr %arrayelementtmp, align 1
  %arrayelementtmp1 = getelementptr inbounds i1, ptr %arrayelementtmp, i64 1
  store i1 false, ptr %arrayelementtmp1, align 1
  %arrayelementtmp2 = getelementptr inbounds i1, ptr %arrayelementtmp1, i64 1
  store i1 true, ptr %arrayelementtmp2, align 1
  %arrayelementtmp3 = getelementptr inbounds i1, ptr %arrayelementtmp2, i64 1
  store i1 false, ptr %arrayelementtmp3, align 1
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i1, ptr %a, i64 3
  %elementtmp = load i1, ptr %subscripttmp, align 1
  ret i1 %elementtmp
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
real 1

```akela
const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]
a[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define double @__top_level() {
entry:
  %a = alloca [4 x double], align 8
  %arrayelementtmp = getelementptr inbounds [4 x double], ptr %a, i64 0, i64 0
  store double 1.000000e+00, ptr %arrayelementtmp, align 8
  %arrayelementtmp1 = getelementptr inbounds double, ptr %arrayelementtmp, i64 1
  store double 2.000000e+00, ptr %arrayelementtmp1, align 8
  %arrayelementtmp2 = getelementptr inbounds double, ptr %arrayelementtmp1, i64 1
  store double 3.000000e+00, ptr %arrayelementtmp2, align 8
  %arrayelementtmp3 = getelementptr inbounds double, ptr %arrayelementtmp2, i64 1
  store double 4.000000e+00, ptr %arrayelementtmp3, align 8
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds double, ptr %a, i64 0
  %elementtmp = load double, ptr %subscripttmp, align 8
  ret double %elementtmp
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
real 2

```akela
const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]
a[1]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define double @__top_level() {
entry:
  %a = alloca [4 x double], align 8
  %arrayelementtmp = getelementptr inbounds [4 x double], ptr %a, i64 0, i64 0
  store double 1.000000e+00, ptr %arrayelementtmp, align 8
  %arrayelementtmp1 = getelementptr inbounds double, ptr %arrayelementtmp, i64 1
  store double 2.000000e+00, ptr %arrayelementtmp1, align 8
  %arrayelementtmp2 = getelementptr inbounds double, ptr %arrayelementtmp1, i64 1
  store double 3.000000e+00, ptr %arrayelementtmp2, align 8
  %arrayelementtmp3 = getelementptr inbounds double, ptr %arrayelementtmp2, i64 1
  store double 4.000000e+00, ptr %arrayelementtmp3, align 8
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds double, ptr %a, i64 1
  %elementtmp = load double, ptr %subscripttmp, align 8
  ret double %elementtmp
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
real 3

```akela
const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]
a[2]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define double @__top_level() {
entry:
  %a = alloca [4 x double], align 8
  %arrayelementtmp = getelementptr inbounds [4 x double], ptr %a, i64 0, i64 0
  store double 1.000000e+00, ptr %arrayelementtmp, align 8
  %arrayelementtmp1 = getelementptr inbounds double, ptr %arrayelementtmp, i64 1
  store double 2.000000e+00, ptr %arrayelementtmp1, align 8
  %arrayelementtmp2 = getelementptr inbounds double, ptr %arrayelementtmp1, i64 1
  store double 3.000000e+00, ptr %arrayelementtmp2, align 8
  %arrayelementtmp3 = getelementptr inbounds double, ptr %arrayelementtmp2, i64 1
  store double 4.000000e+00, ptr %arrayelementtmp3, align 8
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds double, ptr %a, i64 2
  %elementtmp = load double, ptr %subscripttmp, align 8
  ret double %elementtmp
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
real 4

```akela
const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]
a[3]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define double @__top_level() {
entry:
  %a = alloca [4 x double], align 8
  %arrayelementtmp = getelementptr inbounds [4 x double], ptr %a, i64 0, i64 0
  store double 1.000000e+00, ptr %arrayelementtmp, align 8
  %arrayelementtmp1 = getelementptr inbounds double, ptr %arrayelementtmp, i64 1
  store double 2.000000e+00, ptr %arrayelementtmp1, align 8
  %arrayelementtmp2 = getelementptr inbounds double, ptr %arrayelementtmp1, i64 1
  store double 3.000000e+00, ptr %arrayelementtmp2, align 8
  %arrayelementtmp3 = getelementptr inbounds double, ptr %arrayelementtmp2, i64 1
  store double 4.000000e+00, ptr %arrayelementtmp3, align 8
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds double, ptr %a, i64 3
  %elementtmp = load double, ptr %subscripttmp, align 8
  ret double %elementtmp
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
copy array 1

```akela
var a: [4]Int32 = [1, 2, 3, 4]
const b: [4]Int32 = [5, 6, 7, 8]
a = b
a[0]
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
  %a = alloca [4 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [4 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %b = alloca [4 x i32], align 4
  %arrayelementtmp4 = getelementptr inbounds [4 x i32], ptr %b, i64 0, i64 0
  store i32 5, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds i32, ptr %arrayelementtmp4, i64 1
  store i32 6, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 7, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 8, ptr %arrayelementtmp7, align 4
  %lhstmp = getelementptr inbounds i32, ptr %a, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp8 = getelementptr inbounds i32, ptr %a, i64 1
  %rhstmp9 = getelementptr inbounds i32, ptr %b, i64 1
  %rhs10 = load i32, ptr %rhstmp9, align 4
  store i32 %rhs10, ptr %lhstmp8, align 4
  %lhstmp11 = getelementptr inbounds i32, ptr %a, i64 2
  %rhstmp12 = getelementptr inbounds i32, ptr %b, i64 2
  %rhs13 = load i32, ptr %rhstmp12, align 4
  store i32 %rhs13, ptr %lhstmp11, align 4
  %lhstmp14 = getelementptr inbounds i32, ptr %a, i64 3
  %rhstmp15 = getelementptr inbounds i32, ptr %b, i64 3
  %rhs16 = load i32, ptr %rhstmp15, align 4
  store i32 %rhs16, ptr %lhstmp14, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 0
  %elementtmp = load i32, ptr %subscripttmp, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 5
  }
}
```

## Test
subscript assign

```akela
var a: [4]Int32 = [1, 2, 3, 4]
a[0] = 10
a[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
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
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 0
  store i32 10, ptr %subscripttmp, align 4
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp
  %subscripttmp6 = getelementptr inbounds i32, ptr %a, i64 0
  %elementtmp = load i32, ptr %subscripttmp6, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 10
  }
}
```

## Test
subscript 2d assign 1

```akela
var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]
a[1][2] = 60
a[0][0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [3 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [3 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp5 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp4, i64 0, i64 0
  store i32 4, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp9, label %aborttmp8

aborttmp8:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp9

continuetmp9:                                     ; preds = %aborttmp8, %continuetmp
  %subscripttmp10 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  store i32 60, ptr %subscripttmp10, align 4
  br i1 true, label %continuetmp12, label %aborttmp11

aborttmp11:                                       ; preds = %continuetmp9
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp12

continuetmp12:                                    ; preds = %aborttmp11, %continuetmp9
  %subscripttmp13 = getelementptr inbounds [3 x i32], ptr %a, i64 0
  br i1 true, label %continuetmp15, label %aborttmp14

aborttmp14:                                       ; preds = %continuetmp12
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp15

continuetmp15:                                    ; preds = %aborttmp14, %continuetmp12
  %subscripttmp16 = getelementptr inbounds i32, ptr %subscripttmp13, i64 0
  %elementtmp = load i32, ptr %subscripttmp16, align 4
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
subscript 2d assign 2

```akela
var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]
a[1][2] = 60
a[0][1]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [3 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [3 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp5 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp4, i64 0, i64 0
  store i32 4, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp9, label %aborttmp8

aborttmp8:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp9

continuetmp9:                                     ; preds = %aborttmp8, %continuetmp
  %subscripttmp10 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  store i32 60, ptr %subscripttmp10, align 4
  br i1 true, label %continuetmp12, label %aborttmp11

aborttmp11:                                       ; preds = %continuetmp9
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp12

continuetmp12:                                    ; preds = %aborttmp11, %continuetmp9
  %subscripttmp13 = getelementptr inbounds [3 x i32], ptr %a, i64 0
  br i1 true, label %continuetmp15, label %aborttmp14

aborttmp14:                                       ; preds = %continuetmp12
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp15

continuetmp15:                                    ; preds = %aborttmp14, %continuetmp12
  %subscripttmp16 = getelementptr inbounds i32, ptr %subscripttmp13, i64 1
  %elementtmp = load i32, ptr %subscripttmp16, align 4
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
subscript 2d assign 3

```akela
var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]
a[1][2] = 60
a[0][2]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [3 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [3 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp5 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp4, i64 0, i64 0
  store i32 4, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp9, label %aborttmp8

aborttmp8:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp9

continuetmp9:                                     ; preds = %aborttmp8, %continuetmp
  %subscripttmp10 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  store i32 60, ptr %subscripttmp10, align 4
  br i1 true, label %continuetmp12, label %aborttmp11

aborttmp11:                                       ; preds = %continuetmp9
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp12

continuetmp12:                                    ; preds = %aborttmp11, %continuetmp9
  %subscripttmp13 = getelementptr inbounds [3 x i32], ptr %a, i64 0
  br i1 true, label %continuetmp15, label %aborttmp14

aborttmp14:                                       ; preds = %continuetmp12
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp15

continuetmp15:                                    ; preds = %aborttmp14, %continuetmp12
  %subscripttmp16 = getelementptr inbounds i32, ptr %subscripttmp13, i64 2
  %elementtmp = load i32, ptr %subscripttmp16, align 4
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
subscript 2d assign 4

```akela
var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]
a[1][2] = 60
a[1][0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [3 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [3 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp5 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp4, i64 0, i64 0
  store i32 4, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp9, label %aborttmp8

aborttmp8:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp9

continuetmp9:                                     ; preds = %aborttmp8, %continuetmp
  %subscripttmp10 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  store i32 60, ptr %subscripttmp10, align 4
  br i1 true, label %continuetmp12, label %aborttmp11

aborttmp11:                                       ; preds = %continuetmp9
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp12

continuetmp12:                                    ; preds = %aborttmp11, %continuetmp9
  %subscripttmp13 = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp15, label %aborttmp14

aborttmp14:                                       ; preds = %continuetmp12
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp15

continuetmp15:                                    ; preds = %aborttmp14, %continuetmp12
  %subscripttmp16 = getelementptr inbounds i32, ptr %subscripttmp13, i64 0
  %elementtmp = load i32, ptr %subscripttmp16, align 4
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
subscript 2d assign 5

```akela
var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]
a[1][2] = 60
a[1][1]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [3 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [3 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp5 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp4, i64 0, i64 0
  store i32 4, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp9, label %aborttmp8

aborttmp8:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp9

continuetmp9:                                     ; preds = %aborttmp8, %continuetmp
  %subscripttmp10 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  store i32 60, ptr %subscripttmp10, align 4
  br i1 true, label %continuetmp12, label %aborttmp11

aborttmp11:                                       ; preds = %continuetmp9
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp12

continuetmp12:                                    ; preds = %aborttmp11, %continuetmp9
  %subscripttmp13 = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp15, label %aborttmp14

aborttmp14:                                       ; preds = %continuetmp12
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp15

continuetmp15:                                    ; preds = %aborttmp14, %continuetmp12
  %subscripttmp16 = getelementptr inbounds i32, ptr %subscripttmp13, i64 1
  %elementtmp = load i32, ptr %subscripttmp16, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 5
  }
}
```

## Test
subscript 2d assign 6

```akela
var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]
a[1][2] = 60
a[1][2]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [2 x [3 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [2 x [3 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp5 = getelementptr inbounds [3 x i32], ptr %arrayelementtmp4, i64 0, i64 0
  store i32 4, ptr %arrayelementtmp5, align 4
  %arrayelementtmp6 = getelementptr inbounds i32, ptr %arrayelementtmp5, i64 1
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp9, label %aborttmp8

aborttmp8:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp9

continuetmp9:                                     ; preds = %aborttmp8, %continuetmp
  %subscripttmp10 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  store i32 60, ptr %subscripttmp10, align 4
  br i1 true, label %continuetmp12, label %aborttmp11

aborttmp11:                                       ; preds = %continuetmp9
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp12

continuetmp12:                                    ; preds = %aborttmp11, %continuetmp9
  %subscripttmp13 = getelementptr inbounds [3 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp15, label %aborttmp14

aborttmp14:                                       ; preds = %continuetmp12
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp15

continuetmp15:                                    ; preds = %aborttmp14, %continuetmp12
  %subscripttmp16 = getelementptr inbounds i32, ptr %subscripttmp13, i64 2
  %elementtmp = load i32, ptr %subscripttmp16, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 60
  }
}
```

## Test
assign later 1

```akela
var a: [4]Int32
a[0] = 1
a[1] = 2
a[2] = 3
a[3] = 4
a[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 0
  store i32 1, ptr %subscripttmp, align 4
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %a, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %a, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %a, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %a, i64 0
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
assign later 2

```akela
var a: [4]Int32
a[0] = 1
a[1] = 2
a[2] = 3
a[3] = 4
a[1]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 0
  store i32 1, ptr %subscripttmp, align 4
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %a, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %a, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %a, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %a, i64 1
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
assign later 3

```akela
var a: [4]Int32
a[0] = 1
a[1] = 2
a[2] = 3
a[3] = 4
a[2]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 0
  store i32 1, ptr %subscripttmp, align 4
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %a, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %a, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %a, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %a, i64 2
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
assign later 4

```akela
var a: [4]Int32
a[0] = 1
a[1] = 2
a[2] = 3
a[3] = 4
a[3]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.4 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %a, i64 0
  store i32 1, ptr %subscripttmp, align 4
  br i1 true, label %continuetmp2, label %aborttmp1

aborttmp1:                                        ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp2

continuetmp2:                                     ; preds = %aborttmp1, %continuetmp
  %subscripttmp3 = getelementptr inbounds i32, ptr %a, i64 1
  store i32 2, ptr %subscripttmp3, align 4
  br i1 true, label %continuetmp5, label %aborttmp4

aborttmp4:                                        ; preds = %continuetmp2
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp5

continuetmp5:                                     ; preds = %aborttmp4, %continuetmp2
  %subscripttmp6 = getelementptr inbounds i32, ptr %a, i64 2
  store i32 3, ptr %subscripttmp6, align 4
  br i1 true, label %continuetmp8, label %aborttmp7

aborttmp7:                                        ; preds = %continuetmp5
  call void (ptr, ...) @printf(ptr @.str.3)
  call void @exit(i32 1)
  br label %continuetmp8

continuetmp8:                                     ; preds = %aborttmp7, %continuetmp5
  %subscripttmp9 = getelementptr inbounds i32, ptr %a, i64 3
  store i32 4, ptr %subscripttmp9, align 4
  br i1 true, label %continuetmp11, label %aborttmp10

aborttmp10:                                       ; preds = %continuetmp8
  call void (ptr, ...) @printf(ptr @.str.4)
  call void @exit(i32 1)
  br label %continuetmp11

continuetmp11:                                    ; preds = %aborttmp10, %continuetmp8
  %subscripttmp12 = getelementptr inbounds i32, ptr %a, i64 3
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
copy array 2

```akela
const a: [4]Int32 = [1, 2, 3, 4]
var b: [4]Int32
b = a
b[0]
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
  %a = alloca [4 x i32], align 4
  %arrayelementtmp = getelementptr inbounds [4 x i32], ptr %a, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp, align 4
  %arrayelementtmp1 = getelementptr inbounds i32, ptr %arrayelementtmp, i64 1
  store i32 2, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 3, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 4, ptr %arrayelementtmp3, align 4
  %b = alloca [4 x i32], align 4
  %lhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %a, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp4 = getelementptr inbounds i32, ptr %b, i64 1
  %rhstmp5 = getelementptr inbounds i32, ptr %a, i64 1
  %rhs6 = load i32, ptr %rhstmp5, align 4
  store i32 %rhs6, ptr %lhstmp4, align 4
  %lhstmp7 = getelementptr inbounds i32, ptr %b, i64 2
  %rhstmp8 = getelementptr inbounds i32, ptr %a, i64 2
  %rhs9 = load i32, ptr %rhstmp8, align 4
  store i32 %rhs9, ptr %lhstmp7, align 4
  %lhstmp10 = getelementptr inbounds i32, ptr %b, i64 3
  %rhstmp11 = getelementptr inbounds i32, ptr %a, i64 3
  %rhs12 = load i32, ptr %rhstmp11, align 4
  store i32 %rhs12, ptr %lhstmp10, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds i32, ptr %b, i64 0
  %elementtmp = load i32, ptr %subscripttmp, align 4
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
copy array 3.1

```akela
const a: [4][4]Int32 = [
  [1, 2, 3, 4],
  [5, 6, 7, 8],
  [9, 10, 11, 12],
  [13, 14, 15, 16]
]
var b: [4]Int32
b = a[1]
b[0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x [4 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [4 x [4 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 4, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp6 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 0, i64 0
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  %arrayelementtmp8 = getelementptr inbounds i32, ptr %arrayelementtmp7, i64 1
  store i32 7, ptr %arrayelementtmp8, align 4
  %arrayelementtmp9 = getelementptr inbounds i32, ptr %arrayelementtmp8, i64 1
  store i32 8, ptr %arrayelementtmp9, align 4
  %arrayelementtmp10 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 1
  %arrayelementtmp11 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 0, i64 0
  store i32 9, ptr %arrayelementtmp11, align 4
  %arrayelementtmp12 = getelementptr inbounds i32, ptr %arrayelementtmp11, i64 1
  store i32 10, ptr %arrayelementtmp12, align 4
  %arrayelementtmp13 = getelementptr inbounds i32, ptr %arrayelementtmp12, i64 1
  store i32 11, ptr %arrayelementtmp13, align 4
  %arrayelementtmp14 = getelementptr inbounds i32, ptr %arrayelementtmp13, i64 1
  store i32 12, ptr %arrayelementtmp14, align 4
  %arrayelementtmp15 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 1
  %arrayelementtmp16 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp15, i64 0, i64 0
  store i32 13, ptr %arrayelementtmp16, align 4
  %arrayelementtmp17 = getelementptr inbounds i32, ptr %arrayelementtmp16, i64 1
  store i32 14, ptr %arrayelementtmp17, align 4
  %arrayelementtmp18 = getelementptr inbounds i32, ptr %arrayelementtmp17, i64 1
  store i32 15, ptr %arrayelementtmp18, align 4
  %arrayelementtmp19 = getelementptr inbounds i32, ptr %arrayelementtmp18, i64 1
  store i32 16, ptr %arrayelementtmp19, align 4
  %b = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [4 x i32], ptr %a, i64 1
  %lhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %subscripttmp, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp20 = getelementptr inbounds i32, ptr %b, i64 1
  %rhstmp21 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %rhs22 = load i32, ptr %rhstmp21, align 4
  store i32 %rhs22, ptr %lhstmp20, align 4
  %lhstmp23 = getelementptr inbounds i32, ptr %b, i64 2
  %rhstmp24 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  %rhs25 = load i32, ptr %rhstmp24, align 4
  store i32 %rhs25, ptr %lhstmp23, align 4
  %lhstmp26 = getelementptr inbounds i32, ptr %b, i64 3
  %rhstmp27 = getelementptr inbounds i32, ptr %subscripttmp, i64 3
  %rhs28 = load i32, ptr %rhstmp27, align 4
  store i32 %rhs28, ptr %lhstmp26, align 4
  br i1 true, label %continuetmp30, label %aborttmp29

aborttmp29:                                       ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp30

continuetmp30:                                    ; preds = %aborttmp29, %continuetmp
  %subscripttmp31 = getelementptr inbounds i32, ptr %b, i64 0
  %elementtmp = load i32, ptr %subscripttmp31, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 5
  }
}
```

## Test
copy array 3.2

```akela
const a: [4][4]Int32 = [
  [1, 2, 3, 4],
  [5, 6, 7, 8],
  [9, 10, 11, 12],
  [13, 14, 15, 16]
]
var b: [4]Int32
b = a[1]
b[1]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x [4 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [4 x [4 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 4, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp6 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 0, i64 0
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  %arrayelementtmp8 = getelementptr inbounds i32, ptr %arrayelementtmp7, i64 1
  store i32 7, ptr %arrayelementtmp8, align 4
  %arrayelementtmp9 = getelementptr inbounds i32, ptr %arrayelementtmp8, i64 1
  store i32 8, ptr %arrayelementtmp9, align 4
  %arrayelementtmp10 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 1
  %arrayelementtmp11 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 0, i64 0
  store i32 9, ptr %arrayelementtmp11, align 4
  %arrayelementtmp12 = getelementptr inbounds i32, ptr %arrayelementtmp11, i64 1
  store i32 10, ptr %arrayelementtmp12, align 4
  %arrayelementtmp13 = getelementptr inbounds i32, ptr %arrayelementtmp12, i64 1
  store i32 11, ptr %arrayelementtmp13, align 4
  %arrayelementtmp14 = getelementptr inbounds i32, ptr %arrayelementtmp13, i64 1
  store i32 12, ptr %arrayelementtmp14, align 4
  %arrayelementtmp15 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 1
  %arrayelementtmp16 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp15, i64 0, i64 0
  store i32 13, ptr %arrayelementtmp16, align 4
  %arrayelementtmp17 = getelementptr inbounds i32, ptr %arrayelementtmp16, i64 1
  store i32 14, ptr %arrayelementtmp17, align 4
  %arrayelementtmp18 = getelementptr inbounds i32, ptr %arrayelementtmp17, i64 1
  store i32 15, ptr %arrayelementtmp18, align 4
  %arrayelementtmp19 = getelementptr inbounds i32, ptr %arrayelementtmp18, i64 1
  store i32 16, ptr %arrayelementtmp19, align 4
  %b = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [4 x i32], ptr %a, i64 1
  %lhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %subscripttmp, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp20 = getelementptr inbounds i32, ptr %b, i64 1
  %rhstmp21 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %rhs22 = load i32, ptr %rhstmp21, align 4
  store i32 %rhs22, ptr %lhstmp20, align 4
  %lhstmp23 = getelementptr inbounds i32, ptr %b, i64 2
  %rhstmp24 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  %rhs25 = load i32, ptr %rhstmp24, align 4
  store i32 %rhs25, ptr %lhstmp23, align 4
  %lhstmp26 = getelementptr inbounds i32, ptr %b, i64 3
  %rhstmp27 = getelementptr inbounds i32, ptr %subscripttmp, i64 3
  %rhs28 = load i32, ptr %rhstmp27, align 4
  store i32 %rhs28, ptr %lhstmp26, align 4
  br i1 true, label %continuetmp30, label %aborttmp29

aborttmp29:                                       ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp30

continuetmp30:                                    ; preds = %aborttmp29, %continuetmp
  %subscripttmp31 = getelementptr inbounds i32, ptr %b, i64 1
  %elementtmp = load i32, ptr %subscripttmp31, align 4
  ret i32 %elementtmp
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
copy array 3.3

```akela
const a: [4][4]Int32 = [
  [1, 2, 3, 4],
  [5, 6, 7, 8],
  [9, 10, 11, 12],
  [13, 14, 15, 16]
]
var b: [4]Int32
b = a[1]
b[2]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x [4 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [4 x [4 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 4, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp6 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 0, i64 0
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  %arrayelementtmp8 = getelementptr inbounds i32, ptr %arrayelementtmp7, i64 1
  store i32 7, ptr %arrayelementtmp8, align 4
  %arrayelementtmp9 = getelementptr inbounds i32, ptr %arrayelementtmp8, i64 1
  store i32 8, ptr %arrayelementtmp9, align 4
  %arrayelementtmp10 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 1
  %arrayelementtmp11 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 0, i64 0
  store i32 9, ptr %arrayelementtmp11, align 4
  %arrayelementtmp12 = getelementptr inbounds i32, ptr %arrayelementtmp11, i64 1
  store i32 10, ptr %arrayelementtmp12, align 4
  %arrayelementtmp13 = getelementptr inbounds i32, ptr %arrayelementtmp12, i64 1
  store i32 11, ptr %arrayelementtmp13, align 4
  %arrayelementtmp14 = getelementptr inbounds i32, ptr %arrayelementtmp13, i64 1
  store i32 12, ptr %arrayelementtmp14, align 4
  %arrayelementtmp15 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 1
  %arrayelementtmp16 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp15, i64 0, i64 0
  store i32 13, ptr %arrayelementtmp16, align 4
  %arrayelementtmp17 = getelementptr inbounds i32, ptr %arrayelementtmp16, i64 1
  store i32 14, ptr %arrayelementtmp17, align 4
  %arrayelementtmp18 = getelementptr inbounds i32, ptr %arrayelementtmp17, i64 1
  store i32 15, ptr %arrayelementtmp18, align 4
  %arrayelementtmp19 = getelementptr inbounds i32, ptr %arrayelementtmp18, i64 1
  store i32 16, ptr %arrayelementtmp19, align 4
  %b = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [4 x i32], ptr %a, i64 1
  %lhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %subscripttmp, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp20 = getelementptr inbounds i32, ptr %b, i64 1
  %rhstmp21 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %rhs22 = load i32, ptr %rhstmp21, align 4
  store i32 %rhs22, ptr %lhstmp20, align 4
  %lhstmp23 = getelementptr inbounds i32, ptr %b, i64 2
  %rhstmp24 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  %rhs25 = load i32, ptr %rhstmp24, align 4
  store i32 %rhs25, ptr %lhstmp23, align 4
  %lhstmp26 = getelementptr inbounds i32, ptr %b, i64 3
  %rhstmp27 = getelementptr inbounds i32, ptr %subscripttmp, i64 3
  %rhs28 = load i32, ptr %rhstmp27, align 4
  store i32 %rhs28, ptr %lhstmp26, align 4
  br i1 true, label %continuetmp30, label %aborttmp29

aborttmp29:                                       ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp30

continuetmp30:                                    ; preds = %aborttmp29, %continuetmp
  %subscripttmp31 = getelementptr inbounds i32, ptr %b, i64 2
  %elementtmp = load i32, ptr %subscripttmp31, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 7
  }
}
```

## Test
copy array 3.4

```akela
const a: [4][4]Int32 = [
  [1, 2, 3, 4],
  [5, 6, 7, 8],
  [9, 10, 11, 12],
  [13, 14, 15, 16]
]
var b: [4]Int32
b = a[1]
b[3]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

declare void @printf(ptr, ...)

declare void @exit(i32)

define i32 @__top_level() {
entry:
  %a = alloca [4 x [4 x i32]], align 4
  %arrayelementtmp = getelementptr inbounds [4 x [4 x i32]], ptr %a, i64 0, i64 0
  %arrayelementtmp1 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 0, i64 0
  store i32 1, ptr %arrayelementtmp1, align 4
  %arrayelementtmp2 = getelementptr inbounds i32, ptr %arrayelementtmp1, i64 1
  store i32 2, ptr %arrayelementtmp2, align 4
  %arrayelementtmp3 = getelementptr inbounds i32, ptr %arrayelementtmp2, i64 1
  store i32 3, ptr %arrayelementtmp3, align 4
  %arrayelementtmp4 = getelementptr inbounds i32, ptr %arrayelementtmp3, i64 1
  store i32 4, ptr %arrayelementtmp4, align 4
  %arrayelementtmp5 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp, i64 1
  %arrayelementtmp6 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 0, i64 0
  store i32 5, ptr %arrayelementtmp6, align 4
  %arrayelementtmp7 = getelementptr inbounds i32, ptr %arrayelementtmp6, i64 1
  store i32 6, ptr %arrayelementtmp7, align 4
  %arrayelementtmp8 = getelementptr inbounds i32, ptr %arrayelementtmp7, i64 1
  store i32 7, ptr %arrayelementtmp8, align 4
  %arrayelementtmp9 = getelementptr inbounds i32, ptr %arrayelementtmp8, i64 1
  store i32 8, ptr %arrayelementtmp9, align 4
  %arrayelementtmp10 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp5, i64 1
  %arrayelementtmp11 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 0, i64 0
  store i32 9, ptr %arrayelementtmp11, align 4
  %arrayelementtmp12 = getelementptr inbounds i32, ptr %arrayelementtmp11, i64 1
  store i32 10, ptr %arrayelementtmp12, align 4
  %arrayelementtmp13 = getelementptr inbounds i32, ptr %arrayelementtmp12, i64 1
  store i32 11, ptr %arrayelementtmp13, align 4
  %arrayelementtmp14 = getelementptr inbounds i32, ptr %arrayelementtmp13, i64 1
  store i32 12, ptr %arrayelementtmp14, align 4
  %arrayelementtmp15 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp10, i64 1
  %arrayelementtmp16 = getelementptr inbounds [4 x i32], ptr %arrayelementtmp15, i64 0, i64 0
  store i32 13, ptr %arrayelementtmp16, align 4
  %arrayelementtmp17 = getelementptr inbounds i32, ptr %arrayelementtmp16, i64 1
  store i32 14, ptr %arrayelementtmp17, align 4
  %arrayelementtmp18 = getelementptr inbounds i32, ptr %arrayelementtmp17, i64 1
  store i32 15, ptr %arrayelementtmp18, align 4
  %arrayelementtmp19 = getelementptr inbounds i32, ptr %arrayelementtmp18, i64 1
  store i32 16, ptr %arrayelementtmp19, align 4
  %b = alloca [4 x i32], align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [4 x i32], ptr %a, i64 1
  %lhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %subscripttmp, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp20 = getelementptr inbounds i32, ptr %b, i64 1
  %rhstmp21 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %rhs22 = load i32, ptr %rhstmp21, align 4
  store i32 %rhs22, ptr %lhstmp20, align 4
  %lhstmp23 = getelementptr inbounds i32, ptr %b, i64 2
  %rhstmp24 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  %rhs25 = load i32, ptr %rhstmp24, align 4
  store i32 %rhs25, ptr %lhstmp23, align 4
  %lhstmp26 = getelementptr inbounds i32, ptr %b, i64 3
  %rhstmp27 = getelementptr inbounds i32, ptr %subscripttmp, i64 3
  %rhs28 = load i32, ptr %rhstmp27, align 4
  store i32 %rhs28, ptr %lhstmp26, align 4
  br i1 true, label %continuetmp30, label %aborttmp29

aborttmp29:                                       ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp30

continuetmp30:                                    ; preds = %aborttmp29, %continuetmp
  %subscripttmp31 = getelementptr inbounds i32, ptr %b, i64 3
  %elementtmp = load i32, ptr %subscripttmp31, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 8
  }
}
```