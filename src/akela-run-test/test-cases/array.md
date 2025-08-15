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
  .mute = true
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
    .type = Type::Int32
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
    .type = Type::Int32
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
    .type = Type::Int32
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
    .type = Type::Int32
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
    .type = Type::Int32
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

## Test
subscript exit

```akela
const a: [4]Int32 = [1,2,3,4]
a[4]
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
  br i1 false, label %continuetmp, label %aborttmp

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
  .mute = true
  Field {
    .type = Type::Void
  }
}
```

## Test
assign 2d subscript

```akela
var a: [4][4]Int32 = [
    [1,2,3,4],
    [5,6,7,8],
    [9,10,11,12],
    [13,14,15,16]
]
const b: [4]Int32 = [50,60,70,80]
a[1] = b
a[1][0]
```

```llvm
; ModuleID = 'Akela JIT'
source_filename = "Akela JIT"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.1 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1
@.str.2 = private unnamed_addr constant [24 x i8] c"invalid subscript index\00", align 1

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
  %arrayelementtmp20 = getelementptr inbounds [4 x i32], ptr %b, i64 0, i64 0
  store i32 50, ptr %arrayelementtmp20, align 4
  %arrayelementtmp21 = getelementptr inbounds i32, ptr %arrayelementtmp20, i64 1
  store i32 60, ptr %arrayelementtmp21, align 4
  %arrayelementtmp22 = getelementptr inbounds i32, ptr %arrayelementtmp21, i64 1
  store i32 70, ptr %arrayelementtmp22, align 4
  %arrayelementtmp23 = getelementptr inbounds i32, ptr %arrayelementtmp22, i64 1
  store i32 80, ptr %arrayelementtmp23, align 4
  br i1 true, label %continuetmp, label %aborttmp

aborttmp:                                         ; preds = %entry
  call void (ptr, ...) @printf(ptr @.str)
  call void @exit(i32 1)
  br label %continuetmp

continuetmp:                                      ; preds = %aborttmp, %entry
  %subscripttmp = getelementptr inbounds [4 x i32], ptr %a, i64 1
  %lhstmp = getelementptr inbounds i32, ptr %subscripttmp, i64 0
  %rhstmp = getelementptr inbounds i32, ptr %b, i64 0
  %rhs = load i32, ptr %rhstmp, align 4
  store i32 %rhs, ptr %lhstmp, align 4
  %lhstmp24 = getelementptr inbounds i32, ptr %subscripttmp, i64 1
  %rhstmp25 = getelementptr inbounds i32, ptr %b, i64 1
  %rhs26 = load i32, ptr %rhstmp25, align 4
  store i32 %rhs26, ptr %lhstmp24, align 4
  %lhstmp27 = getelementptr inbounds i32, ptr %subscripttmp, i64 2
  %rhstmp28 = getelementptr inbounds i32, ptr %b, i64 2
  %rhs29 = load i32, ptr %rhstmp28, align 4
  store i32 %rhs29, ptr %lhstmp27, align 4
  %lhstmp30 = getelementptr inbounds i32, ptr %subscripttmp, i64 3
  %rhstmp31 = getelementptr inbounds i32, ptr %b, i64 3
  %rhs32 = load i32, ptr %rhstmp31, align 4
  store i32 %rhs32, ptr %lhstmp30, align 4
  br i1 true, label %continuetmp34, label %aborttmp33

aborttmp33:                                       ; preds = %continuetmp
  call void (ptr, ...) @printf(ptr @.str.1)
  call void @exit(i32 1)
  br label %continuetmp34

continuetmp34:                                    ; preds = %aborttmp33, %continuetmp
  %subscripttmp35 = getelementptr inbounds [4 x i32], ptr %a, i64 1
  br i1 true, label %continuetmp37, label %aborttmp36

aborttmp36:                                       ; preds = %continuetmp34
  call void (ptr, ...) @printf(ptr @.str.2)
  call void @exit(i32 1)
  br label %continuetmp37

continuetmp37:                                    ; preds = %aborttmp36, %continuetmp34
  %subscripttmp38 = getelementptr inbounds i32, ptr %subscripttmp35, i64 0
  %elementtmp = load i32, ptr %subscripttmp38, align 4
  ret i32 %elementtmp
}
```

```cent
use lib::base::*
Test {
  Field {
    .type = Type::Int32
    .value = 50
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
const a: [5 const]Nat8 = "John"
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
  %a = alloca [5 x i8], align 1
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
array const smaller to bigger 1

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
const array copy 3

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
  %b = alloca [100 x i8], align 1
  %lhstmp9 = getelementptr inbounds i8, ptr %b, i64 0
  %rhstmp = getelementptr inbounds i8, ptr %a, i64 0
  %rhs10 = load i8, ptr %rhstmp, align 1
  store i8 %rhs10, ptr %lhstmp9, align 1
  %lhstmp11 = getelementptr inbounds i8, ptr %b, i64 1
  %rhstmp12 = getelementptr inbounds i8, ptr %a, i64 1
  %rhs13 = load i8, ptr %rhstmp12, align 1
  store i8 %rhs13, ptr %lhstmp11, align 1
  %lhstmp14 = getelementptr inbounds i8, ptr %b, i64 2
  %rhstmp15 = getelementptr inbounds i8, ptr %a, i64 2
  %rhs16 = load i8, ptr %rhstmp15, align 1
  store i8 %rhs16, ptr %lhstmp14, align 1
  %lhstmp17 = getelementptr inbounds i8, ptr %b, i64 3
  %rhstmp18 = getelementptr inbounds i8, ptr %a, i64 3
  %rhs19 = load i8, ptr %rhstmp18, align 1
  store i8 %rhs19, ptr %lhstmp17, align 1
  %lhstmp20 = getelementptr inbounds i8, ptr %b, i64 4
  %rhstmp21 = getelementptr inbounds i8, ptr %a, i64 4
  %rhs22 = load i8, ptr %rhstmp21, align 1
  store i8 %rhs22, ptr %lhstmp20, align 1
  %lhstmp23 = getelementptr inbounds i8, ptr %b, i64 5
  %rhstmp24 = getelementptr inbounds i8, ptr %a, i64 5
  %rhs25 = load i8, ptr %rhstmp24, align 1
  store i8 %rhs25, ptr %lhstmp23, align 1
  %lhstmp26 = getelementptr inbounds i8, ptr %b, i64 6
  %rhstmp27 = getelementptr inbounds i8, ptr %a, i64 6
  %rhs28 = load i8, ptr %rhstmp27, align 1
  store i8 %rhs28, ptr %lhstmp26, align 1
  %lhstmp29 = getelementptr inbounds i8, ptr %b, i64 7
  %rhstmp30 = getelementptr inbounds i8, ptr %a, i64 7
  %rhs31 = load i8, ptr %rhstmp30, align 1
  store i8 %rhs31, ptr %lhstmp29, align 1
  %lhstmp32 = getelementptr inbounds i8, ptr %b, i64 8
  %rhstmp33 = getelementptr inbounds i8, ptr %a, i64 8
  %rhs34 = load i8, ptr %rhstmp33, align 1
  store i8 %rhs34, ptr %lhstmp32, align 1
  %lhstmp35 = getelementptr inbounds i8, ptr %b, i64 9
  %rhstmp36 = getelementptr inbounds i8, ptr %a, i64 9
  %rhs37 = load i8, ptr %rhstmp36, align 1
  store i8 %rhs37, ptr %lhstmp35, align 1
  %lhstmp38 = getelementptr inbounds i8, ptr %b, i64 10
  %rhstmp39 = getelementptr inbounds i8, ptr %a, i64 10
  %rhs40 = load i8, ptr %rhstmp39, align 1
  store i8 %rhs40, ptr %lhstmp38, align 1
  %lhstmp41 = getelementptr inbounds i8, ptr %b, i64 11
  %rhstmp42 = getelementptr inbounds i8, ptr %a, i64 11
  %rhs43 = load i8, ptr %rhstmp42, align 1
  store i8 %rhs43, ptr %lhstmp41, align 1
  %lhstmp44 = getelementptr inbounds i8, ptr %b, i64 12
  %rhstmp45 = getelementptr inbounds i8, ptr %a, i64 12
  %rhs46 = load i8, ptr %rhstmp45, align 1
  store i8 %rhs46, ptr %lhstmp44, align 1
  %lhstmp47 = getelementptr inbounds i8, ptr %b, i64 13
  %rhstmp48 = getelementptr inbounds i8, ptr %a, i64 13
  %rhs49 = load i8, ptr %rhstmp48, align 1
  store i8 %rhs49, ptr %lhstmp47, align 1
  %lhstmp50 = getelementptr inbounds i8, ptr %b, i64 14
  %rhstmp51 = getelementptr inbounds i8, ptr %a, i64 14
  %rhs52 = load i8, ptr %rhstmp51, align 1
  store i8 %rhs52, ptr %lhstmp50, align 1
  %lhstmp53 = getelementptr inbounds i8, ptr %b, i64 15
  %rhstmp54 = getelementptr inbounds i8, ptr %a, i64 15
  %rhs55 = load i8, ptr %rhstmp54, align 1
  store i8 %rhs55, ptr %lhstmp53, align 1
  %lhstmp56 = getelementptr inbounds i8, ptr %b, i64 16
  %rhstmp57 = getelementptr inbounds i8, ptr %a, i64 16
  %rhs58 = load i8, ptr %rhstmp57, align 1
  store i8 %rhs58, ptr %lhstmp56, align 1
  %lhstmp59 = getelementptr inbounds i8, ptr %b, i64 17
  %rhstmp60 = getelementptr inbounds i8, ptr %a, i64 17
  %rhs61 = load i8, ptr %rhstmp60, align 1
  store i8 %rhs61, ptr %lhstmp59, align 1
  %lhstmp62 = getelementptr inbounds i8, ptr %b, i64 18
  %rhstmp63 = getelementptr inbounds i8, ptr %a, i64 18
  %rhs64 = load i8, ptr %rhstmp63, align 1
  store i8 %rhs64, ptr %lhstmp62, align 1
  %lhstmp65 = getelementptr inbounds i8, ptr %b, i64 19
  %rhstmp66 = getelementptr inbounds i8, ptr %a, i64 19
  %rhs67 = load i8, ptr %rhstmp66, align 1
  store i8 %rhs67, ptr %lhstmp65, align 1
  %lhstmp68 = getelementptr inbounds i8, ptr %b, i64 20
  %rhstmp69 = getelementptr inbounds i8, ptr %a, i64 20
  %rhs70 = load i8, ptr %rhstmp69, align 1
  store i8 %rhs70, ptr %lhstmp68, align 1
  %lhstmp71 = getelementptr inbounds i8, ptr %b, i64 21
  %rhstmp72 = getelementptr inbounds i8, ptr %a, i64 21
  %rhs73 = load i8, ptr %rhstmp72, align 1
  store i8 %rhs73, ptr %lhstmp71, align 1
  %lhstmp74 = getelementptr inbounds i8, ptr %b, i64 22
  %rhstmp75 = getelementptr inbounds i8, ptr %a, i64 22
  %rhs76 = load i8, ptr %rhstmp75, align 1
  store i8 %rhs76, ptr %lhstmp74, align 1
  %lhstmp77 = getelementptr inbounds i8, ptr %b, i64 23
  %rhstmp78 = getelementptr inbounds i8, ptr %a, i64 23
  %rhs79 = load i8, ptr %rhstmp78, align 1
  store i8 %rhs79, ptr %lhstmp77, align 1
  %lhstmp80 = getelementptr inbounds i8, ptr %b, i64 24
  %rhstmp81 = getelementptr inbounds i8, ptr %a, i64 24
  %rhs82 = load i8, ptr %rhstmp81, align 1
  store i8 %rhs82, ptr %lhstmp80, align 1
  %lhstmp83 = getelementptr inbounds i8, ptr %b, i64 25
  %rhstmp84 = getelementptr inbounds i8, ptr %a, i64 25
  %rhs85 = load i8, ptr %rhstmp84, align 1
  store i8 %rhs85, ptr %lhstmp83, align 1
  %lhstmp86 = getelementptr inbounds i8, ptr %b, i64 26
  %rhstmp87 = getelementptr inbounds i8, ptr %a, i64 26
  %rhs88 = load i8, ptr %rhstmp87, align 1
  store i8 %rhs88, ptr %lhstmp86, align 1
  %lhstmp89 = getelementptr inbounds i8, ptr %b, i64 27
  %rhstmp90 = getelementptr inbounds i8, ptr %a, i64 27
  %rhs91 = load i8, ptr %rhstmp90, align 1
  store i8 %rhs91, ptr %lhstmp89, align 1
  %lhstmp92 = getelementptr inbounds i8, ptr %b, i64 28
  %rhstmp93 = getelementptr inbounds i8, ptr %a, i64 28
  %rhs94 = load i8, ptr %rhstmp93, align 1
  store i8 %rhs94, ptr %lhstmp92, align 1
  %lhstmp95 = getelementptr inbounds i8, ptr %b, i64 29
  %rhstmp96 = getelementptr inbounds i8, ptr %a, i64 29
  %rhs97 = load i8, ptr %rhstmp96, align 1
  store i8 %rhs97, ptr %lhstmp95, align 1
  %lhstmp98 = getelementptr inbounds i8, ptr %b, i64 30
  %rhstmp99 = getelementptr inbounds i8, ptr %a, i64 30
  %rhs100 = load i8, ptr %rhstmp99, align 1
  store i8 %rhs100, ptr %lhstmp98, align 1
  %lhstmp101 = getelementptr inbounds i8, ptr %b, i64 31
  %rhstmp102 = getelementptr inbounds i8, ptr %a, i64 31
  %rhs103 = load i8, ptr %rhstmp102, align 1
  store i8 %rhs103, ptr %lhstmp101, align 1
  %lhstmp104 = getelementptr inbounds i8, ptr %b, i64 32
  %rhstmp105 = getelementptr inbounds i8, ptr %a, i64 32
  %rhs106 = load i8, ptr %rhstmp105, align 1
  store i8 %rhs106, ptr %lhstmp104, align 1
  %lhstmp107 = getelementptr inbounds i8, ptr %b, i64 33
  %rhstmp108 = getelementptr inbounds i8, ptr %a, i64 33
  %rhs109 = load i8, ptr %rhstmp108, align 1
  store i8 %rhs109, ptr %lhstmp107, align 1
  %lhstmp110 = getelementptr inbounds i8, ptr %b, i64 34
  %rhstmp111 = getelementptr inbounds i8, ptr %a, i64 34
  %rhs112 = load i8, ptr %rhstmp111, align 1
  store i8 %rhs112, ptr %lhstmp110, align 1
  %lhstmp113 = getelementptr inbounds i8, ptr %b, i64 35
  %rhstmp114 = getelementptr inbounds i8, ptr %a, i64 35
  %rhs115 = load i8, ptr %rhstmp114, align 1
  store i8 %rhs115, ptr %lhstmp113, align 1
  %lhstmp116 = getelementptr inbounds i8, ptr %b, i64 36
  %rhstmp117 = getelementptr inbounds i8, ptr %a, i64 36
  %rhs118 = load i8, ptr %rhstmp117, align 1
  store i8 %rhs118, ptr %lhstmp116, align 1
  %lhstmp119 = getelementptr inbounds i8, ptr %b, i64 37
  %rhstmp120 = getelementptr inbounds i8, ptr %a, i64 37
  %rhs121 = load i8, ptr %rhstmp120, align 1
  store i8 %rhs121, ptr %lhstmp119, align 1
  %lhstmp122 = getelementptr inbounds i8, ptr %b, i64 38
  %rhstmp123 = getelementptr inbounds i8, ptr %a, i64 38
  %rhs124 = load i8, ptr %rhstmp123, align 1
  store i8 %rhs124, ptr %lhstmp122, align 1
  %lhstmp125 = getelementptr inbounds i8, ptr %b, i64 39
  %rhstmp126 = getelementptr inbounds i8, ptr %a, i64 39
  %rhs127 = load i8, ptr %rhstmp126, align 1
  store i8 %rhs127, ptr %lhstmp125, align 1
  %lhstmp128 = getelementptr inbounds i8, ptr %b, i64 40
  %rhstmp129 = getelementptr inbounds i8, ptr %a, i64 40
  %rhs130 = load i8, ptr %rhstmp129, align 1
  store i8 %rhs130, ptr %lhstmp128, align 1
  %lhstmp131 = getelementptr inbounds i8, ptr %b, i64 41
  %rhstmp132 = getelementptr inbounds i8, ptr %a, i64 41
  %rhs133 = load i8, ptr %rhstmp132, align 1
  store i8 %rhs133, ptr %lhstmp131, align 1
  %lhstmp134 = getelementptr inbounds i8, ptr %b, i64 42
  %rhstmp135 = getelementptr inbounds i8, ptr %a, i64 42
  %rhs136 = load i8, ptr %rhstmp135, align 1
  store i8 %rhs136, ptr %lhstmp134, align 1
  %lhstmp137 = getelementptr inbounds i8, ptr %b, i64 43
  %rhstmp138 = getelementptr inbounds i8, ptr %a, i64 43
  %rhs139 = load i8, ptr %rhstmp138, align 1
  store i8 %rhs139, ptr %lhstmp137, align 1
  %lhstmp140 = getelementptr inbounds i8, ptr %b, i64 44
  %rhstmp141 = getelementptr inbounds i8, ptr %a, i64 44
  %rhs142 = load i8, ptr %rhstmp141, align 1
  store i8 %rhs142, ptr %lhstmp140, align 1
  %lhstmp143 = getelementptr inbounds i8, ptr %b, i64 45
  %rhstmp144 = getelementptr inbounds i8, ptr %a, i64 45
  %rhs145 = load i8, ptr %rhstmp144, align 1
  store i8 %rhs145, ptr %lhstmp143, align 1
  %lhstmp146 = getelementptr inbounds i8, ptr %b, i64 46
  %rhstmp147 = getelementptr inbounds i8, ptr %a, i64 46
  %rhs148 = load i8, ptr %rhstmp147, align 1
  store i8 %rhs148, ptr %lhstmp146, align 1
  %lhstmp149 = getelementptr inbounds i8, ptr %b, i64 47
  %rhstmp150 = getelementptr inbounds i8, ptr %a, i64 47
  %rhs151 = load i8, ptr %rhstmp150, align 1
  store i8 %rhs151, ptr %lhstmp149, align 1
  %lhstmp152 = getelementptr inbounds i8, ptr %b, i64 48
  %rhstmp153 = getelementptr inbounds i8, ptr %a, i64 48
  %rhs154 = load i8, ptr %rhstmp153, align 1
  store i8 %rhs154, ptr %lhstmp152, align 1
  %lhstmp155 = getelementptr inbounds i8, ptr %b, i64 49
  %rhstmp156 = getelementptr inbounds i8, ptr %a, i64 49
  %rhs157 = load i8, ptr %rhstmp156, align 1
  store i8 %rhs157, ptr %lhstmp155, align 1
  %lhstmp158 = getelementptr inbounds i8, ptr %b, i64 50
  %rhstmp159 = getelementptr inbounds i8, ptr %a, i64 50
  %rhs160 = load i8, ptr %rhstmp159, align 1
  store i8 %rhs160, ptr %lhstmp158, align 1
  %lhstmp161 = getelementptr inbounds i8, ptr %b, i64 51
  %rhstmp162 = getelementptr inbounds i8, ptr %a, i64 51
  %rhs163 = load i8, ptr %rhstmp162, align 1
  store i8 %rhs163, ptr %lhstmp161, align 1
  %lhstmp164 = getelementptr inbounds i8, ptr %b, i64 52
  %rhstmp165 = getelementptr inbounds i8, ptr %a, i64 52
  %rhs166 = load i8, ptr %rhstmp165, align 1
  store i8 %rhs166, ptr %lhstmp164, align 1
  %lhstmp167 = getelementptr inbounds i8, ptr %b, i64 53
  %rhstmp168 = getelementptr inbounds i8, ptr %a, i64 53
  %rhs169 = load i8, ptr %rhstmp168, align 1
  store i8 %rhs169, ptr %lhstmp167, align 1
  %lhstmp170 = getelementptr inbounds i8, ptr %b, i64 54
  %rhstmp171 = getelementptr inbounds i8, ptr %a, i64 54
  %rhs172 = load i8, ptr %rhstmp171, align 1
  store i8 %rhs172, ptr %lhstmp170, align 1
  %lhstmp173 = getelementptr inbounds i8, ptr %b, i64 55
  %rhstmp174 = getelementptr inbounds i8, ptr %a, i64 55
  %rhs175 = load i8, ptr %rhstmp174, align 1
  store i8 %rhs175, ptr %lhstmp173, align 1
  %lhstmp176 = getelementptr inbounds i8, ptr %b, i64 56
  %rhstmp177 = getelementptr inbounds i8, ptr %a, i64 56
  %rhs178 = load i8, ptr %rhstmp177, align 1
  store i8 %rhs178, ptr %lhstmp176, align 1
  %lhstmp179 = getelementptr inbounds i8, ptr %b, i64 57
  %rhstmp180 = getelementptr inbounds i8, ptr %a, i64 57
  %rhs181 = load i8, ptr %rhstmp180, align 1
  store i8 %rhs181, ptr %lhstmp179, align 1
  %lhstmp182 = getelementptr inbounds i8, ptr %b, i64 58
  %rhstmp183 = getelementptr inbounds i8, ptr %a, i64 58
  %rhs184 = load i8, ptr %rhstmp183, align 1
  store i8 %rhs184, ptr %lhstmp182, align 1
  %lhstmp185 = getelementptr inbounds i8, ptr %b, i64 59
  %rhstmp186 = getelementptr inbounds i8, ptr %a, i64 59
  %rhs187 = load i8, ptr %rhstmp186, align 1
  store i8 %rhs187, ptr %lhstmp185, align 1
  %lhstmp188 = getelementptr inbounds i8, ptr %b, i64 60
  %rhstmp189 = getelementptr inbounds i8, ptr %a, i64 60
  %rhs190 = load i8, ptr %rhstmp189, align 1
  store i8 %rhs190, ptr %lhstmp188, align 1
  %lhstmp191 = getelementptr inbounds i8, ptr %b, i64 61
  %rhstmp192 = getelementptr inbounds i8, ptr %a, i64 61
  %rhs193 = load i8, ptr %rhstmp192, align 1
  store i8 %rhs193, ptr %lhstmp191, align 1
  %lhstmp194 = getelementptr inbounds i8, ptr %b, i64 62
  %rhstmp195 = getelementptr inbounds i8, ptr %a, i64 62
  %rhs196 = load i8, ptr %rhstmp195, align 1
  store i8 %rhs196, ptr %lhstmp194, align 1
  %lhstmp197 = getelementptr inbounds i8, ptr %b, i64 63
  %rhstmp198 = getelementptr inbounds i8, ptr %a, i64 63
  %rhs199 = load i8, ptr %rhstmp198, align 1
  store i8 %rhs199, ptr %lhstmp197, align 1
  %lhstmp200 = getelementptr inbounds i8, ptr %b, i64 64
  %rhstmp201 = getelementptr inbounds i8, ptr %a, i64 64
  %rhs202 = load i8, ptr %rhstmp201, align 1
  store i8 %rhs202, ptr %lhstmp200, align 1
  %lhstmp203 = getelementptr inbounds i8, ptr %b, i64 65
  %rhstmp204 = getelementptr inbounds i8, ptr %a, i64 65
  %rhs205 = load i8, ptr %rhstmp204, align 1
  store i8 %rhs205, ptr %lhstmp203, align 1
  %lhstmp206 = getelementptr inbounds i8, ptr %b, i64 66
  %rhstmp207 = getelementptr inbounds i8, ptr %a, i64 66
  %rhs208 = load i8, ptr %rhstmp207, align 1
  store i8 %rhs208, ptr %lhstmp206, align 1
  %lhstmp209 = getelementptr inbounds i8, ptr %b, i64 67
  %rhstmp210 = getelementptr inbounds i8, ptr %a, i64 67
  %rhs211 = load i8, ptr %rhstmp210, align 1
  store i8 %rhs211, ptr %lhstmp209, align 1
  %lhstmp212 = getelementptr inbounds i8, ptr %b, i64 68
  %rhstmp213 = getelementptr inbounds i8, ptr %a, i64 68
  %rhs214 = load i8, ptr %rhstmp213, align 1
  store i8 %rhs214, ptr %lhstmp212, align 1
  %lhstmp215 = getelementptr inbounds i8, ptr %b, i64 69
  %rhstmp216 = getelementptr inbounds i8, ptr %a, i64 69
  %rhs217 = load i8, ptr %rhstmp216, align 1
  store i8 %rhs217, ptr %lhstmp215, align 1
  %lhstmp218 = getelementptr inbounds i8, ptr %b, i64 70
  %rhstmp219 = getelementptr inbounds i8, ptr %a, i64 70
  %rhs220 = load i8, ptr %rhstmp219, align 1
  store i8 %rhs220, ptr %lhstmp218, align 1
  %lhstmp221 = getelementptr inbounds i8, ptr %b, i64 71
  %rhstmp222 = getelementptr inbounds i8, ptr %a, i64 71
  %rhs223 = load i8, ptr %rhstmp222, align 1
  store i8 %rhs223, ptr %lhstmp221, align 1
  %lhstmp224 = getelementptr inbounds i8, ptr %b, i64 72
  %rhstmp225 = getelementptr inbounds i8, ptr %a, i64 72
  %rhs226 = load i8, ptr %rhstmp225, align 1
  store i8 %rhs226, ptr %lhstmp224, align 1
  %lhstmp227 = getelementptr inbounds i8, ptr %b, i64 73
  %rhstmp228 = getelementptr inbounds i8, ptr %a, i64 73
  %rhs229 = load i8, ptr %rhstmp228, align 1
  store i8 %rhs229, ptr %lhstmp227, align 1
  %lhstmp230 = getelementptr inbounds i8, ptr %b, i64 74
  %rhstmp231 = getelementptr inbounds i8, ptr %a, i64 74
  %rhs232 = load i8, ptr %rhstmp231, align 1
  store i8 %rhs232, ptr %lhstmp230, align 1
  %lhstmp233 = getelementptr inbounds i8, ptr %b, i64 75
  %rhstmp234 = getelementptr inbounds i8, ptr %a, i64 75
  %rhs235 = load i8, ptr %rhstmp234, align 1
  store i8 %rhs235, ptr %lhstmp233, align 1
  %lhstmp236 = getelementptr inbounds i8, ptr %b, i64 76
  %rhstmp237 = getelementptr inbounds i8, ptr %a, i64 76
  %rhs238 = load i8, ptr %rhstmp237, align 1
  store i8 %rhs238, ptr %lhstmp236, align 1
  %lhstmp239 = getelementptr inbounds i8, ptr %b, i64 77
  %rhstmp240 = getelementptr inbounds i8, ptr %a, i64 77
  %rhs241 = load i8, ptr %rhstmp240, align 1
  store i8 %rhs241, ptr %lhstmp239, align 1
  %lhstmp242 = getelementptr inbounds i8, ptr %b, i64 78
  %rhstmp243 = getelementptr inbounds i8, ptr %a, i64 78
  %rhs244 = load i8, ptr %rhstmp243, align 1
  store i8 %rhs244, ptr %lhstmp242, align 1
  %lhstmp245 = getelementptr inbounds i8, ptr %b, i64 79
  %rhstmp246 = getelementptr inbounds i8, ptr %a, i64 79
  %rhs247 = load i8, ptr %rhstmp246, align 1
  store i8 %rhs247, ptr %lhstmp245, align 1
  %lhstmp248 = getelementptr inbounds i8, ptr %b, i64 80
  %rhstmp249 = getelementptr inbounds i8, ptr %a, i64 80
  %rhs250 = load i8, ptr %rhstmp249, align 1
  store i8 %rhs250, ptr %lhstmp248, align 1
  %lhstmp251 = getelementptr inbounds i8, ptr %b, i64 81
  %rhstmp252 = getelementptr inbounds i8, ptr %a, i64 81
  %rhs253 = load i8, ptr %rhstmp252, align 1
  store i8 %rhs253, ptr %lhstmp251, align 1
  %lhstmp254 = getelementptr inbounds i8, ptr %b, i64 82
  %rhstmp255 = getelementptr inbounds i8, ptr %a, i64 82
  %rhs256 = load i8, ptr %rhstmp255, align 1
  store i8 %rhs256, ptr %lhstmp254, align 1
  %lhstmp257 = getelementptr inbounds i8, ptr %b, i64 83
  %rhstmp258 = getelementptr inbounds i8, ptr %a, i64 83
  %rhs259 = load i8, ptr %rhstmp258, align 1
  store i8 %rhs259, ptr %lhstmp257, align 1
  %lhstmp260 = getelementptr inbounds i8, ptr %b, i64 84
  %rhstmp261 = getelementptr inbounds i8, ptr %a, i64 84
  %rhs262 = load i8, ptr %rhstmp261, align 1
  store i8 %rhs262, ptr %lhstmp260, align 1
  %lhstmp263 = getelementptr inbounds i8, ptr %b, i64 85
  %rhstmp264 = getelementptr inbounds i8, ptr %a, i64 85
  %rhs265 = load i8, ptr %rhstmp264, align 1
  store i8 %rhs265, ptr %lhstmp263, align 1
  %lhstmp266 = getelementptr inbounds i8, ptr %b, i64 86
  %rhstmp267 = getelementptr inbounds i8, ptr %a, i64 86
  %rhs268 = load i8, ptr %rhstmp267, align 1
  store i8 %rhs268, ptr %lhstmp266, align 1
  %lhstmp269 = getelementptr inbounds i8, ptr %b, i64 87
  %rhstmp270 = getelementptr inbounds i8, ptr %a, i64 87
  %rhs271 = load i8, ptr %rhstmp270, align 1
  store i8 %rhs271, ptr %lhstmp269, align 1
  %lhstmp272 = getelementptr inbounds i8, ptr %b, i64 88
  %rhstmp273 = getelementptr inbounds i8, ptr %a, i64 88
  %rhs274 = load i8, ptr %rhstmp273, align 1
  store i8 %rhs274, ptr %lhstmp272, align 1
  %lhstmp275 = getelementptr inbounds i8, ptr %b, i64 89
  %rhstmp276 = getelementptr inbounds i8, ptr %a, i64 89
  %rhs277 = load i8, ptr %rhstmp276, align 1
  store i8 %rhs277, ptr %lhstmp275, align 1
  %lhstmp278 = getelementptr inbounds i8, ptr %b, i64 90
  %rhstmp279 = getelementptr inbounds i8, ptr %a, i64 90
  %rhs280 = load i8, ptr %rhstmp279, align 1
  store i8 %rhs280, ptr %lhstmp278, align 1
  %lhstmp281 = getelementptr inbounds i8, ptr %b, i64 91
  %rhstmp282 = getelementptr inbounds i8, ptr %a, i64 91
  %rhs283 = load i8, ptr %rhstmp282, align 1
  store i8 %rhs283, ptr %lhstmp281, align 1
  %lhstmp284 = getelementptr inbounds i8, ptr %b, i64 92
  %rhstmp285 = getelementptr inbounds i8, ptr %a, i64 92
  %rhs286 = load i8, ptr %rhstmp285, align 1
  store i8 %rhs286, ptr %lhstmp284, align 1
  %lhstmp287 = getelementptr inbounds i8, ptr %b, i64 93
  %rhstmp288 = getelementptr inbounds i8, ptr %a, i64 93
  %rhs289 = load i8, ptr %rhstmp288, align 1
  store i8 %rhs289, ptr %lhstmp287, align 1
  %lhstmp290 = getelementptr inbounds i8, ptr %b, i64 94
  %rhstmp291 = getelementptr inbounds i8, ptr %a, i64 94
  %rhs292 = load i8, ptr %rhstmp291, align 1
  store i8 %rhs292, ptr %lhstmp290, align 1
  %lhstmp293 = getelementptr inbounds i8, ptr %b, i64 95
  %rhstmp294 = getelementptr inbounds i8, ptr %a, i64 95
  %rhs295 = load i8, ptr %rhstmp294, align 1
  store i8 %rhs295, ptr %lhstmp293, align 1
  %lhstmp296 = getelementptr inbounds i8, ptr %b, i64 96
  %rhstmp297 = getelementptr inbounds i8, ptr %a, i64 96
  %rhs298 = load i8, ptr %rhstmp297, align 1
  store i8 %rhs298, ptr %lhstmp296, align 1
  %lhstmp299 = getelementptr inbounds i8, ptr %b, i64 97
  %rhstmp300 = getelementptr inbounds i8, ptr %a, i64 97
  %rhs301 = load i8, ptr %rhstmp300, align 1
  store i8 %rhs301, ptr %lhstmp299, align 1
  %lhstmp302 = getelementptr inbounds i8, ptr %b, i64 98
  %rhstmp303 = getelementptr inbounds i8, ptr %a, i64 98
  %rhs304 = load i8, ptr %rhstmp303, align 1
  store i8 %rhs304, ptr %lhstmp302, align 1
  %lhstmp305 = getelementptr inbounds i8, ptr %b, i64 99
  %rhstmp306 = getelementptr inbounds i8, ptr %a, i64 99
  %rhs307 = load i8, ptr %rhstmp306, align 1
  store i8 %rhs307, ptr %lhstmp305, align 1
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