#ifndef _PARSE_H
#define _PARSE_H

#include "parse_tools.h"
#include "dag.h"
#include "zinc/result.h"

enum result parse(struct parse_state* ps, struct dag_node** root);

#endif
