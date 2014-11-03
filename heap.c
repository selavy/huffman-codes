#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "heap.h"
#include "node.h"

static struct node_t* arr[HEAP_SIZE];
static int sz;

int heap_initialize() {
  memset(&arr, 0, sizeof(struct node_t*) * HEAP_SIZE);
  sz = 0;
  return SUCCESS;
}

int heap_finalize() {
  sz = 0;
  return SUCCESS;
}

int heap_build(struct node_t * arr_in[HEAP_SIZE - 1]) {
  int hole;
  memcpy(&(arr[1]), &(arr_in[0]), sizeof(arr_in[0]) * (HEAP_SIZE - 1));
  sz = HEAP_SIZE - 1;
  for (hole = sz / 2; hole > 0; --hole) {
    // percolate down
    int child;
    struct node_t * tmp = arr[hole];

    for (; hole * 2 <= sz; hole = child) {
      child = hole * 2;
      if (child != sz && arr[child+1]->weight < arr[child]->weight) {
	++child;
      }
      if (arr[child]->weight < tmp->weight) {
	arr[hole] = arr[child];
      } else {
	break;
      }
    }
    
    arr[hole] = tmp;
    
  }
  return SUCCESS;
}

int heap_insert(struct node_t * node) {
  int hole;
  int x;
  if (sz == HEAP_SIZE) { return FAILURE; }
  hole = ++sz;
  x = node->weight;
  for ( ; hole > 1 && x < (arr[hole / 2])->weight; hole /= 2) {
    arr[hole] = arr[hole/2];
  }
  arr[hole] = node;
  return SUCCESS;
}

struct node_t * heap_pop() {
  int child;
  int hole = 1;
  struct node_t * retVal;
  struct node_t * tmp;
  if (sz == 0) { return 0; }
  retVal = arr[1];
  arr[1] = arr[sz--];
  // percolate down
  tmp = arr[1];
  for ( ; hole * 2 <= sz; hole = child) {
    child = hole * 2;
    if (child != sz && arr[child+1]->weight < arr[child]->weight) {
      ++child;
    }
    if (arr[child]->weight < tmp->weight) {
      arr[hole] = arr[child];
    } else {
      break;
    }
  }
  arr[hole] = tmp;
  return retVal;
}

int heap_empty() {
  return sz == 0;
}

void heap_print() {
  int i = 1;
  printf("HEAP PRINT\nSize = %d\n", sz);
  for ( ; i <= sz; ++i) { printf("%c ", arr[i]->c); }
  printf("\n");
}

int heap_size() {
  return sz;
}
