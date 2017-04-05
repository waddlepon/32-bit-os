#ifndef _KERNEL_VMM_H
#define _KERNEL_VMM_H

#include <stdint.h>

void vmm_alloc(uint32_t virt);
void vmm_free(uint32_t virt);

#endif
