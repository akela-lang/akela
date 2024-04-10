# Todo
* use combinators for scanner
* change scan to lex
* create input_unicode interface for input
* remove old input interfaces
* remove ICU dependency
* allow only English and Greek letters for identifiers
  * A-Z
  * a-z
  * ΑαΒβΓγΔδΕεΖζΗηΘθΙιΚκΛλΜμΝνΞξΟοΠπΡρΣσςΤτΥυΦφΧχΨψΩω
* store IR value in ast node
* store IR variable references in symbol table
* keep LLVM references out of parsing code and symbol table - use void* to reference them
* language server
* 2 libraries - (1) parsing and type checking and (2) full compiler
* run code gen on first pass
* get text interface for lexer
* get char interface for lexer
