#include "comp_table.h"
#include "comp_unit.h"
#include "zinc/memory.h"
#include "zinc/os_unix.h"
#include <stdio.h>
#include "zinc/input_unicode_file.h"

void Ake_comp_table_init(struct Ake_comp_table* ct)
{
	Zinc_hash_map_string_init(&ct->ht, COMP_TABLE_HASH_TABLE_SIZE);
}

void Ake_comp_table_put(struct Ake_comp_table* ct, struct Zinc_string* path, struct Ake_comp_unit* cu)
{
	Zinc_hash_map_string_add(&ct->ht, path, (void*)cu);
}

struct Ake_comp_unit* Ake_comp_table_get(struct Ake_comp_table* ct, struct Zinc_string* path)
{
	return (struct Ake_comp_unit*)Zinc_hash_map_string_get(&ct->ht, path);
}

void Ake_comp_table_destroy_comp_unit(struct Ake_comp_unit* cu)
{
	Ake_comp_unit_destroy(cu);
	free(cu);
}

void Ake_comp_table_destroy(struct Ake_comp_table* ct)
{
	Zinc_hash_map_string_map(&ct->ht, (void (*)(void*))Ake_comp_table_destroy_comp_unit);
	Zinc_hash_map_string_destroy(&ct->ht);
}

bool Ake_include_base(struct Ake_comp_table* ct, struct Ake_comp_unit* cu, struct Ake_comp_unit** cu_base)
{
	bool valid = true;
    enum Zinc_result r;
	char* path = NULL;

    struct Zinc_location loc;
    Zinc_location_init(&loc);

	r = Zinc_get_exe_path(&path);
    if (r == Zinc_result_error) {
        valid = Zinc_error_list_set(&cu->el, &loc, "could not get executable path");
    }

	struct Zinc_string path2;
	struct Zinc_string dir;
	struct Zinc_string filename;
	Zinc_string_init(&path2);
	Zinc_string_init(&dir);
	Zinc_string_init(&filename);

	Zinc_string_add_str(&path2, path);
	Zinc_split_path(&path2, &dir, &filename);

	struct Zinc_string math_dir;
	Zinc_string_init(&math_dir);
	Zinc_string_copy(&dir, &math_dir);
	Zinc_string_clear(&filename);
	Zinc_string_add_str(&filename, "math.ake");
    struct Zinc_string math_path;
    Zinc_string_init(&math_path);
	Zinc_path_join(&math_dir, &filename, &math_path);
	Zinc_string_finish(&math_path);

	FILE* fp = NULL;
	int err = Zinc_fopen_s(&fp, math_path.buf, "r");
	if (err || !fp) {
		valid = Zinc_error_list_set(&cu->el, &loc, "could not open file: %s\n", math_path.buf);
		goto exit;
	}

    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

	Zinc_malloc_safe((void**)cu_base, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(*cu_base);
	Ake_comp_table_put(ct, &math_path, *cu_base);

	Ake_comp_unit_compile(*cu_base, input, input->input_vtable);

	Ake_transfer_global_symbols(&(*cu_base)->st, &cu->st);

    free(input);

exit:
	free(path);
	Zinc_string_destroy(&path2);
	Zinc_string_destroy(&dir);
	Zinc_string_destroy(&filename);
	Zinc_string_destroy(&math_path);
    Zinc_string_destroy(&math_dir);
	if (fp) fclose(fp);
	return valid;
}
