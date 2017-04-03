#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H

#include <stdint.h>
#include <stddef.h>

#include <kernel/multiboot.h>

struct pageframe
{
    uint16_t index;
    uint8_t bit;
};

void pmm_init(uint32_t kernel_start, uint32_t kernel_end, multiboot_info_t* mbt);
uint32_t alloc_frame();
void free_frame(uint32_t a);

#endif
