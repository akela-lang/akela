#include <stdio.h>
#include <stdlib.h>
#include "scan.h"
#include "ustring.h"
#include "defer.h"

int main(int argc, char** argv)
{
    enum result_enum r;
    int last_line = 0;
    struct string line;
    char* a;
    char* filename;
    struct defer_node* stack = NULL;
    struct token_list tl;

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
    if (r == error_result) {
        fprintf(stderr, "%s\n", error_message);
        cleanup(stack);
        return 1;
    }

    string_init(&line);
    r = defer(string_reset, &line, &stack);
    if (r == error_result) {
        fprintf(stderr, "%s\n", error_message);
        cleanup(stack);
        return 1;
    }

    token_list_init(&tl);
    r = defer(token_list_reset, &tl, &stack);
    if (r == error_result) {
        fprintf(stderr, "%s\n", error_message);
        cleanup(stack);
        return 1;
    }

    while(!last_line) {
        r = next_line(f, &line, 1, &last_line);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }
        r = string2array(&line, &a);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }
        printf("line: %s\n", a);
        free(a);
        r = scan(&line, &tl);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }

        r = token_list_print(&tl);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            cleanup(stack);
            return 1;
        }

        string_clear(&line);
        token_list_reset(&tl);
    }

    cleanup(stack);
    return 0;
}
