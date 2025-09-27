#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "stack_func.h"
#include "stack.h"
#include "hash.h"
#include "mistakes_code.h"
#include "my_assert.h"

const size_t RESERVED = 2 * sizeof(uintptr_t);

stack_t_t* stack_ctor(long long int num_of_elem, long long int size_of_elem){

    if(MY_ASSERT(0 < num_of_elem && num_of_elem<LLONG_MAX)) return NULL;
    if(MY_ASSERT(0< size_of_elem && size_of_elem<LLONG_MAX)) return NULL;

    num_of_elem = (size_t) num_of_elem;
    size_of_elem = (size_t) size_of_elem;

    stack_t_t* stack = (stack_t_t*) calloc(1, sizeof(stack_t_t));
    if(MY_ASSERT(stack!=NULL)) return NULL;

    size_t real_size = num_of_elem*size_of_elem + RESERVED;
    stack->data = (char*)calloc(1, real_size);

    if(MY_ASSERT(stack->data!=NULL)){
        free(stack);
        return NULL;
    }

    stack -> front_canary = FRONTCANARY;
    stack -> tail_canary = TAILCANARY;
    stack -> ptr = stack -> data + sizeof(uintptr_t); // сдвиг на элемент после служеюного
    stack -> capacity = num_of_elem;
    stack -> size_of_elem = size_of_elem;
    stack -> djb2 = calculate_struct_hash(stack);
    stack -> djb2_data = create_djb2_hash(stack -> data, real_size);

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NULL;

    return stack;
}

stack_err_t stack_verify(stack_t_t* stack){

    if(stack == NULL){
        // печать в лог файл
        fprintf(stderr, "NULL stack pointer\n");
        return NULL_STACK_PTR;
    }

    else if(stack->front_canary != FRONTCANARY || stack->tail_canary != TAILCANARY){
        // печать в лог файл
        fprintf(stderr, "CANARIES CORRUPTED\n");
        return CANARY_NOT_IN_PLACES;
    }

    else if(stack -> data == NULL || stack->ptr == NULL || stack->capacity == 0 || stack->size_of_elem == 0){
        // печать в лог файл
        fprintf(stderr, "stack important data is zero or null-pointed\n");
        //распечатка стека в лог файл
        return NULL_STACK_PTR;
    }

    else if(stack->ptr < stack->data || stack -> ptr > stack->data + stack->capacity * stack->size_of_elem ||
    (stack->ptr - (stack->data + sizeof(uintptr_t))) % stack->size_of_elem != 0){
        // печать в лог файл
        fprintf(stderr, "stack pointer out of index\n");
        //распечатка стека в лог файл
        return PTR_OUT_OF_IDX;
    }

    else if (stack -> djb2 != calculate_struct_hash(stack) || stack->djb2 == -1){
        // печать в лог файл
        fprintf(stderr, "stack hash is not correct\n");
        //распечатка стека в лог файл
        return STACK_HASH_NOT_CORRECT;
    }

    else if (stack -> djb2_data != create_djb2_hash(stack -> data, stack->capacity * stack->size_of_elem + RESERVED) || stack->djb2_data == -1){
        // печать в лог файл
        fprintf(stderr, "stack hash is not correct\n");
        //распечатка стека в лог файл
        return DATA_HASH_NOT_CORRECT;
    }

    return NO_MISTAKE;
}

correct stack_push(stack_t_t* stack, const void* elem){

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NOT_CORRECT;
    if(MY_ASSERT(elem != NULL)) return NOT_CORRECT;

    size_t current_size = (stack->ptr - (stack->data + RESERVED/ 2)) / stack->size_of_elem;
    if (current_size >= stack->capacity){
        stack_realloc(stack);
    }
    memcpy(stack->ptr, elem, stack->size_of_elem);
    stack->ptr += stack -> size_of_elem;

    stack ->djb2 = calculate_struct_hash(stack);
    stack -> djb2_data = create_djb2_hash(stack -> data, stack->size_of_elem * stack->capacity + RESERVED);

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NOT_CORRECT;

    return CORRECT;
}

correct stack_push(stack_t_t* stack, void* elem){

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NOT_CORRECT;
    if(MY_ASSERT(elem != NULL)) return NOT_CORRECT;
    
    if (stack->ptr >= stack->data + RESERVED / 2 + stack->size_of_elem){
        stack -> ptr -= stack -> size_of_elem;
        memcpy(elem, stack -> ptr, stack->size_of_elem);
        memset(stack->ptr, 0, stack->size_of_elem);
    }

    stack ->djb2 = calculate_struct_hash(stack);
    stack -> djb2_data = create_djb2_hash(stack -> data, stack->size_of_elem * stack->capacity + RESERVED);

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NOT_CORRECT;

    return CORRECT;
}

correct stack_top(stack_t_t* stack, void* elem){

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NOT_CORRECT;
    if(MY_ASSERT(elem != NULL)) return NOT_CORRECT;
    
    if (stack->ptr >= stack->data + RESERVED / 2 + stack->size_of_elem){
        stack -> ptr -= stack -> size_of_elem;
        memcpy(elem, stack -> ptr, stack->size_of_elem);
        stack -> ptr += stack -> size_of_elem;
        return CORRECT;
    }
    return NOT_CORRECT;
}

correct stack_realloc(stack_t_t* stack){

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NOT_CORRECT;
    
    size_t new_capacity = stack->capacity * 2;
    size_t ptr_pos = stack->ptr - (stack->data + RESERVED/2);

    char* new_data = (char*)realloc(stack->data, new_capacity*stack->size_of_elem + RESERVED);
    if(MY_ASSERT(new_data != NULL)) return NOT_CORRECT;

    stack->data = new_data;
    memset(stack->data + RESERVED/2 + stack->capacity * stack->size_of_elem, 0, (new_capacity-stack->capacity)*stack->size_of_elem + RESERVED/2);

    stack->capacity = new_capacity;
    stack->ptr = stack->data + RESERVED/2 + ptr_pos;

    stack ->djb2 = calculate_struct_hash(stack);
    stack -> djb2_data = create_djb2_hash(stack -> data, stack->size_of_elem * stack->capacity + RESERVED);

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return NOT_CORRECT;

    return CORRECT;
}