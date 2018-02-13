#include "bigint.h"
#include "array.h"
#include "allocator.h"
#include "linux_wrapper.h"

struct bigint_impl
{
    allocator_info alloc;
    char negative;
    array_t array;
};

bigint bigint_copy(bigint other)
{
    bigint ret = allocate(other->alloc, sizeof(struct bigint_impl));
    *ret = *other;
    ret->array = array_copy(other->array);
    return ret;
}

bigint bigint_create(int x, allocator_info alloc)
{
    bigint ret = allocate(alloc, sizeof(struct bigint_impl));
    if (x < 0)
    {
        ret->negative = 1;
        x = -x;
    }
    else
    {
        ret->negative = 0;
    }
    ret->array = array_create(sizeof(uint32t), alloc);
    ret->alloc = alloc;
    array_push(ret->array, &x);
    return ret;
}

void bigint_destroy(bigint x)
{
    array_destroy(x->array);
    deallocate(x->alloc, x);
}

int bigint_less(bigint first, bigint second)
{
    if (array_size(first->array) < array_size(second->array))
        return 1;
    if (array_size(first->array) > array_size(second->array))
        return 0;
    int s = array_size(first->array);
    int i;
    for (i = s - 1; i >= 0; --i)
    {
        uint32t first_curr = *(uint32t*)array_get(first->array, i);
        uint32t second_curr = *(uint32t*)array_get(second->array, i);
        if (first_curr == second_curr) continue;
        return first_curr < second_curr;
    }
    return 0;
}

int bigint_greater(bigint first, bigint second)
{
    if (array_size(first->array) > array_size(second->array))
        return 1;
    if (array_size(first->array) < array_size(second->array))
        return 0;
    int s = array_size(first->array);
    int i;
    for (i = s - 1; i >= 0; --i)
    {
        uint32t first_curr = *(uint32t*)array_get(first->array, i);
        uint32t second_curr = *(uint32t*)array_get(second->array, i);
        if (first_curr == second_curr) continue;
        return first_curr > second_curr;
    }
    return 0;
}

void bigint_mult(bigint arg, int x)
{
    if (x < 0)
    {
        arg->negative = !arg->negative;
        x = -x;
    }
    int i;
    uint64t carry = 0;
    for (i = 0; i < array_size(arg->array); ++i)
    {
        uint32t* curr = (uint32t*)array_get(arg->array, i);
        uint64t aux = *curr;
        aux *= x;
        aux += carry;
        *curr = aux & 0xFFFFFFFFllu;
        aux = aux >> (sizeof(uint32t) * __CHAR_BIT__);
        carry = aux;
    }
    if (carry > 0)
    {
        uint32t aux = carry;
        array_push(arg->array, &aux);
    }
}

int bigint_iszero(bigint arg)
{
    for (int i = 0; i < array_size(arg->array); ++i)
        if (*(uint32t*)array_get(arg->array, i) != 0)
            return 0;
    return 1;
}

void bigint_impl_reverse(char* begin, char* end)
{
    --end;
    while (begin != end)
    {
        char aux = *begin;
        *begin = *end;
        *end = aux;
        ++begin;
        if (begin == end) break;
        --end;
    }
}

uint32t bigint_impl_div(bigint x, uint32t by)
{
    int s = array_size(x->array);
    uint32t reminder = 0;
    for (int i = s - 1; i >= 0; --i)
    {
        uint32t* curr = (uint32t*)array_get(x->array, i);
        uint64t aux = reminder;
        aux = aux << (sizeof(uint32t) * __CHAR_BIT__);
        aux += *curr;
        reminder = aux % by;
        aux /= by;
        if (aux == 0 && i + 1 == array_size(x->array))
            array_pop(x->array);
        else
            *curr = aux;
    }
    return reminder;;
}

void bigint_positive_tostr(bigint x, char* buffer, int size)
{
    int i = 0;
    bigint xcopy = bigint_copy(x);
    while ((i + 1 < size) && !bigint_iszero(xcopy))
    {
        uint32t r = bigint_impl_div(xcopy, 10);
        buffer[i++] = (char)r + '0';
    }
    bigint_impl_reverse(buffer, buffer + i);
    buffer[i] = '\0';
}

int bigint_toint(bigint x)
{
    uint32t aux = *(uint32t*)array_get(x->array, 0);
    if (x->negative) return -(int)aux;
    return (int)aux;
}

void bigint_tostr(bigint x, char* buffer, int size)
{
    int i = 0;
    if (x->negative && (i + 1 < size))
        buffer[i++] = '-';
    if (bigint_iszero(x))
    {
        buffer[i++] = '0';
        buffer[i] = '\0';
    }
    else
        bigint_positive_tostr(x, buffer + i, size - i);
}

void bigint_add(bigint arg, unsigned x)
{
    int i;
    uint64t carry = x;
    for (i = 0; i < array_size(arg->array); ++i)
    {
        uint32t* curr = (uint32t*)array_get(arg->array, i);
        uint64t aux = *curr;
        aux += carry;
        *curr = aux & 0xFFFFFFFFllu;
        aux = aux >> (sizeof(uint32t) * __CHAR_BIT__);
        carry = aux;
        if (carry == 0) break;
    }
    if (carry > 0)
    {
        uint32t aux = carry;
        array_push(arg->array, &aux);
    }
}

bigint bigint_fromstr(const char* buffer, int size, allocator_info alloc)
{
    bigint ret = bigint_create(0, alloc);
    for (int i = 0; i < size; ++i)
    {
        bigint_mult(ret, 10);
        bigint_add(ret, (int)(buffer[i] - '0'));
    }
    return ret;
}

