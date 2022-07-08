#ifndef _PARSE_H
#define _PARSE_H

#include "allocator.h"
#include "parse_tools.h"
#include "dag.h"
#include "result.h"

enum result parse(struct allocator* al, struct parse_state* ps, struct dag_node** root);

#endif
