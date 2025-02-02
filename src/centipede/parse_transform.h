#ifndef CENTIPEDE_PARSE_TRANSFORM_H
#define CENTIPEDE_PARSE_TRANSFORM_H

#include "parse_data.h"

void Cent_update_element_type(Cent_parse_result* pr, Cent_ast* n);
void Cent_update_enum(Cent_parse_result* pr, Cent_ast* n);
void Cent_update_namespace(Cent_parse_result* pr, Cent_ast* n);

#endif