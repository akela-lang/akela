#include "parse_data.h"

void Ake_parse_result_init(Ake_parse_result* pr, Ake_Ast* root, Zinc_error_list* errors)
{
    pr->root = root;
    pr->errors = errors;
}

void Ake_parse_result_destroy(Ake_parse_result* pr)
{
    Ake_ast_destroy(pr->root);
    Zinc_error_list_destroy(pr->errors);
}