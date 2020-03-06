#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void run_repl(){
    char line[1024];
    for(;;){
        printf(">> ");
        if(!fgets(line,sizeof(line), stdin)){
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static char* read_file(const char* path){
    FILE* file = fopen(path, "rb");
    if(file == NULL){
        fprintf(stderr, "could not open file %s", path);
        exit(-2);
    }
    fseek(file, 0L, SEEK_END);
    size_t size_file = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(size_file + 1);
    if(buffer == NULL){
        fprintf(stderr, "alloc error");
        exit(-2);
    }
    size_t bytes_read = fread(buffer, sizeof(char), size_file, file);
    if(bytes_read < size_file){
        fprintf(stderr, "file not read completely... terminating");
        exit(-2);
    }
    buffer[bytes_read] = '\0';
    fclose(file);
    return buffer;
}

static void run_file(const char* path){
    char* source = read_file(path);
    InterpretResult result = interpret(source);
    free(source);
    if(result == INTERPRET_COMPILE_ERROR)
        exit(3);
    if(result == INTERPRET_RUNTIME_ERROR)
        exit(4);
}

int main(int argc, const char * argv[]){
    init_virtual_machine();

    if(argc == 1){
        run_repl();
    }else if(argc == 2){
        run_file(argv[1]);
    }else {
        fprintf(stderr, "Usage: Compiler [path] \n");
        exit(-1);
    }

    free_virtual_machine();
    return 0;
}

