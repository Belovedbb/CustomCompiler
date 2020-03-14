#include "compiler.h"

Parser parser;

Chunk* compiling_chunk;


ParseRule rules[]= {
    {grouping, NULL, PREC_NONE}, //TOKEN_LEFT_PAREN
    {NULL, NULL, PREC_NONE},//TOKEN_RIGHT_PAREN
    {NULL, NULL, PREC_NONE},//TOKEN_LEFT_BRACE
    {NULL, NULL, PREC_NONE},//TOKEN_RIGHT_BRACE
    {NULL, NULL, PREC_NONE},//TOKEN_COMMA
    {NULL,NULL,PREC_NONE},//TOKEN_DOT
    {NULL,binary,PREC_TERM},//TOKEN_PLUS
    {unary, binary, PREC_TERM},//TOKEN_SUBTRACT
    {NULL, NULL, PREC_NONE},//TOKEN_SEMICOLON
    {NULL, binary,PREC_FACTOR},//TOKEN_SLASH
    {NULL, binary, PREC_FACTOR},//TOKEN_STAR
    {NULL, NULL, PREC_NONE},//TOKEN_NOT
    {NULL, NULL, PREC_NONE}, //TOKEN_NOT_EQUAL
    {NULL,NULL, PREC_NONE}, //TOKEN_EQUAL
    {NULL, NULL, PREC_NONE}, //TOKEN_EQUAL_EQUAL
    {NULL, NULL, PREC_NONE},//TOKEN_GREATER
    {NULL, NULL, PREC_NONE},//TOKEN_GREATER_EQUAL
    {NULL, NULL, PREC_NONE},//TOKEN_LESS
    {NULL, NULL, PREC_NONE}, // TOKEN_LESS
    {NULL, NULL, PREC_NONE}, //TOKEN_IDENTIFIER
    {NULL, NULL, PREC_NONE}, //TOKEN_STRING
    {number, NULL, PREC_NONE}, //TOKEN_NUMBER
    {NULL, NULL, PREC_NONE}, //TOKEN_AND
    {NULL, NULL, PREC_NONE},//TOKEN_CLASS
    {NULL, NULL, PREC_NONE},//TOKEN_ELSE
    {NULL, NULL, PREC_NONE},//TOKEN_IF
    {NULL, NULL, PREC_NONE},//TOKEN_FALSE
    {NULL, NULL, PREC_NONE},//TOKEN_TRUE
    {NULL, NULL, PREC_NONE},//TOKEN_FOR
    {NULL, NULL, PREC_NONE},//TOKEN_FUN
    {NULL, NULL, PREC_NONE},//TOKEN_NIL
    {NULL, NULL, PREC_NONE},//TOKEN_OR
    {NULL, NULL, PREC_NONE},//TOKEN_PRINT
    {NULL, NULL, PREC_NONE},//TOKEN_RETURN
    {NULL, NULL, PREC_NONE},//TOKEN_SUPER
    {NULL, NULL, PREC_NONE},//TOKEN_THIS
    {NULL, NULL, PREC_NONE},//TOKEN_VAR
    {NULL, NULL, PREC_NONE},//TOKEN_WHILE
    {NULL, NULL, PREC_NONE},//TOKEN_ERROR
    {NULL, NULL, PREC_NONE},//TOKEN_EOF
};

bool compile(const char* source, Chunk* chunk){
    init_scanner(source);
    compiling_chunk = chunk;
    parser.had_error = false;
    parser.panic_mode = false;
    advance_();
    expression();
    consume(TOKEN_EOF, "Expect end of expression");
    end_compiler();
    return !parser.had_error;
}

void advance_(){
    parser.previous = parser.current;

    for(;;){
        parser.current = scan_token();
        if(parser.current.type != TOKEN_ERROR)
            break;
        
        error_at_current(parser.current.start);
    }
}

void error_at_current(const char* message){
    error_at(&parser.current, message);
}

void error_at(Token* current_token, const char* message){
    if(parser.panic_mode)
        return;
    parser.panic_mode = true;
    fprintf(stderr, "[At line %d] ", current_token->line);
    if(current_token->type == TOKEN_EOF)
        fprintf(stderr, " end of file");
    else if(current_token->type == TOKEN_ERROR)
        ;
    else{
        fprintf(stderr, " at '%.*s' ", current_token->length, current_token->start);
    }
    fprintf(stderr, " : %s \n", message);
    parser.had_error = true;
}

void consume(TokenType type, const char* message){
    if(parser.current.type == type){
        advance_();
        return;
    }
    error_at_current(message);
}

void emit_byte(uint8_t byte){
    write_chunk(current_chunk(), byte, parser.previous.line);
}

void emit_bytes(uint8_t byte1, uint8_t byte2){
    emit_byte(byte1);
    emit_byte(byte2);
}

Chunk* current_chunk(){
    return compiling_chunk;
}

void end_compiler(){
    emit_return();
    #ifdef DEBUG_PRINT_CODE
        if(!parser.had_error){
            disassemble_chunk(current_chunk(), "code");
        }
    #endif
}

void emit_return(){
    emit_byte(OP_RETURN);
}

void emit_constant(double value){
    emit_bytes(OP_CONSTANT, make_constant(value));
}

void expression(){
    parse_precedence(PREC_ASSN);
}

void number(){
    double value = strtod(parser.previous.start, NULL);
    emit_constant(value);
}

void grouping(){
    expression();
    consume(TOKEN_RIGHT_PAREN, " expected ')' after expression");
}

void unary(){
    TokenType operator_type  = parser.previous.type;

    //compute the unary number
    parse_precedence(PREC_UNARY);

    //add operator afterwards
    switch(operator_type){
        case TOKEN_SUBTRACT:
            emit_byte(OP_SUBTRACT);
        defualt:
            return;
    }
}

void binary(){
    TokenType token_type = parser.previous.type;

    ParseRule* rule = get_rule(token_type);
    parse_precedence((Precedence)(rule->precedence + 1));

    switch(token_type){
        case TOKEN_PLUS:
            emit_byte(OP_ADD);
            break;
        case TOKEN_STAR:
            emit_byte(OP_MULTIPLY);
            break;
        case TOKEN_SUBTRACT:
            emit_byte(OP_SUBTRACT);
            break;
        case TOKEN_SLASH:
            emit_byte(OP_DIVIDE);
            break;
    }
}

uint8_t make_constant(double value){
    int constant_index = add_constant(current_chunk(), value);
    if( constant_index > STACK_MAX){
        error_at_current("too many constant");
        return;
    }
    return (uint8_t)constant_index;
}

void parse_precedence(Precedence precedence){
    advance_();
    ParseFn parse_func = get_rule(parser.previous.type)->prefix;
    if(parse_func == NULL){
        error_at_current("expected an expression ");
        return;
    }
    parse_func();
    while(precedence <= get_rule(parser.current.type)->precedence){
        advance();
        ParseFn infix_rule = get_rule(parser.current.type)->infix;
        infix_rule();
    }
}

ParseRule* get_rule(TokenType type){
    return &rules[type];
}