#include <sys/types.h>
#include "handlers.h"
#include "stack.h"
#include "my_assert.h"
#include "stack_func.h"

static stack_t_t* HANDLERS[HANDLER_MAX_SIZE] = {NULL};
static size_t FREE_HANDLE = 0;

static ssize_t create_handle(stack_t_t* stack){

    if(MY_ASSERT(stack_verify(stack) == NO_MISTAKE)) return -1;
    if(MY_ASSERT(FREE_HANDLE < HANDLER_MAX_SIZE)) return -1;

    while(HANDLERS[FREE_HANDLE]){
        FREE_HANDLE++;
    }

    HANDLERS[FREE_HANDLE] = stack;

    return FREE_HANDLE;
}

static stack_t_t* delete_handle(size_t handle){

    if(MY_ASSERT(handle < HANDLER_MAX_SIZE)) return NULL;

    if(HANDLERS[handle]){
        stack_t_t* stack = HANDLERS[handle];
        HANDLERS[handle] == NULL;
        if(handle <=FREE_HANDLE)
            FREE_HANDLE = handle;
        return stack;
    }
    return NULL;
}

ssize_t stak_ctor_handler(long long int num_of_elem, long long int size_of_elem){
    stack_t_t* stack = stack_ctor(num_of_elem, size_of_elem);
    return create_handle(stack);
}

void stack_push_handle(size_t handle, const void* elem){
    if(MY_ASSERT(handle>=HANDLER_MAX_SIZE)) return;
    if(MY_ASSERT(HANDLERS[handle])) return;

    stack_t_t *stack = HANDLERS[handle];
    stack_push(stack, elem);
}

void stack_pop_handle(size_t handle, void* elem){
    if(MY_ASSERT(handle>=HANDLER_MAX_SIZE)) return;
    if(MY_ASSERT(HANDLERS[handle])) return;

    stack_t_t *stack = HANDLERS[handle];
    stack_pop(stack, elem);
}

void stack_top_handle(size_t handle, void* elem){
    if(MY_ASSERT(handle>=HANDLER_MAX_SIZE)) return;
    if(MY_ASSERT(HANDLERS[handle])) return;

    stack_t_t *stack = HANDLERS[handle];
    stack_pop(stack, elem);
}

void stack_free_handle(size_t handle){
    if(MY_ASSERT(handle>=HANDLER_MAX_SIZE)) return;
    if(MY_ASSERT(HANDLERS[handle])) return;

    free_stack(delete_handle(handle));
}

// и на освобождение стека мы должны тоже вызываать освобождение полки