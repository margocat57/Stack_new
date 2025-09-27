#ifndef HASH_H
#define HASH_H
#include "stdlib.h"
#include <sys/types.h>

struct temp_struct{
    size_t front_canary;
    char *ptr;
    char *data;
    size_t capacity;
    size_t size_of_elem;
    size_t tail_canary;
};

ssize_t calculate_struct_hash(const stack_t_t* stack);

ssize_t create_djb2_hash(const char* data_for_hash, size_t size);

#endif //HASH_H