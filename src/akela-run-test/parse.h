#ifndef AKELA_RUN_TEST_PARSE_H
#define AKELA_RUN_TEST_PARSE_H

#include "data.h"

bool Run_validate_directory(const char* path);
void Run_parse_files(Art_data* data, const char* dir_name);

#endif