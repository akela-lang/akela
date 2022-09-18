#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "alba/input.h"
#include "alba/uconv.h"
#include "alba/parse_tools.h"
#include "alba/parse.h"
#include "alba/scan.h"
#include "alba/source.h"
#include "alba/comp_unit.h"

/* dynamic-output ps{} root root{} */
bool parse_setup2(char* line, struct comp_unit* cu)
{
	/* allocate bf */
	struct buffer* bf;
	malloc_safe((void**)&bf, sizeof(struct buffer));
	buffer_init(bf);

	/* allocate bf{} */
	array2buffer(line, bf);

	/* allocate sd */
	struct string_data* sd = NULL;
	malloc_safe((void**)&sd, sizeof(struct string_data));

	/* transfer bf -> sd{} */
	string_data_init(bf, sd);

	comp_unit_init(cu);
	bool valid = comp_unit_compile(cu, string_getchar, sd);

	buffer_destroy(bf);
	free(bf);
	free(sd);

	return valid;
}

void parse_teardown2(struct comp_unit* cu)
{
	comp_unit_destroy(cu);
}
