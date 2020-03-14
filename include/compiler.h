#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

#include <stdlib.h>
#include "common.h"
#include "scanner.h"
#include "vm.h"

#ifdef DEBUG_PRINT_CODE
    #include "debug.h"
#endif

typedef void (*ParseFn)();

typedef struct{
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
}Parser;

typedef enum{
    PREC_NONE,
    PREC_ASSN,
    PREC_OR,
    PREC_AND,
    PREC_EQU,
    PREC_COMP,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY
}Precedence;

typedef struct{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
}ParseRule;

Chunk* current_chunk();

void end_compiler();
bool compile(const char* source, Chunk* chunk);

void consume(TokenType type, const char* message);

void advance_();

void expression();

uint8_t make_constant(double value);

void emit_byte(uint8_t byte);
void emit_bytes(uint8_t byte1, uint8_t byte2);
void emit_return();
void emit_constant(double value);

void parse_precedence(Precedence precedence);

//expressions
void number();
void grouping();
void unary();
void binary();


//handle syntax error
void error_at_current(const char* message);
void error_at(Token* current_token, const char* message);

//precedence
ParseRule* get_rule(TokenType type);

#endif 