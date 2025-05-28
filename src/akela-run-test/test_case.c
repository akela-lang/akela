#include "data.h"
#include "zinc/zstring.h"
#include "zinc/input_unicode_string.h"
#include "akela/comp_unit.h"
#include "akela-llvm/cg.h"
#include "cobble/compile.h"
#include "cobble/match.h"
#include <assert.h>
#include <float.h>
#include <stdint.h>
#include <akela/comp_table.h>

#include "zinc/os.h"

void Art_run_suite(Zinc_test* top_test, Zinc_test* suite_test);
void Art_run_test(Zinc_test* top_test, Zinc_test* suite_test, Zinc_test* case_test);
Art_pair Art_diff(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected);
bool Art_diff_value(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected);
Zinc_string_list* Art_split(Zinc_string* string);
Zinc_string* Art_join(Zinc_string_list* list);
void Art_print_akela(Zinc_string* ake);
void Art_print_llvm(Art_pair* pair);
void Art_print_result(Art_pair* pair);
void Art_setup_address(Zinc_test* top_test, Zinc_test* suite_test, Zinc_test* case_test);
bool Art_check_address(
    Zinc_test* top_test,
    Zinc_test* case_test,
    Ake_comp_table* ct,
    Ake_code_gen_result* cg_result);

void Art_run_top(Zinc_test* top_test)
{
    Art_top_data* top_data = top_test->data;

    Zinc_test* suite_test = top_test->head;
    while (suite_test) {
        Art_suite_data* suite_data = suite_test->data;
        if (suite_test->mute) {
            printf("Muting: %s\n", Zinc_string_c_str(&suite_data->path));
        }
        bool can_run = !suite_test->mute
            && ((top_data->has_solo && suite_test->solo) || !top_data->has_solo);
        if (can_run) {
            if (suite_test->solo) {
                printf("Running solo: %s\n", Zinc_string_c_str(&suite_data->path));
            }
            Art_run_suite(top_test, suite_test);
        }

        suite_test = suite_test->next;
    }
}

void Art_run_suite(Zinc_test* top_test, Zinc_test* suite_test)
{
    Art_top_data* top_data = top_test->data;
    Art_suite_data* suite_data = suite_test->data;

    Zinc_test* case_test = suite_test->head;
    while (case_test) {
        Art_case_data* case_data = case_test->data;
        if (case_test->mute) {
            printf("Muting: %s\n", Zinc_string_c_str(&case_data->description));
        }
        bool can_run = !case_test->mute
            && ((suite_test->has_solo && case_test->solo) || !suite_test->has_solo);
        if (can_run) {
            if (suite_test->solo) {
                printf("Running solo: %s\n", Zinc_string_c_str(&case_data->description));
            }
            Art_run_test(top_test, suite_test, case_test);
        }

        case_test = case_test->next;
    }
}

void Art_run_test(Zinc_test* top_test, Zinc_test* suite_test, Zinc_test* case_test)
{
    Art_top_data* top_data = top_test->data;
    Art_suite_data* suite_data = suite_test->data;
    Art_case_data* case_data = case_test->data;

    FILE* fp = fopen(Zinc_string_c_str(&suite_data->path), "r");
    if (!fp) {
        Zinc_error_list_set(
            &suite_data->errors,
            NULL,
            "could not open: %bf",
            Zinc_string_c_str(&suite_data->path));
        return;
    }

    Ake_comp_table* ct = NULL;
    Ake_comp_table_create_fp(&ct, suite_data->path, fp);
    Ake_comp_unit_set_bounds(ct->primary, &case_data->source_bounds);
    Ake_comp_unit_parse(ct->primary);

    bool passed = true;
    if (!ct->primary->valid) {
        /* is parsing valid */
        Zinc_error* e = ct->primary->errors.head;
        while (e) {
            Zinc_string_finish(&e->message);
            fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
        passed = false;
    } else {
        Art_setup_address(top_test, suite_test, case_test);

        /* run program on jit */
        Akela_llvm_cg* cg = NULL;
        Akela_llvm_cg_create(&cg, &ct->primary->errors, &ct->primary->extern_list);
        Ake_code_gen_result cg_result;
        Ake_code_gen_result_init(&cg_result);
        cg_result.return_address = case_data->return_address;
        cg_result.return_size = case_data->return_size;
        if (case_data->snapshot) {
            cg_result.debug = true;
            cg_result.dry_run = true;
        }

        Ake_code_gen_jit(cg, &Akela_llvm_vtable, ct->primary->root, &cg_result);
        Akela_llvm_cg_destroy(cg);

        /* check llvm output */
        if (!case_data->snapshot) {
            case_test->ran = true;
            Art_pair llvm_pair = Art_diff(top_data->regex_re, &cg_result.module_text, &case_data->llvm);
            if (!llvm_pair.matched) {
                passed = false;
                Art_print_llvm(&llvm_pair);
            }
            Art_pair_destroy(&llvm_pair);

            if (ct->primary->errors.head) {
                /* any other errors */
                Zinc_error* e = ct->primary->errors.head;
                while (e) {
                    Zinc_string_finish(&e->message);
                    fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
                    e = e->next;
                }
                passed = false;
            }

            if (!Art_check_address(top_test, case_test, ct, &cg_result)) {
                passed = false;
            }

            Ake_code_gen_result_destroy(&cg_result);
        }

    }

    if (passed) {
        case_test->pass = true;
    }

    Ake_comp_table_destroy(ct);
    free(ct);
}

void Art_setup_address(Zinc_test* top_test, Zinc_test* suite_test, Zinc_test* case_test)
{
    Art_top_data* top_data = top_test->data;
    Art_suite_data* suite_data = suite_test->data;
    Art_case_data* case_data = case_test->data;

    Cent_value* data_type_list_value = top_data->type_info->primary->value;
    assert(data_type_list_value);

    assert(case_data->value);

    long long byte_count = 0;

    Cent_value* field = case_data->value;
    assert(field);
    while (field) {
        Cent_value* type_value = Cent_value_get_str(field, "type");
        assert(type_value);

        int type = type_value->data.enumeration.enum_value->value;
        Zinc_string* type_string = &type_value->data.enumeration.enum_value->display;

        bool found = false;
        Cent_value* found_value = NULL;
        Cent_value* data_type_value = data_type_list_value->data.dag.head;
        while (data_type_value) {
            Cent_value* type_value2 = Cent_value_get_str(data_type_value, "type");
            assert(type_value2);
            int type2 = type_value2->data.enumeration.enum_value->value;
            if (type2 == type) {
                found = true;
                found_value = data_type_value;
                break;
            }
            data_type_value = data_type_value->next;
        }
        if (!found) {
            Zinc_string_finish(type_string);
            fprintf(stderr, "Could not find field type %d-%s", type, type_string->buf);
            return;
        }

        Cent_value* align_value = Cent_value_get_str(found_value, "align");
        assert(align_value);
        long long align = align_value->data.integer;
        byte_count += align;
        case_data->return_size = byte_count;
        Zinc_malloc_safe((void**)&case_data->return_address, byte_count);

        field = field->next;
    }
}

bool Art_check_address(
    Zinc_test* top_test,
    Zinc_test* case_test,
    Ake_comp_table* ct,
    Ake_code_gen_result* cg_result)
{
    bool matched = true;

    Art_top_data* top_data = top_test->data;
    Art_case_data* case_data = case_test->data;

    Cent_value* field = case_data->value;
    assert(field);
    while (field) {
        Cent_value* type_value = Cent_value_get_str(field, "type");
        assert(type_value);

        int type = type_value->data.enumeration.enum_value->value;
        Zinc_string* type_string = &type_value->data.enumeration.enum_value->display;

        Cent_value* value_value = Cent_value_get_str(field, "value");
        assert(value_value);

        /* handle case of string which could be regex */
        if (value_value->type == Cent_value_type_string) {
            Zinc_string* actual = &cg_result->value;
            Zinc_string* expected = &value_value->data.string;
            if (!Art_diff_value(top_data->regex_re, actual, expected)) {
                matched = false;
                Zinc_string_finish(actual);
                Zinc_string_finish(expected);
                Zinc_test_print_unseen(case_test);
                fprintf(stderr, "result does not match: (%s) (%s)\n", actual->buf, expected->buf);
            }
            return matched;
        }

        /* otherwise look at type */
        if (type == Run_type_int8) {
            if (value_value->type == Cent_value_type_natural) {
                if (value_value->data.natural <= INT64_MAX) {
                    value_value->type = Cent_value_type_integer;
                    value_value->data.integer = (int64_t)value_value->data.natural;
                }
            }
            if (value_value->type == Cent_value_type_integer) {
                int8_t actual = *(int8_t*)case_data->return_address;
                int8_t expected = (int8_t)value_value->data.integer;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%hhd) (%hhd)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_int16) {
            if (value_value->type == Cent_value_type_natural) {
                if (value_value->data.natural <= INT64_MAX) {
                    value_value->type = Cent_value_type_integer;
                    value_value->data.integer = (int64_t)value_value->data.natural;
                }
            }
            if (value_value->type == Cent_value_type_integer) {
                int16_t actual = *(int16_t*)case_data->return_address;
                int16_t expected = (int16_t)value_value->data.integer;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%hd) (%hd)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_int32) {
            if (value_value->type == Cent_value_type_natural) {
                if (value_value->data.natural <= INT64_MAX) {
                    value_value->type = Cent_value_type_integer;
                    value_value->data.integer = (int64_t)value_value->data.natural;
                }
            }
            if (value_value->type == Cent_value_type_integer) {
                int32_t actual = *(int32_t*)case_data->return_address;
                int32_t expected = (int32_t)value_value->data.integer;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%d) (%d)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_int64) {
            if (value_value->type == Cent_value_type_natural) {
                if (value_value->data.natural <= INT64_MAX) {
                    value_value->type = Cent_value_type_integer;
                    value_value->data.integer = (int64_t)value_value->data.natural;
                }
            }
            if (value_value->type == Cent_value_type_integer) {
                int64_t actual = *(int64_t*)case_data->return_address;
                int64_t expected = (int64_t)value_value->data.integer;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%ld) (%ld)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_nat8) {
            if (value_value->type == Cent_value_type_natural) {
                uint8_t actual = *(uint8_t*)case_data->return_address;
                uint8_t expected = (uint8_t)value_value->data.natural;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%hhu) (%hhu)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_nat16) {
            if (value_value->type == Cent_value_type_natural) {
                uint16_t actual = *(uint16_t*)case_data->return_address;
                uint16_t expected = (uint16_t)value_value->data.natural;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%hu) (%hu)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_nat32) {
            if (value_value->type == Cent_value_type_natural) {
                uint32_t actual = *(uint32_t*)case_data->return_address;
                uint32_t expected = (uint32_t)value_value->data.natural;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%u) (%u)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_nat64) {
            if (value_value->type == Cent_value_type_natural) {
                uint64_t actual = *(uint64_t*)case_data->return_address;
                uint64_t expected = (uint64_t)value_value->data.natural;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%lu) (%lu)\n", actual, expected);
                }
            } else {
                assert(false && "expected integer or string value type");
            }
        } else if (type == Run_type_real16) {
            if (value_value->type == Cent_value_type_real) {
#if IS_UNIX
                _Float16 actual = *(_Float16*)case_data->return_address;
                _Float16 expected = (_Float16)value_value->data.real;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%f) (%f)\n", (float)actual, (float)expected);
                }
#elif IS_WIN
                assert(false && "16-bit floats not supported");
#else
#error "unsupported OS"
#endif
            } else {
                assert(false && "expected real or string value type");
            }
        } else if (type == Run_type_real32) {
            if (value_value->type == Cent_value_type_real) {
                float actual = *(float*)case_data->return_address;
                float expected = (float)value_value->data.real;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%f) (%f)\n", actual, expected);
                }
            } else {
                assert(false && "expected real or string value type");
            }
        } else if (type == Run_type_real64) {
            if (value_value->type == Cent_value_type_real) {
                double actual = *(double*)case_data->return_address;
                double expected = (double)value_value->data.real;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%lf) (%lf)\n", actual, expected);
                }
            } else {
                assert(false && "expected real or string value type");
            }
        } else if (type == Run_type_bool) {
            if (value_value->type == Cent_value_type_boolean) {
                bool actual = *(bool*)case_data->return_address;
                bool expected = (bool)value_value->data.boolean;
                if (actual != expected) {
                    matched = false;
                    Zinc_test_print_unseen(case_test);
                    fprintf(stderr, "result does not match: (%d) (%d)\n", actual, expected);
                }
            } else {
                assert(false && "expected boolean or string value type");
            }
        } else {
            assert(false && "unhandled type");
        }

        field = field->next;
    }

    return matched;
}

Art_pair Art_diff(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected)
{
    Zinc_string_list* actual_list = Art_split(actual);
    Zinc_string_list* expected_list = Art_split(expected);

    size_t actual_count = Zinc_string_list_count(actual_list);
    size_t expected_count = Zinc_string_list_count(expected_list);
    size_t count = 0;
    if (actual_count > expected_count) {
        count = actual_count;
    } else {
        count = expected_count;
    }

    Art_pair pair;
    Art_pair_init(&pair);

    Zinc_string_list actual_diff_list;
    Zinc_string_list_init(&actual_diff_list);
    Zinc_string_list expected_diff_list;
    Zinc_string_list_init(&expected_diff_list);

    for (size_t i = 0; i < count; i++) {

        if (i < actual_count && i < expected_count) {
            Zinc_string* actual_line = Zinc_string_list_get(actual_list, i);
            Zinc_string* expected_line = Zinc_string_list_get(expected_list, i);

            Zinc_string actual_line2;
            Zinc_string_init(&actual_line2);
            Zinc_string expected_line2;
            Zinc_string_init(&expected_line2);

            if (Art_diff_value(regex_re, actual_line, expected_line)) {
                Zinc_string_add_str(&actual_line2, "s ");
                Zinc_string_add_str(&expected_line2, "s ");
            } else {
                pair.matched = false;
                Zinc_string_add_str(&actual_line2, "d ");
                Zinc_string_add_str(&expected_line2, "d ");
            }

            Zinc_string_add_string(&actual_line2, actual_line);
            Zinc_string_list_add_bf(&actual_diff_list, &actual_line2);

            Zinc_string_add_string(&expected_line2, expected_line);
            Zinc_string_list_add_bf(&expected_diff_list, &expected_line2);

            Zinc_string_destroy(&actual_line2);
            Zinc_string_destroy(&expected_line2);
        } else if (i < actual_count) {
            pair.matched = false;
            Zinc_string* actual_line = Zinc_string_list_get(actual_list, i);
            Zinc_string actual_line2;
            Zinc_string_init(&actual_line2);
            Zinc_string_add_str(&actual_line2, "d ");
            Zinc_string_add_string(&actual_line2, actual_line);
            Zinc_string_list_add_bf(&actual_diff_list, &actual_line2);
            Zinc_string_destroy(&actual_line2);
        } else if (i < expected_count) {
            pair.matched = false;
            Zinc_string* expected_line = Zinc_string_list_get(expected_list, i);
            Zinc_string expected_line2;
            Zinc_string_init(&expected_line2);
            Zinc_string_add_str(&expected_line2, "d ");
            Zinc_string_add_string(&expected_line2, expected_line);
            Zinc_string_list_add_bf(&expected_diff_list, &expected_line2);
            Zinc_string_destroy(&expected_line2);
        }
    }

    Zinc_string* actual_diff = Art_join(&actual_diff_list);
    Zinc_string* expected_diff = Art_join(&expected_diff_list);

    pair.actual = actual_diff;
    pair.expected = expected_diff;

    Zinc_string_list_destroy(&expected_diff_list);
    Zinc_string_list_destroy(&actual_diff_list);

    Zinc_string_list_destroy(actual_list);
    free(actual_list);
    Zinc_string_list_destroy(expected_list);
    free(expected_list);

    return pair;
}

bool Art_diff_value(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected)
{
    Zinc_string_slice expected_slice = {expected->buf, expected->size};
    Cob_result regex_mr = Cob_match(&regex_re, expected_slice);
    if (regex_mr.matched) {
        Zinc_string* expected_inner = Zinc_string_list_get(&regex_mr.groups, 1);
        Cob_re expected_re = Cob_compile(expected_inner);
        if (expected_re.errors->head) {
            Zinc_error* e = expected_re.errors->head;
            while (e) {
                Zinc_string_finish(&e->message);
                fprintf(stderr, "(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
                e = e->next;
            }
            return false;
        }
        Zinc_string_slice actual_slice = {actual->buf, actual->size};
        Cob_result expected_mr = Cob_match(&expected_re, actual_slice);
        bool matched = expected_mr.matched;
        Cob_re_destroy(&expected_re);
        Cob_result_destroy(&regex_mr);
        Cob_result_destroy(&expected_mr);
        return matched;
    }

    Cob_result_destroy(&regex_mr);
    return Zinc_string_compare(actual, expected);
}

Zinc_string_list* Art_split(Zinc_string* string)
{
    Zinc_string_list* list = NULL;
    Zinc_string_list_create(&list);

    size_t pos = 0;
    bool done = false;
    while (!done) {
        Zinc_string line;
        Zinc_string_init(&line);
        while (!done) {
            char c[5];
            int num;
            const Zinc_result r = Zinc_string_next(string, &pos, c, &num, &done);
            if (r == Zinc_result_error) {
                fprintf(stderr, "run split error: %s\n", Zinc_error_message);
                return list;
            }

            if (done) {
                break;
            }

            Zinc_string_add(&line, c, num);

            if (num == 1 && c[0] == '\n') {
                Zinc_string_list_add_bf(list, &line);
                break;
            }
        }
        Zinc_string_destroy(&line);
    }

    return list;
}

Zinc_string* Art_join(Zinc_string_list* list)
{
    Zinc_string* result = NULL;
    Zinc_string_create(&result);

    Zinc_string_node* p = list->head;
    while (p) {
        Zinc_string_add_string(result, &p->value);
        p = p->next;
    }

    return result;
}

void Art_print_akela(Zinc_string* ake)
{
    fprintf(stderr, "Source:\n");
    Zinc_string_finish(ake);
    fprintf(stderr, "%s\n", ake->buf);
}

void Art_print_llvm(Art_pair* pair)
{
    if (!pair->matched) {
        fprintf(stderr, "llvm is different.\n");
        fprintf(stderr, "Actual:\n");
        Zinc_string_finish(pair->actual);
        fprintf(stderr, "%s\n", pair->actual->buf);
        fprintf(stderr, "Expected:\n");
        Zinc_string_finish(pair->expected);
        fprintf(stderr, "%s\n", pair->expected->buf);
    }
}

void Art_print_result(Art_pair* pair)
{
    if (!pair->matched) {
        fprintf(stderr, "result different.\n");
        fprintf(stderr, "Actual:\n");
        Zinc_string_finish(pair->actual);
        fprintf(stderr, "%s\n", pair->actual->buf);
        fprintf(stderr, "Expected:\n");
        Zinc_string_finish(pair->expected);
        fprintf(stderr, "%s\n", pair->expected->buf);
    }
}