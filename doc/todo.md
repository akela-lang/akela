# Todo
* return ast_node in factor parsing functions
  * parenthesis [x]
  * aseq [x]
  * array_literal [x]
  * sign []
  * id_nt []
  * literal_nt []
  * not_nt []
  * anonymous_function []
  * factor []
* return ast node from parser functions instead of boolean
* create error type for ast nodes
* store source location in ast node
* store IR value in ast node
* store IR variable references in symbol table
* keep LLVM references out of parsing code and symbol table - use void* to reference them
* language server
* 2 libraries - (1) parsing and type checking and (2) full compiler
* run code gen on first pass
* support only one lookahead token - there is an assert so just the data structure needs changing