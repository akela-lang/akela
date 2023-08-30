#include <stdlib.h>
#include <string.h>
#include "zinc/unit_test.h"
#include "source.h"
#include "uconv.h"
#include "zinc/result.h"
#include "zinc/utf8.h"
#include "unit_test_compiler.h"

/* dynamic-output-none */
void expect_utf8_char(char* a, char* b, char* message)
{
	test_called();
	int count_a = NUM_BYTES(a[0]);
	int count_b = NUM_BYTES(b[0]);
	if (count_a == count_b) {
		for (int i = 0; i < count_a; i++) {
			if (a[i] != b[i]) {
				break;
			}
		}
		return;
	}
	error_triggered();
	printf("utf8 chars not equal: %s\n", message);
}

/* dynamic-output-none */
void expect_utf32_char(UChar32 a, UChar32 b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("(%d:%c) = (%d:%c) utf32 error: %s\n", a, a, b, b, message);
}