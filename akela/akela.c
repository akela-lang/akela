#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "source.h"
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
            fprintf(stderr, "%s\n", e->message);
            e = e->next;
        }
        return 1;
    }

    char* names[ast_type_count];
    enum result r = ast_set_names(names);
    if (r == result_error) {
        printf("%s\n", get_error_message());
    }
    ast_node_print(cu.root, names, false);

    struct buffer bf;
    buffer_init(&bf);
    CodeGenLLVM* cg = NULL;
    CodeGenLLVMCreate(&cg, &cu.st);
    CodeGenJIT(cg, &CodeGenLLVMVTable, cu.root, &bf);
    buffer_finish(&bf);
    printf("\n%s\n", bf.buf);
    buffer_destroy(&bf);

    comp_unit_destroy(&cu);

    free(input);
    fclose(fp);
    free(cg);

    return 0;
}
