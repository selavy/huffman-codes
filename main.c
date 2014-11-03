#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

#define prt_n_quit(msg) perror(msg); exit(1)

int main(int argc, char ** argv) {
  int * cnt = NULL;
  if (argc != 2)  usage("huffman [FILENAME]");
  if (Huffman_Initialize(UCHAR_MAX) != 0) prt_n_quit("Huffman_Initialize");
  if (get_char_counts(argv[1], &cnt) != 0) prt_n_quit("get_char_counts");
  free(cnt);
  Huffman_Finalize();
  return 0;
}
