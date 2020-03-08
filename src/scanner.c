#include "scanner.h"

typedef struct{
    const char* start;
    const char* current;
    int line;
}Scanner;

Scanner scanner;


void skip_whitespaces(){
    for(;;){
        char character = peek();
        switch(character){
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':{
                if(peek_next() == '/' && !is_at_end()){
                    while(peek() != '\n' &&  !is_at_end)
                        advance();
                }
                else
                    return;
            }
                break;
            default:
                return;
        }
    }

}

void init_scanner(const char* source){
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token scan_token(){
    scanner.start = scanner.current;
    if(is_at_end())
        return make_token(TOKEN_EOF);

    char character = advance();

    if(is_digit(character))
        return make_token_number();

    if(is_alpha(character))
        return make_token_identifier();

    switch(character){
        case '(':
            return make_token(TOKEN_LEFT_PAREN);
        case ')':
            return make_token(TOKEN_RIGHT_PAREN);
        case '{':
            return make_token(TOKEN_LEFT_BRACE);
        case '}':
            return make_token(TOKEN_RIGHT_BRACE);
        case ';':
            return make_token(TOKEN_SEMICOLON);
        case ',':
            return make_token(TOKEN_COMMA);
        case '.':
            return make_token(TOKEN_DOT);
        case '+':
            return make_token(TOKEN_PLUS);
        case '-':
            return make_token(TOKEN_SUBTRACT);
        case '*':
            return make_token(TOKEN_STAR);
        case '/':
            return make_token(TOKEN_SLASH);
        case '!':
            return make_token(match('=') ? TOKEN_NOT_EQUAL  : TOKEN_NOT);
        case '=':
            return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return make_token_string();
        
    }
    return error_token("unexpected token");
}

bool is_at_end(){
    return *scanner.current == '\0';
}

bool is_digit(char character){
   return character > '0' && character < '9';
}

bool is_alpha(char character){
    return (character > 'a' && character < 'z') || (character > 'A' && character < 'Z') || character == '_';
}

Token make_token(TokenType token_type){
    Token token;
    token.type = token_type;
    token.start = scanner.start;
    token.line = scanner.line;
    token.length = (int) (scanner.current - scanner.start);
    return token;
}

Token error_token(const char* message){
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

char advance(){
    scanner.current++;
    return scanner.current[-1];
}

bool match(char expected){
    if(is_at_end())
        return false;
    if(*scanner.current != expected)
        return false;
    scanner.current++;
    return true;
}

char peek(){
    return *scanner.current;
}

char peek_next(){
    if(is_at_end())
        return '\0';
    return scanner.current[1];
}

Token make_token_string(){
    while(peek() != '"' && !is_at_end()){
        if(peek() == '\n')
            scanner.line++;
        advance();
    }
    if(is_at_end())
        return error_token("unterminated string");
    advance();
    return make_token(TOKEN_STRING);
}

Token make_token_number(){
    while(is_digit(peek()) && !is_at_end())
        advance();
    if(peek() == '.' &&  is_digit(peek_next())){
        advance();
        while(is_digit(peek()) && !is_at_end())
            advance();
    }
    return make_token(TOKEN_NUMBER);
}

Token make_token_identifier(){
    while(is_alpha(peek()) || is_digit(peek()))
        advance();
    return make_token(identifier_token_type());
}

TokenType identifier_token_type(){
    switch(scanner.start[0]){
        case 'a':
            return check_keyword(1,2, "nd",TOKEN_AND);
        case 'c': 
            return check_keyword(1, 4, "lass", TOKEN_CLASS);  
        case 'e': 
            return check_keyword(1, 3, "lse", TOKEN_ELSE);    
        case 'i': 
            return check_keyword(1, 1, "f", TOKEN_IF);        
        case 'n': 
            return check_keyword(1, 2, "il", TOKEN_NIL);      
        case 'o': 
            return check_keyword(1, 1, "r", TOKEN_OR);        
        case 'p': 
            return check_keyword(1, 4, "rint", TOKEN_PRINT);  
        case 'r': 
            return check_keyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': 
            return check_keyword(1, 4, "uper", TOKEN_SUPER);  
        case 'v': 
            return check_keyword(1, 2, "ar", TOKEN_VAR);      
        case 'w': 
            return check_keyword(1, 4, "hile", TOKEN_WHILE);
        case 'f':
            if(scanner.current - scanner.start > 1){
                switch(scanner.start[0]){
                    case 'o':
                        return check_keyword(2,1,"r", TOKEN_FOR);
                    case 'a':
                        return check_keyword(2,3, "lse", TOKEN_FALSE);
                    case 'u':
                        return check_keyword(2, 1,"n",TOKEN_FUN);
                }
            }
        case 't':
            if(scanner.current - scanner.start > 1){
                switch(scanner.start[0]){
                    case 'h':
                        return check_keyword(2,2,"is", TOKEN_THIS);
                    case 'r':
                        return check_keyword(2,2, "ue", TOKEN_TRUE);
                }
            }
    }
    return TOKEN_IDENTIFIER;
}

TokenType check_keyword(int start, int rem_len, const char* rest, TokenType type){
    if(scanner.current - scanner.start == rem_len + start && memcmp(scanner.start + start, rest, rem_len) == 0){
        return type;
    }
    return TOKEN_IDENTIFIER;
}