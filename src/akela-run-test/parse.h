#ifndef AKELA_RUN_TEST_PARSE_H
#define AKELA_RUN_TEST_PARSE_H

#include "data.h"

bool Art_validate_directory(const char* path);
void Art_parse_files(Art_data* data, const char* dir_name);

#endif