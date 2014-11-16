#include "extract.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <limits.h>
#include "return_codes.h"

#define CNT_SZ UCHAR_MAX
#define NOT_FOUND -1

/********************************************************************************/
/* Module global variables                                                      */
/********************************************************************************/
FILE * in_stream;
FILE * out_stream;
uint64_t conversion_map[CNT_SZ];

/********************************************************************************/
/* Helper functions                                                             */
/********************************************************************************/
static int open_input_file(char * filename);
static int module_finalize();
static int open_output_file(char * filename);
static int read_header();
static int read_file();
static int find_in_map(uint64_t n);

/********************************************************************************/
/* Do work                                                                      */
/********************************************************************************/
int extract(char * file_in, char * file_out) {
  in_stream = 0;
  out_stream = 0;
  printf("[Extract] in = %s ; out = %s\n", file_in, file_out);
  if (open_input_file(file_in) != SUCCESS) {
    printf("Unable to open input file!\n");
    module_finalize();
    return FAILURE;
  }
  if (open_output_file(file_out) != SUCCESS) {
    printf("Unable to open output file!\n");
    module_finalize();
    return FAILURE;
  }
  if (read_header() != SUCCESS) {
    printf("Unable to read header!\n");
    return FAILURE;
  }
  if (read_file() != SUCCESS) {
    printf("Unable to correctly read the file!\n");
    module_finalize();  
    return FAILURE;
  }
  module_finalize();
  return SUCCESS;
}

int open_input_file(char * filename) {
  in_stream = fopen(filename, "r");
  return (!!in_stream) ? SUCCESS : FAILURE;
}

int open_output_file(char * filename) {
  out_stream = fopen(filename, "w");
  return (!!out_stream) ? SUCCESS : FAILURE;
}

int module_finalize() {
  if (in_stream) {
    fclose(in_stream);
  }
  if (out_stream) {
    fclose(out_stream);
  }
  return SUCCESS;
}

int read_header() {
  int size = 0;
  int i = 0;
  int j;
  char c;
  uint64_t r;
  char * p;
  p = (char *) &size; 
  for (i = 0; i < sizeof(int); ++i) {
    if(fscanf(in_stream, "%c", p++) != 1) {
      return FAILURE;
    }
  }
  printf("----BEGIN HEADER----\n");
  printf("size = %d.\n", size);
  memset(&(conversion_map[0]), 0, sizeof(conversion_map[0]) * CNT_SZ);
  for (i = 0; i < size; ++i) {
    fscanf(in_stream, "%c", &c);
    p = (char *) &r;
    for (j = 0; j < sizeof(uint64_t); ++j) {
      if(fscanf(in_stream, "%c", p++) != 1) {
        return FAILURE;
      }
    }
    printf("%c --> %" PRIu64 "\n", c, r);
    conversion_map[c] = r;
  }
  printf("----END HEADER----\n");
  return SUCCESS;
}

int read_file() {
  /* The mark should be at the end of the header */
  /* and output file should be open.             */

  char c;
  int i;
  uint64_t val = 1;
  int res;
  printf("---READ FILE---\n");
  /* while ((c = fgetc(in_stream) != EOF)) { */
  while (fscanf(in_stream, "%c", &c) == 1) {
    printf("%d\n", c);
    for (i = 0; i < 8; ++i) {
      val = (val << 1) | (c & (1 << i));
      if ((res = find_in_map(val)) != NOT_FOUND) {
        printf("FOUND: %c\n", res);
        val = 1;
      }
    }
  }
  printf("---END READ FILE---\n");
  return SUCCESS;
}

int find_in_map(uint64_t n) {
  int i;
  for (i = 0; i < CNT_SZ; ++i) {
    if (conversion_map[i] == n) {
      return i;
    }
  }
  return NOT_FOUND;
}

