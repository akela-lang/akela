#ifndef _EXPR_H
#define _EXPR_H

#include "zinc/result.h"
#include "token.h"
#include "dag.h"

enum result expr(struct parse_state* ps, struct dag_node** root);

#endif
