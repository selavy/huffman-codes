#include "extract.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include "return_codes.h"

FILE * in_stream;
FILE * out_stream;

/********************************************************************************/
/* Helper functions                                                             */
/********************************************************************************/
static int open_input_file(char * filename);
static int module_finalize();
static int open_output_file(char * filename);
static int read_header();

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
  printf("size = %d.\n", size);
  for (i = 0; i < size; ++i) {
    fscanf(in_stream, "%c", &c);
    p = (char *) &r;
    for (j = 0; j < sizeof(uint64_t); ++j) {
      if(fscanf(in_stream, "%c", p++) != 1) {
	return FAILURE;
      }
    }
    printf("%c --> %" PRIu64 "\n", c, r);
  }
  return SUCCESS;
}
