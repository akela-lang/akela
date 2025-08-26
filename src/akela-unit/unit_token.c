#include "akela/token.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_TokenListAdd(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        return;
    }

    Ake_token_list token_list;
    Ake_token_list_init(&token_list);

    Ake_token* t0 = NULL;
    Ake_token_create(&t0);
    Ake_token_list_add(&token_list, t0);

    Ake_token* t1 = NULL;
    Ake_token_create(&t1);
    Ake_token_list_add(&token_list, t1);

    Ake_token* t2 = NULL;
    Ake_token_create(&t2);
    Ake_token_list_add(&token_list, t2);

    Zinc_expect_ptr_equal(test, token_list.head, t0, "head token_list");
    Zinc_expect_ptr_equal(test, t0->prev, NULL, "prev t0");
    Zinc_expect_ptr_equal(test, t0->next, t1, "next t0");
    Zinc_expect_ptr_equal(test, t1->prev, t0, "prev t1");
    Zinc_expect_ptr_equal(test, t1->next, t2, "next t1");
    Zinc_expect_ptr_equal(test, t2->prev, t1, "prev t2");
    Zinc_expect_ptr_equal(test, t2->next, NULL, "next t2");
    Zinc_expect_ptr_equal(test, token_list.tail, t2, "tail token_list");

    Ake_token_list_destroy(&token_list);
}

void AkeUnit_Token(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);

        Zinc_test_register(test, AkeUnit_TokenListAdd);

        return;
    }

    Zinc_test_perform(test);
}