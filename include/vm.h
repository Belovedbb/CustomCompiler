#ifndef COMPILER_VM_H
#define COMPILER_VM_H

#include "common.h"
#include "value.h"
#include "chunk.h"
#include "debug.h"
#include "compiler.h"

#define STACK_MAX 256

typedef struct{
    Chunk* chunk;
    uint8_t* instruction_pointer;
    Value stack[STACK_MAX];
    Value* stack_top;
}VirtualMachine;

typedef enum{
    INTERPRET_OK, 
    INTERPRET_COMPILE_ERROR, 
    INTERPRET_RUNTIME_ERROR, 
}InterpretResult;

//vm state process
void init_virtual_machine();

void reset_stack();

void free_virtual_machine();

//vm operations
void push(Value value);
Value pop();


//vm utils
//InterpretResult interpret(Chunk* chunk);

InterpretResult interpret(const char* source);

InterpretResult run();

#endif