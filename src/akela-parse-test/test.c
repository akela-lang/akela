#include <stdio.h>
#include "parse.h"

#define NAME "akela-parse-test"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [DIR]\n", NAME);
        return 1;
    }

    char* path = argv[1];
    if (!Run_validate_directory(path)) {
        return 1;
    }

    Run_parse_files(path);

    return 0;
}