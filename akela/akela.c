#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/os_win.h"
#include "zinc/os_unix.h"
#include "comp_unit.h"
#include "code_gen.h"
#include "zinc/input_unicode_file.h"
#include "code_gen_llvm.h"

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
            fprintf(stderr, "%s\n", e->message.buf);
            e = e->next;
        }
        return 1;
    }

    ast_node_print(cu.root, false);

    struct buffer bf;
    buffer_init(&bf);

    CodeGenLLVM2* cg = NULL;
    CodeGenLLVMCreate(&cg, &cu.el);
    CodeGenResult result;
    CodeGenResultInit(&result);
    CodeGenJIT(cg, &CodeGenLLVM2VTable, cu.root, &result);

    printf("\n%s\n", result.value.buf);
    CodeGenResultDestroy(&result);

    comp_unit_destroy(&cu);

    free(input);
    fclose(fp);
    free(cg);

    return 0;
}
