#include "scanner.h"

typedef struct{
    const char* start;
    const char* current;
    int line;
}Scanner;

Scanner scanner;

void init_scanner(const char* source){
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token scan_token(){
    scanner.start = scanner.current;
    if(is_at_end())
        return make_token(TOKEN_EOF);
    return error_token("unexpected token");
}