# Test Suite
statements

## Test
last

```akela
const a: Int32 = 1
const b: Int32 = 2
a + b
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define i32 @__top_level() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 2, ptr %b, align 4
  %0 = load i32, ptr %a, align 4
  %1 = load i32, ptr %b, align 4
  %addtmp = add i32 %0, %1
  ret i32 %addtmp
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
if

```akela
if true
    1
end
```

```llvm
/ModuleID/
/source_filename/
/target datalayout/

define void @__top_level() {
entry:
  br i1 true, label %thentmp, label %nexttmp

thentmp:                                          ; preds = %entry
  br label %nexttmp

nexttmp:                                          ; preds = %thentmp, %entry
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