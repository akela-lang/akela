#include "zinc/os.h"

#if (IS_UNIX)
#include "zinc/os_unix.h"
#include <string.h>
#include "zinc/memory.h"
#include <stdlib.h>
#include <dirent.h>
#include "zinc/fs.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_os_unix_get_temp_file(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    FILE* fp;
    Zinc_string name;
    Zinc_string_init(&name);

    Zinc_result r = Zinc_get_temp_file(&fp, &name);
    Zinc_test_assert_ok(test, r, "get_temp_file");

    const char* in = "hello";
    size_t count = strlen(in);
    size_t n = fwrite(in, 1, count, fp);
    Zinc_test_expect_size_t_equal(test, n, count, "fwrite");

    r = Zinc_close_temp_file(fp);
    Zinc_test_assert_ok(test, r, "close_temp_file");

    fp = fopen(name.buf, "r");
    Zinc_test_assert_ptr(test, fp, "ptr fp");

    char* out = NULL;
    Zinc_malloc_safe((void**)&out, count + 1);
    memset(out, 0, count + 1);
    n = fread(out, 1, count, fp);

    Zinc_test_expect_size_t_equal(test, n, count, "fread");

    r = Zinc_close_temp_file(fp);
    Zinc_test_assert_ok(test, r, "close_temp_file 2");

    Zinc_test_expect_true(test, strcmp(in, out) == 0, "strcmp");

    r = Zinc_delete_temp_file(&name);
    Zinc_test_assert_ok(test, r, "delete_temp_file");

    free(out);

    Zinc_string_destroy(&name);
}

void Zinc_unit_os_unix_get_user_home_directory(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    const char* temp = getenv("HOME");
    setenv("HOME", "/home/abc", 1);

    struct Zinc_string dir;
    Zinc_string_init(&dir);
    enum Zinc_result r = Zinc_get_user_home_directory(&dir);
    Zinc_test_assert_ok(test, r, "get_user_home_directory");
    Zinc_test_expect_string(test, &dir, "/home/abc", "/home/abc");
    Zinc_string_destroy(&dir);

    setenv("HOME", temp, 1);
}

void Zinc_unit_os_unix_path_join(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Zinc_string a;
    struct Zinc_string b;
    struct Zinc_string c;

    Zinc_string_init(&a);
    Zinc_string_init(&b);
    Zinc_string_init(&c);

    Zinc_string_add_str(&a, "/home");
    Zinc_string_add_str(&b, "alf");

    Zinc_path_join(&a, &b, &c);

    Zinc_test_expect_string(test, &c, "/home/alf", "/home/alf");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
    Zinc_string_destroy(&c);
}

void Zinc_unit_os_unix_get_user_app_directory(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Zinc_string app_name;
    Zinc_string_init(&app_name);
    Zinc_string_add_str(&app_name, "stone");

    struct Zinc_string dir;
    Zinc_string_init(&dir);

    const char* temp = getenv("HOME");
    setenv("HOME", "/home/abc", 1);
    Zinc_get_user_app_directory(&app_name, &dir);
    Zinc_test_expect_string(test, &dir, "/home/abc/.app/stone", "/home/abc/.app/stone");

    Zinc_string_destroy(&app_name);
    Zinc_string_destroy(&dir);

    setenv("HOME", temp, 1);
}

void Zinc_unit_os_unix_make_directory(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string_add_str(&dir, "/tmp/apple/bear/creek/doe/eddy");
    Zinc_string_finish(&dir);
    enum Zinc_result r = Zinc_make_directory(&dir);
    Zinc_test_assert_ok(test, r, "make directory");
    DIR* dp = opendir(dir.buf);
    Zinc_test_assert_ptr(test, dp, "ptr dp");
    closedir(dp);
    Zinc_string_destroy(&dir);
    system("cd /tmp && rmdir -p apple/bear/creek/doe/eddy");
}

void Zinc_unit_os_unix_delete_directory(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    system("mkdir -p /tmp/one/two");
    system("touch /tmp/one/two/file");

    struct Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string_add_str(&dir, "/tmp/one");
    enum Zinc_result r = Zinc_delete_directory(&dir);
    Zinc_test_assert_ok(test, r, "delete_directory");
    DIR* dp = opendir("/tmp/one");
    Zinc_test_assert_null(test, dp, "null dp");
    if (dp) {
        closedir(dp);
    }
    Zinc_string_destroy(&dir);
}

void Zinc_unit_os_unix_file_exists(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string filename;
    Zinc_string_init(&filename);
    Zinc_string_add_str(&filename, "/tmp/test_os_unix_file_exits");
    Zinc_string_finish(&filename);

    system("touch /tmp/test_os_unix_file_exits");
    Zinc_test_expect_true(test, Zinc_file_exists(Zinc_string_c_str(&filename)), "file exists true");

    system("rm /tmp/test_os_unix_file_exits");
    Zinc_test_expect_false(test, Zinc_file_exists(Zinc_string_c_str(&filename)), "file exits false");

    Zinc_string_destroy(&filename);
}

void Zinc_unit_os_unix_get_dir_files(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    system("mkdir -p /tmp/one/two");
    system("touch /tmp/one/file1");
    system("touch /tmp/one/file2");
    system("touch /tmp/one/two/file3");
    system("touch /tmp/one/two/file4");

    Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string_add_str(&dir, "/tmp/one");

    Zinc_string_list bl;
    Zinc_string_list_init(&bl);

    Zinc_result r = Zinc_get_dir_files(&dir, &bl);
    Zinc_test_expect_ok(test, r, "get_dir_files");

    bool seen_file1 = false;
    bool seen_file2 = false;
    bool seen_file3 = false;
    bool seen_file4 = false;
    Zinc_string_node* bn = bl.head;
    while (bn) {
        if (Zinc_string_compare_str(&bn->value, "/tmp/one/file1"))
            seen_file1 = true;
        if (Zinc_string_compare_str(&bn->value, "/tmp/one/file2"))
            seen_file2 = true;
        if (Zinc_string_compare_str(&bn->value, "/tmp/one/two/file3"))
            seen_file3 = true;
        if (Zinc_string_compare_str(&bn->value, "/tmp/one/two/file4"))
            seen_file4 = true;
        bn = bn->next;
    }

    Zinc_test_expect_true(test, seen_file1, "seen_file1");
    Zinc_test_expect_true(test, seen_file2, "seen_file2");
    Zinc_test_expect_true(test, seen_file3, "seen_file3");
    Zinc_test_expect_true(test, seen_file4, "seen_file4");

    system("rm -rf /tmp/one");
    Zinc_string_destroy(&dir);
    Zinc_string_list_destroy(&bl);
}
#endif

void Zinc_unit_os_unix(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

#if IS_UNIX
        Zinc_test_register(test, Zinc_unit_os_unix_get_temp_file);
        Zinc_test_register(test, Zinc_unit_os_unix_get_user_home_directory);
        Zinc_test_register(test, Zinc_unit_os_unix_path_join);
        Zinc_test_register(test, Zinc_unit_os_unix_get_user_app_directory);
        Zinc_test_register(test, Zinc_unit_os_unix_make_directory);
        Zinc_test_register(test, Zinc_unit_os_unix_delete_directory);
        Zinc_test_register(test, Zinc_unit_os_unix_file_exists);
        Zinc_test_register(test, Zinc_unit_os_unix_get_dir_files);
#endif

        return;
    }

    Zinc_test_perform(test);
}