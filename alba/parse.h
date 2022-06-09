#ifndef _PARSE_H
#define _PARSE_H

#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "result.h"

enum result parse(struct allocator* al, struct token_list* tl, struct dag_node** root);

#endif
