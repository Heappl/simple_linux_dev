#ifndef LINUX_WRAPPER_H
#define LINUX_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

void* user_alloc(unsigned size);
void user_dealloc(void* bfr);
void* kernel_alloc(unsigned size);
void kernel_dealloc(void* bfr);
void kernel_error(const char* info);
void kernel_info(const char* info);
void copy(char* dst, const char* src, unsigned bytes);

#ifdef __cplusplus
}
#endif

#endif // LINUX_WRAPPER_H

