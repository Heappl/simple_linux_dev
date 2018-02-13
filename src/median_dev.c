#include "linux/string.h"
#include "linux/kernel.h"
#include "median_dev.h"
#include "median_container.h"
#include "linux_wrapper.h"
#include "bigint.h"

typedef struct {} word_stream;
struct median_dev_impl
{
    median_container_t mc;
    char* input_buffer;
    char* output_buffer;
    unsigned input_size;
    unsigned output_size;
    int just_read;
};

int iswhitespace(char x)
{
    switch (x)
    {
        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
            return 1;
        default:
            return 0;
    }
}

string_view pop_next_word(char** begin, char* end)
{
    string_view ret = {*begin, 0};
    while (1)
    {
        if (*begin == end) return ret;
        if (iswhitespace(**begin))
        {
            **begin = '\0';
            (*begin)++;
            return ret;
        }
        (*begin)++;
        ret.len++;
    }
    return ret;
}

int bigint_less_wrapper(void* first, void* second)
{
    return bigint_less(*(bigint*)first, *(bigint*)second);
}

int bigint_greater_wrapper(void* first, void* second)
{
    return bigint_greater(*(bigint*)first, *(bigint*)second);
}

median_dev median_dev_init(void)
{
    median_dev ret = kernel_alloc(sizeof(median_dev));
    ret->mc = median_container_create(
        sizeof(bigint), bigint_less_wrapper, bigint_greater_wrapper, make_kernel_alloc());
    ret->input_size = 10 * 1024;
    ret->input_buffer = kernel_alloc(ret->input_size);
    ret->output_size = 1024;
    ret->output_buffer = kernel_alloc(ret->output_size);
    ret->just_read = 0;
    return ret;
}
void median_dev_release(median_dev dev)
{
    median_container_destroy(dev->mc);
    kernel_dealloc(dev->input_buffer);
    kernel_dealloc(dev->output_buffer);
    kernel_dealloc(dev);
}

string_view median_dev_get(median_dev dev)
{
    bigint lower, upper;
    int rem = 0;
    string_view ret = {dev->output_buffer, 0};
    if (dev->just_read)
    {
        dev->just_read = 0;
        return ret;
    }
    lower = *(bigint*)median_container_get_lower(dev->mc);
    upper = *(bigint*)median_container_get_upper(dev->mc);
    if (!lower)
        return ret;

    bigint median = bigint_copy(lower);
    bigint_add(median, upper);
    rem = bigint_div_u(median, 2);
    
    if (bigint_est_size(median) > dev->output_size)
    {
        kernel_dealloc(dev->output_buffer);
        dev->output_size = bigint_est_size(median);
        dev->output_buffer = kernel_alloc(dev->output_size);
    }
    ret.len = bigint_tostr(median, dev->output_buffer, dev->output_size);
    if (rem != 0)
    {
        dev->output_buffer[ret.len++] = '.';
        dev->output_buffer[ret.len++] = '5';
    }
    dev->just_read = 1;
    bigint_destroy(median);
    return ret;
}
char* median_dev_prepare_buff(median_dev dev, unsigned size)
{
    size++;
    if (dev->input_size < size)
    {
        kernel_dealloc(dev->input_buffer);
        dev->input_buffer = kernel_alloc(size);
        dev->input_size = size;
    }
    return dev->input_buffer;
}

void median_dev_append(median_dev dev, char* buff, unsigned size)
{
    char* end = buff + size;
    *end = '\0';
    while (buff != end)
    {
        string_view next_word = pop_next_word(&buff, end);
        bigint x = bigint_fromstr(next_word.ptr, next_word.len, make_kernel_alloc());
        median_container_insert(dev->mc, &x);
    }
}

