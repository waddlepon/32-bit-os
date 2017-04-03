#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include <stdint.h>

void paging_init(uint32_t kernel_start, uint32_t kernel_end);

#endif
