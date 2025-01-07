#include "parse.h"
#include "build.h"
#include "module.h"
#include "module_data.h"
#include <assert.h>
#include "ast.h"

void Cent_parse_import_module(Cent_parse_data* pd, Cent_ast* n)
{
    struct buffer path;
    buffer_init(&path);

    bool is_glob = false;
    Cent_ast* p = n->head;
    while (p) {
        if (p->type == Cent_ast_type_glob) {
            is_glob = true;
            break;
        }
        if (path.size > 0) {
            buffer_add_char(&path, '/');
        }
        buffer_copy(&p->text, &path);
        p = p->next;
    }

    buffer_copy_str(&path, ".aken");
    Cent_comp_unit* cu = Cent_module_find_interface(pd->module_obj, pd->module_vtable, &path);
    if (!cu) {
        error_list_set(pd->errors, &n->loc, "could not find module: %b", &path);
        n->has_error = true;
        return;
    }

    Cent_comp_table_add(pd->comp_table, &path, cu);

    Cent_parse_data* pd2 = NULL;
    Cent_parse_data_create(&pd2);
    pd2->comp_table = pd->comp_table;
    Cent_lex_data* ld = NULL;
    Cent_lex_data_create(&ld, cu->errors, cu->input, cu->input_vtable);
    pd2->ld = ld;
    pd2->errors = cu->errors;
    pd2->file_name.p = path.buf;
    pd2->file_name.size = path.size;
    pd2->module_obj = pd->module_obj;
    pd2->module_vtable = pd->module_vtable;

    Cent_parse_result pr = Cent_parse(pd2);
    Cent_value* value = Cent_build(&pr);

    p = n->head;
    Cent_module* mod = NULL;
    Cent_module_create(&mod);
    Cent_symbol* sym = NULL;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_module);
    sym->data.module = mod;
    struct buffer base_name;
    buffer_init(&base_name);
    buffer_copy(&p->text, &base_name);
    Cent_environment_add_symbol(pd->top, &base_name, sym);
    buffer_destroy(&base_name);

    Cent_module* last_mod = mod;
    p = p->next;
    while (p) {
        Cent_module* new_mod = NULL;
        Cent_module_create(&new_mod);
        Cent_module_add(last_mod, &p->text, new_mod);
        last_mod = new_mod;
        p = p->next;
    }

    last_mod->pr = &pr;
    last_mod->value = value;
    assert(pr.root);
    assert(pr.root->type == Cent_ast_type_stmts);
    last_mod->env = pr.root->env;
}