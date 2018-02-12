#ifndef MEDIAN_DEV_H
#define MEDIAN_DEV_H

struct median_dev_impl;
typedef struct median_dev_impl* median_dev;

struct string_view_impl
{
    char* ptr;
    unsigned len;
};
typedef struct string_view_impl string_view;

median_dev median_dev_init(void);
void median_dev_release(median_dev dev);
string_view median_dev_get(median_dev dev);
char* median_dev_prepare_buff(median_dev dev, unsigned size);
void median_dev_append(median_dev, char*, unsigned size);

#endif //MEDIAN_DEV_H

