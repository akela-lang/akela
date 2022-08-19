# Todo
* set etype
  * factor
    * literal [x]
    * id [x]
    * sign [x]
    * not [x]
    * parenthesis []
    * array literal []
    * anonymous function []
    * function call []
    * factor []
  * expr
    * add []
    * array_subscript []
    * assignment []
    * boolean []
    * comparison []
    * expr []
    * mult []
    * var [x]
    * parenthesis []
  * stmts
    * function []
    * else
    * elseif
    * for iteration []
    * for range []
    * function []
    * separator []
    * stmt []
    * stmts []
    * while []
* change type_info to type_definition
* change type_node to value_type
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