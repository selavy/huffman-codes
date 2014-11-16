#include "extract.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "return_codes.h"

FILE * in_stream;
FILE * out_stream;

int extract(char * file_in, char * file_out) {
  in_stream = 0;
  out_stream = 0;
  printf("[Extract] in = %s ; out = %s\n", file_in, file_out);
  return SUCCESS;
}

int open_input_file(char * filename) {
}
