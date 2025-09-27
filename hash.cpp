#include <stdlib.h>
#include <sys/types.h>
#include "stack.h"
#include "hash.h"
#include "my_assert.h"
#include "stack_func.h"

ssize_t calculate_struct_hash(stack_t_t* stack) {

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return -1;

    temp_struct temp{
        .front_canary = stack->front_canary,
        .ptr = stack->ptr,
        .data = stack->data,
        .capacity = stack->capacity,
        .size_of_elem = stack->size_of_elem,
        .tail_canary = stack->tail_canary
    };
    
    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return -1;

    return create_djb2_hash((char*)&temp, sizeof(temp));
}

ssize_t create_djb2_hash(char* data, size_t size){

    // check data

    size_t hash = 5381;
    
    for (size_t i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + data[i];
    }

    // check data

    return hash;
}
