#include "value.h"

void init_value_array(ValueArray* value_array){
    value_array->len  =  0;
    value_array->cap  = 0;
    value_array->values = NULL;
}

void write_value_array(ValueArray* value_array, Value value){
    if( value_array->cap < value_array->len + 1){
        int old_cap = value_array->cap;
        value_array->cap = GROW_CAP(old_cap);
        value_array->values = GROW_ARRAY_CODE(value_array->values, Value, old_cap, value_array->cap);
    }
    value_array->values[value_array->len] = value;
    value_array->len++;
}

void free_value_array(ValueArray* value_array){
    FREE_ARRAY(Value, value_array->values, value_array->cap);
    init_value_array(value_array);
}

void print_value_array(Value value){
    printf("%g", AS_NUMBER(value));
}