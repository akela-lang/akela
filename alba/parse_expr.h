#ifndef _EXPR_H
#define _EXPR_H

#include "result.h"
#include "allocator.h"
#include "token.h"
#include "dag.h"

enum result expr(struct allocator* al, struct parse_state* ps, struct dag_node** root);

#endif
