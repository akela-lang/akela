#include <stdio.h>
#include <stdlib.h>
#include "scan.h"

int main(int argc, char** argv)
{
    enum result_enum res;
    char* buf;
    int last_line = 0;

    while(!last_line) {
        res = next_line(&buf, 0, &last_line);
        if (res == error_result) {
            fprintf(stderr, "%s\n", error_message);
            return 1;
        }
        printf("%s\n", buf);
        fflush(stdout);
        free(buf);
    }
    return 0;
}
