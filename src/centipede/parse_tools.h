#ifndef CENTIPEDE_PARSE_TOOLS_H
#define CENTIPEDE_PARSE_TOOLS_H

#include <stdbool.h>
#include "parse_data.h"
#include "ast.h"
#include "lex.h"

typedef enum Cent_task_type {
    Cent_task_type_none,
    Cent_task_type_update_element_type,
    Cent_task_type_update_enum_type,
    Cent_task_type_transform_variant_set,
    Cent_task_type_check_namespace,
    Cent_task_type_check_variable,
    Cent_task_type_update_namespace,
    Cent_task_type_check_follow_on,
} Cent_task_type;

void Cent_lookahead(Cent_parse_data* pd);
bool Cent_match(Cent_parse_data* pd, Cent_token_type type, char* message, Cent_token** t, Cent_ast* n);
void Cent_ignore_newlines(Cent_parse_data* pd);
bool Cent_parse_sep(Cent_parse_data* pd, Cent_ast* n);
Cent_environment* Cent_get_environment(Cent_ast* n);
Cent_namespace_result Cent_namespace_lookup(Cent_ast* n);
void Cent_parse_process_tasks(Cent_parse_data* pd, Cent_parse_result* pr);

#endif
