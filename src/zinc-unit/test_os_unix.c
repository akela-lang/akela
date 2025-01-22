#include "zinc/unit_test.h"
#include "zinc/os_unix.h"
#include <string.h>
#include "zinc/memory.h"
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void test_os_unix_get_temp_file()
{
    Zinc_test_name(__func__);
    FILE* fp;
    struct Zinc_string name;
    Zinc_string_init(&name);

    enum Zinc_result r = Zinc_get_temp_file(&fp, &name);
    Zinc_assert_ok(r, "get_temp_file");

    const char* in = "hello";
    size_t count = strlen(in);
    size_t n = fwrite(in, 1, count, fp);
    Zinc_expect_size_t_equal(n, count, "fwrite");

    r = Zinc_close_temp_file(fp);
    Zinc_assert_ok(r, "close_temp_file");

    fp = fopen(name.buf, "r");
    Zinc_assert_ptr(fp, "ptr fp");

    char* out = NULL;
    Zinc_malloc_safe((void**)&out, count + 1);
    memset(out, 0, count + 1);
    n = fread(out, 1, count, fp);

    Zinc_expect_size_t_equal(n, count, "fread");

    r = Zinc_close_temp_file(fp);
    Zinc_assert_ok(r, "close_temp_file 2");

    Zinc_expect_true(strcmp(in, out) == 0, "strcmp");

    r = Zinc_delete_temp_file(&name);
    Zinc_assert_ok(r, "delete_temp_file");

    free(out);

    Zinc_string_destroy(&name);
}

void test_os_unix_get_user_home_directory()
{
    Zinc_test_name(__func__);

    const char* temp = getenv("HOME");
    setenv("HOME", "/home/abc", 1);

    struct Zinc_string dir;
    Zinc_string_init(&dir);
    enum Zinc_result r = Zinc_get_user_home_directory(&dir);
    Zinc_assert_ok(r, "get_user_home_directory");
    Zinc_expect_string(&dir, "/home/abc", "/home/abc");
    Zinc_string_destroy(&dir);

    setenv("HOME", temp, 1);
}

void test_os_unix_path_join()
{
    Zinc_test_name(__func__);

    struct Zinc_string a;
    struct Zinc_string b;
    struct Zinc_string c;

    Zinc_string_init(&a);
    Zinc_string_init(&b);
    Zinc_string_init(&c);

    Zinc_string_add_str(&a, "/home");
    Zinc_string_add_str(&b, "alf");

    Zinc_path_join(&a, &b, &c);

    Zinc_expect_string(&c, "/home/alf", "/home/alf");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
    Zinc_string_destroy(&c);
}

void test_os_unix_get_user_app_directory()
{
    Zinc_test_name(__func__);

    struct Zinc_string app_name;
    Zinc_string_init(&app_name);
    Zinc_string_add_str(&app_name, "stone");

    struct Zinc_string dir;
    Zinc_string_init(&dir);

    const char* temp = getenv("HOME");
    setenv("HOME", "/home/abc", 1);
    Zinc_get_user_app_directory(&app_name, &dir);
    Zinc_expect_string(&dir, "/home/abc/.app/stone", "/home/abc/.app/stone");

    Zinc_string_destroy(&app_name);
    Zinc_string_destroy(&dir);

    setenv("HOME", temp, 1);
}

void test_os_unix_make_directory()
{
    Zinc_test_name(__func__);

    struct Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string_add_str(&dir, "/tmp/apple/bear/creek/doe/eddy");
    Zinc_string_finish(&dir);
    enum Zinc_result r = Zinc_make_directory(&dir);
    Zinc_assert_ok(r, "make directory");
    DIR* dp = opendir(dir.buf);
    Zinc_assert_ptr(dp, "ptr dp");
    closedir(dp);
    Zinc_string_destroy(&dir);
    system("cd /tmp && rmdir -p apple/bear/creek/doe/eddy");
}

void test_os_unix_delete_directory()
{
    Zinc_test_name(__func__);

    system("mkdir -p /tmp/one/two");
    system("touch /tmp/one/two/file");

    struct Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string_add_str(&dir, "/tmp/one");
    enum Zinc_result r = Zinc_delete_directory(&dir);
    Zinc_assert_ok(r, "delete_directory");
    DIR* dp = opendir("/tmp/one");
    Zinc_assert_null(dp, "null dp");
    if (dp) {
        closedir(dp);
    }
    Zinc_string_destroy(&dir);
}

void test_os_unix_file_exists()
{
    Zinc_test_name(__func__);

    struct Zinc_string filename;
    Zinc_string_init(&filename);
    Zinc_string_add_str(&filename, "/tmp/test_os_unix_file_exits");
    Zinc_string_finish(&filename);

    system("touch /tmp/test_os_unix_file_exits");
    Zinc_expect_true(Zinc_file_exists(&filename), "file exists true");

    system("rm /tmp/test_os_unix_file_exits");
    Zinc_expect_false(Zinc_file_exists(&filename), "file exits false");

    Zinc_string_destroy(&filename);
}

void test_os_unix_get_dir_files()
{
    Zinc_test_name(__func__);

    system("mkdir -p /tmp/one/two");
    system("touch /tmp/one/file1");
    system("touch /tmp/one/file2");
    system("touch /tmp/one/two/file3");
    system("touch /tmp/one/two/file4");

    struct Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string_add_str(&dir, "/tmp/one");

    struct Zinc_string_list bl;
    Zinc_string_list_init(&bl);

    enum Zinc_result r = Zinc_get_dir_files(&dir, &bl);
    Zinc_expect_ok(r, "get_dir_files");

    bool seen_file1 = false;
    bool seen_file2 = false;
    bool seen_file3 = false;
    bool seen_file4 = false;
    struct Zinc_string_node* bn = bl.head;
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

    Zinc_expect_true(seen_file1, "seen_file1");
    Zinc_expect_true(seen_file2, "seen_file2");
    Zinc_expect_true(seen_file3, "seen_file3");
    Zinc_expect_true(seen_file4, "seen_file4");

    system("rm -rf /tmp/one");
    Zinc_string_destroy(&dir);
    Zinc_string_list_destroy(&bl);
}

void test_os_unix()
{
    test_os_unix_get_temp_file();
    test_os_unix_get_user_home_directory();
    test_os_unix_path_join();
    test_os_unix_get_user_app_directory();
    test_os_unix_make_directory();
    test_os_unix_delete_directory();
    test_os_unix_file_exists();
    test_os_unix_get_dir_files();
}