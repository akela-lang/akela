#ifndef _STMT_H
#define _STMT_H

#include <stdbool.h>

/* dynamic-output ps{} root root{} */
bool stmts(struct parse_state* ps, struct dag_node** root);

#endif
