#ifndef HANDLERS_H
#define HANDLERS_H
#include "stdlib.h"
#include "stack.h"

const size_t HANDLER_MAX_SIZE = 2048;

static ssize_t create_handle(stack_t_t* stack);

static stack_t_t* delete_handle(size_t handle);

#endif //HANDLERS_H