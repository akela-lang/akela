#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "comp_unit.h"

void cg_jit(struct comp_unit* cu, struct buffer* bf, bool output_bc, bool output_ll);

#endif