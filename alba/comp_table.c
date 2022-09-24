#include "comp_table.h"
#include "hash.h"
#include "comp_unit.h"
#include "zinc/memory.h"
#include "alba/os_win.h"
#include <stdio.h>

void comp_table_init(struct comp_table* ct)
{
	hash_table_init(&ct->ht, COMP_TABLE_HASH_TABLE_SIZE);
}

void comp_table_put(struct comp_table* ct, struct buffer* path, struct comp_unit* cu)
{
	hash_table_add(&ct->ht, path, (void*)cu);
}

struct comp_unit* comp_table_get(struct comp_table* ct, struct buffer* path)
{
	return (struct comp_unit*)hash_table_get(&ct->ht, path);
}

void comp_table_destroy_comp_unit(struct comp_unit* cu)
{
	comp_unit_destroy(cu);
	free(cu);
}

void comp_table_destroy(struct comp_table* ct)
{
	hash_table_map(&ct->ht, (void (*)(void*))comp_table_destroy_comp_unit);
	hash_table_destroy(&ct->ht);
}

bool include_base(struct comp_table* ct, struct comp_unit* cu, struct comp_unit** cu_base)
{
	bool valid = true;
	char* path = NULL;
	path = get_exe_path();
	struct location loc;
	location_init(&loc);

	if (!path) {
		valid = set_source_error(&cu->el, &loc, "could not get executable path");
		return valid;
	}

	struct buffer path2;
	struct buffer dir;
	struct buffer filename;
	buffer_init(&path2);
	buffer_init(&dir);
	buffer_init(&filename);

	buffer_copy_str(&path2, path);
	split_path(&path2, &dir, &filename);

	struct buffer math_path;
	buffer_init(&math_path);
	buffer_copy(&dir, &math_path);
	buffer_clear(&filename);
	buffer_copy_str(&filename, "math.alba");
	path_join(&math_path, &filename);
	buffer_finish(&math_path);

	FILE* fp = NULL;
	int err = fopen_s(&fp, math_path.buf, "r");
	if (err || !fp) {
		valid = set_source_error(&cu->el, &loc, "could not open file: %s\n", math_path.buf);
		goto exit;
	}

	malloc_safe((void**)cu_base, sizeof(struct comp_unit));
	comp_unit_init(*cu_base);
	comp_table_put(ct, &math_path, *cu_base);

	comp_unit_compile(*cu_base, (input_getchar)file_getchar, (input_data)fp);

	transfer_global_symbols(&(*cu_base)->st, &cu->st);

exit:
	free(path);
	buffer_destroy(&path2);
	buffer_destroy(&dir);
	buffer_destroy(&filename);
	buffer_destroy(&math_path);
	if (fp) fclose(fp);
	return valid;
}
