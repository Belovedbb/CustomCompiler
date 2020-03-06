#ifndef COMPILER_VALUE_H
#define COMPILER_VALUE_H

#include "common.h"
#include "memory.h"

typedef double Value;

typedef struct{
    int len;
    int cap;
    Value* values;
}ValueArray;

void init_value_array(ValueArray* value_array);
void write_value_array(ValueArray* value_array, Value value);
void free_value_array(ValueArray* value_array);
void print_value_array(Value value);

#endif