#include "zinc/unit_test.h"
#include "lava/section.h"

void unit_section_add_item()
{
    Zinc_test_name(__func__);

    Lava_section section;
    Lava_section_init(&section);

    Lava_item* item0 = NULL;
    Lava_item_create(&item0);

    Lava_item* item1 = NULL;
    Lava_item_create(&item1);

    Lava_item* item2 = NULL;
    Lava_item_create(&item2);

    Lava_item_list_add(&section.items, item0);
    Lava_item_list_add(&section.items, item1);
    Lava_item_list_add(&section.items, item2);

    Zinc_expect_ptr_equal(section.items.head, item0, "head items");
    Zinc_expect_ptr_equal(item0->prev, NULL, "prev item0");
    Zinc_expect_ptr_equal(item0->next, item1, "next item0");
    Zinc_expect_ptr_equal(item1->prev, item0, "prev item1");
    Zinc_expect_ptr_equal(item1->next, item2, "next item1");
    Zinc_expect_ptr_equal(item2->prev, item1, "prev item2");
    Zinc_expect_ptr_equal(item2->next, NULL, "next item2");
    Zinc_expect_ptr_equal(section.items.tail, item2, "tail items");

    Lava_section_destroy(&section);
}

void unit_section()
{
    unit_section_add_item();
}