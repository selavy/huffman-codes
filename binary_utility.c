#include "binary_utility.h"

int32_t b_len(uint64_t n) {
  int32_t retVal = 0;
  while (n > 0) {
    n >>= 1;
    ++retVal;
  }
  return retVal;
}
