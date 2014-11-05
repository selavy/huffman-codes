#include "compress.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "heap.h"
#include "node.h"
#include "return_codes.h"

#define CNT_SZ UCHAR_MAX
#define NOT_LEAF_NODE -1

static int cnt[CNT_SZ];
static struct node_t * huffman_tree;
static int conversion_map[CNT_SZ];

// error funcs
//------------------------------------------------------------------------------
static void print_unable_to_open_file_msg(char * filename);
static void print_unable_to_generate_huffman_tree();
static void print_unable_to_create_conversion_map();

// helper funcs
//------------------------------------------------------------------------------
static int get_counts();
static int generate_huffman_tree();
static int create_conversion_map();
static int module_finalize();
static void delete_huffman_tree();
#ifdef DEBUG
static void traverse_tree(struct node_t * node);
#endif
static void create_conversion_map_helper(struct node_t * node, int value);

//------------------------------------------------------------------------------
int compress(char * file_in, char * file_out) {
  huffman_tree = 0;
  if (get_counts(file_in) != SUCCESS) {
    print_unable_to_open_file_msg(file_in);
    module_finalize();
    return FAILURE;
  }
  if (generate_huffman_tree() != SUCCESS) {
    print_unable_to_generate_huffman_tree();
    module_finalize();
    return FAILURE;
  }
  if (create_conversion_map() != SUCCESS) {
    print_unable_to_create_conversion_map();
    module_finalize();
    return FAILURE;
  }
#ifdef DEBUG
  printf("TREE TRAVERSE BEGIN\n");
  traverse_tree(huffman_tree);
  printf("TREE_TRAVERSE END\n");
#endif
  module_finalize();
  return SUCCESS;
}

// error funcs
//------------------------------------------------------------------------------
void print_unable_to_open_file_msg(char * filename) {
  printf("Unable to open file: %s\n", filename);
}

void print_unable_to_generate_huffman_tree() {
  printf("Unable to generate huffman tree\n");
}

void print_unable_to_create_conversion_map() {
  printf("Unable to create conversion map\n");
}

// helper funcs
//------------------------------------------------------------------------------
int get_counts(char * file) {
  FILE * stream;
  int ch;
  bzero(&cnt[0], sizeof(cnt[0]) * CNT_SZ);
  stream = fopen(file, "r");
  if (!stream) {
    return FAILURE;
  }
  while((ch = fgetc(stream)) != EOF) {
    cnt[ch]++;
  }
  fclose(stream);
  return SUCCESS;
}

int generate_huffman_tree() {
  struct node_t * nodes[CNT_SZ];
  struct node_t * node;
  struct node_t * smaller;
  struct node_t * larger;
  struct node_t * new_node;
  int i;
  
  for (i = 0; i < CNT_SZ; ++i) {
    nodes[i] = malloc(sizeof(struct node_t));
    if (!nodes[i]) {
      for (i = i - 1; i >= 0; --i) {
	free(nodes[i]);
      }
      return FAILURE;
    }
    node         = nodes[i];
    node->left   = 0;
    node->right  = 0;
    node->weight = cnt[i];
    node->c      = i;
  }
  
  heap_initialize();
  heap_build(nodes);
  while(heap_size() > 1) {
    smaller = heap_pop();
    if (smaller->weight == 0) {
      free(smaller);
      continue;
    }
    larger = heap_pop();
    new_node = malloc(sizeof(struct node_t));
    if (!new_node) { /* TODO */ return FAILURE; }
    new_node->c = NOT_LEAF_NODE;
    new_node->left = smaller;
    new_node->right = larger;
#ifdef DEBUG
    printf("Combining SMALLER(%d) with LARGER(%d)\n", smaller->weight, larger->weight);
    if (smaller->c != NOT_LEAF_NODE) {
      printf("smaller char(%c)\n", smaller->c);
    }
    if (larger->c != NOT_LEAF_NODE) {
      printf("larger char(%c)\n", larger->c);
    }
#endif
    new_node->weight = smaller->weight + larger->weight;
    heap_insert(new_node);
  }
  huffman_tree = heap_pop();
#ifdef DEBUG
  printf("%d\n", huffman_tree->weight);
#endif
  heap_finalize();
  // clean up
  /* for (i = 0; i < CNT_SZ; ++i) { */
  /*   free(nodes[i]); */
  /*   nodes[i] = 0; */
  /* } */
  return SUCCESS;
}

int create_conversion_map() {
  bzero(&(conversion_map[0]), sizeof(conversion_map[0]));
  create_conversion_map_helper(huffman_tree, 1);
  return SUCCESS;
}

void create_conversion_map_helper(struct node_t * node, int value) {
  if (!node) { return; }
  create_conversion_map_helper(node->left, value * 10);
  create_conversion_map_helper(node->right, value * 10 + 1);
  if (node->c != NOT_LEAF_NODE) {
    conversion_map[node->c] = value;
    printf("Adding to conversion map: char(%c) ==> %d\n", node->c, value);
  }
}

int module_finalize() {
  delete_huffman_tree(huffman_tree);
  return SUCCESS;
}

/* TODO */
/* Convert this to BFS to avoid recursion */
void delete_huffman_tree(struct node_t * node) {
  if (!node) return;
#ifdef DEBUG
  printf("visiting %c => %d\n", (node->c != NOT_LEAF_NODE ? (char) node->c : '0'), node->weight);
#endif
  delete_huffman_tree(node->left);
  delete_huffman_tree(node->right);
  node->left = 0;
  node->right = 0;
  free(node);
}

#ifdef DEBUG
void traverse_tree(struct node_t * node) {
  if (!node) return;
  traverse_tree(node->left);
  if (node->c == NOT_LEAF_NODE) {
    printf("NOT LEAF\n");
  } else {
    printf("CHAR(%c)\n", node->c);
  }
  traverse_tree(node->right);
}
#endif
