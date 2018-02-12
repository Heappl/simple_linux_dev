#include "median_dev.h"
#include "linux_wrapper.h"

typedef struct {} word_stream;
struct median_dev_impl
{
    /*buffer input, output;*/
    /*word_stream stream;*/
    /*ordered_map sofar;*/
    /*ordered_map_it median_it;*/
    int x;
};

void swap(char* x, char* y)
{
    *x ^= *y;
    *y ^= *x;
    *x ^= *y;
}

void reverse(char* begin, char* end)
{
    while (begin != end)
    {
        --end;
        if (begin == end) break;
        swap(begin, end);
    }
}

/*string_view int_to_stream(stream_buffer stream, long x)*/
/*{*/
    /*long i = 0;*/
    /*while (x > 0)*/
    /*{*/
        /*long dig = x % 10;*/
        /*buff[i++] = (char)dig;*/
        /*x /= 10;*/
    /*}*/
    /*reverse(buff, buff + i);*/
/*}*/

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
        if (*begin == end) break;
        if (iswhitespace(*(*begin)++))
            break;
        ret.len++;
    }
    return ret;
}

int long_cmp(void* first, void* second)
{
    return *(long*)first < *(long*)second;
}

median_dev median_dev_init(void)
{
    median_dev ret = kernel_alloc(sizeof(median_dev));
    /*ret->input = char_buffer_alloc(25);*/
    /*ret->output = stream_buffer_alloc(10 * 1024);*/
    /*ret->sofar = ordered_map_alloc(dev->sofar, sizeof(long), long_cmp);*/
    /*ret->stream = stream_buffer_init(buff, size);*/
    return ret;
}
void median_dev_release(median_dev dev)
{
    /*stream_buffer_dealloc(dev->input);*/
    /*stream_buffer_dealloc(dev->output);*/
    /*ordered_map_dealloc(dev->sofar);*/
    kernel_dealloc(dev);
}

string_view median_dev_get(median_dev dev)
{
    /*long median = ordered_map_(dev->sofar);*/
    /*return int_to_stream(dev->output, median);*/
}
char* median_dev_prepare_buff(median_dev dev, unsigned size)
{
    /*return stream_buffer_pack(dev->stream, size);*/
}

/*void update_median(ordered_map sofar, ordered_map_it& it, long x)*/
/*{*/
    /*if (x <= *(long*)order_map_get(it))*/
    /*{*/
    /*}*/
/*}*/
void median_dev_append(median_dev dev, char* buff, unsigned size)
{
    /*char* end = buff + size;*/
    /*while (true)*/
    /*{*/
        /*string_view next_word = pop_next_word(buff, end);*/
        /*long x;*/
        /*kstrtol(next_word.ptr, 10, &x);*/

        /*update_median(dev->sofar, dev->median_it, dev->median, x);*/
    /*}*/
}

