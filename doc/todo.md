# Todo
* add support for file io to event loop test
* get larger strings from stdin in event loop
* test get char in event loop
* return ast_node in parse_expr functions
  * dot_nt [x]
  * cseq [x]
  * function_call [x]
  * subscript [x]
  * power [x]
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
* support only one lookahead token - there is an assert so just the data structure needs changing
* store IR value in ast node
* store IR variable references in symbol table
* keep LLVM references out of parsing code and symbol table - use void* to reference them
* language server
* 2 libraries - (1) parsing and type checking and (2) full compiler
* run code gen on first pass
* request text interface - requests some text; could be stdin repl, a buffer, or a file; until done
* request char interface - gets utf8 and utf32 character; until done
