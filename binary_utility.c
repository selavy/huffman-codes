#include "binary_utility.h"

/* DEBUG */
#include <stdio.h>
#include <inttypes.h>
/* GUBED */


int32_t b_len(uint64_t n) {
  int32_t retVal = 0;
  while (n > 0) {
    n >>= 1;
    ++retVal;
  }
  return retVal;
}

uint64_t b_reverse(uint64_t n) {
  int32_t len = b_len(n) - 1;
  uint64_t retVal = 0;
  while (n > 0) {
    retVal |= ((n & 1) << len--);
    n >>= 1;
  }
  return retVal;
}

uint64_t b_flip_code(uint64_t n) {
  int32_t const len = b_len(n) - 1;
  uint64_t const code = n & ~(1 << len);
  uint64_t const flipped_code = b_reverse(code);
  printf("len = %d; code = %" PRIu64"\n", len, code);
  return flipped_code | (1 << len);
}

uint64_t b_flip(uint64_t n, int32_t len) {
  return 0;
}
					  
