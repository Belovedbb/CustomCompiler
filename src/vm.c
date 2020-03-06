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

InterpretResult interpret(const char* source){
    compile(source);
    return INTERPRET_OK;
}

/*InterpretResult interpret(Chunk* chunk){
    virtual_machine.chunk = chunk;
    virtual_machine.instruction_pointer = chunk->code;
    return run();
}*/

InterpretResult run(){
    #define READ_BYTE() (*virtual_machine.instruction_pointer++)
    #define READ_CONSTANT() (virtual_machine.chunk->constants.values[READ_BYTE()])
    #define BINARY_OP(operator) \
            do{\
                double b  = pop();\
                double a = pop();\
                push(a operator b);\
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
                    push(-pop());
                    break;
                }
                case OP_ADD:
                    BINARY_OP(+);
                    break;
                case OP_SUBTRACT:
                    BINARY_OP(-);
                    break;
                case OP_MULTIPLY:
                    BINARY_OP(*);
                    break;
                case OP_DIVIDE: 
                    BINARY_OP(/);
                    break;
            }
        }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}