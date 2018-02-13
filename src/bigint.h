#ifndef BIGINT_H
#define BIGINT_H

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bigint_impl;
typedef struct bigint_impl* bigint;

bigint bigint_create(int x, allocator_info alloc);
void bigint_destroy(bigint x);
int bigint_less(bigint first, bigint second);
int bigint_greater(bigint first, bigint second);
unsigned bigint_div_u(bigint arg, unsigned x);
void bigint_mult_i(bigint arg, int x);
void bigint_add_u(bigint arg, unsigned x);
void bigint_add(bigint arg, bigint other);

int bigint_tostr(bigint x, char* buffer, int size);
bigint bigint_fromstr(const char* buffer, int size, allocator_info alloc);
int bigint_toint(bigint x);
bigint bigint_copy(bigint x);
int bigint_est_size(bigint x);

#ifdef __cplusplus
}
#endif

#endif //BIGINT_H

