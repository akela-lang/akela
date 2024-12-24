#include "parse_data.h"

Cent_parse_result parse(Cent_parse_data* pd)
{
    Cent_parse_result result;
    Cent_parse_result_init(&result);
    result.errors = pd->errors;

    return result;
}

void parse_element(Cent_parse_data* pd, Cent_environment* env)
{
    Cent_element* element = NULL;
    Cent_element_create(&element);

    Cent_token* e = NULL;
    // Cent_match(pd, Cent_token_element, "expected element", e, value);

    Cent_symbol* sym = NULL;
    Cent_symbol_create(&sym);
    sym->type = Cent_symbol_type_enumerate;
    sym->element = element;
    // Cent_environment_add_symbol(env, name, sym);
}