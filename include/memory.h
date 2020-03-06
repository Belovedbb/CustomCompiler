#ifndef COMPILER_MEMORY_H
#define COMPILER_MEMORY_H

#include <stdlib.h>
#include "common.h"

#define GROW_CAP(capacity)    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY_CODE(prev_array, type, old_cap, current_cap)\
 (type*)reallocate(prev_array, sizeof(type) * (old_cap), sizeof(type) * (current_cap))

 #define FREE_ARRAY(type, array, cur_cap) reallocate(array, sizeof(type) * (cur_cap), 0)


void* reallocate(void* previous, size_t old_size, size_t new_size);

#endif