#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "parse_tools.h"
#include "ast.h"
#include "source.h"
#include "symbol_table.h"
#include "zinc/memory.h"
#include "parse_types.h"

bool declaration(struct parse_state* ps, struct ast_node** root);
bool type(struct parse_state* ps, struct token* id, struct ast_node** root);

/* dseq -> declaration dseq' | e */
/* dseq' -> , declaration dseq' | e */
/* dynamic-output ps{} root root{} */
bool dseq(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate n */
	ast_create_node(&n);
	n->type = ast_type_dseq;

	/* transfer n -> root */
	*root = n;

	/* allocate a */
	struct ast_node* dec = NULL;
	valid = declaration(ps, &dec) && valid;

	if (!dec) {
		return valid;
	}

	if (dec && valid) {
		/* transfer dec -> n */
		ast_add_child(n, dec);
	}

	while (true)
	{
		/* allocate ps{} */
		int num;
		valid = get_lookahead(ps, 2, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);
		if (!t0 || t0->type != token_comma) {
			break;
		}

		/* allocate ps{} comma comma{} */
		struct token* comma = NULL;
		valid = match(ps, token_comma, "expecting comma", &comma) && valid;

		token_destroy(comma);
		free(comma);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* allocate a */
		struct ast_node* dec = NULL;
		valid = declaration(ps, &dec) && valid;
		
		if (!dec || !valid) {
			/* allocate ps{} */
			set_source_error(ps->el, &loc, "expecting declaration after comma");
			valid = false;
			break;
		}

		if (dec && valid) {
			/* transfer dec -> n */
			ast_add_child(n, dec);
		}
	}

	return valid;
}

/* declaration -> id :: type | id */
/* dynamic-output ps{} root root{} */
bool declaration(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_id) {
		/* id::type */

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		valid = match(ps, token_id, "expected id", &id) && valid;

		/* allocate ps{} dc dc{} */
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expected double colon", &dc) && valid;

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		struct ast_node* type_node = NULL;
		valid = type(ps, id, &type_node) && valid;

		if (!type_node) {
			valid = set_source_error(ps->el, &loc, "expected a type");
		}

		if (valid) {
			/* allocate n */
			ast_create_node(&n);
			n->type = ast_type_declaration;

			/* allocate a */
			struct ast_node* a = NULL;
			ast_create_node(&a);
			a->type = ast_type_id;

			/* allocate a{} */
			buffer_copy(&id->value, &a->value);

			/* transfer a a{} -> n */
			ast_add_child(n, a);

			/* transfer b b{} -> n */
			ast_add_child(n, type_node);

			/* transfer n -> root */
			*root = n;
		} else {
			ast_destroy(type_node);
		}

		token_destroy(id);
		free(id);

		/* destroy dc dc{} */
		token_destroy(dc);
		free(dc);
	}

	return valid;
}

bool type(struct parse_state* ps, struct token* id, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	int num;
	valid = get_lookahead(ps, 2, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	if (t0 && t0->type == token_type_name && t1 && t1->type == token_left_curly_brace) {
		struct location loc;
		get_token_location(t0, &loc);
		char* a;
		buffer2array(&t0->value, &a);
		valid = set_source_error(ps->el, &loc, "type is not an array type: %s", a);
		free(a);
	} else if (t0 && t0->type == token_array_type_name && t1 && t1->type == token_left_curly_brace) {
		struct token* name = NULL;
		valid = match(ps, token_array_type_name, "expected array type name", &name) && valid;

		struct token* lcb = NULL;
		valid = match(ps, token_left_curly_brace, "expected left curly brace", &lcb) && valid;
		token_destroy(lcb);
		free(lcb);

		struct token* name2 = NULL;
		valid = match(ps, token_type_name, "expected type name", &name2) && valid;

		struct token* rcb = NULL;
		valid = match(ps, token_right_curly_brace, "expected right curly brace", &rcb) && valid;
		token_destroy(rcb);
		free(rcb);

		if (valid) {
			ast_create_node(&n);
			n->type = ast_type_array;

			struct ast_node* a = NULL;
			ast_create_node(&a);
			a->type = ast_type_array_type_name;
			buffer_copy(&name->value, &a->value);
			ast_add_child(n, a);

			struct ast_node* b = NULL;
			ast_create_node(&b);
			b->type = ast_type_type_name;
			buffer_copy(&name2->value, &b->value);
			ast_add_child(n, b);

			if (id) {
				struct symbol* search = environment_get_local(ps->st->top, &id->value);
				if (search) {
					struct location loc;
					get_token_location(id, &loc);
					char* a;
					buffer2array(&id->value, &a);
					valid = set_source_error(ps->el, &loc, "duplicate declaration in same scope: %s", a);
					free(a);
				} else {
					struct symbol* sym = NULL;
					malloc_safe((void**)&sym, sizeof(struct symbol));
					symbol_init(sym);
					sym->tk_type = id->type;
					sym->dec = n;
					environment_put(ps->st->top, &id->value, sym);
				}
			}

			*root = n;
		}

		/* destroy name name{} name2 name2{} */
		token_destroy(name);
		free(name);
		token_destroy(name2);
		free(name2);

	} else if (t0 && t0->type == token_type_name) {
		struct token* name = NULL;
		valid = match(ps, token_type_name, "expected type name", &name) && valid;

		if (valid) {
			ast_create_node(&n);
			n->type = ast_type_type_name;
			buffer_copy(&name->value, &n->value);

			if (id) {
				struct symbol* search = environment_get_local(ps->st->top, &id->value);
				if (search) {
					struct location loc;
					get_token_location(id, &loc);
					char* a;
					buffer2array(&id->value, &a);
					valid = set_source_error(ps->el, &loc, "duplicate declaration in same scope: %s", a);
					free(a);
				} else {
					struct symbol* sym = NULL;
					malloc_safe((void**)&sym, sizeof(struct symbol));
					symbol_init(sym);
					sym->tk_type = id->type;
					sym->dec = n;
					environment_put(ps->st->top, &id->value, sym);
				}
			}

			*root = n;
		}

		/* destroy name name{} */
		token_destroy(name);
		free(name);
	}

	return valid;
}

struct ast_node* af2etype(struct ast_node* n)
{
	struct ast_node* etype = NULL;
	ast_create_node(&etype);
	etype->type = ast_type_type_function;

	/* dseq */
	struct ast_node* dseq = ast_get_child(n, 0);
	struct ast_node* type_dseq = NULL;
	ast_create_node(&type_dseq);
	type_dseq->type = ast_type_type_dseq;
	struct ast_node* dec = dseq->head;
	while (dec) {
		struct ast_node* t = ast_get_child(dec, 1);
		ast_add_child(type_dseq, ast_copy(t));
		dec = dec->next;
	}
	ast_add_child(etype, type_dseq);

	/* dret */
	struct ast_node* dret = ast_get_child(n, 1);
	struct ast_node* type_dret = ast_copy(dret);
	type_dret->type = ast_type_type_dret;
	ast_add_child(etype, type_dret);

	return etype;
}

bool base_type_match(struct symbol_table* st, struct buffer* a, struct buffer* b, bool* promoted, struct buffer* c)
{
	*promoted = false;
	buffer_clear(c);

	if (buffer_compare(a, b)) {
		return true;
	}

	struct symbol* sym_a = environment_get(st->top, a);
	struct symbol* sym_b = environment_get(st->top, b);

	if (sym_a && sym_b) {
		struct type_info* ti_a = sym_a->ti;
		struct type_info* ti_b = sym_b->ti;
		if (ti_a && ti_b) {
			if (ti_a->is_integer && ti_b->is_integer) {
				bool is_signed = false;
				int bit_count = ti_a->bit_count;
				if (ti_a->is_signed || ti_b->is_signed) {
					is_signed = true;
				}
				if (ti_b->bit_count > bit_count) {
					bit_count = ti_b->bit_count;
				}

				struct type_info* ti = st->ti_head;
				while (ti) {
					if (ti->is_integer && ti->is_signed == is_signed && ti->bit_count == bit_count) {
						*promoted = true;
						buffer_copy(&ti->name, c);
						return true;
					}
					ti = ti->next;
				}
			} else if (ti_a->is_float && ti_b->is_float) {
				struct type_info* ti = ti_a;
				if (ti_b->bit_count > ti_a->bit_count) {
					ti = ti_b;
				}
				*promoted = true;
				buffer_copy(&ti->name, c);
				return true;
			} else if ((ti_a->is_integer && ti_b->is_float) || (ti_a->is_float && ti_b->is_integer)) {
				struct type_info* ti = ti_a;
				if (ti_b->is_float) {
					ti = ti_b;
				}
				*promoted = true;
				buffer_copy(&ti->name, c);
				return true;
			}
		}
	}

	return false;
}

bool type_match(struct symbol_table* st, struct ast_node* a, struct ast_node* b)
{
	struct ast_node* copy = NULL;
	bool promoted;
	struct buffer c;

	buffer_init(&c);

	if (a && b) {
		if (a->type != b->type) {
			return false;
		}

		if (!base_type_match(st, &a->value, &b->value, &promoted, &c)) {
			return false;
		}

		if (promoted) {
			buffer_clear(&a->value);
			buffer_copy(&c, &a->value);
			buffer_destroy(&c);
		}

		struct ast_node* c = a->head;
		struct ast_node* d = b->head;
		do {
			if (!ast_match(c, d)) {
				return false;
			}
			if (c) c = c->next;
			if (d) d = d->next;
		} while (c || d);
	} else if (!a && !b) {
		return true;
	} else {
		return false;
	}

	return true;
}

bool binary_arithmetic_check(struct parse_state* ps, struct ast_node* left, struct ast_node* a, struct ast_node* b, struct location* loc_a, struct location* loc_op, struct location* loc_b, char* op_name, struct ast_node** ret)
{
	bool valid = true;
	struct ast_node* etype = ast_copy(left->etype);

	if (!etype) {
		if (left == a) {
			valid = set_source_error(ps->el, loc_a, "cannot perform operation on expression with no value");
		} else {
			valid = set_source_error(ps->el, loc_op, "cannot perform operation on expression with no value");
		}
	} else if (!b->etype) {
		valid = set_source_error(ps->el, loc_b, "cannot perform operation on expression with no value");
	} else {
		if (etype->type != ast_type_type_name) {
			if (left == a) {
				valid = set_source_error(ps->el, loc_a, "invalid operand for %s", op_name);
			} else {
				valid = set_source_error(ps->el, loc_op, "invalid operand for %s", op_name);
			}
		} else if (b->etype->type != ast_type_type_name) {
			valid = set_source_error(ps->el, loc_b, "invalid operand for %s", op_name);
		} else {
			struct buffer* name_left = &etype->value;
			struct symbol* sym_left = environment_get(ps->st->top, name_left);
			struct buffer* name_b = &b->etype->value;
			struct symbol* sym_b = environment_get(ps->st->top, name_b);
			if (!sym_left) {
				if (left == a) {
					valid = set_source_error(ps->el, loc_a, "invalid operand for %s", op_name);
				} else {
					valid = set_source_error(ps->el, loc_op, "invalid operand for %s", op_name);
				}
			} else if (!sym_b) {
				valid = set_source_error(ps->el, loc_b, "invalid operand for %s", op_name);
			} else {
				if (!sym_left->ti) {
					if (left == a) {
						valid = set_source_error(ps->el, loc_a, "invalid operand for %s", op_name);
					} else {
						valid = set_source_error(ps->el, loc_op, "invalid operand for %s", op_name);
					}
				} else if (!sym_b->ti) {
					valid = set_source_error(ps->el, loc_b, "invalid operand for %s", op_name);
				} else {
					if (!sym_left->ti->is_integer && !sym_left->ti->is_float) {
						if (left == a) {
							valid = set_source_error(ps->el, loc_a, "invalid operand for %s", op_name);
						} else {
							valid = set_source_error(ps->el, loc_op, "invalid operand for %s", op_name);
						}
					} else if (!sym_b->ti->is_integer && !sym_b->ti->is_float) {
						valid = set_source_error(ps->el, loc_b, "invalid operand for %s", op_name);
					} else {
						if (!type_match(ps->st, etype, b->etype)) {
							valid = set_source_error(ps->el, loc_b, "invalid operand for %s", op_name);
						}
					}
				}
			}
		}
	}

	*ret = etype;
	return valid;
}
