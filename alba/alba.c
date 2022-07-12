#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scan.h"
#include "buffer.h"
#include "parse.h"
#include "dag.h"
#include "allocator.h"
#include "input.h"
#include "uconv.h"

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

    struct allocator al;
    UConverter* conv;
    struct scan_state sns;
    struct lookahead_char lc;
    struct input_state is;
    struct word_table wt;
    struct parse_state ps;

    allocator_init(&al);
    r = conv_open(&conv);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }
    input_state_init(file_getchar, fp, conv, &is);
    word_table_init(&al, &wt, WORD_TABLE_SIZE);
    lookahead_char_init(&lc, file_getchar, fp, conv);
    scan_state_init(&sns, &lc, &is, &wt);
    parse_state_init(&ps, &sns);

    r = parse(&al, &ps, &root);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }

    fclose(fp);

    char* names[dag_type_count];
    dag_set_names(names);
    dag_print(&al, root, names);

    allocator_destroy(&al);
    conv_close(conv);
    printf("end\n");

    return 0;
}
