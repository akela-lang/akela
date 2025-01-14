#ifndef _PARSE_UNIT_TEST_H
#define _PARSE_UNIT_TEST_H

#include "api.h"
#include "error.h"

ZINC_API void Zinc_assert_no_errors(struct Zinc_error_list* el);
ZINC_API void Zinc_expect_no_errors(struct Zinc_error_list* el);
ZINC_API void Zinc_assert_has_errors(struct Zinc_error_list* el);
ZINC_API void Zinc_expect_has_errors(struct Zinc_error_list* el);
ZINC_API struct Zinc_error* Zinc_assert_source_error(struct Zinc_error_list* el, const char message[]);
ZINC_API struct Zinc_error* Zinc_expect_source_error(struct Zinc_error_list* el, const char message[]);
ZINC_API void Zinc_expect_error_count(struct Zinc_error_list* el, size_t count);

#endif
