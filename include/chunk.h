#ifndef COMPILER_CHUNK_H
#define COMPILER_CHUNK_H

#include <stdlib.h>
#include "common.h"
#include "memory.h"
#include "value.h"


//opcode for bytecode rep
typedef enum{
    OP_CONSTANT,
    OP_NEGATE,
    OP_ADD, 
    OP_SUBTRACT,
    OP_DIVIDE,
    OP_MULTIPLY,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_RETURN
}OpCode;

//array of opcode
typedef struct{
    int len;
    int cap;
    uint8_t* code;
    ValueArray constants;
    int* lines;
}Chunk;

//initialize bytecode chunk
void init_chunk(Chunk* chunk);
//write byte to chunk array
void write_chunk(Chunk* chunk, uint8_t byte, int line);
//free array chunk
void free_chunk(Chunk* chunk);

int add_constant(Chunk* chunk, Value value);


#endif