#include <stdio.h>
#include "mistakes_code.h"

bool my_assert_func(bool condition, const char* str_condition, const char* file, const char* func, int line){
    //добавить вывод в лог файл
    if(!condition){
        fprintf(stderr, "%s: %s line %d, assertion failed at %s\n", file, func, line, str_condition);
        return true;
    }
    return false;
}