# Todo
* set location in ast_node; always return ast_node
  * parse_type
    * tseq [x]
    * type [x]
    * declaration []
    * dseq []
  * parse_factor []
    * parenthesis []
    * aseq []
    * array_literal []
    * sign []
    * id_nt []
    * literal_nt []
    * not_nt []
    * cseq []
    * function_call []
    * anonymous_function []
    * var []
  * parse_expr
    * array_subscript []
    * mult []
    * add []
    * comparison []
    * boolean []
    * assignment []
    * expr []
  * parse_stmts
    * else_nt []
    * elseif_nt []
    * function_finish []
    * function_start []
    * function []
    * for_iteration []
    * for_range []
    * for_nt []
    * while_nt []
    * if_nt []
    * stmt []
    * separator []
    * stmts []
* annotate test case name where each error is generated
* create string type that is a variable sized buffer and inserts null char at end
* derive type from list in for x in list
* verify type for operations
* check that subscripted variable is an array type
* allow comma in var
* add let expression
* structs; make type_def not a dag; add composite for structs
* add composite_name to type_use for structs
* add return statements
* type check return statements
* allow struct at global level only
* use lookahead_char to lex compound operators
* link to LLVM
* generate LLVM IR
