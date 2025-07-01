#ifndef AKELA_TYPE_TO_CENT_H
#define AKELA_TYPE_TO_CENT_H

#include "type_list.h"
#include "zinc/vector.h"

typedef struct Ake_TypeMatcher Ake_TypeMatcher;

struct Ake_TypeMatcher {
    Ake_TypeList list;
    Zinc_vector index_list;     // vector of size_t
};

void Ake_TypeMatcherInit(Ake_TypeMatcher* matcher);
void Ake_TypeMatcherProcess(Ake_TypeMatcher* matcher, Ake_Type* type);

#endif