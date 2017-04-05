#include <kernel/vmm.h>
#include <kernel/paging.h>
#include <kernel/pmm.h>

extern struct page_directory_struct * current_page_directory;

void vmm_alloc(uint32_t virt)
{
    uint32_t pd_index = virt >> 22;
    uint32_t pt_index = (virt >> 12) & 0x3FF;

    uint32_t * pd = current_page_directory->virt;
    uint32_t * pt = current_page_directory[pd_index].virt;

    if (pd[pd_index] & 0x1)
    {
        if ((pt[pt_index] & 0x1) == 0)
        {
            map_page(virt, alloc_frame());
        }
    }
    else
    {
        map_page(virt, alloc_frame());
    }
}

void vmm_free(uint32_t virt)
{
    uint32_t pd_index = virt >> 22;
    uint32_t pt_index = (virt >> 12) & 0x3FF;

    uint32_t * pt = current_page_directory[pd_index].virt;

    if (pt[pt_index] & 0x1)
    {
        free_frame(pt[pt_index] & 0xFFFFF000);
    }

    unmap_page(virt);
}
