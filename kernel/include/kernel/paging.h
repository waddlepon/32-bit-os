#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H
#include <stdint.h>

void paging_init();
uint32_t alloc_frame();
void free_frame(uint32_t a);
#endif
