#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h> 
#include <linux/init.h> 
#include "median_dev.h"
#include "linux_wrapper.h"

MODULE_LICENSE("GPL");

#define SUCCESS 0
#define FAIL -1
#define DEVICE_NAME "median"
#define CLASS_NAME "medianclass"

static median_dev dev = NULL;
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t* offset)
{
    string_view ret = median_dev_get(dev);
    copy_to_user(buffer, ret.ptr, ret.len);
    return ret.len;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    char* kbuf = median_dev_prepare_buff(dev, len);
    copy_from_user(kbuf, buff, len);
    median_dev_append(dev, kbuf, len);
    return len;
}

static int device_opened = 0;
static int device_open(struct inode *inode, struct file *file)
{
    if (device_opened++) return -EBUSY;
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    device_opened--;
    return 0;
}

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};
static int Major = -1;
static struct class* moduleClass = NULL;
static int median_module_init(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    moduleClass = class_create(THIS_MODULE, CLASS_NAME);
    if (moduleClass == NULL) {
        printk(KERN_ALERT "Creating device class failed\n");
        return Major;
    }
    if (device_create(moduleClass, NULL, MKDEV(Major, 0), NULL, "median") == NULL) {
        printk(KERN_ALERT "Creating device failed\n");
        return Major;
    }

    printk(KERN_INFO "Loaded media module with major number %d\n", Major);
    dev = median_dev_init();
    if (dev == NULL)
    {
        return FAIL;
    }
    
    return SUCCESS;
}

static void median_module_release(void)
{
    median_dev_release(dev);
    unregister_chrdev(Major, DEVICE_NAME);
}
module_init(median_module_init);
module_exit(median_module_release);

