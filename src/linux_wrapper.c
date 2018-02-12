#include "linux_wrapper.h"
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/string.h>

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

