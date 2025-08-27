#ifndef AKELA_UPDATE_SYMBOL_H
#define AKELA_UPDATE_SYMBOL_H

#include "parse_tools.h"

#ifdef __cplusplus
extern "C" {
#endif
    void Ake_UpdateSymbolExtern(Ake_symbol_table* st, Ake_Ast* n);
    void Ake_UpdateSymbolFor(Ake_symbol_table* st, Ake_Ast* n);
    void Ake_UpdateSymbolStruct(Ake_symbol_table* st, Ake_Ast* n);
    void Ake_UpdateSymbolLet(Ake_symbol_table* st, Ake_Ast* n);
    void Ake_UpdateSymbolPrototype(Ake_symbol_table* st, Ake_Ast* n);
    void Ake_UpdateSymbolFunction(Ake_symbol_table* st, Ake_Ast* n);
#ifdef __cplusplus
}
#endif


#endif
