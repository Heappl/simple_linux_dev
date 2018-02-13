#include "linux_wrapper.h"
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/types.h>

int static_assert_on_u32[sizeof(u32) == sizeof(uint32_t)];
int static_assert_on_sizeof_uint32_t[4 == sizeof(uint32_t)];
int static_assert_on_u64[sizeof(u64) == sizeof(uint64_t)];
int static_assert_on_sizeof_uint64_t[4 == sizeof(uint64_t)];

void* user_alloc(unsigned size)
{
    return vmalloc(size);
}

void user_dealloc(void* bfr)
{
    vfree(bfr);
}

void* kernel_alloc(unsigned size)
{
    return kmalloc(size, GFP_KERNEL);
}

void kernel_dealloc(void* bfr)
{
    kfree(bfr);
}

void kernel_error(const char* info)
{
    printk(KERN_ALERT "%s\n", info);
}

void kernel_info(const char* info)
{
    printk(KERN_INFO "%s\n", info);
}

void copy(char* dst, const char* src, unsigned bytes)
{
    memcpy(dst, src, bytes);
}

