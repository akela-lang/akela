#ifndef _PARSE_UNIT_TEST_H
#define _PARSE_UNIT_TEST_H

#include "api.h"
#include "error.h"

ZINC_API void assert_no_errors(struct error_list* el);
ZINC_API void expect_no_errors(struct error_list* el);
ZINC_API void assert_has_errors(struct error_list* el);
ZINC_API void expect_has_errors(struct error_list* el);
ZINC_API struct error* assert_source_error(struct error_list* el, const char message[]);
ZINC_API struct error* expect_source_error(struct error_list* el, const char message[]);

#endif
