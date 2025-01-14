#include "parse.h"
#include "build.h"
#include "module.h"
#include "module_data.h"
#include <assert.h>
#include "ast.h"
#include "parse_tools.h"
#include "comp_table.h"

Cent_environment* Cent_module_dest_env = NULL;

void Cent_module_copy_symbol(struct Zinc_string* name, Cent_symbol* sym)
{
    if (Cent_symbol_should_copy(sym)) {
        Cent_symbol* new_sym = Cent_symbol_clone_shallow(sym);
        Cent_environment_add_symbol(Cent_module_dest_env, name, new_sym);
    }
}

void Cent_parse_import_module(Cent_parse_data* pd, Cent_ast* n)
{
    struct Zinc_string path;
    Zinc_string_init(&path);

    /* find module */
    bool is_glob = false;
    Cent_ast* p = n->head;
    while (p) {
        if (p->type == Cent_ast_type_glob) {
            is_glob = true;
            break;
        }
        if (path.size > 0) {
            Zinc_string_add_char(&path, '/');
        }
        Zinc_string_copy(&p->text, &path);
        p = p->next;
    }

    Zinc_string_add_str(&path, ".aken");
    Cent_comp_unit* cu = Cent_comp_table_find_unit(pd->ct, &path);
    if (!cu) {
        error_list_set(pd->errors, &n->loc, "could not find module: %b", &path);
        n->has_error = true;
        return;
    }
    assert(cu->input);
    assert(cu->input_vtable);

    Cent_comp_table_add(pd->ct, &path, cu);

    String_slice file_name;
    file_name.p = path.buf;
    file_name.size = path.size;

    Cent_comp_unit_parse(cu);
    if (&cu->errors.head) {
        struct error* e = cu->errors.head;
        while (e) {
            error_list_set(pd->errors, &e->loc, "%b: %b", &path, &e->message);
            e = e->next;
        }
    }

    Zinc_string_destroy(&path);

    Cent_value* value = NULL;
    if (!cu->errors.head) {
        Cent_comp_unit_build(cu);
    }

    if (is_glob) {
        /* copy symbols */
        Cent_environment* src_env = cu->pr.root->env;
        Cent_module_dest_env = pd->top;
        hash_table_map_name(&src_env->symbols, (hash_table_func_name)Cent_module_copy_symbol);
    } else {
        p = n->head;
        struct Zinc_string base_name;
        Zinc_string_init(&base_name);
        Zinc_string_copy(&p->text, &base_name);
        Cent_symbol* sym = Cent_environment_get(pd->top, &base_name);
        if (sym) {
            error_list_set(pd->errors, &p->loc, "module identifier collides with existing identifier: %b", &base_name);
            p->has_error = true;
            Zinc_string_destroy(&base_name);
        } else {
            /* create module and symbol */
            Cent_module* mod = NULL;
            Cent_module_create(&mod);
            Cent_symbol_create(&sym);
            Cent_symbol_set_type(sym, Cent_symbol_type_module);
            sym->data.module = mod;
            Cent_environment_add_symbol(pd->top, &base_name, sym);
            Zinc_string_destroy(&base_name);

            /* create submodules */
            Cent_module* last_mod = mod;
            p = p->next;
            while (p) {
                Cent_module* new_mod = NULL;
                Cent_module_create(&new_mod);
                Cent_module_add(last_mod, &p->text, new_mod);
                last_mod = new_mod;
                p = p->next;
            }

            /* link environment to last submodule */
            last_mod->pr = &cu->pr;
            last_mod->value = value;
            assert(cu->pr.root);
            assert(cu->pr.root->type == Cent_ast_type_stmts);
            last_mod->env = cu->pr.root->env;
        }
    }
}