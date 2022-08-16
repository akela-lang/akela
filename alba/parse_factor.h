#ifndef	_PARSE_FACTOR_H
#define _PARSE_FACTOR_H

#include "alba_api.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool factor(struct parse_state* ps, struct dag_node** root);

#endif
