#ifndef CENTIPEDE_PARSE_VALIDATION_H
#define CENTIPEDE_PARSE_VALIDATION_H

#include "parse_data.h"

void Cent_check_namespace(Cent_parse_result* pr, Cent_ast* n);
void Cent_check_variables(Cent_parse_result* pr, Cent_ast* n);
void Cent_check_follow_on(Cent_parse_result* pr, Cent_ast* n);

#endif