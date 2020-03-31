#include "vm.h"

VirtualMachine virtual_machine;


void init_virtual_machine(){
    reset_stack();
}

void reset_stack(){
    virtual_machine.stack_top = virtual_machine.stack;
}

void push(Value value){
    *virtual_machine.stack_top =  value;
    ++virtual_machine.stack_top;
}

Value pop(){
    --virtual_machine.stack_top;
    return *virtual_machine.stack_top;
}

void free_virtual_machine(){

}

Value peek_value(int distance){
    return virtual_machine.stack_top[-1 - distance];
}

void runtime_error(const char* format, ...){
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    uint8_t instruction = virtual_machine.instruction_pointer - virtual_machine.chunk->code - 1;
    int line = virtual_machine.chunk->lines[instruction];
    fprintf(stderr, "error in line [%d]", line);
    reset_stack();
}

InterpretResult interpret(const char* source){
    Chunk chunk;
    init_chunk(&chunk);
    if(!compile(source, &chunk)){
        free_chunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
    virtual_machine.chunk = &chunk;
    virtual_machine.instruction_pointer = chunk.code;
    InterpretResult result = run();
    free_chunk(&chunk);
    return result;
}

InterpretResult run(){
    #define READ_BYTE() (*virtual_machine.instruction_pointer++)
    #define READ_CONSTANT() (virtual_machine.chunk->constants.values[READ_BYTE()])
    #define BINARY_OP(value_type, operator) \
            do{\
                if(!IS_NUMBER(peek_value(0)) || IS_NUMBER(peek_value(1))){\
                    runtime_error("literal should be a number");\
                    return INTERPRET_RUNTIME_ERROR;\
                }\
                double b  = AS_NUMBER(pop());\
                double a = AS_NUMBER(pop());\
                push(value_type(a operator b));\
            }while(false)

        for(;;){
            #ifdef DEBUG_TRACE_EXECUTION
                printf(" -----------Constant Value Stack------------   \n");
                for(Value* stack = virtual_machine.stack; stack < virtual_machine.stack_top ; stack++){
                    printf("[");
                    print_value_array(*stack);
                    printf("]");
                }
                printf("\n     VM Bytecode Eval            \n");
                disassemble_instruction(virtual_machine.chunk, 
                (int) (virtual_machine.instruction_pointer - virtual_machine.chunk->code));
            #endif
            uint8_t instruction;
            switch(instruction = READ_BYTE()){
                case OP_RETURN:{
                    print_value_array(pop());
                    printf("\n");
                    return INTERPRET_OK;
                }
                case OP_CONSTANT:{
                    Value constant = READ_CONSTANT();
                    push(constant);
                    break;
                }
                case OP_NEGATE:{
                    if(!IS_NUMBER(peek_value(0))){
                        runtime_error("operand must be a number");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                    push(NUMBER_VALUE(-AS_NUMBER(pop())));
                    break;
                }
                case OP_ADD:
                    BINARY_OP(NUMBER_VALUE, +);
                    break;
                case OP_SUBTRACT:
                    BINARY_OP(NUMBER_VALUE, -);
                    break;
                case OP_MULTIPLY:
                    BINARY_OP(NUMBER_VALUE, *);
                    break;
                case OP_DIVIDE: 
                    BINARY_OP(NUMBER_VALUE, /);
                    break;
                case OP_TRUE:
                    push(BOOL_VALUE(true));
                    break;
                case OP_FALSE:
                    push(BOOL_VALUE(false));
                    break;
                case OP_NIL:
                    push(NIL_VALUE(0));
                    break;
            }
        }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}