#include <stdio.h>
#include <string.h>

#include "cobble/compile.h"
#include "cobble/match.h"

#define NAME "cobble"

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [REGEX] [STRING]\n", NAME);
        return 1;
    }

    char* regex = argv[1];
    char* string = argv[2];
    size_t len = strlen(string);

    Cob_re re = Cob_compile_str(regex);
    Cob_result mr = Cob_match(&re, (Zinc_string_slice) {
        .p = string,
        .size = len,
    });

    if (mr.matched) {
        printf("match:\n");
        for (int i = 0; i < Zinc_string_list_count(&mr.groups); i++) {
            Zinc_string* gr = Zinc_string_list_get(&mr.groups, i);
            printf("%d: \"%s\"\n", i, Zinc_string_c_str(gr));
        }
    } else {
        printf("no match\n");
    }

    return 0;
}
