#include <math.h>
#include <stdint.h>
#include "os.h"
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "test.h"
#include "vector.h"
#include "string_list.h"
#include "zinc/utf8.h"

void Zinc_assert()
{
	fprintf(stderr, "Exiting because of assertion error.\n");
}

void Zinc_expect_check(Zinc_test* test)
{
	test->check_count++;
}

void Zinc_expect_passed(Zinc_test* test)
{
	test->check_passed++;
}

void Zinc_expect_failed(Zinc_test* test)
{
	test->check_failed++;
	test->pass = false;
}

bool Zinc_expect_result(Zinc_test* test, Zinc_result r, const char* message)
{
    test->check_count++;

    if (r == Zinc_result_error) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\tno error detected assertion error: %s\n", message);
    return false;
}

bool Zinc_expect_null(Zinc_test* test, void* p, const char* message)
{
    test->check_count++;

    if (p == NULL) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p == null error: %s\n", p, message);
	return false;
}

bool Zinc_expect_size_t_equal(Zinc_test* test, size_t a, size_t b, const char* message)
{
    test->check_count++;
    if (a == b) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t(%zu) = (%zu) error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_string(Zinc_test* test, Zinc_string* a, const char* b, const char* message)
{
    test->check_count++;

    if (Zinc_string_compare_str(a, b)) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(
        stderr,
        "\tstrings not equal (%s):\n\t(%s)\n\t(%s)\n",
        message,
        Zinc_string_c_str(a),
        b);
	return false;
}

bool Zinc_expect_true(Zinc_test* test, bool value, const char* fmt, ...)
{
    test->check_count++;

    if (value) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\ttrue (%d): ", value);
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	return false;
}

bool Zinc_expect_false(Zinc_test* test, int value, const char* message)
{
    test->check_count++;

    if (!value) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t(%d) = (false) error: %s\n", value, message);
	return false;
}

bool Zinc_expect_strcmp(Zinc_test* test, const char* a, const char* b, const char* message)
{
    test->check_count++;
    if (strcmp(a, b) == 0) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t(%s) = (%s) strcmp error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_int_equal(Zinc_test* test, int a, int b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%d = %d error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_ptr_equal(Zinc_test* test, const void* a, const void* b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p = %p error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_long_equal(Zinc_test* test, long a, long b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%ld = %ld error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_unsigned_long_equal(Zinc_test* test, unsigned long a, unsigned long b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%lu = %lu error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_long_long_equal(Zinc_test* test, long long a, long long b, const char* message)
{
	test->check_count++;

	if (a == b) {
	    test->check_passed++;
	    return true;
	}

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%lld = %lld error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_u_long_equal(Zinc_test* test, unsigned long a, unsigned long b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%ld = %ld error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_char_equal(Zinc_test* test, char a, char b, const char* message)
{
	test->check_count++;

	if (a == b) {
	    test->check_passed++;
	    return true;
	}

	test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\t(%c) = (%c) error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_ptr(Zinc_test* test, void* p, const char* fmt, ...)
{
	test->check_count++;
	if (p) {
	    test->check_passed++;
	    return true;
	}

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	va_list args;
	fprintf(stderr, "\t%p == NULL\n", p);
	fprintf(stderr, "\t");
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	return false;
}

bool Zinc_expect_ok(Zinc_test* test, Zinc_result r, const char* message)
{
	test->check_count++;

	if (r == Zinc_result_ok) {
	    test->check_passed++;
	    return true;
	}

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\tok error: %s: %s\n", message, Zinc_error_message);
	return false;
}

bool Zinc_expect_vector_str(Zinc_test* test, Zinc_vector* a, const char* b, const char* message)
{
    test->check_count++;

    if (a->value_size != sizeof(char)) {
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
        fprintf(stderr,
                "\tVector elements must be size of char to compare to string: %s\n",
                message);
        return false;
    }

    Zinc_vector_add_null(a);
    size_t len = strlen(b);
    if (a->count != len) {
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
        fprintf(stderr,
                "\tVector and string not the same size: (%s) (%s): %s\n",
                ZINC_VECTOR_STRING(a), b, message);
        return false;
    }

    for (int i = 0; i < a->count; i++) {
        if (ZINC_VECTOR_CHAR(a, i) != b[i]) {
            test->check_failed++;
            test->pass = false;
            Zinc_test_print_unseen(test);
            fprintf(stderr,
                    "\tVector and string not equal: (%s) (%s): %s\n",
                    ZINC_VECTOR_STRING(a), b, message);
            return false;
        }

    }

    test->check_passed++;
	return true;
}

bool Zinc_expect_vector(Zinc_test* test, Zinc_vector* a, Zinc_vector* b, const char* message)
{
    test->check_count++;
    if (a->value_size != b->value_size) {
        fprintf(stderr,
                "Vector elements must be the same size: %s\n",
                message);
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
        return false;
    }
    if (a->count != b->count) {
        fprintf(stderr,
                "Vectors not the same size: (%zu) (%zu): %s\n",
                a->count, b->count, message);
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
    	return false;
    } else {
        for (int i = 0; i < a->count * a->value_size; i++) {
            if (ZINC_VECTOR_CHAR(a, i) != ZINC_VECTOR_CHAR(b, i)) {
                fprintf(stderr,
                        "Vectors not equal: %d: %s\n",
                        i, message);
                test->check_failed++;
                test->pass = false;
                Zinc_test_print_unseen(test);
                return false;
            }
        }
        test->check_passed++;
    	return true;
    }
}

bool Zinc_expect_error_message(Zinc_test* test, const char* s)
{
	test->check_count++;

	if (strcmp(Zinc_error_message, s) == 0) {
	    test->check_passed++;
	    return true;
	}

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\t(%s) = (%s): error message does not memory_match\n", Zinc_error_message, s);
	return false;
}

bool Zinc_expect_utf8_char_str(Zinc_test* test, char a[4], int num, char* b, char* message)
{
	test->check_count++;
	int count_a = ZINC_NUM_BYTES(a[0]);
	int count_b = ZINC_NUM_BYTES(b[0]);
	size_t len = strlen(b);
	if (num == count_a && num == count_b && num == len) {
		for (int i = 0; i < num; i++) {
			if (a[i] != b[i]) {
				test->check_failed++;
			    test->pass = false;
			    Zinc_test_print_unseen(test);
				fprintf(stderr, "\tutf8 chars not equal: %s\n", message);
				return false;
			}
		}
		return true;
	}
	test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\tutf8 chars not equal: %s\n", message);
	return false;
}

bool Zinc_expect_buffer_list_count(Zinc_test* test, Zinc_string_list* bl, size_t count, char* message)
{
	test->check_count++;
	size_t actual_count = Zinc_string_list_count(bl);
	if (actual_count == count) {
	    test->check_passed++;
	    return true;
	}

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\tbuffer list count not equal: %zu != %zu: %s\n", actual_count, count, message);
	return false;
}

bool Zinc_expect_buffer_list_item(Zinc_test* test, Zinc_string_list* bl, size_t index, char* text, char* message)
{
    test->check_count++;
	Zinc_string* bf = Zinc_string_list_get(bl, index);
	if (!bf) {
	    test->check_failed++;
	    test->pass = false;
	    Zinc_test_print_unseen(test);
		fprintf(stderr, "\tbuffer list index out of bounds: %zu: %s\n", index, message);
		return false;
	}

	if (!Zinc_string_compare_str(bf, text)) {
	    test->check_failed++;
	    test->pass = false;
	    Zinc_test_print_unseen(test);
		Zinc_string_finish(bf);
		fprintf(stderr,
			"\tbuffer list item (%zu) text not equal: (%s) (%s): %s\n",
			index,
			bf->buf,
			text,
			message);
	    return false;
	}

    test->check_passed++;
	return true;
}

bool Zinc_expect_no_errors(Zinc_test* test, Zinc_error_list* el)
{
    test->check_count++;

    if (!el->head) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\texpect no errors: has errors\n");
    Zinc_error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        fprintf(stderr, "\t(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
        e = e->next;
    }

	return false;
}

bool Zinc_expect_error_count(Zinc_test* test, Zinc_error_list* el, size_t count)
{
    test->check_count++;

    size_t actual_count = 0;
    struct Zinc_error* e = el->head;
    while (e) {
        actual_count++;
        e = e->next;
    }
    if (actual_count == count) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\texpected error count (%zu) (%zu)\n", actual_count, count);
	return false;
}

bool Zinc_expect_has_errors(Zinc_test* test, Zinc_error_list* el)
{
    test->check_count++;

    if (el->head) {
        test->check_passed++;
        return true;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\texpect has errors: there are no errors\n");
	return false;
}

Zinc_error* Zinc_expect_source_error(Zinc_test* test, Zinc_error_list* el, const char message[])
{
    test->check_count++;

    Zinc_error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        if (strcmp(e->message.buf, message) == 0) {
            test->check_passed++;
            return e;
        }
        e = e->next;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\terror not found: %s\n", message);
    e = el->head;
    if (e) {
        fprintf(stderr, "Errors:\n");
    }
    while (e) {
        Zinc_string_finish(&e->message);
        fprintf(stderr, "\t%s\n", e->message.buf);
        e = e->next;
    }
    return NULL;
}

bool Zinc_expect_uint_equal(Zinc_test* test, unsigned int a, unsigned int b, const char* message)
{
	test->check_count++;
	if (a == b) {
	    test->check_passed++;
	    return true;
	}
    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\t(%u) = (%u) error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_string_slice(Zinc_test* test, Zinc_string_slice* sl, Zinc_string* bf, char* message)
{
	test->check_count++;
	bool has_error = false;

	if (sl->size == bf->size) {
		for (size_t i = 0; i < sl->size; i++) {
		    test->check_count++;
			if (sl->p[i] != bf->buf[i]) {
				has_error = true;
				break;
			}
		}
	} else {
		has_error = true;
	}

	if (has_error) {
	    test->check_failed++;
	    test->pass = false;
	    Zinc_test_print_unseen(test);

	    struct Zinc_string actual;
		Zinc_string_init(&actual);
		for (size_t i = 0; i < sl->size; i++) {
			Zinc_string_add_char(&actual, sl->p[i]);
		}
		Zinc_string_finish(&actual);

		Zinc_string_finish(bf);
		fprintf(stderr, "/t%s = %s error: %s\n", actual.buf, bf->buf, message);
		Zinc_string_destroy(&actual);
		return false;
	} else {
		test->check_passed++;
		return true;
	}
}

bool Zinc_expect_string_slice_str(Zinc_test* test, Zinc_string_slice* sl, char* expected, char* message)
{
	bool has_error = false;

	size_t size = strlen(expected);
	test->check_count++;
	if (sl->size == size) {
		for (size_t i = 0; i < sl->size; i++) {
			test->check_count++;
			if (sl->p[i] != expected[i]) {
				has_error = true;
				break;
			}
		}
	} else {
		has_error = true;
	}

	if (has_error) {
		test->check_failed++;
		test->pass = false;
		Zinc_test_print_unseen(test);
		struct Zinc_string actual;
		Zinc_string_init(&actual);
		for (size_t i = 0; i < sl->size; i++) {
			Zinc_string_add_char(&actual, sl->p[i]);
		}
		Zinc_string_finish(&actual);

		fprintf(stderr, "\t%s = %s error: %s\n", actual.buf, expected, message);
		Zinc_string_destroy(&actual);
		return false;
	} else {
		test->check_passed++;
		return true;
	}
}

bool Zinc_expect_utf8_char(Zinc_test* test, char* a, char* b, char* message)
{
    int count_a = ZINC_NUM_BYTES(a[0]);
    int count_b = ZINC_NUM_BYTES(b[0]);
    test->check_count++;
    if (count_a == count_b) {
        for (int i = 0; i < count_a; i++) {
        	test->check_count++;
        	if (a[i] != b[i]) {
            	test->check_failed++;
        		test->pass = false;
        		Zinc_test_print_unseen(test);
            	fprintf(stderr, "\tutf8 chars not equal: %s\n", message);
            	return false;
            }
        }
    	test->check_passed++;
        return true;
    }
    test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
    fprintf(stderr, "\tutf8 chars not equal: %s\n", message);
	return false;
}

bool Zinc_expect_vector_double(Zinc_test* test, Zinc_vector* a, Zinc_vector* b, double threshold, const char* message)
{
    test->check_count++;
    if (a->value_size != b->value_size) {
        fprintf(stderr,
                "\tVector elements must be the same size: %s\n",
                message);
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
        return false;
    }
    test->check_count++;
    if (a->count != b->count) {
        fprintf(stderr,
                "\tVectors not the same size: (%zu) (%zu): %s\n",
                a->count, b->count, message);
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
    	return false;
    } else {
        test->check_passed++;
        for (size_t i = 0; i < a->count; i++) {
            double a_value = ZINC_VECTOR_DOUBLE(a, i);
            double b_value = ZINC_VECTOR_DOUBLE(b, i);
            test->check_count++;
            if (isnan(a_value) && isnan(b_value)) {
                continue;
            }
            double delta = a_value - b_value;
            if (delta < 0) {
                delta *= -1;
            }
            if (isnan(delta) || delta >= threshold) {
                fprintf(stderr,
                        "\tVector element %zu not equal: (%15.15lf) (%15.15lf): %s\n",
                        i, a_value, b_value, message);
                test->check_failed++;
                test->pass = false;
                Zinc_test_print_unseen(test);
                return false;
            } else {
                test->check_passed++;
            }
        }
    }
	return true;
}

bool Zinc_expect_nts(Zinc_test* test, const char* a, const char* b, const char* message)
{
	test->check_count++;
	if (strcmp(a, b) == 0) {
		test->check_passed++;
		return true;
	}
	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\tstring compare: (%s) = (%s): %s\n", a, b, message);
	return false;
}

//
// Sized Integer
//

bool Zinc_expect_int8_t_equal(Zinc_test* test, int8_t a, int8_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%hhd = %hhd error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_int16_t_equal(Zinc_test* test, int16_t a, int16_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%hd = %hd error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_int32_t_equal(Zinc_test* test, int32_t a, int32_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%d = %d error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_int64_t_equal(Zinc_test* test, int64_t a, int64_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
#if IS_UNIX
	fprintf(stderr, "%ld = %ld error: %s\n", a, b, message);
#elif IS_WIN
	fprintf(stderr, "%lld = %lld error: %s\n", a, b, message);
#else
#error unsupported platform
#endif
	return false;
}

//
// Sized unsigned int
//

bool Zinc_expect_uint8_t_equal(Zinc_test* test, uint8_t a, uint8_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\n%hhu = %hhu error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_uint16_t_equal(Zinc_test* test, uint16_t a, uint16_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\n%hu = %hu error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_uint32_t_equal(Zinc_test* test, uint32_t a, uint32_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\n%u = %u error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_uint64_t_equal(Zinc_test* test, uint64_t a, uint64_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
#if IS_UNIX
	fprintf(stderr, "%lu = %lu error: %s\n", a, b, message);
#elif IS_WIN
	fprintf(stderr, "%llu = %llu error: %s\n", a, b, message);
#else
#error unsupported platform
#endif
	return false;
}

//
// Float
//

bool Zinc_expect_float_equal(Zinc_test* test, float a, float b, const char* message)
{
	test->check_count++;

	if (isnan(a) && isnan(b)) {
		test->check_passed++;
		return true;
	}

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%f = %f error: %s\n", a, b, message);
	return false;
}

bool Zinc_expect_double_equal(Zinc_test* test, double a, double b, const char* message)
{
	test->check_count++;

	if (isnan(a) && isnan(b)) {
		test->check_passed++;
		return true;
	}

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%lf = %lf error: %s\n", a, b, message);
	return false;
}

//
// Boolean
//

bool Zinc_expect_boolean_equal(Zinc_test* test, bool a, bool b, const char* message)
{
	test->check_count++;

	if (a == b) {
		test->check_passed++;
		return true;
	}

	test->check_failed++;
	test->pass = false;
	Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%d = %d error: %s\n", a, b, message);
	return false;
}
