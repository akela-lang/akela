#include <stdio.h>
#include <stdlib.h>
#include "scan.h"

int main(int argc, char** argv)
{
    enum result_enum r;
    int last_line = 0;
    struct string s;
    char* a;

    string_init(&s);
    while(!last_line) {
        r = next_line(&s, 0, &last_line);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }
        r = string2array(&s, &a);
        if (r == error_result) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }
        printf("%s\n", a);
        fflush(stdout);
        string_clear(&s);
        free(a);
    }
    return 0;
}
