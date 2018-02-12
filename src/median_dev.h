#ifndef MEDIAN_DEV_H
#define MEDIAN_DEV_H

struct median_dev;

struct median_dev* median_dev_init(void);
void median_dev_release(struct median_dev* dev);
unsigned median_dev_get(struct median_dev* dev, char*, unsigned);
unsigned median_dev_append(struct median_dev* dev, const char*, unsigned);

#endif //MEDIAN_DEV_H

