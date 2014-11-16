#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"
#include "node.h"
#include "time.h"
#include "input_module.h"
#include "stack.h"

#define SIZE UCHAR_MAX

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
  /*  assert(sizeof(struct node_t) == 8); */
  assert(sizeof(struct node_t) == 24);
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
  /* struct node_t a; */
  /* struct node_t b; */
  /* struct node_t c; */
  /* struct node_t d; */
  /* struct node_t e; */
  /* struct node_t f; */
  /* struct node_t g; */
  /* struct node_t* arr[7]; */
  /* struct node_t* retVal; */

  /* NODE_INIT(a, 100, 'a') */
  /* NODE_INIT(b, 98, 'b') */
  /* NODE_INIT(c, 102, 'c') */
  /* NODE_INIT(d, 104, 'd') */
  /* NODE_INIT(e, 99, 'e') */
  /* NODE_INIT(f, 103, 'f') */
  /* NODE_INIT(g, 140, 'g') */

  /* arr[0] = &a; */
  /* arr[1] = &b; */
  /* arr[2] = &c; */
  /* arr[3] = &d; */
  /* arr[4] = &e; */
  /* arr[5] = &f; */
  /* arr[6] = &g; */

  /* heap_build(arr); */

  /* assert(heap_size() == 7); */
  /* retVal = heap_pop(); */
  /* assert(retVal->weight == 98); */
  /* retVal = heap_pop(); */
  /* assert(retVal->weight == 99); */
  /* retVal = heap_pop(); */
  /* assert(retVal->weight == 100); */
  /* retVal = heap_pop(); */
  /* assert(retVal->weight == 102); */
  /* retVal = heap_pop(); */
  /* assert(retVal->weight == 103); */
  /* retVal = heap_pop(); */
  /* assert(retVal->weight == 104); */
  /* retVal = heap_pop(); */
  /* assert(retVal->weight == 140); */


  struct node_t * arr[SIZE];
  int i;
  int prev = 0;
  struct node_t * node;

  srand(time(NULL));

  for (i = 0; i < SIZE; ++i) {
    arr[i] = malloc(sizeof(struct node_t));
    if (!arr[i]) { perror("malloc"); exit(1); }
    arr[i]->weight = rand() % 500;
    arr[i]->c = i;
    arr[i]->left = 0;
    arr[i]->right = 0;
  }

  heap_initialize();
  heap_build(arr);
  
  node = heap_pop();
  prev = node->weight;
  free(node);
  while(!heap_empty()) {
    node = heap_pop();
    assert(prev <= node->weight);
    free(node);
  }

  heap_finalize();
}

void test_stack_initialize() {
  assert(stack_initialize() == SUCCESS);
  assert(stack_finalize() == SUCCESS);
}

void test_stack_empty() {
  assert(stack_initialize() == SUCCESS);
  assert(stack_empty());
  assert(stack_push(24) == SUCCESS);
  assert(!stack_empty());
  assert(stack_pop() == 24);
  assert(stack_empty());
  assert(stack_finalize() == SUCCESS);
}

void test_stack_push_pop() {
  int i = 0;
  assert(stack_initialize() == SUCCESS);
  
  for (; i < 100; ++i) {
    assert(stack_push(i) == SUCCESS);
  }

  for (i = 99; i >= 0; --i) {
    assert(stack_pop() == i);
  }

  assert(stack_finalize() == SUCCESS);
}

void test_b_len() {
  assert(b_len(12) == 4);
  assert(b_len(64) == 7);
  assert(b_len(66) == 7);
  assert(b_len(2) == 2);
  assert(b_len(1) == 1);
  assert(b_len(0) == 0);
}

void test_b_reverse() {
  assert(b_reverse(0) == 0);
  assert(b_reverse(1) == 1);
  assert(b_reverse(12) == 3);
  assert(b_reverse(64) == 1);
  assert(b_reverse(65) == 65);
}

void test_b_flip_code() {
  assert(b_flip_code(6) == 5);
  assert(b_flip_code(5) == 6);
  assert(b_flip_code(9) == 12);
  assert(b_flip_code(12) == 9);
}

int main(int argc, char ** argv) {
  test_initialize_heap(); printf("Passed test_initialize_heap\n");
  test_initialize_node(); printf("Passed test_initialize_node\n");
  test_heap_insert();     printf("Passed test_heap_insert\n");
  test_heap_empty();      printf("Passed test_heap_empty\n");
  test_heap_build();      printf("Passed test_heap_build\n");
  test_stack_initialize(); printf("Passed test_stack_initialize\n");
  test_stack_empty();     printf("Passed test_stack_empty\n");
  test_stack_push_pop();  printf("Passed test_stack_push_pop\n");
  test_b_len();           printf("Passed test_b_len\n");
  test_b_reverse();       printf("Passed test_b_reverse\n");
  test_b_flip_code();     printf("Passed test_b_flip_code\n");
  printf("Passed\n");
  return 0;
}
