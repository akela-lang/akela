#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "input.h"
#include "source.h"
#include "os_win.h"
#include "os_linux.h"
#include "comp_unit.h"
#include "code_gen.h"

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

    comp_unit_init(&cu);
    comp_unit_compile(&cu, (input_getchar)file_getchar, fp);

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

    code_gen(&cu);

    comp_unit_destroy(&cu);

    /* resource destroy fp */
    fclose(fp);
    printf("end\n");

    return 0;
}
