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
    * anonymous function []
    * function call []
  * expr
    * add [x]
    * mult [x]
    * array_subscript []
    * assignment []
    * boolean []
    * comparison []
    * expr []
    * var [x]
    * parenthesis []
  * stmts
    * if []
    * elseif
    * else
    * function []
    * separator []
    * stmt []
    * stmts []
    * while []
    * for iteration []
    * for range []
    * function []
* set location in dag on match and non-terminal
* add type_set and type to dag_node
* derive type from list in for x in list
* verify type for operations
* check that subscripted variable is an array type
* allow comma in var
* add let expression
* structs
* allow struct at global level only
* use lookahead_char to lex compound operators
* suppress new environment in stmts with parameter if new environment already created
* add derived bool to token
* link to LLVM
* generate LLVM IR