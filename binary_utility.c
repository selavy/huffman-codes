#include "binary_utility.h"

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
