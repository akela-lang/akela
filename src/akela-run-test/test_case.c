#include "data.h"
#include "zinc/zstring.h"
#include "zinc/input_unicode_string.h"
#include "akela/comp_unit.h"
#include "akela-llvm/cg.h"
#include "cobble/compile.h"
#include "cobble/match.h"

void Run_akela(Run_data* data, Run_test* test);
Run_pair Run_diff(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected);
bool Run_diff_value(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected);
Zinc_string_list* Run_split(Zinc_string* string);
Zinc_string* Run_join(Zinc_string_list* list);
void Run_print_akela(Zinc_string* ake);
void Run_print_llvm(Run_pair* pair);
void Run_print_result(Run_pair* pair);
void Run_print_results(Run_data* data);

void Run_test_cases(Run_data* data)
{
    Run_test* test = data->tests.head;
    while (test) {
        bool can_run = !test->mute
            && ((data->has_solo && test->solo) || !data->has_solo);
        if (can_run) {
            Run_akela(data, test);
        }
        test = test->next;
    }
}

void Run_akela(Run_data* data, Run_test* test)
{
    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, test->ake.buf, test->ake.size);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

    Ake_comp_unit* cu = NULL;
    Ake_comp_unit_create(&cu);
    Ake_comp_unit_compile(cu, input, input->input_vtable);

    data->test_count++;

    bool passed = true;

    if (!cu->valid) {
        /* is parsing valid */
        Zinc_error* e = cu->el.head;
        while (e) {
            Zinc_string_finish(&e->message);
            fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
        data->test_failed_count++;
        passed = false;
    } else {
        /* run program on jit */
        Akela_llvm_cg* cg = NULL;
        Akela_llvm_cg_create(&cg, &cu->el, &cu->extern_list);
        Ake_code_gen_result cg_result;
        Ake_code_gen_result_init(&cg_result);
        Ake_code_gen_jit(cg, &Akela_llvm_vtable, cu->root, &cg_result);

        /* check llvm output */
        Run_pair llvm_pair = Run_diff(data->regex_re, &cg_result.module_text, &test->llvm);
        if (!llvm_pair.matched) {
            passed = false;
        }

        if (cu->el.head) {
            /* any other errors */
            Zinc_error* e = cu->el.head;
            while (e) {
                Zinc_string_finish(&e->message);
                fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
                e = e->next;
            }
            passed = false;
        }

        if (!llvm_pair.matched) {
            Run_print_llvm(&llvm_pair);
        }

        Ake_code_gen_result_destroy(&cg_result);

    }

    if (passed) {
        data->test_passed_count++;
    } else {
        data->test_failed_count++;
    }

    Ake_comp_unit_destroy(cu);
    Zinc_vector_destroy(text);
    free(text);
    free(input);
}

Run_pair Run_diff(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected)
{
    Zinc_string_list* actual_list = Run_split(actual);
    Zinc_string_list* expected_list = Run_split(expected);

    size_t actual_count = Zinc_string_list_count(actual_list);
    size_t expected_count = Zinc_string_list_count(expected_list);
    size_t count = 0;
    if (actual_count > expected_count) {
        count = actual_count;
    } else {
        count = expected_count;
    }

    Run_pair pair;
    Run_pair_init(&pair);

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

            if (Run_diff_value(regex_re, actual_line, expected_line)) {
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

    Zinc_string* actual_diff = Run_join(&actual_diff_list);
    Zinc_string* expected_diff = Run_join(&expected_diff_list);

    pair.actual = actual_diff;
    pair.expected = expected_diff;
    return pair;
}

bool Run_diff_value(Cob_re regex_re, Zinc_string* actual, Zinc_string* expected)
{
    Zinc_string_slice expected_slice = {expected->buf, expected->size};
    Cob_result regex_mr = Cob_match(&regex_re, expected_slice);
    if (regex_mr.matched) {
        Zinc_string* expected_inner = Zinc_string_list_get(&regex_mr.groups, 1);
        Cob_re expected_re = Cob_compile(expected_inner);
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

Zinc_string_list* Run_split(Zinc_string* string)
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

Zinc_string* Run_join(Zinc_string_list* list)
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

void Run_compare_result(Zinc_string* result, Cent_value* config)
{

}

void Run_print_akela(Zinc_string* ake)
{
    fprintf(stderr, "Source:\n");
    Zinc_string_finish(ake);
    fprintf(stderr, "%s\n", ake->buf);
}

void Run_print_llvm(Run_pair* pair)
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

void Run_print_result(Run_pair* pair)
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

void Run_print_results(Run_data* data)
{
    printf("test case count: %zu\n", data->test_count);
    printf("passed count: %zu\n", data->test_passed_count);
    printf("failed count: %zu\n", data->test_failed_count);
}