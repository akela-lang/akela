# Todo
* return ast_node in parse_stmts functions
  * parse_var_rseq [x]
  * parse_var_lseq [x]
  * parse_var [x]
  * return_nt [x]
  * struct_nt [x]
  * module_nt [x]
  * else_nt [x]
  * elseif_nt [x]
  * if_nt [x]
  * parse_function_finish [x]
  * parse_function_start [x]
  * function [x]
  * for_iteration [x]
  * for_range [x]
  * for_nt [x]
  * while_nt [x]
  * stmt []
  * separator []
  * stmts []
* return ast_node in parse_types functions
* return ast_node in parse function
* return valid in check_return_type
* parse_compound
* parse_assign_or_expr
* combine function and function_anonymous
* error test cases for eseq
* don't bubble up lexer errors to the parser
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
* remove ICU dependency
* allow only English and Greek letters for identifiers
  * A-Z
  * a-z
  * ΑαΒβΓγΔδΕεΖζΗηΘθΙιΚκΛλΜμΝνΞξΟοΠπΡρΣσςΤτΥυΦφΧχΨψΩω
