#include "chunk.h"

void init_chunk(Chunk* chunk){
    chunk-> cap = 0;
    chunk-> len = 0;
    chunk-> code = NULL;
    chunk->lines = NULL;
    init_value_array(&chunk->constants);
}

void write_chunk(Chunk* chunk, uint8_t byte, int line){
    if(chunk->cap < chunk->len + 1){
        int old_cap = chunk->cap;
        chunk->cap = GROW_CAP(old_cap);
        chunk->code = GROW_ARRAY_CODE(chunk->code, uint8_t, old_cap, chunk->cap);
        chunk->lines = GROW_ARRAY_CODE(chunk->lines, int, old_cap, chunk->cap);
    }
    chunk->code[chunk->len] = byte;
    chunk->lines[chunk->len] = line;
    ++chunk->len;
}

void free_chunk(Chunk* chunk){
    FREE_ARRAY(uint8_t, chunk->code, chunk->cap);
    FREE_ARRAY(int, chunk->lines, chunk->cap);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}

int add_constant(Chunk* chunk, Value value){
    write_value_array(&chunk->constants, value);
    return chunk->constants.len - 1;
}