#include "linux_wrapper.h"
#include <cstring>
#include <iostream>

void* user_alloc(unsigned size)
{
    return malloc(size);
}

void user_dealloc(void* bfr)
{
    free(bfr);
}

void* kernel_alloc(unsigned size)
{
    return malloc(size);
}

void kernel_dealloc(void* bfr)
{
    free(bfr);
}

void kernel_error(const char* info)
{
    std::cerr << info << std::endl;
}

void kernel_info(const char* info)
{
    std::cout << info << std::endl;
}

void copy(char* dst, const char* src, unsigned bytes)
{
    std::memcpy(dst, src, bytes);
}

