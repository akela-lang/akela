#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scan.h"
#include "zinc/buffer.h"
#include "parse.h"
#include "dag.h"
#include "input.h"
#include "uconv.h"
#include "source.h"
#include "os_win.h"
#include "source.h"
#include "symbol_table.h"

int main(int argc, char** argv)
{
    enum result r;
    char* filename;
    struct dag_node* root;

    if (argc != 2) {
        fprintf(stderr, "Usage: alba <filename>\n");
        return 1;
    }

#if defined(_WIN32) || defined(WIN32)
    set_console_utf8();
#endif

    /* resource fp */
    filename = argv[1];
    FILE* fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 1;
    }

    UConverter* conv;
    struct scan_state sns;
    struct lookahead_char lc;
    struct compile_error_list el;
    struct parse_state ps;
    struct symbol_table st;

    /* resource conv */
    r = conv_open(&conv);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }

    /* resource input fp */
    lookahead_char_init(&lc, (input_getchar)file_getchar, fp, conv);

    compile_error_list_init(&el);
    symbol_table_init(&st);

    scan_state_init(&sns, &lc, &el, &st);
    parse_state_init(&ps, &sns, &el, &st);

    /* allocate ps{} root root{} */
    r = parse(&ps, &root);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }

    /* resource destroy fp */
    fclose(fp);

    char* names[dag_type_count];
    dag_set_names(names);
    dag_print(root, names);

    /* resource destroy conv */
    conv_close(conv);

    /* destroy ps{lookahead} */
    token_list_destroy(&ps.lookahead);

    /* destroy ps{el} sns{el} */
    compile_error_list_destroy(&el);

    symbol_table_destroy(&st);

    printf("end\n");

    return 0;
}
