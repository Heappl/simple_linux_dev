#include "median_dev.h"
#include "linux_wrapper.h"

struct median_dev
{
    int x;
};

struct median_dev* median_dev_init(void)
{
    return user_alloc(sizeof(struct median_dev));
}
void median_dev_release(struct median_dev* dev)
{
    user_dealloc(dev);
}

unsigned median_dev_get(struct median_dev* dev, char* buffer, unsigned length)
{
    buffer[0] = '1';
    buffer[1] = '0';
    /*if (dev == nullptr) return -1;*/
    /*int x = 10;*/
    /*std::memcpy(buffer, &x, sizeof(x));*/
    return 2;
}
unsigned median_dev_append(struct median_dev* dev, const char* b, unsigned l)
{
    return l;
}

