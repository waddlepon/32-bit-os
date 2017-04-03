#include <string.h>

#include <kernel/paging.h>

extern uint32_t * framemap;
extern uint32_t nframes;

extern uint32_t get_cr4();

void paging_init(uint32_t kernel_start, uint32_t kernel_end)
{
    uint32_t page_directory_virt_addr = (uint32_t) framemap + nframes / 8;

    if (page_directory_virt_addr % 0x1000 != 0)
    {
        page_directory_virt_addr = (page_directory_virt_addr & ~(0xFFF)) + 0x1000;
    }

    uint32_t page_table_virt_addr = page_directory_virt_addr + 0x1000;

    uint32_t page_directory_phys_addr = page_directory_virt_addr - 0xC0000000;
    uint32_t page_table_phys_addr = page_table_virt_addr - 0xC0000000;

    uint32_t * page_directory_ptr = (uint32_t *) page_directory_virt_addr;

    memset(page_directory_ptr, 0, 0x1000);

    for (uint32_t i = 0; i < 1024; i++)
    {
        page_directory_ptr[i] = 0 | 2;
    }

    uint32_t * page_table_ptr = (uint32_t *) page_table_virt_addr;

    memset((uint8_t *) page_table_ptr, 0, 0x1000);

    for (uint32_t i = 0; i < 1024; i++)
    {
        page_table_ptr[i] = (i * 0x1000) | 3;
    }

    uint32_t kernel_index = 0xC0000000 >> 22;

    page_directory_ptr[kernel_index] = page_table_phys_addr | 3;

    page_directory_ptr[1023] = page_directory_phys_addr | 3;

    uint32_t new_cr4 = get_cr4() & ~(0x00000010);

    asm volatile (
            "mov %0, %%cr3\n\t"
            "mov %1, %%cr4"
            :
            : "r" (page_directory_phys_addr), "r" (new_cr4)
    );
}
