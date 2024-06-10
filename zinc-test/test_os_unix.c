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
    test_name(__func__);
    FILE* fp;
    struct buffer name;
    buffer_init(&name);

    enum result r = get_temp_file(&fp, &name);
    assert_ok(r, "get_temp_file");

    const char* in = "hello";
    size_t count = strlen(in);
    size_t n = fwrite(in, 1, count, fp);
    expect_size_t_equal(n, count, "fwrite");

    r = close_temp_file(fp);
    assert_ok(r, "close_temp_file");

    fp = fopen(name.buf, "r");
    assert_ptr(fp, "ptr fp");

    char* out = NULL;
    malloc_safe((void**)&out, count + 1);
    memset(out, 0, count + 1);
    n = fread(out, 1, count, fp);

    expect_size_t_equal(n, count, "fread");

    r = close_temp_file(fp);
    assert_ok(r, "close_temp_file 2");

    expect_true(strcmp(in, out) == 0, "strcmp");

    r = delete_temp_file(&name);
    assert_ok(r, "delete_temp_file");

    free(out);

    buffer_destroy(&name);
}

void test_os_unix_get_user_home_directory()
{
    test_name(__func__);

    const char* temp = getenv("HOME");
    setenv("HOME", "/home/abc", 1);

    struct buffer dir;
    buffer_init(&dir);
    enum result r = get_user_home_directory(&dir);
    assert_ok(r, "get_user_home_directory");
    expect_str(&dir, "/home/abc", "/home/abc");
    buffer_destroy(&dir);

    setenv("HOME", temp, 1);
}

void test_os_unix_path_join()
{
    test_name(__func__);

    struct buffer a;
    struct buffer b;
    struct buffer c;

    buffer_init(&a);
    buffer_init(&b);
    buffer_init(&c);

    buffer_copy_str(&a, "/home");
    buffer_copy_str(&b, "alf");

    path_join(&a, &b, &c);

    expect_str(&c, "/home/alf", "/home/alf");

    buffer_destroy(&a);
    buffer_destroy(&b);
    buffer_destroy(&c);
}

void test_os_unix_get_user_app_directory()
{
    test_name(__func__);

    struct buffer app_name;
    buffer_init(&app_name);
    buffer_copy_str(&app_name, "stone");

    struct buffer dir;
    buffer_init(&dir);

    const char* temp = getenv("HOME");
    setenv("HOME", "/home/abc", 1);
    get_user_app_directory(&app_name, &dir);
    expect_str(&dir, "/home/abc/.app/stone", "/home/abc/.app/stone");

    buffer_destroy(&app_name);
    buffer_destroy(&dir);

    setenv("HOME", temp, 1);
}

void test_os_unix_make_directory()
{
    test_name(__func__);

    struct buffer dir;
    buffer_init(&dir);
    buffer_copy_str(&dir, "/tmp/apple/bear/creek/doe/eddy");
    buffer_finish(&dir);
    enum result r = make_directory(&dir);
    assert_ok(r, "make directory");
    DIR* dp = opendir(dir.buf);
    assert_ptr(dp, "ptr dp");
    closedir(dp);
    buffer_destroy(&dir);
    system("cd /tmp && rmdir -p apple/bear/creek/doe/eddy");
}

void test_os_unix_delete_directory()
{
    test_name(__func__);

    system("mkdir -p /tmp/one/two");
    system("touch /tmp/one/two/file");

    struct buffer dir;
    buffer_init(&dir);
    buffer_copy_str(&dir, "/tmp/one");
    enum result r = delete_directory(&dir);
    assert_ok(r, "delete_directory");
    DIR* dp = opendir("/tmp/one");
    assert_null(dp, "null dp");
    if (dp) {
        closedir(dp);
    }
    buffer_destroy(&dir);
}

void test_os_unix_file_exists()
{
    test_name(__func__);

    struct buffer filename;
    buffer_init(&filename);
    buffer_copy_str(&filename, "/tmp/test_os_unix_file_exits");
    buffer_finish(&filename);

    system("touch /tmp/test_os_unix_file_exits");
    expect_true(file_exists(&filename), "file exists true");

    system("rm /tmp/test_os_unix_file_exits");
    expect_false(file_exists(&filename), "file exits false");

    buffer_destroy(&filename);
}

void test_os_unix_get_dir_files()
{
    test_name(__func__);

    system("mkdir -p /tmp/one/two");
    system("touch /tmp/one/file1");
    system("touch /tmp/one/file2");
    system("touch /tmp/one/two/file3");
    system("touch /tmp/one/two/file4");

    struct buffer dir;
    buffer_init(&dir);
    buffer_copy_str(&dir, "/tmp/one");

    struct buffer_list bl;
    buffer_list_init(&bl);

    enum result r = get_dir_files(&dir, &bl);
    expect_ok(r, "get_dir_files");

    bool seen_file1 = false;
    bool seen_file2 = false;
    bool seen_file3 = false;
    bool seen_file4 = false;
    struct buffer_node* bn = bl.head;
    while (bn) {
        if (buffer_compare_str(&bn->value, "/tmp/one/file1"))
            seen_file1 = true;
        if (buffer_compare_str(&bn->value, "/tmp/one/file2"))
            seen_file2 = true;
        if (buffer_compare_str(&bn->value, "/tmp/one/two/file3"))
            seen_file3 = true;
        if (buffer_compare_str(&bn->value, "/tmp/one/two/file4"))
            seen_file4 = true;
        bn = bn->next;
    }

    expect_true(seen_file1, "seen_file1");
    expect_true(seen_file2, "seen_file2");
    expect_true(seen_file3, "seen_file3");
    expect_true(seen_file4, "seen_file4");

    system("rm -rf /tmp/one");
    buffer_destroy(&dir);
    buffer_list_destroy(&bl);
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