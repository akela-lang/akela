#ifndef AKELA_UPDATE_SYMBOL_H
#define AKELA_UPDATE_SYMBOL_H

#include "parse_tools.h"

void Ake_UpdateSymbolExtern(Ake_parse_state* ps, Ake_Ast* n);
void Ake_UpdateSymbolFor(Ake_parse_state* ps, Ake_Ast* n);
void Ake_UpdateSymbolStruct(Ake_parse_state* ps, Ake_Ast* n);
void Ake_UpdateSymbolLet(Ake_parse_state* ps, Ake_Ast* n);
void Ake_UpdateSymbolPrototype(Ake_parse_state* ps, Ake_Ast* n);
void Ake_UpdateSymbolFunction(Ake_parse_state* ps, Ake_Ast* n);

#endif
