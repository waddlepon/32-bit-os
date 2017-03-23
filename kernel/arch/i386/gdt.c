#include <stdio.h>
#include <stdint.h>

#include <kernel/gdt.h>
 
#define GDT_NUM_ENTRIES 5

struct gdt_ptr_struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
 
struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags;
    uint8_t base_high;
} __attribute__((packed));

extern void gdt_flush();

static struct gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    struct gdt_entry g;

    g.base_low = (base & 0xFFFF);
    g.base_middle = (base >> 16) & 0xFF;
    g.base_high = (base >> 24) & 0xFF;

    g.limit_low = (limit & 0xFFFF);
    g.flags = (limit >> 16) & 0x0F;

    g.flags |= flags & 0xF0;
    g.access = access;

    return g;
}
 
struct gdt_ptr_struct gdt_ptr;
struct gdt_entry gdt_entries[GDT_NUM_ENTRIES];

void gdt_init()
{
    gdt_ptr.limit = (sizeof(struct gdt_entry) * GDT_NUM_ENTRIES) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;
    
    gdt_entries[0] = create_gdt_entry(0, 0, 0, 0);
    gdt_entries[1] = create_gdt_entry(0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_entries[2] = create_gdt_entry(0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_entries[3] = create_gdt_entry(0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_entries[4] = create_gdt_entry(0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush();
}
