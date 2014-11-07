#ifndef STACK_H_
#define STACK_H_

#include "return_codes.h"
#include <stdint.h>

#ifndef STACK_SIZE
#define STACK_SIZE 256
#endif

int stack_initialize();
int stack_push(int64_t val);
int64_t stack_pop();
int stack_empty();
int stack_finalize();
int stack_size();

#endif // STACK_H_
