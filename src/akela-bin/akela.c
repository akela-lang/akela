#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "akela/ast.h"
#include "zinc/os_win.h"
#include "zinc/os_unix.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"
#include "zinc/input_unicode_file.h"
#include "akela-llvm/cg.h"
#include "zinc/os.h"

#if IS_WIN
#include <windows.h>
#endif

int main(int argc, char** argv)
{
    char* filename;
    struct Ake_comp_unit cu;

    if (argc != 2) {
        fprintf(stderr, "Usage: akela <filename>\n");
        return 1;
    }

#if IS_WIN
    SetConsoleOutputCP(CP_UTF8);
#endif

    /* resource fp */
    filename = argv[1];
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 1;
    }

    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    Ake_comp_unit_init(&cu);
    Ake_comp_unit_compile(&cu, input, input->vtable);

    if (!cu.valid) {
        struct Zinc_error* e = cu.el.head;
        while (e) {
            Zinc_string_finish(&e->message);
            fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
        return 1;
    }

    Akela_llvm_cg* cg = NULL;
    Akela_llvm_cg_create(&cg, &cu.el, &cu.extern_list);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);
    Ake_code_gen_jit(cg, &Akela_llvm_vtable, cu.root, &result);

    if (cu.el.head) {
        struct Zinc_error* e = cu.el.head;
        while (e) {
            Zinc_string_finish(&e->message);
            fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
        return 1;
    }

    if (result.value.size > 0) {
        printf("%s\n", result.value.buf);
    }
    Ake_code_gen_result_destroy(&result);

    Ake_comp_unit_destroy(&cu);

    free(input);
    fclose(fp);
    Akela_llvm_cg_destroy(cg);

    return 0;
}
