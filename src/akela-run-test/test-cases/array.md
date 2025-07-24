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