#include "zinc/hash_map_size_t.h"
#include <zinc/unit_test.h>

void test_hash_map_size_t_add()
{
    test_name(__func__);

    Hash_map_size_t map;
    Hash_map_size_t_init(&map, 8);

    char d10[] = "ten";
    char d20[] = "twenty";
    char d30[] = "thirty";

    Hash_map_size_t_add(&map, 10, d10);
    Hash_map_size_t_add(&map, 20, d20);
    Hash_map_size_t_add(&map, 30, d30);

    char* got_d10 = Hash_map_size_t_get(&map, 10);
    char* got_d20 = Hash_map_size_t_get(&map, 20);
    char* got_d30 = Hash_map_size_t_get(&map, 30);

    expect_strcmp(got_d10, "ten", "got_d10");
    expect_strcmp(got_d20, "twenty", "got_d20");
    expect_strcmp(got_d30, "thirty", "got_d30");

    Hash_map_size_t_destroy(&map);
}

void test_hash_map_size_t_remove()
{
    test_name(__func__);

    Hash_map_size_t map;
    Hash_map_size_t_init(&map, 8);

    char d10[] = "ten";
    char d20[] = "twenty";
    char d30[] = "thirty";

    Hash_map_size_t_add(&map, 10, d10);
    Hash_map_size_t_add(&map, 20, d20);
    Hash_map_size_t_add(&map, 30, d30);

    char* got_d20 = Hash_map_size_t_get(&map, 20);
    expect_strcmp(got_d20, "twenty", "get");

    got_d20 = Hash_map_size_t_remove(&map, 20);
    expect_strcmp(got_d20, "twenty", "remove");

    got_d20 = Hash_map_size_t_get(&map, 20);
    expect_null(got_d20, "get 2");

    Hash_map_size_t_destroy(&map);
}

void test_hash_map_size_t()
{
    test_hash_map_size_t_add();
    test_hash_map_size_t_remove();
}
