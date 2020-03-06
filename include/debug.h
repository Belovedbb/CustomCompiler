#ifndef COMPILER_DEBUG_H
#define COMPILER_DEBUG_H

#include <stdio.h>
#include "chunk.h"
#include "value.h"

void disassemble_chunk(Chunk* chunk, const char* name);
int disassemble_instruction(Chunk* chunk, int offset);
static int simple_instruction(const char* name, int offset);
static int  constant_instruction(const char* name, Chunk* chunk, int offset);

#endif