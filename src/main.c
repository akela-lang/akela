#include <stdio.h>
#include <stdlib.h>
#include "scan.h"

int main(int argc, char** argv)
{
    enum result_enum r;
    int last_line = 0;
    struct string line;
    char* a;

    char* filename;

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
    while(!last_line) {
        r = next_line(f, &line, 1, &last_line);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }
        r = string2array(&line, &a);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            string_clear(&line);
            return 1;
        }
        printf("line: %s\n", a);
        fflush(stdout);
        free(a);
        r = scan(&line);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            string_clear(&line);
            return 1;
        }
        string_clear(&line);
    }

    fclose(f);
    return 0;
}
