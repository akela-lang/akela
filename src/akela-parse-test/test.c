#include <stdio.h>
#include "parse.h"

#define NAME "akela-parse-test"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", NAME);
        return 1;
    }

    char* path = argv[1];
    if (!Apt_validate_directory(path)) {
        return 1;
    }

    Apt_parse_files(path);

    return 0;
}