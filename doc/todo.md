# Todo
* type check
  * factor
    * literal [x]
    * id [x]
    * sign [x]
    * not [x]
    * parenthesis [x]
    * array literal [x]
    * factor [x]
    * var [x]
    * anonymous function [x]
    * function [x]
      * check return type [x]
      * test assignment [x]
    * function call []
  * expr
    * add [x]
    * mult [x]
    * comparison [x]
    * boolean [x]
    * array_subscript [x]
    * assignment [x]
      * check compatible types [x]
      * check var [x]
      * check lvalue [x]
  * stmts
    * for iteration [x]
    * for range [x]
    * stmt [x]
    * stmts [x]
    * function [x]
      * check return type [x]
    * if []
    * elseif
    * else
    * function []
    * separator []
    * while []
* annotate test case name where each error is generated
* set location in dag on match and non-terminal
* add type_set and type to dag_node
* create string type that is a variable sized buffer and inserts null char at end
* derive type from list in for x in list
* verify type for operations
* check that subscripted variable is an array type
* allow comma in var
* add let expression
* structs
* add return statements
* allow struct at global level only
* use lookahead_char to lex compound operators
* suppress new environment in stmts with parameter if new environment already created
* add derived bool to token
* link to LLVM
* generate LLVM IR