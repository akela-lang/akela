# Todo
* return loc from parse_factor functions
  * parse_parenthesis [x]
  * parse_aseq [x]
  * parse_array_literal [x]
  * parse_sign []
  * parse_id []
  * parse_literal []
  * parse_not []
  * parse_anonymous_function []
  * factor []
* return ast_node in parse_expr functions
  * dot_nt []
  * cseq []
  * function_call []
  * subscript []
  * power []
  * mult []
  * add []
  * comparison []
  * boolean []
  * simple_expr []
  * eseq []
  * assignment []
  * expr []
* return ast_node in parse_stmts functions
* return ast_node in parse_types functions
* get location of ast_node once
* parse_function and parse_function_part and parse_function_anonymous
* store IR value in ast node
* store IR variable references in symbol table
* keep LLVM references out of parsing code and symbol table - use void* to reference them
* language server
* 2 libraries - (1) parsing and type checking and (2) full compiler
* run code gen on first pass
* support only one lookahead token - there is an assert so just the data structure needs changing