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
  * less than [x]
  * less than or equal [x]
  * greater than [x]
  * greater than or equal [x]
  * not [x]
  * and [x]
  * or [x]
  * while [x]
  * for range [x]
  * for iteration [x]
  * declaration [x]
  * array literal [x]
  * array subscript [x]
  * boolean [x]
  * parenthesis [x]
  * type [x]
  * power [x]
  * dot [x]
  * struct [x]
  * return [x]
  * prototype [x]
  * extern [x]
  * struct literal [x]
  * struct literal field [x]
  * ellipsis []
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
  * var/const []
  * dret []
  * declaration []
* remove id node from dot
  * dot []
  * struct field []
* test case for const array changed
* convert Cent_value to Ake_Ast
* match stack output with Ake_AstMatch
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
