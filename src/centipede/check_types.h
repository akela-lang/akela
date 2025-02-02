#ifndef CENTIPEDE_CHECK_TYPES_H
#define CENTIPEDE_CHECK_TYPES_H

#include "parse_data.h"

void Cent_check_types(Cent_parse_result* pr);
void Cent_check_namespace(Cent_parse_result* pr, Cent_ast* n);
void Cent_check_variables(Cent_parse_result* pr, Cent_ast* n);

#endif