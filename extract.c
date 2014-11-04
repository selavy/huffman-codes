#include "extract.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "return_codes.h"

int extract(char * file_in, char * file_out) {
  printf("[Extract] in = %s ; out = %s\n", file_in, file_out);
  return SUCCESS;
}
