#include <string.h>

#include <kernel/pmm.h>
#include <kernel/multiboot.h>

#define PAGE_SIZE 0x1000

extern uint32_t BootPageDirectory;

uint32_t *framemap;
uint32_t nframes;

uint32_t * boot_page_directory = (uint32_t*) &BootPageDirectory;

void pmm_init(uint32_t kernel_start, uint32_t kernel_end, multiboot_info_t *mbt)
{
    uint32_t memory_mb = mbt->mem_upper / 1024 + 2;
    uint32_t memory_kb = memory_mb * 1024;
    nframes = memory_kb / 4;

    framemap = (uint32_t *) ((kernel_end & ~(0xFFF)) + 0x1000);
    memset((uint8_t *) framemap, 0xFF, (nframes / 8));

    multiboot_memory_map_t * mmap = (multiboot_memory_map_t *) (mbt->mmap_addr + 0xC0000000);

    while ((uint32_t) mmap < mbt->mmap_addr + 0xC0000000 + mbt->mmap_length) 
    {
        if (mmap->type == 0x1)
        {
            for (uint32_t i = 0; i < ((mmap->len & 0x0000FFFF) / PAGE_SIZE); i++)
            {
                uint32_t bits = (((mmap->len & 0x0000FFFF) / 0x1000) + i * PAGE_SIZE) / PAGE_SIZE;
                framemap[(bits  - (bits % 32)) / 32] &= ((0x80000000 >> (bits % 32)) ^ 1);
            }
        }
        mmap = (multiboot_memory_map_t *) ((uint32_t) mmap + mmap->size + sizeof(uint32_t));
    }

    for (uint32_t i = 0; i < 1024; i++)
    {
        if (boot_page_directory[i] & 0x1)
        {
            uint32_t phys = boot_page_directory[i] & ~(0xFFF);
            for (uint32_t j = 0; j < 1024; j++)
            {
                uint32_t bits = (j * PAGE_SIZE + phys) / PAGE_SIZE;
                framemap[(bits - (bits % 32)) / 32] |= 0x80000000 >> (bits % 32);
            }
        }
    }
}

uint32_t alloc_frame()
{
    uint16_t index = 0;
    uint8_t bit = 0;

    uint32_t i = 0;
    while((framemap[i] & 0xFFFFFFFF) == 0xFFFFFFFF)
    {
        i++;
        if (i == (nframes/32 + 1))
        {
            return 0xFFFFFFFF;
        }
    }
    uint32_t b = 0x80000000;
    uint32_t test = framemap[i] ^ 1;
    int j = 0; 
    while((b & test) == 0)
    {
        j++;
        b >>= 1;
    }

    if (bit == 31)
    {
        index = i+1;
        bit = 0;
    }
    else
    {
        index = i;
        bit = j;
    }

    framemap[index] |= 0x80000000 >> bit;
    return (uint32_t) (32 * index + bit) * PAGE_SIZE;
}

void free_frame(uint32_t a)
{
    a = a & ~(0xFFF);
    a = a / PAGE_SIZE;
    uint8_t bit = a % 32;
    uint16_t index = (a - bit)/32;

    framemap[index] &= ((0x80000000 >> bit) ^ 1);
}
