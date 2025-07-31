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
real

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