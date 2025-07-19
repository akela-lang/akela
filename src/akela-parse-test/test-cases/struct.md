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
