#include <stdint.h>

#include "os.h"
#include <stdlib.h>
#include <string.h>

#include "test.h"

void Zinc_test_panic()
{
    fprintf(stderr, "Exiting because of assertion error.\n");
    exit(1);
}

void Zinc_test_assert_ok(Zinc_test* test, Zinc_result r, const char* message)
{
    test->check_count++;

    if (r == Zinc_result_ok) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\tok assertion error: %s: %s\n", message, Zinc_error_message);
    Zinc_test_panic();
}

void Zinc_test_assert_ptr(Zinc_test* test, void* p, const char* message)
{
    test->check_count++;

    if (p) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p != NULL assertion error: %s\n", p, message);
    Zinc_test_panic();
}


void Zinc_test_assert_int_equal(Zinc_test* test, int a, int b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%d = %d assertion error: %s\n", a, b, message);
    Zinc_test_panic();
}

void Zinc_test_assert_true(Zinc_test* test, int value, const char* message)
{
    test->check_count++;

    if (value) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%d = true assertion error: %s\n", value, message);
    Zinc_test_panic();
}

void Zinc_test_assert_false(Zinc_test* test, int value, const char* message)
{
    test->check_count++;

    if (!value) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%d = false assertion error: %s\n", value, message);
    Zinc_test_panic();
}

void Zinc_test_assert_null(Zinc_test* test, void* p, const char* message)
{
    test->check_count++;

    if (p == NULL) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p == null assertion error: %s\n", p, message);
    Zinc_test_panic();
}

void Zinc_test_assert_error(Zinc_test* test, Zinc_result r, const char* message)
{
    test->check_count++;

    if (r == Zinc_result_error) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\tno error detected assertion error: %s\n", message);
    Zinc_test_panic();
}

void Zinc_test_expect_null(Zinc_test* test, void* p, const char* message)
{
    test->check_count++;

    if (p == NULL) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p == null error: %s\n", p, message);
}

void Zinc_test_expect_size_t_equal(Zinc_test* test, size_t a, size_t b, const char* message)
{
    test->check_count++;
    if (a == b) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t(%zu) = (%zu) error: %s\n", a, b, message);
}

void Zinc_test_expect_string(Zinc_test* test, Zinc_string* a, const char* b, const char* message)
{
    test->check_count++;

    if (Zinc_string_compare_str(a, b)) {
        test->check_passed++;
        return;
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
}

void Zinc_test_expect_true(Zinc_test* test, int value, const char* message)
{
    test->check_count++;

    if (value) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%d = true error: %s\n", value, message);
}

void Zinc_test_expect_false(Zinc_test* test, int value, const char* message)
{
    test->check_count++;

    if (!value) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t(%d) = (false) error: %s\n", value, message);
}

void Zinc_test_expect_strcmp(Zinc_test* test, const char* a, const char* b, const char* message)
{
    test->check_count++;
    if (strcmp(a, b) == 0) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t(%s) = (%s) strcmp error: %s\n", a, b, message);
}

void Zinc_test_expect_int_equal(Zinc_test* test, int a, int b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%d = %d error: %s\n", a, b, message);
}

void Zinc_test_expect_ptr_equal(Zinc_test* test, const void* a, const void* b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p = %p error: %s\n", a, b, message);
}

void Zinc_test_expect_int64_t_equal(Zinc_test* test, int64_t a, int64_t b, const char* message)
{
    test->check_count++;

	if (a == b) {
	    test->check_passed++;
	    return;
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
}

void Zinc_test_expect_uint64_t_equal(Zinc_test* test, uint64_t a, uint64_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
	    test->check_passed++;
	    return;
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
}

void Zinc_test_expect_int32_t_equal(Zinc_test* test, int32_t a, int32_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
	    test->check_passed++;
	    return;
	}

	test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\t%u = %u error: %s\n", a, b, message);
}

void Zinc_test_expect_uint32_t_equal(Zinc_test* test, uint32_t a, uint32_t b, const char* message)
{
	test->check_count++;

	if (a == b) {
	    test->check_passed++;
	    return;
	}

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
	fprintf(stderr, "\n%u = %u error: %s\n", a, b, message);
}

void Zinc_test_expect_long_equal(Zinc_test* test, long a, long b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%ld = %ld error: %s\n", a, b, message);
}

// void Zinc_expect_unsigned_long_equal(unsigned long a, unsigned long b, const char* message)
// {
//     Zinc_test_called();
//     if (a == b) return;
//     Zinc_error_triggered();
//     fprintf(stderr, "%lu = %lu error: %s\n", a, b, message);
// }
//
// void Zinc_expect_long_long_equal(long long a, long long b, const char* message)
// {
// 	Zinc_test_called();
// 	if (a == b) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "%lld = %lld error: %s\n", a, b, message);
// }
//
// void Zinc_expect_u_long_equal(unsigned long a, unsigned long b, const char* message)
// {
//     Zinc_test_called();
//     if (a == b) return;
//     Zinc_error_triggered();
//     fprintf(stderr, "%ld = %ld error: %s\n", a, b, message);
// }
//
// void Zinc_expect_double_equal(double a, double b, const char* message)
// {
//     Zinc_test_called();
//     if (isnan(a) && isnan(b)) return;
//     if (a == b) return;
//     Zinc_error_triggered();
//     fprintf(stderr, "%lf = %lf error: %s\n", a, b, message);
// }
//
// void Zinc_expect_ptr_equal(const void* a, const void* b, const char* message)
// {
// 	Zinc_test_called();
// 	if (a == b) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "%p = %p error: %s\n", a, b, message);
// }
//
// void Zinc_expect_char_equal(char a, char b, const char* message)
// {
// 	Zinc_test_called();
// 	if (a == b) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "(%c) = (%c) error: %s\n", a, b, message);
// }
//
// void Zinc_expect_uint_equal(unsigned int a, unsigned int b, const char* message)
// {
// 	Zinc_test_called();
// 	if (a == b) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "(%u) = (%u) error: %s\n", a, b, message);
// }
//
// void Zinc_assert_size_t_equal(size_t a, size_t b, const char* message)
// {
//     Zinc_test_called();
//     if (a == b) return;
//     Zinc_error_triggered();
//     fprintf(stderr, "(%zu) = (%zu) error: %s\n", a, b, message);
//     Zinc_panic();
// }
//
// void Zinc_expect_size_t_equal(size_t a, size_t b, const char* message)
// {
// 	Zinc_test_called();
// 	if (a == b) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "(%zu) = (%zu) error: %s\n", a, b, message);
// }
//
// void Zinc_expect_true(int value, const char* message)
// {
// 	Zinc_test_called();
// 	if (value) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "%d = true error: %s\n", value, message);
// }
//
// void Zinc_expect_false(int value, const char* message)
// {
// 	Zinc_test_called();
// 	if (!value) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "(%d) = (false) error: %s\n", value, message);
// }
//
// void Zinc_expect_ptr(void* p, const char* message)
// {
// 	Zinc_test_called();
// 	if (p) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "%p != NULL error: %s\n", p, message);
// }
//
// void Zinc_expect_null(void* p, const char* message)
// {
// 	Zinc_test_called();
// 	if (p == NULL) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "%p == null error: %s\n", p, message);
// }
//
// void Zinc_expect_ok(enum Zinc_result r, const char* message)
// {
// 	Zinc_test_called();
// 	if (r == Zinc_result_ok) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "ok error: %s: %s\n", message, Zinc_error_message);
// }
//
// void Zinc_expect_string(struct Zinc_string* a, const char* b, const char* message)
// {
// 	Zinc_test_called();
// 	if (Zinc_string_compare_str(a, b)) return;
// 	Zinc_error_triggered();
// 	char* temp;
//
// 	Zinc_string_create_str(a, &temp);
// 	fprintf(stderr, "%s equals %s error: %s\n", temp, b, message);
//
// 	free(temp);
// }
//
// void Zinc_expect_vector_str(Zinc_vector* a, const char* b, const char* message)
// {
//     Zinc_test_called();
//     if (a->value_size != sizeof(char)) {
//         fprintf(stderr,
//                 "Vector elements must be size of char to compare to string: %s\n",
//                 message);
//         Zinc_error_triggered();
//         return;
//     }
//     Zinc_vector_add_null(a);
//     size_t len = strlen(b);
//     if (a->count != len) {
//         fprintf(stderr,
//                 "Vector and string not the same size: (%s) (%s): %s\n",
//                 ZINC_VECTOR_STRING(a), b, message);
//         Zinc_error_triggered();
//     } else {
//         for (int i = 0; i < a->count; i++) {
//             if (ZINC_VECTOR_CHAR(a, i) != b[i]) {
//                 fprintf(stderr,
//                         "Vector and string not equal: (%s) (%s): %s\n",
//                         ZINC_VECTOR_STRING(a), b, message);
//                 Zinc_error_triggered();
//                 break;
//             }
//         }
//     }
// }
//
// void Zinc_expect_vector(Zinc_vector* a, Zinc_vector* b, const char* message)
// {
//     Zinc_test_called();
//     if (a->value_size != b->value_size) {
//         fprintf(stderr,
//                 "Vector elements must be the same size: %s\n",
//                 message);
//         Zinc_error_triggered();
//         return;
//     }
//     if (a->count != b->count) {
//         fprintf(stderr,
//                 "Vectors not the same size: (%zu) (%zu): %s\n",
//                 a->count, b->count, message);
//         Zinc_error_triggered();
//     } else {
//         for (int i = 0; i < a->count * a->value_size; i++) {
//             if (ZINC_VECTOR_CHAR(a, i) != ZINC_VECTOR_CHAR(b, i)) {
//                 fprintf(stderr,
//                         "Vectors not equal: %d: %s\n",
//                         i, message);
//                 Zinc_error_triggered();
//                 break;
//             }
//         }
//     }
// }
//
// void Zinc_expect_vector_double(Zinc_vector* a, Zinc_vector* b, double threshold, const char* message)
// {
//     Zinc_test_called();
//     if (a->value_size != b->value_size) {
//         fprintf(stderr,
//                 "Vector elements must be the same size: %s\n",
//                 message);
//         Zinc_error_triggered();
//         return;
//     }
//     if (a->count != b->count) {
//         fprintf(stderr,
//                 "Vectors not the same size: (%zu) (%zu): %s\n",
//                 a->count, b->count, message);
//         Zinc_error_triggered();
//     } else {
//         for (size_t i = 0; i < a->count; i++) {
//             double a_value = ZINC_VECTOR_DOUBLE(a, i);
//             double b_value = ZINC_VECTOR_DOUBLE(b, i);
//             if (isnan(a_value) && isnan(b_value)) {
//                 continue;
//             }
//             double delta = a_value - b_value;
//             if (delta < 0) {
//                 delta *= -1;
//             }
//             if (isnan(delta) || delta >= threshold) {
//                 fprintf(stderr,
//                         "Vector element %zu not equal: (%15.15lf) (%15.15lf): %s\n",
//                         i, a_value, b_value, message);
//                 Zinc_error_triggered();
//                 break;
//             }
//         }
//     }
// }
//
// void Zinc_expect_strcmp(const char* a, const char* b, const char* message)
// {
// 	Zinc_test_called();
// 	if (strcmp(a, b) == 0) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "(%s) = (%s) strcmp error: %s\n", a, b, message);
// }
//
// void Zinc_expect_nts(const char* a, const char* b, const char* message)
// {
// 	Zinc_test_called();
// 	if (strcmp(a, b) == 0) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "string compare: (%s) = (%s): %s\n", a, b, message);
// }
//
// void Zinc_expect_error_message(const char* s)
// {
// 	Zinc_test_called();
// 	if (strcmp(Zinc_error_message, s) == 0) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "(%s) = (%s): error message does not memory_match\n", Zinc_error_message, s);
// }
//
// void Zinc_expect_utf8_char(char* a, char* b, char* message)
// {
//     Zinc_test_called();
//     int count_a = ZINC_NUM_BYTES(a[0]);
//     int count_b = ZINC_NUM_BYTES(b[0]);
//     if (count_a == count_b) {
//         for (int i = 0; i < count_a; i++) {
//             if (a[i] != b[i]) {
//             	Zinc_error_triggered();
//             	fprintf(stderr, "utf8 chars not equal: %s\n", message);
//             	return;
//             }
//         }
//         return;
//     }
//     Zinc_error_triggered();
//     fprintf(stderr, "utf8 chars not equal: %s\n", message);
// }
//
// void Zinc_expect_utf8_char_str(char a[4], int num, char* b, char* message)
// {
// 	Zinc_test_called();
// 	int count_a = ZINC_NUM_BYTES(a[0]);
// 	int count_b = ZINC_NUM_BYTES(b[0]);
// 	size_t len = strlen(b);
// 	if (num == count_a && num == count_b && num == len) {
// 		for (int i = 0; i < num; i++) {
// 			if (a[i] != b[i]) {
// 				Zinc_error_triggered();
// 				fprintf(stderr, "utf8 chars not equal: %s\n", message);
// 				return;
// 			}
// 		}
// 		return;
// 	}
// 	Zinc_error_triggered();
// 	fprintf(stderr, "utf8 chars not equal: %s\n", message);
// }
//
// void Zinc_expect_buffer_list_count(Zinc_string_list* bl, size_t count, char* message)
// {
// 	Zinc_test_called();
// 	size_t actual_count = Zinc_string_list_count(bl);
// 	if (actual_count == count) return;
// 	Zinc_error_triggered();
// 	fprintf(stderr, "buffer list count not equal: %zu != %zu: %s\n", actual_count, count, message);
// }
//
// void Zinc_expect_buffer_list_item(Zinc_string_list* bl, size_t index, char* text, char* message)
// {
// 	Zinc_string* bf = Zinc_string_list_get(bl, index);
// 	if (!bf) {
// 		Zinc_error_triggered();
// 		fprintf(stderr, "buffer list index out of bounds: %zu: %s\n", index, message);
// 		return;
// 	}
//
// 	if (!Zinc_string_compare_str(bf, text)) {
// 		Zinc_error_triggered();
// 		Zinc_string_finish(bf);
// 		fprintf(stderr,
// 			"buffer list item (%zu) text not equal: (%s) (%s): %s\n", index, bf->buf, text, message);
// 	}
// }
//
// void Zinc_expect_string_slice(Zinc_string_slice* sl, Zinc_string* bf, char* message)
// {
// 	Zinc_test_called();
// 	bool has_error = false;
//
// 	if (sl->size == bf->size) {
// 		for (size_t i = 0; i < sl->size; i++) {
// 			if (sl->p[i] != bf->buf[i]) {
// 				has_error = true;
// 				break;
// 			}
// 		}
// 	} else {
// 		has_error = true;
// 	}
//
// 	if (has_error) {
// 		Zinc_error_triggered();
// 		struct Zinc_string actual;
// 		Zinc_string_init(&actual);
// 		for (size_t i = 0; i < sl->size; i++) {
// 			Zinc_string_add_char(&actual, sl->p[i]);
// 		}
// 		Zinc_string_finish(&actual);
//
// 		Zinc_string_finish(bf);
// 		fprintf(stderr, "%s = %s error: %s\n", actual.buf, bf->buf, message);
// 		Zinc_string_destroy(&actual);
// 	}
// }
//
// void Zinc_expect_string_slice_str(Zinc_string_slice* sl, char* expected, char* message)
// {
// 	Zinc_test_called();
// 	bool has_error = false;
//
// 	size_t size = strlen(expected);
// 	if (sl->size == size) {
// 		for (size_t i = 0; i < sl->size; i++) {
// 			if (sl->p[i] != expected[i]) {
// 				has_error = true;
// 				break;
// 			}
// 		}
// 	} else {
// 		has_error = true;
// 	}
//
// 	if (has_error) {
// 		Zinc_error_triggered();
// 		struct Zinc_string actual;
// 		Zinc_string_init(&actual);
// 		for (size_t i = 0; i < sl->size; i++) {
// 			Zinc_string_add_char(&actual, sl->p[i]);
// 		}
// 		Zinc_string_finish(&actual);
//
// 		fprintf(stderr, "%s = %s error: %s\n", actual.buf, expected, message);
// 		Zinc_string_destroy(&actual);
// 	}
// }
