#include "zinc/hash_map_size_t.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_hash_map_size_t_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_hash_map_size_t map;
    Zinc_hash_map_size_t_init(&map, 8);

    char d10[] = "ten";
    char d20[] = "twenty";
    char d30[] = "thirty";

    Zinc_hash_map_size_t_add(&map, 10, d10);
    Zinc_hash_map_size_t_add(&map, 20, d20);
    Zinc_hash_map_size_t_add(&map, 30, d30);

    char* got_d10 = Zinc_hash_map_size_t_get(&map, 10);
    char* got_d20 = Zinc_hash_map_size_t_get(&map, 20);
    char* got_d30 = Zinc_hash_map_size_t_get(&map, 30);

    Zinc_test_expect_strcmp(test, got_d10, "ten", "got_d10");
    Zinc_test_expect_strcmp(test, got_d20, "twenty", "got_d20");
    Zinc_test_expect_strcmp(test, got_d30, "thirty", "got_d30");

    Zinc_hash_map_size_t_destroy(&map);
}

void Zinc_unit_hash_map_size_t_remove(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_hash_map_size_t map;
    Zinc_hash_map_size_t_init(&map, 8);

    char d10[] = "ten";
    char d20[] = "twenty";
    char d30[] = "thirty";

    Zinc_hash_map_size_t_add(&map, 10, d10);
    Zinc_hash_map_size_t_add(&map, 20, d20);
    Zinc_hash_map_size_t_add(&map, 30, d30);

    char* got_d20 = Zinc_hash_map_size_t_get(&map, 20);
    Zinc_test_expect_strcmp(test, got_d20, "twenty", "get");

    got_d20 = Zinc_hash_map_size_t_remove(&map, 20);
    Zinc_test_expect_strcmp(test, got_d20, "twenty", "remove");

    got_d20 = Zinc_hash_map_size_t_get(&map, 20);
    Zinc_test_expect_null(test, got_d20, "get 2");

    Zinc_hash_map_size_t_destroy(&map);
}

void Zinc_unit_hash_map_size_t(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_hash_map_size_t_add);
        Zinc_test_register(test, Zinc_unit_hash_map_size_t_remove);

        return;
    }

    Zinc_test_perform(test);
}
