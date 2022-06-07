#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scan.h"
#include "ustring.h"
#include "defer.h"
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
    struct defer_node* stack = NULL;
    struct token_list tl;
    char* token_name[token_count];
    struct dag_node* root;

    if (argc != 2) {
        fprintf(stderr, "Usage: alba <filename>\n");
        cleanup(stack);
        return 1;
    }

    filename = argv[1];
    FILE* f;
    f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        cleanup(stack);
        return 1;
    }
    r = defer(fclose, f, &stack);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        cleanup(stack);
        return 1;
    }

    string_init(&line);
    r = defer(string_reset, &line, &stack);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        cleanup(stack);
        return 1;
    }

    token_list_init(&tl);
    r = defer(token_list_reset, &tl, &stack);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        cleanup(stack);
        return 1;
    }
    r = token_name_init(token_name);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
        cleanup(stack);
        return 1;
    }

    while(!last_line) {
        struct allocator al;
        allocator_init(&al);

        r = next_line(&al, f, &line, 1, &last_line);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }
        r = string2array(&al, &line, &a);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }
        printf("line: %s\n", a);
        r = scan(&al, &line, &tl);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }

        r = token_list_print(&al, &tl, token_name);
        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }

        r = parse(&al, &tl, &root);
        if (r == result_error) {
            cleanup(stack);
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }

        string_clear(&line);
        token_list_reset(&tl);
        dag_destroy(root);

        allocator_destroy(&al);
    }


    cleanup(stack);
    printf("end\n");
    return 0;
}
