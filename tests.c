#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"
#include "node.h"
#include "input_module.h"

#define NODE_INIT(node, w, cc) node.weight = w; node.c = cc;

void test_initialize_heap() {
  heap_initialize();
  heap_finalize();  
}

void test_initialize_node() {
  struct node_t node;
  node.weight = 5;
  node.c = 'c';
  assert(node.weight == 5);
  assert(node.c == 'c');
  assert(sizeof(struct node_t) == 8);
}

void test_heap_insert() {
  struct node_t a;
  struct node_t b;
  struct node_t c;
  struct node_t d;
  struct node_t e;
  struct node_t f;
  struct node_t g;
  struct node_t * retVal;
  NODE_INIT(a, 3, 'a')
  NODE_INIT(b, 2, 'b')
  NODE_INIT(c, 1, 'c')
  NODE_INIT(d, 25, 'd')
  NODE_INIT(e, 14, 'e')
  NODE_INIT(f, 16, 'f')
  NODE_INIT(g, 27, 'g')
    

  heap_initialize();
  heap_insert(&a);
  heap_insert(&b);
  heap_insert(&c);
  heap_insert(&d);
  heap_insert(&e);
  heap_insert(&f);
  heap_insert(&g);  

  retVal = heap_pop();
  assert(retVal->weight == 1);
  retVal = heap_pop();
  assert(retVal->weight == 2);
  retVal = heap_pop();
  assert(retVal->weight == 3);
  retVal = heap_pop();
  assert(retVal->weight == 14);
  retVal = heap_pop();
  assert(retVal->weight == 16);
  retVal = heap_pop();
  assert(retVal->weight == 25);
  retVal = heap_pop();
  assert(retVal->weight == 27);
  assert(heap_empty());
  heap_finalize();
}

void test_heap_empty() {
  struct node_t a;
  NODE_INIT(a, 3, 'a');
  heap_initialize();
  assert(heap_empty());
  heap_insert(&a);
  assert(!heap_empty());
  heap_finalize();
}

void test_heap_build() {
  struct node_t a;
  struct node_t b;
  struct node_t c;
  struct node_t d;
  struct node_t e;
  struct node_t f;
  struct node_t g;
  struct node_t* arr[7];
  struct node_t* retVal;

  NODE_INIT(a, 100, 'a')
  NODE_INIT(b, 98, 'b')
  NODE_INIT(c, 102, 'c')
  NODE_INIT(d, 104, 'd')
  NODE_INIT(e, 99, 'e')
  NODE_INIT(f, 103, 'f')
  NODE_INIT(g, 140, 'g')

  arr[0] = &a;
  arr[1] = &b;
  arr[2] = &c;
  arr[3] = &d;
  arr[4] = &e;
  arr[5] = &f;
  arr[6] = &g;

  heap_build(arr);

  assert(heap_size() == 7);
  retVal = heap_pop();
  assert(retVal->weight == 98);
  retVal = heap_pop();
  assert(retVal->weight == 99);
  retVal = heap_pop();
  assert(retVal->weight == 100);
  retVal = heap_pop();
  assert(retVal->weight == 102);
  retVal = heap_pop();
  assert(retVal->weight == 103);
  retVal = heap_pop();
  assert(retVal->weight == 104);
  retVal = heap_pop();
  assert(retVal->weight == 140);
  
  heap_initialize();
  heap_finalize();
  
}

int main(int argc, char ** argv) {
  test_initialize_heap();
  test_initialize_node();
  test_heap_insert();
  test_heap_empty();
  test_heap_build();
  printf("Passed\n");
  return 0;
}
