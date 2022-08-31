# Todo
* output location in parse functions
  * parse_types [x]
    * tseq [x]
    * type [x]
    * declaration [x]
    * dseq [x]
  * parse_factor []
    * parenthesis [x]
    * aseq [x]
    * array_literal [x]
    * sign [x]
    * id_nt [x]
    * literal_nt [x]
    * not_nt [x]
    * cseq [x]
    * function_call [x]
    * anonymous_function []
    * var []
    * factor []
  * parse_expr []
  * parse_strmts []
* change update_location to location_update
* change update_location_token to location_update_token
* change default_location to location_default
* error location specific to call parameter / argument
* power operator
* replace test_use with ast_node
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