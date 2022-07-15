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

int main(int argc, char** argv)
{
    enum result r;
    char* filename;
    struct dag_node* root;

    if (argc != 2) {
        fprintf(stderr, "Usage: alba <filename>\n");
        return 1;
    }

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
    struct word_table wt;
    struct compile_error_list el;
    struct parse_state ps;

    r = conv_open(&conv);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }
    word_table_init(&wt, WORD_TABLE_SIZE);
    lookahead_char_init(&lc, file_getchar, fp, conv);
    compile_error_list_init(&el);
    scan_state_init(&sns, &lc, &wt, &el);
    parse_state_init(&ps, &sns, &el);

    r = parse(&ps, &root);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }

    fclose(fp);

    char* names[dag_type_count];
    dag_set_names(names);
    dag_print(root, names);

    conv_close(conv);
    printf("end\n");

    return 0;
}
