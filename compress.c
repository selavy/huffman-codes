#include "compress.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include <errno.h>
#include "stack.h"
#include "heap.h"
#include "node.h"
#include "binary_utility.h"
#include "return_codes.h"

#define CNT_SZ UCHAR_MAX
#define NOT_LEAF_NODE -1

static int cnt[CNT_SZ];
static struct node_t * huffman_tree;
static uint64_t conversion_map[CNT_SZ];
static int conversion_map_size;
static FILE * in_stream;
/* DEBUG */
static FILE * out_stream;

// error funcs
//------------------------------------------------------------------------------
static void print_unable_to_open_file_msg(char * filename);
static void print_unable_to_generate_huffman_tree();
static void print_unable_to_create_conversion_map();
static void print_unable_to_write_to_output();
static void print_unable_to_open_output_file(char * filename);

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
static void create_conversion_map_helper(struct node_t * node, uint64_t value, uint64_t level);
static int write_to_output();
static int write_number(int value, int * offset, unsigned char * curr_byte);
static int print_conversion_map_header();
static int open_output(char * filename);

// do work
//------------------------------------------------------------------------------
int compress(char * file_in, char * file_out) {
  huffman_tree = 0;
  out_stream = stdout;
  int i;
  conversion_map_size = 0;
  if (get_counts(file_in) != SUCCESS) {
    print_unable_to_open_file_msg(file_in);
    module_finalize();
    return FAILURE;
  }

  printf("--COUNTS--\n");
  for (i = 0; i < CNT_SZ; ++i) {
    if (cnt[i]) {
      printf("%c : %d\n", i, cnt[i]);
    }
  } 
  printf("--END COUNTS--\n");
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

  if (open_output(file_out) != SUCCESS) {
    print_unable_to_open_output_file(file_out);
    module_finalize();
    return FAILURE;
  }

  if (print_conversion_map_header() != SUCCESS) {
    print_unable_to_write_to_output();
    module_finalize();
    return FAILURE;
  }

  if (write_to_output() != SUCCESS) {
    print_unable_to_write_to_output();
    module_finalize();
    return FAILURE;
  }

  module_finalize();
  return SUCCESS;
}

// error funcs
//------------------------------------------------------------------------------
void print_unable_to_open_file_msg(char * filename) {
  fprintf(stderr, "Unable to open file: %s\n", filename);
}

void print_unable_to_generate_huffman_tree() {
  fprintf(stderr, "Unable to generate huffman tree\n");
}

void print_unable_to_create_conversion_map() {
  fprintf(stderr, "Unable to create conversion map\n");
}

void print_unable_to_write_to_output() {
  fprintf(stderr, "Unable to write to output\n");
}

void print_unable_to_open_output_file(char * filename) {
  fprintf(stderr, "Unable to open output file: %s\n", filename);
}

// helper funcs
//------------------------------------------------------------------------------
int get_counts(char * file) {
  int ch;
  memset(&cnt[0], '\0', sizeof(cnt[0] * CNT_SZ));
  in_stream = fopen(file, "r");
  if (!in_stream) {
    return FAILURE;
  }
  while((ch = fgetc(in_stream)) != EOF) {
    cnt[ch]++;
  }
  if (fseek(in_stream, 0, 0) != 0) {
    fprintf(stderr, "fseek error: %s\n", strerror(errno));
    return FAILURE;
  }
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
    if (!new_node) { 
      /* TODO */
      /* clean up nodes */
      return FAILURE;
    }
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
  heap_finalize();
  return SUCCESS;
}

int create_conversion_map() {
  memset(&conversion_map[0], 0, sizeof(conversion_map[0]));
  create_conversion_map_helper(huffman_tree, 0, 0);
  return SUCCESS;
}

/* REVISIT (plesslie) */
/* The maximum height of the tree is n-1 where         */
/* n is the number of distinct characters in the file. */
/* So, it _is_ possible that the tree would have a     */
/* height over 32, there by causing value to overflow  */
/* I need to find a way to handle this case.           */
void create_conversion_map_helper(struct node_t * node, uint64_t value, uint64_t level) {
  if (level > (sizeof(uint64_t) * 8)) {
    /* TODO */
    /* find a way to report error and exit gracefully */
    fprintf(stderr, "Height of huffman tree too large!\n Unable to create huffman tree");
    exit(0);
  }
  if (!node) {
    return;
  } else if (node->c != NOT_LEAF_NODE) {
    conversion_map[node->c] = value | (1 << level);
    ++conversion_map_size;
    printf("Adding to conversion map: char(%c) ==> %"PRIu64"\n", node->c, value | (1 << level));
  } else {
    create_conversion_map_helper(node->left, value, level + 1);
    create_conversion_map_helper(node->right, value | (1 << level), level + 1);
  }
}

int module_finalize() {
  if (in_stream) {
    fclose(in_stream);
  } 
  if (out_stream) {
    fclose(out_stream);
  }
  delete_huffman_tree(huffman_tree);
  return SUCCESS;
}

void delete_huffman_tree(struct node_t * node) {
  struct node_t * n;
  if (!node) {
    return;
  }
  stack_initialize();
  stack_push((int64_t) node);
  while (!stack_empty()) {
    n = (struct node_t *) stack_pop();
    if (n->left) {
      stack_push((int64_t) n->left);
    }
    if (n->right) {
      stack_push((int64_t) n->right);
    }
    free(n);
    n = 0;
  }
  stack_finalize();
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

int write_to_output() {
  int offset = 0;
  unsigned char byte = 0;
  int ch;
  if (!in_stream) {
    return FAILURE;
  }
  while((ch = fgetc(in_stream)) != EOF) {
    write_number(conversion_map[ch], &offset, &byte);
  }
  if (byte > 0) {
    printf("|%d\n", byte); 
    fprintf(out_stream, "%c", byte);
  }
  fprintf(out_stream, "%c", (char) offset);
  printf("offset = %d\n", offset);
  /* printf("\n"); */

  return SUCCESS;
}

int write_number(int value, int * curr_offset, unsigned char * curr_byte) {
  int offset = *curr_offset;
  unsigned char byte = *curr_byte;

  if (offset >= 8) {
    printf("|%d", byte);
    fprintf(out_stream, "%c", byte);
  }
  if (value == 0) {
    ++offset;
  } else {
    while (value > 1) {
      if (offset >= 8) {
	printf("|%d", byte);
	fprintf(out_stream, "%c", byte);
	byte = 0;
	offset = 0;
      }
      byte |= ((value & 1) << (7 - offset++));
      value >>= 1;
    }
  }
  if (offset >= 8) {
    printf("|%d", byte);
    fprintf(out_stream, "%c", byte);
    byte = 0;
    offset = 0;
  }

  *curr_offset = offset;
  *curr_byte = byte;
  return SUCCESS;
}

/* Header format: */
/* 4 bytes, size of header in 9 byte blocks   */
/* 9 byte blocks:                             */
/*   + 1 byte,  ascii character                */
/*   + 8 bytes, huffman encoding for character */
int print_conversion_map_header() {
  int i = 0;
  int j;
  char * p;
  uint64_t flipped_code;

  /* fprintf(out_stream, "|%d", conversion_map_size); */
  /* fprintf(out_stream, "%d", conversion_map_size); */
  p = (char*) &conversion_map_size;
  for (; i < sizeof(int); ++i) {
    fprintf(out_stream, "%c", *p++);
  }

  for (; i < CNT_SZ; ++i) {
    if (conversion_map[i] == 0) {
      continue;
    }
    /* fprintf(out_stream, "|%c|%d", (char) i, conversion_map[i]); */
    /* fprintf(out_stream, "%c%" PRIu64, (char) i, conversion_map[i]); */
    flipped_code = b_flip_code(conversion_map[i]);
    fprintf(out_stream, "%c", (char) i);
    /* p = (char*) &(conversion_map[i]); */
    p = (char*) &flipped_code;
    /* for (j = 0; j < sizeof(conversion_map[i]); ++j) { */
    for (j = 0; j < sizeof(flipped_code); ++j) {
      fprintf(out_stream, "%c", (char) *p++);
    }
  }
  return SUCCESS;
}

int open_output(char * filename) {
  out_stream = fopen(filename, "w");
  return (!!out_stream) ? SUCCESS : FAILURE;
}
