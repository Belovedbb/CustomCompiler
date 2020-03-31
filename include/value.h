#ifndef COMPILER_VALUE_H
#define COMPILER_VALUE_H

#include "common.h"
#include "memory.h"

typedef enum{
    VAL_BOOL,
    VAL_NUMBER,
    VAL_NIL
}ValueType;

typedef struct{
    ValueType type;
    union{
        bool boolean;
        double number;
    }as;
}Value;

typedef struct{
    int len;
    int cap;
    Value* values;
}ValueArray;

#define BOOL_VALUE(value) ((Value){VAL_BOOL,{.boolean = value}})
#define NIL_VALUE(value) ((Value){VAL_NIL,{.number = 0}})
#define NUMBER_VALUE(value) ((Value){VAL_NUMBER,{.number = value}})

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
 
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

void init_value_array(ValueArray* value_array);
void write_value_array(ValueArray* value_array, Value value);
void free_value_array(ValueArray* value_array);
void print_value_array(Value value);

#endif