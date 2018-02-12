#include "linux_wrapper.h"
#include <linux/vmalloc.h>
#include <linux/kernel.h>

void* user_alloc(unsigned size)
{
    return vmalloc(size);
}

void user_dealloc(void* bfr)
{
    vfree(bfr);
}

void kernel_error(const char* info)
{
    printk(KERN_ALERT "%s\n", info);
}

void kernel_info(const char* info)
{
    printk(KERN_INFO "%s\n", info);
}
