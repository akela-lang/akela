#ifndef AKELA_PARSE_H
#define AKELA_PARSE_H

#include "api.h"
#include <stdbool.h>
#include "parse_tools.h"
#include "ast.h"
#include "zinc/result.h"
#include "parse_data.h"

AKELA_API Ake_parse_result Ake_parse(struct Ake_parse_state* ps);

#endif
