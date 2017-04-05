#include <string.h>

#include <kernel/paging.h>
#include <kernel/pmm.h>

extern uint32_t * framemap;
extern uint32_t nframes;

extern uint32_t get_cr4();

struct page_directory_struct kernel_page_directory;
struct page_directory_struct * current_page_directory;

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

    kernel_page_directory.virt = (uint32_t *) page_directory_virt_addr;
    kernel_page_directory.phys = page_directory_phys_addr;

    for (uint32_t i = 0; i < 1024; i++)
    {
        kernel_page_directory.tables[i].virt = 0;
        kernel_page_directory.tables[i].phys = 0;
    }

    kernel_page_directory.tables[kernel_index].virt = (uint32_t *) 0xFFF00000;
    kernel_page_directory.tables[kernel_index].phys = page_table_phys_addr;

    kernel_page_directory.tables[1023].virt = (uint32_t *) 0xFFC00000;
    kernel_page_directory.tables[1023].phys = page_directory_phys_addr;

    current_page_directory = (struct page_directory_struct *) &kernel_page_directory;

    uint32_t new_cr4 = get_cr4() & ~(0x00000010);

    asm volatile (
            "mov %0, %%cr3\n\t"
            "mov %1, %%cr4"
            :
            : "r" (page_directory_phys_addr), "r" (new_cr4)
    );
}

void map_page(uint32_t virt, uint32_t phys)
{
    virt &= ~(0xFFF);
    phys &= ~(0xFFF);

    uint32_t pd_index = virt >> 22;
    uint32_t pt_index = (virt >> 12) & 0x3FF;

    uint32_t * pd = current_page_directory->virt;

    if ((pd[pd_index] & 0x1) == 0)
    {
        uint32_t table_phys_addr = alloc_frame();

        if (table_phys_addr == phys)
        {
            uint32_t t = table_phys_addr;
            table_phys_addr = alloc_frame();
            free_frame(t);
        }

        uint32_t * kernel_page_table = current_page_directory->tables[768].virt;

        uint32_t tmp = kernel_page_table[10];

        kernel_page_table[10] = table_phys_addr | 3;

        uint32_t * page_table = (uint32_t *) 0xC000A000;

        memset(page_table, 0, 4096);

        for (uint32_t i = 0; i < 1024; i++)
        {
            page_table[i] = 0 | 2;
        }

        pd[pd_index] = table_phys_addr | 3;

        uint32_t page_table_recursive_addr = 0xFFC00000 + (0x1000 * pd_index);

        current_page_directory->tables[pd_index].virt = (uint32_t *) page_table_recursive_addr;
        current_page_directory->tables[pd_index].phys = table_phys_addr;

        kernel_page_table[10] = tmp;
    }

    uint32_t * page_table = current_page_directory->tables[pd_index].virt;

    page_table[pt_index] = phys | (current_page_directory->virt[pd_index] & 0xFFF);

    asm volatile ("invlpg (%0)" : : "b" (virt) : "memory");
}

void unmap_page(uint32_t virt)
{
    virt &= ~(0xFFF);

    uint32_t pd_index = virt >> 22;
    uint32_t pt_index = (virt >> 12) & 0x3FF;

    uint32_t * page_directory = current_page_directory->virt;

    if (page_directory[pd_index] & 0x1)
    {
        uint32_t * page_table = current_page_directory->tables[pd_index].virt;

        page_table[pt_index] = 0;

        asm volatile ("invlpg (%0)" : : "b" (virt) : "memory");
    }
}
