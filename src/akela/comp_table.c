#include "comp_table.h"
#include "comp_unit.h"
#include "zinc/memory.h"
#include "zinc/os_unix.h"
#include <stdio.h>
#include "zinc/input_unicode_file.h"
#include "zinc/os_win.h"
#include "zinc/fs.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void Ake_comp_table_init_str(Ake_comp_table* ct, char* s)
{
	Ake_comp_table_init(ct);

	Ake_comp_unit* cu = NULL;
	Ake_comp_unit_create(&cu);

	Zinc_vector* v = NULL;
	Zinc_vector_create(&v, sizeof(char));
	Zinc_vector_add(v, s, strlen(s));

	Zinc_input_unicode_string* input = NULL;
	Zinc_input_unicode_string_create(&input, v);

	Ake_lex_state* ls = NULL;
	Ake_lex_state_create(&ls, input, input->vtable, &cu->errors, &cu->st);

	Ake_parse_state* ps = NULL;
	Ake_parse_state_create(&ps, ls, &cu->errors, &cu->extern_list, &cu->st);

	cu->ls = ls;
	cu->ps = ps;
	ct->primary = cu;

	Zinc_string path;
	Zinc_string_init(&path);
	Zinc_string_add_str(&path, "**string**");

	Ake_comp_table_put(ct, &path, cu);
}

void Ake_comp_table_create_str(Ake_comp_table** ct, char* s)
{
	Zinc_malloc_safe((void**)ct, sizeof(Ake_comp_table));
	Ake_comp_table_init_str(*ct, s);
}

void Ake_comp_table_init(Ake_comp_table* ct)
{
	ct->primary = NULL;
	Zinc_hash_map_string_init(&ct->ht, COMP_TABLE_HASH_TABLE_SIZE);
}

void Ake_comp_table_put(Ake_comp_table* ct, Zinc_string* path, Ake_comp_unit* cu)
{
	Zinc_hash_map_string_add(&ct->ht, path, (void*)cu);
}

Ake_comp_unit* Ake_comp_table_get(Ake_comp_table* ct, Zinc_string* path)
{
	return Zinc_hash_map_string_get(&ct->ht, path);
}

void Ake_comp_table_destroy_comp_unit(Ake_comp_unit* cu)
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
        valid = Zinc_error_list_set(&cu->errors, &loc, "could not get executable path");
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
		valid = Zinc_error_list_set(&cu->errors, &loc, "could not open file: %s\n", math_path.buf);
		goto exit;
	}

    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

	Zinc_malloc_safe((void**)cu_base, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(*cu_base);
	Ake_comp_table_put(ct, &math_path, *cu_base);

	Ake_comp_unit_compile(*cu_base, input, input->vtable);

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
