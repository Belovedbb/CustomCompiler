#ifndef COMPILER_SCANNER_H
#define COMPILER_SCANNER_H

#include "common.h"
#include "stdio.h"
#include "string.h"


typedef enum{
    //single character
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_PLUS,
    TOKEN_SUBTRACT, TOKEN_SEMICOLON,
    TOKEN_SLASH, TOKEN_STAR,
    //one or two characters
    TOKEN_NOT, TOKEN_NOT_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    //literals
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    //keywords
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_IF,
    TOKEN_FALSE, TOKEN_TRUE, TOKEN_FOR, TOKEN_FUN, 
    TOKEN_NIL, TOKEN_OR, TOKEN_PRINT, TOKEN_RETURN,
    TOKEN_SUPER, TOKEN_THIS, TOKEN_VAR, TOKEN_WHILE,

    TOKEN_ERROR,
    TOKEN_EOF

}TokenType;


typedef struct{
    TokenType type;
    const char* start;
    int length;
    int line;
}Token;

void init_scanner(const char* source);

Token scan_token();

bool is_at_end();

bool is_digit(char character);

bool is_alpha(char character);

char advance();

bool match(char expected);

void skip_whitespaces();

char peek();

char peek_next();

Token make_token(TokenType token_type);

Token make_token_string();

Token make_token_number();

Token make_token_identifier();

TokenType check_keyword(int start, int rem_len, const char* rest, TokenType type);

TokenType identifier_token_type();

Token error_token(const char* message);

#endif