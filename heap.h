#ifndef HEAP_H_
#define HEAP_H_

#include <limits.h>

#define HEAP_SIZE (UCHAR_MAX + 1)

struct node_t;
int heap_initialize();
int heap_finalize();
int heap_build(struct node_t * arr_in[HEAP_SIZE - 1]);
int heap_insert(struct node_t* node);
struct node_t * heap_pop();
void heap_print();
int heap_empty();
int heap_size();

#endif // HEAP_H_
