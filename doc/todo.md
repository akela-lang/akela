# Akela
* syntax tree with tagged union
  * none [x]
  * id [x]
  * sign [x]
  * number [x]
  * string [x]
  * assign [x]
  * plus [x]
  * minus [x]
  * mult [x]
  * divide [x]
  * stmts [x]
  * function [x]
  * dseq [x]
  * dret [x]
  * call [x]
  * if [x]
  * conditional branch [x]
  * default branch [x]
  * equality [x]
  * not equal [x]
  * less than []
  * less than or equal []
  * greater than []
  * greater than or equal []
  * not []
  * and []
  * or []
  * while []
  * for range []
  * for iteration []
  * declaration []
  * array literal []
  * array subscript []
  * boolean []
  * parenthesis []
  * type []
  * power []
  * type pool []
  * dot []
  * struct []
  * return []
  * eseq []
  * prototype []
  * extern []
  * struct literal []
  * struct literal field []
  * ellipsis []
  * impl []
  * self []
  * const []
  * var []
* add checking of symbols to UpdateSymbol functions
  * stmts
    * struct []
  * expr
    * dot []
  * factor
    * literal []
    * struct literal []
* remove Ast::Type nodes
  * let []
  * dret []
  * declaration []
* modules
* print function
* insert error context lines related to current check; used for line and col numbers
* wrap pointers
* run IR generation in same pass
* order independence of global variables
* cast numerical values
* disallow array copying like C

# Akela LLVM

# Akela Parse Test

# Akela Run Test
* field variants
  * pointers

# Centipede
* characters

# Cobble

# Coverage

# Dataframe

# JSON

# Lava

# Zinc
* convert os specific to generic functions
* support suite-level mute and solo

# Test

# Example
* test suite
* test
  * akela (code)
  * text (output)
* check examples
* output html
* array subscript assert exit