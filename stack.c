#include "stack.h"
#include <string.h>

#define SIZE STACK_SIZE


static int64_t stack[SIZE];
int top = 0;

int stack_initialize() {
  memset(&(stack[0]), 0, sizeof(stack[0]) * SIZE);
  return SUCCESS;
}

int stack_finalize() {
  top = 0;
  return SUCCESS;
}

int stack_empty() {
  return top == 0;
}

int stack_push(int64_t val) {
  if (top >= SIZE) {
    return FAILURE;
  }
  stack[top++] = val;
  return SUCCESS;
}

int64_t stack_pop() {
  if (top <= 0) {
    return FAILURE;
  } else {
    return stack[--top];
  }
}

int stack_size() {
  return top;
}

