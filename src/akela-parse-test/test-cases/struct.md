# Test Suite
struct

```cent
TestSuite {
}
```

## Test
field assign

```cent
Test {
}
```

```akela
struct Person
  firstName: [100 const]Nat8
  lastName: [100 const]Nat8
  age: Int32
end
var p: Person
p.firstName = "John"
p.firstName
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = true
  .dim = 100
  .type = Type::Natural {
    .name = "Nat8"
    .bit_count = 8
  }
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type2 = Type::Struct {
  .name = "Person"
  TypeField {
    .name = "firstName"
    .type = Type::Array {
      .is_const = true
      .dim = 100
      .type = Type::Natural {
        .name = "Nat8"
        .bit_count = 8
      }
    }
  }
  TypeField {
    .name = "lastName"
    .type = Type::Array {
      .is_const = true
      .dim = 100
      .type = Type::Natural {
        .name = "Nat8"
        .bit_count = 8
      }
    }
  }
  TypeField {
    .name = "age"
    .type = Type::Integer {
      .name = "Int32"
      .bit_count = 32
    }
  }
}
const type3 = Type::Array {
  .is_const = true
  .dim = 5
  .type = Type::Natural {
    .name = "Nat8"
    .bit_count = 8
  }
}
Ast::Stmts {
  .type = type0
  Ast::Struct {
    .value = "Person"
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "firstName"
      }
      .type_node = Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "lastName"
      }
      .type_node = Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "age"
      }
      .type_node = Ast::Type {
        .type = type1
      }
    }
  }
  Ast::Var {
    Ast::Id {
      .value = "p"
    }
    Ast::Type {
      .type = type2
    }
  }
  Ast::Assign {
    .left = Ast::Dot {
      .left = Ast::Id {
        .value = "p"
        .type = type2
      }
      .right = Ast::Id {
        .value = "firstName"
      }
      .type = type0
    }
    .right = Ast::String {
      .value = "John"
      .type = type3
    }
  }
  Ast::Dot {
    .left = Ast::Id {
      .value = "p"
      .type = type2
    }
    .right = Ast::Id {
      .value = "firstName"
    }
    .type = type0
  }
}
```

## Test
const literal struct

```cent
Test {
}
```

```akela
struct Person
  first_name: [100 const]Nat8
  last_name: [100 const]Nat8
  age: Int32
end
const p: Person = Person
  first_name: "John"
  last_name: "Smith"
  age: 35
end
```

```cent
use lib::base::*
const type0 = Type::Array {
  .is_const = true
  .dim = 100
  .type = Type::Natural {
    .name = "Nat8"
    .bit_count = 8
  }
}
const type1 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
const type2 = Type::Struct {
  .name = "Person"
  TypeField {
    .name = "first_name"
    .type = Type::Array {
      .is_const = true
      .dim = 100
      .type = Type::Natural {
        .name = "Nat8"
        .bit_count = 8
      }
    }
  }
  TypeField {
    .name = "last_name"
    .type = Type::Array {
      .is_const = true
      .dim = 100
      .type = Type::Natural {
        .name = "Nat8"
        .bit_count = 8
      }
    }
  }
  TypeField {
    .name = "age"
    .type = Type::Integer {
      .name = "Int32"
      .bit_count = 32
    }
  }
}
const type3 = Type::Array {
  .is_const = true
  .dim = 5
  .type = Type::Natural {
    .name = "Nat8"
    .bit_count = 8
  }
}
const type4 = Type::Array {
  .is_const = true
  .dim = 6
  .type = Type::Natural {
    .name = "Nat8"
    .bit_count = 8
  }
}
Ast::Stmts {
  Ast::Struct {
    .value = "Person"
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "first_name"
      }
      .type_node = Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "last_name"
      }
      .type_node = Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "age"
      }
      .type_node = Ast::Type {
        .type = type1
      }
    }
  }
  Ast::Const {
    Ast::Id {
      .value = "p"
    }
    Ast::Type {
      .type = type2
    }
    Ast::StructLiteral {
      .type = type2
      Ast::StructLiteralField {
        Ast::Id {
          .value = "first_name"
        }
        Ast::String {
          .value = "John"
          .type = type3
        }
      }
      Ast::StructLiteralField {
        Ast::Id {
          .value = "last_name"
        }
        Ast::String {
          .value = "Smith"
          .type = type4
        }
      }
      Ast::StructLiteralField {
        Ast::Id {
          .value = "age"
        }
        Ast::Number {
          .value = "35"
          .type = type1
        }
      }
    }
  }
}
```

## Test
(error invalid field)

```cent
Test {
  .has_error = true
}
```

```akela
struct Person
    first_name: [100 const]Nat8
    last_name: [100 const]Nat8
    age: Int32
end

const p: Person = Person
    first_name: "John"
    middle_name: "Freddy"
    last_name: "Smith"
    age: 35
end
```

```cent
Error {
  .message = "Not a valid field for Person: middle_name"
  .line = 9
  .col = 5
}
```

## Test
(error missing field)

```cent
Test {
  .has_error = true
}
```

```akela
struct Person
    first_name: [100 const]Nat8
    last_name: [100 const]Nat8
    age: Int32
end
const p: Person = Person
    first_name: "John"
    age: 35
end
```

```cent
Error {
  .message = "struct field missing: last_name"
  .line = 6
  .col = 5
}
```

## Test
dot (error invalid field)

```cent
Test {
  .has_error = true
}
```

```akela
struct Person
    firstName: [100 const]Nat8
    lastName: [100 const]Nat8
    age: Int32
end
var p: Person
p.abc
```

```cent
Error {
  .message = "identifier not a field of struct: abc"
  .line = 7
  .col = 3
}
``` 

## Test
(error expected identifier)

```cent
Test {
  .has_error = true
}
```

```akela
struct end
```

```cent
Error {
  .message = "expected identifier"
  .line = 1
  .col = 8
}
```

## Test
(error expected end)

```cent
Test {
  .has_error = true
}
```

```akela
struct Person
```

```cent
Error {
  .message = "expected end"
  .line = 2
  .col = 1
}
```

## Test
(error expected end 2)

```cent
Test {
  .has_error = true
}
```

```akela
struct Person x: Int32
```

```cent
Error {
  .message = "expected end"
  .line = 2
  .col = 1
}
```

## Test
(error duplicate)

```cent
Test {
  .has_error = true
}
```

```akela
const Person: Int32 = 1
struct Person end
```

```cent
Error {
  .message = "duplicate variable in scope: Person"
  .line = 2
  .col = 8
}
```

## Test
struct impl

```cent
Test {
}
```

```akela
extern pow(Real64, Real64)->Real64
extern sqrt(Real64)->Real64
struct Point
    x: Real64
    y: Real64
end
struct Line
    p0: Point
    p1: Point
end
impl Line
    fn length(self: Line)->Real64
        sqrt(pow(self.p1.x - self.p0.x, 2) + pow(self.p1.y - self.p0.y, 2))
    end
end
```

```cent
use lib::base::*
const type0 = Type::Function {
  .name = "pow"
  .input = Input {
    TypeParam::Regular {
      .name = ""
      .type = Type::Real {
        .name = "Real64"
        .bit_count = 64
      }
    }
    TypeParam::Regular {
      .name = ""
      .type = Type::Real {
        .name = "Real64"
        .bit_count = 64
      }
    }
  }
  .output = Type::Real {
    .name = "Real64"
    .bit_count = 64
  }
}
const type1 = Type::Real {
  .name = "Real64"
  .bit_count = 64
}
const type2 = Type::Function {
  .name = "sqrt"
  .input = Input {
    TypeParam::Regular {
      .name = ""
      .type = Type::Real {
        .name = "Real64"
        .bit_count = 64
      }
    }
  }
  .output = Type::Real {
    .name = "Real64"
    .bit_count = 64
  }
}
const type3 = Type::Struct {
  .name = "Point"
  TypeField {
    .name = "x"
    .type = Type::Real {
      .name = "Real64"
      .bit_count = 64
    }
  }
  TypeField {
    .name = "y"
    .type = Type::Real {
      .name = "Real64"
      .bit_count = 64
    }
  }
}
const type4 = Type::Function {
  .name = "length"
  .input = Input {
    TypeParam::Regular {
      .name = "self"
      .type = Type::Struct {
        .name = "Line"
        TypeField {
          .name = "p0"
          .type = Type::Struct {
            .name = "Point"
            TypeField {
              .name = "x"
              .type = Type::Real {
                .name = "Real64"
                .bit_count = 64
              }
            }
            TypeField {
              .name = "y"
              .type = Type::Real {
                .name = "Real64"
                .bit_count = 64
              }
            }
          }
        }
        TypeField {
          .name = "p1"
          .type = Type::Struct {
            .name = "Point"
            TypeField {
              .name = "x"
              .type = Type::Real {
                .name = "Real64"
                .bit_count = 64
              }
            }
            TypeField {
              .name = "y"
              .type = Type::Real {
                .name = "Real64"
                .bit_count = 64
              }
            }
          }
        }
      }
    }
  }
  .output = Type::Real {
    .name = "Real64"
    .bit_count = 64
  }
}
const type5 = Type::Struct {
  .name = "Line"
  TypeField {
    .name = "p0"
    .type = Type::Struct {
      .name = "Point"
      TypeField {
        .name = "x"
        .type = Type::Real {
          .name = "Real64"
          .bit_count = 64
        }
      }
      TypeField {
        .name = "y"
        .type = Type::Real {
          .name = "Real64"
          .bit_count = 64
        }
      }
    }
  }
  TypeField {
    .name = "p1"
    .type = Type::Struct {
      .name = "Point"
      TypeField {
        .name = "x"
        .type = Type::Real {
          .name = "Real64"
          .bit_count = 64
        }
      }
      TypeField {
        .name = "y"
        .type = Type::Real {
          .name = "Real64"
          .bit_count = 64
        }
      }
    }
  }
}
const type6 = Type::Integer {
  .name = "Int32"
  .bit_count = 32
}
Ast::Stmts {
  Ast::Extern {
    .proto = Ast::Prototype {
      .id = Ast::Id {
        .value = "pow"
      }
      .dseq = Ast::Dseq {
        Ast::Declaration {
          .id_node = Ast::Id {
          }
          .type_node = Ast::Type {
            .type = type1
          }
        }
        Ast::Declaration {
          .id_node = Ast::Id {
          }
          .type_node = Ast::Type {
            .type = type1
          }
        }
      }
      .ret = Ast::Dret {
        .node = Ast::Type {
          .type = type1
        }
      }
    }
    .type = type0
  }
  Ast::Extern {
    .proto = Ast::Prototype {
      .id = Ast::Id {
        .value = "sqrt"
      }
      .dseq = Ast::Dseq {
        Ast::Declaration {
          .id_node = Ast::Id {
          }
          .type_node = Ast::Type {
            .type = type1
          }
        }
      }
      .ret = Ast::Dret {
        .node = Ast::Type {
          .type = type1
        }
      }
    }
    .type = type2
  }
  Ast::Struct {
    .value = "Point"
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "x"
      }
      .type_node = Ast::Type {
        .type = type1
      }
    }
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "y"
      }
      .type_node = Ast::Type {
        .type = type1
      }
    }
  }
  Ast::Struct {
    .value = "Line"
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "p0"
      }
      .type_node = Ast::Type {
        .type = type3
      }
    }
    Ast::Declaration {
      .id_node = Ast::Id {
        .value = "p1"
      }
      .type_node = Ast::Type {
        .type = type3
      }
    }
  }
  Ast::Impl {
    Ast::Function {
      .proto = Ast::Prototype {
        .id = Ast::Id {
          .value = "length"
        }
        .dseq = Ast::Dseq {
          Ast::Declaration {
            .id_node = Ast::Id {
              .value = "self"
            }
            .type_node = Ast::Type {
              .type = type5
            }
          }
        }
        .ret = Ast::Dret {
          .node = Ast::Type {
            .type = type1
          }
        }
      }
      .body = Ast::Stmts {
        .type = type1
        Ast::Call {
          .func = Ast::Id {
            .value = "sqrt"
            .type = type2
          }
          .args = AstList {
            Ast::Plus {
              .type = type1
              .left = Ast::Call {
                .func = Ast::Id {
                  .value = "pow"
                  .type = type0
                }
                .args = AstList {
                  Ast::Minus {
                    .left = Ast::Dot {
                      .left = Ast::Dot {
                        .left = Ast::Id {
                          .value = "self"
                          .type = type5
                        }
                        .right = Ast::Id {
                          .value = "p1"
                        }
                        .type = type3
                      }
                      .right = Ast::Id {
                        .value = "x"
                      }
                      .type = type1
                    }
                    .right = Ast::Dot {
                      .left = Ast::Dot {
                        .left = Ast::Id {
                          .value = "self"
                          .type = type5
                        }
                        .right = Ast::Id {
                          .value = "p0"
                        }
                        .type = type3
                      }
                      .right = Ast::Id {
                        .value = "x"
                      }
                      .type = type1
                    }
                    .type = type1
                  }
                  Ast::Number {
                    .value = "2"
                    .type = type6
                  }
                }
                .type = type1
              }
              .right = Ast::Call {
                .func = Ast::Id {
                  .value = "pow"
                  .type = type0
                }
                .args = AstList {
                  Ast::Minus {
                    .left = Ast::Dot {
                      .left = Ast::Dot {
                        .left = Ast::Id {
                          .value = "self"
                          .type = type5
                        }
                        .right = Ast::Id {
                          .value = "p1"
                        }
                        .type = type3
                      }
                      .right = Ast::Id {
                        .value = "y"
                      }
                      .type = type1
                    }
                    .right = Ast::Dot {
                      .left = Ast::Dot {
                        .left = Ast::Id {
                          .value = "self"
                          .type = type5
                        }
                        .right = Ast::Id {
                          .value = "p0"
                        }
                        .type = type3
                      }
                      .right = Ast::Id {
                        .value = "y"
                      }
                      .type = type1
                    }
                    .type = type1
                  }
                  Ast::Number {
                    .value = "2"
                    .type = type6
                  }
                }
                .type = type1
              }
            }
          }
          .type = type1
        }
      }
      .type = type4
    }
  }
}
```
