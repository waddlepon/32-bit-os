#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include <stdint.h>

struct page_table_struct
{
    uint32_t * virt;
    uint32_t phys;
};

struct page_directory_struct
{
    uint32_t * virt;
    uint32_t phys;
    struct page_table_struct tables[1024];
};

void paging_init(uint32_t kernel_start, uint32_t kernel_end);
void map_page(uint32_t virt, uint32_t phys);
void unmap_page(uint32_t virt);

#endif
