#include <stdlib.h>
#include <string.h>
#include "zinc/unit_test.h"
#include "source.h"
#include "uconv.h"
#include "zinc/result.h"
#include "zinc/utf8.h"
#include "unit_test_compiler.h"

/* dynamic-output-none */
void assert_has_errors(struct compile_error_list* el)
{
	test_called();
	if (el->head) return;
	error_triggered();
	printf("assert has errors: there are no errors\n");
	panic();
}

/* dynamic-output-none */
void expect_has_errors(struct compile_error_list* el)
{
	test_called();
	if (el->head) return;
	error_triggered();
	printf("expect has errors: there are no errors\n");
}

/* dynamic-output-none */
void assert_no_errors(struct compile_error_list* el)
{
	test_called();
	if (!el->head) return;
	error_triggered();
	printf("assert no errors: there are some errors\n");
	struct compile_error* p = el->head;
	while (p) {
		printf("%s\n", p->message);
		p = p->next;
	}
	panic();
}

/* dynamic-output-none */
void expect_no_errors(struct compile_error_list* el)
{
	test_called();
	if (!el->head) return;
	error_triggered();
	printf("expect no errors: there are some errors\n");
	struct compile_error* p = el->head;
	while (p) {
		printf("%s\n", p->message);
		p = p->next;
	}
}

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

/* dynamic-output-none */
struct compile_error* assert_compile_error(struct compile_error_list* el, char* s)
{
	test_called();
	for (struct compile_error* p = el->head; p; p = p->next) {
		if (strcmp(p->message, s) == 0) {
			return p;
		}
	}

	error_triggered();
	printf("(%s): no matching compile error message\n", s);
	struct compile_error* p = el->head;
	while (p) {
		printf("%s\n", p->message);
		p = p->next;
	}
	panic();
	return NULL;
}

/* dynamic-output-none */
struct compile_error* expect_compile_error(struct compile_error_list* el, char* s)
{
	test_called();
	for (struct compile_error* p = el->head; p; p = p->next) {
		if (strcmp(p->message, s) == 0) {
			return p;
		}
	}

	error_triggered();
	printf("(%s): no matching compile error message\n", s);
	struct compile_error* p = el->head;
	while (p) {
		printf("%s\n", p->message);
		p = p->next;
	}
	return NULL;
}

/* dynamic-output-none */
void expect_compile_error_fields(struct compile_error* e, size_t line, size_t col, size_t byte_pos)
{
	bool has_error = 0;
	test_called();

	if (e->line != line) {
		set_error("(%zu) = (%zu) line error", e->line, line);
		has_error = true;
	} else if (e->col != col) {
		set_error("(%zu) = (%zu) col error", e->col, col);
		has_error = true;
	} else if (e->byte_pos != byte_pos) {
		set_error("(%zu) = (%zu) col error", e->byte_pos, byte_pos);
		has_error = true;
	}

	if (!has_error) return;

	error_triggered();
	printf("%s\n", error_message);
}