#include "linux/string.h"
#include "linux/kernel.h"
#include "median_dev.h"
#include "median_container.h"
#include "linux_wrapper.h"

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
    return ret;
    while (1)
    {
        if (*begin == end) break;
        if (iswhitespace(**begin))
        {
            **begin = '\0';
            (*begin)++;
            return ret;
        }
        ret.len++;
    }
    return ret;
}

int long_less(void* first, void* second)
{
    return *(long*)first < *(long*)second;
}
int long_greater(void* first, void* second)
{
    return *(long*)first > *(long*)second;
}

median_dev median_dev_init(void)
{
    median_dev ret = kernel_alloc(sizeof(median_dev));
    ret->mc = median_container_create(sizeof(long), long_less, long_greater, make_kernel_alloc());
    ret->input_size = 10 * 1024;
    ret->input_buffer = kernel_alloc(ret->input_size);
    ret->output_size = sizeof(long) * 4;
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
    string_view ret = {};
    if (dev->just_read)
    {
        dev->just_read = 0;
        return ret;
    }
    long* lower = median_container_get_lower(dev->mc);
    long* upper = median_container_get_upper(dev->mc);
    if (!lower)
        return ret;
    long median = (*lower + *upper) / 2;
    char* out = dev->output_buffer;
    if ((*upper - *lower) % 2 == 0)
        sprintf(out, "%li\n", median);
    else
        sprintf(out, "%li.5\n", median);
    ret.ptr = out;
    ret.len = strlen(out);
    dev->just_read = 1;
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
        printk(KERN_INFO "next word %s\n", next_word.ptr);
        long x;
        kstrtol(next_word.ptr, 10, &x);
        printk(KERN_INFO "inserted %li %s\n", x, buff);
        median_container_insert(dev->mc, &x);
        break;
    }
}

