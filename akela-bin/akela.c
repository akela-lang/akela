#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "akela/ast.h"
#include "zinc/os_win.h"
#include "zinc/os_unix.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"
#include "zinc/input_unicode_file.h"
#include "akela-llvm/akela_llvm.h"

int main(int argc, char** argv)
{
    char* filename;
    struct comp_unit cu;

    if (argc != 2) {
        fprintf(stderr, "Usage: akela <filename>\n");
        return 1;
    }

#if defined(_WIN32) || defined(WIN32)
    set_console_utf8();
#endif

    /* resource fp */
    filename = argv[1];
    FILE* fp;
    int err = fopen_s(&fp, filename, "r");
    if (err || fp == NULL) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 1;
    }

    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    comp_unit_init(&cu);
    comp_unit_compile(&cu, input, input->input_vtable);

    if (!cu.valid) {
        struct error* e = cu.el.head;
        while (e) {
            buffer_finish(&e->message);
            fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
        return 1;
    }

    struct buffer bf;
    buffer_init(&bf);

    Code_gen_llvm* cg = NULL;
    Code_gen_llvm_create(&cg, &cu.el, &cu.extern_list);
    Code_gen_result result;
    Code_gen_result_init(&result);
    Code_gen_jit(cg, &Code_gen_llvm_vtable, cu.root, &result);

    if (cu.el.head) {
        struct error* e = cu.el.head;
        while (e) {
            buffer_finish(&e->message);
            fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
        return 1;
    }

    if (result.value.size > 0) {
        printf("%s\n", result.value.buf);
    }
    Code_gen_result_destroy(&result);

    comp_unit_destroy(&cu);

    free(input);
    fclose(fp);
    free(cg);

    return 0;
}
