#include "comp_unit.h"

void Cent_check_parent_node(Cent_comp_unit* cu, Cent_ast* n);
void Cent_check_parent_environment(Cent_comp_unit* cu, Cent_environment* env);
void Cent_check_parent_symbol(Cent_symbol* sym);

void Cent_check_parent(Cent_comp_unit* cu)
{
    Cent_check_parent_node(cu, cu->pr.root);

    if (cu->pr.root && cu->pr.root->type == Cent_ast_type_stmts) {
        Cent_ast* p = cu->pr.root->head;
        while (p) {
            if (p->type == Cent_ast_type_expr_object) {
                if (p != cu->pr.root->tail) {
                    Zinc_error_list_set(&cu->errors, &p->loc, "value is not used");
                    p->has_error = true;
                }
            }
            p = p->next;
        }
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_check_parent_node(Cent_comp_unit* cu, Cent_ast* n)
{
    Cent_ast* p = n->head;
    while (p) {
        Cent_check_parent_node(cu, p);
        p = p->next;
    }

    if (n->env) {
        Cent_check_parent_environment(cu, n->env);
    }
}

Cent_comp_unit* Cent_check_parent_cu = NULL;
void Cent_check_parent_environment(Cent_comp_unit* cu, Cent_environment* env)
{
    Cent_check_parent_cu = cu;
    hash_table_map(&env->symbols, (hash_table_func)Cent_check_parent_symbol);
}

void Cent_check_parent_symbol(Cent_symbol* sym)
{
    Cent_comp_unit* cu = Cent_check_parent_cu;
    if (sym->type == Cent_symbol_type_variable && !sym->is_copy) {
        Cent_value* value = sym->data.variable.value;
        if (!value->parent && value != cu->value) {
            Cent_ast* n = value->n;
            Zinc_error_list_set(&cu->errors, &n->loc, "value is not used");
            n->has_error = true;
            /* test case: test_check_value_types_not_attached */
        }
    }
}