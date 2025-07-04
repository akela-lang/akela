#include "akela/type_list.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_TypeListAdd(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_TypeList* tl = NULL;
    Ake_TypeListCreate(&tl);

    Ake_TypeNode* n0 = NULL;
    Ake_TypeNodeCreate(&n0, NULL);

    Ake_TypeNode* n1 = NULL;
    Ake_TypeNodeCreate(&n1, NULL);

    Ake_TypeNode* n2 = NULL;
    Ake_TypeNodeCreate(&n2, NULL);

    Ake_TypeListAdd(tl, n0);
    Ake_TypeListAdd(tl, n1);
    Ake_TypeListAdd(tl, n2);

    Zinc_expect_ptr_equal(test, tl->head, n0, "head tl");
    Zinc_expect_ptr_equal(test, n0->prev, NULL, "prev n0");
    Zinc_expect_ptr_equal(test, n0->next, n1, "next n0");
    Zinc_expect_ptr_equal(test, n1->prev, n0, "prev n1");
    Zinc_expect_ptr_equal(test, n1->next, n2, "next n1");
    Zinc_expect_ptr_equal(test, n2->prev, n1, "prev n2");
    Zinc_expect_ptr_equal(test, n2->next, NULL, "next n2");
    Zinc_expect_ptr_equal(test, tl->tail, n2, "tail tl");

    Ake_TypeListDestroy(tl);
    free(tl);
}

void AkeUnit_TypeList(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_TypeListAdd);

        return;
    }

    Zinc_test_perform(test);
}