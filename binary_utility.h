#ifndef BINARY_UTILITY_H_
#define BINARY_UTILITY_H_

#include <stdint.h>

int32_t  b_len(uint64_t n);
uint64_t b_reverse(uint64_t n);
uint64_t b_flip_code(uint64_t n);
uint64_t b_flip(uint64_t n, int32_t len);

#endif // BINARY_UTILITY_H_
