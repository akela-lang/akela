# Todo
* return ast_node in parse_expr functions
  * dot_nt [x]
  * cseq [x]
  * function_call [x]
  * subscript [x]
  * power [x]
  * mult [x]
  * add [x]
  * comparison [x]
  * boolean [x]
  * simple_expr []
  * eseq []
  * assignment []
  * expr []
* return ast_node in parse_stmts functions
* return ast_node in parse_types functions
* get location of ast_node once
* add support for file io to event loop test
* get larger strings from stdin in event loop
* test get char in event loop
* parse_function and parse_function_part and parse_function_anonymous
* support only one lookahead token - there is an assert so just the data structure needs changing
* store IR value in ast node
* store IR variable references in symbol table
* keep LLVM references out of parsing code and symbol table - use void* to reference them
* language server
* 2 libraries - (1) parsing and type checking and (2) full compiler
* run code gen on first pass
* get text interface for lexer
* get char interface for lexer
* allow only English and Greek letters for identifiers
  * A-Z
  * a-z
  * ΑαΒβΓγΔδΕεΖζΗηΘθΙιΚκΛλΜμΝνΞξΟοΠπΡρΣσςΤτΥυΦφΧχΨψΩω
