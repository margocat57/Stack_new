#ifndef HANDLERS_H
#define HANDLERS_H
#include "stdlib.h"
#include "stack.h"

const size_t HANDLER_MAX_SIZE = 2048;

static ssize_t create_handle(stack_t_t* stack);

static stack_t_t* delete_handle(size_t handle);

ssize_t stak_ctor_handler(long long int num_of_elem, long long int size_of_elem);

void stack_push_handle(size_t handle, const void* elem);

void stack_pop_handle(size_t handle, void* elem);

void stack_top_handle(size_t handle, void* elem);

void stack_free_handle(size_t handle);

#endif //HANDLERS_H