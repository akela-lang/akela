#include "zinc/os_unix.h"
#include "zinc/zstring.h"
#include "parse.h"

#define NAME "akela-run-test"

int main(int argc, const char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [DIR]\n", NAME);
        return 1;
    }

    const char* dir_name = argv[1];
    if (!Run_validate_directory(dir_name)) {
        return 1;
    }

    Run_parse_files(dir_name);

    return 0;
}