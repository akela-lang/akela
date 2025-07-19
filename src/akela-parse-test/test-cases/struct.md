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
      Ast::Id {
        .value = "firstName"
      }
      Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      Ast::Id {
        .value = "lastName"
      }
      Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      Ast::Id {
        .value = "age"
      }
      Ast::Type {
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
    .type = type0
    Ast::Dot {
      .type = type0
      Ast::Id {
        .value = "p"
        .type = type2
      }
      Ast::Id {
        .value = "firstName"
      }
    }
    Ast::String {
      .value = "John"
      .type = type3
    }
  }
  Ast::Dot {
    .type = type0
    Ast::Id {
      .value = "p"
      .type = type2
    }
    Ast::Id {
      .value = "firstName"
    }
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
      Ast::Id {
        .value = "first_name"
      }
      Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      Ast::Id {
        .value = "last_name"
      }
      Ast::Type {
        .type = type0
      }
    }
    Ast::Declaration {
      Ast::Id {
        .value = "age"
      }
      Ast::Type {
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
  .line = 319
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
  .line = 348
  .col = 5
}
```