#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scan.h"
#include "ustring.h"
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
    struct input_state is;
    struct token_state ts;

    allocator_init(&al);
    r = conv_open(&conv);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }
    input_state_init(file_getchar, fp, conv, &is);
    token_state_init(&is, &ts);

    r = parse(&al, &ts, &root);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }

    allocator_destroy(&al);
    conv_close(conv);
    fclose(fp);
    printf("end\n");
    return 0;
}
