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

    // проверка что мы не обращаемся за пределы массива хотя функция и внутренняя

    if(HANDLERS[handle]){
        stack_t_t* stack = HANDLERS[handle];
        HANDLERS[handle] == NULL;
        if(handle <=FREE_HANDLE)
            FREE_HANDLE = handle;
        return stack;
    }
    return NULL;
}

// далее при функции инициализации стека мы должны возвращать номер свободной полки

// потом все функции принимают номер полки, а этом файле они должгы вызывать функции со структурой
// также проверять что мы не обращаемся к полке за пределами структуры
// обязательная проверка что номер полки не больше максимального значения

// и на освобождение стека мы должны тоже вызываать освобождение полки