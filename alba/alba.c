#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scan.h"
#include "ustring.h"
#include "parse.h"
#include "dag.h"
#include "allocator.h"

int main(int argc, char** argv)
{
    enum result r;
    int last_line = 0;
    struct string line;
    char* a;
    char* filename;
    struct token_list tl;
    char* token_name[token_count];
    struct dag_node* root;

    if (argc != 2) {
        fprintf(stderr, "Usage: alba <filename>\n");
        return 1;
    }

    filename = argv[1];
    FILE* f;
    f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 1;
    }

    string_init(&line);

    token_list_init(&tl);

    r = token_name_init(token_name);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        return 1;
    }

    while(!last_line) {
        struct allocator al;
        allocator_init(&al);

        r = next_line(&al, f, &line, 1, &last_line);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }
        r = string2array(&al, &line, &a);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }
        printf("line: %s\n", a);
        r = scan(&al, &line, &tl);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }

        r = token_list_print(&al, &tl, token_name);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }

        r = parse(&al, &tl, &root);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }

        string_clear(&line);
        token_list_reset(&tl);
        allocator_destroy(&al);
    }

    printf("end\n");
    return 0;
}
