#include "segment.h"

#define DESCRIPTOR_COUNT 3

// Type Flags
#define SEG_PRES(x) ((x) << 0x07) // Present
#define SEG_PRIV(x) (((x) & 0x03) << 0x05) // Privilege Level (0-3)
#define SEG_DESCTYPE(x) ((x) << 0x04) // Descriptor type

#define SEG_CODE_EXRD 0x0A // Execute/Read
#define SEG_CODE_RDWR 0x02 // Read/Write

// Flags
#define SEG_GRAN(x) ((x) << 0x03)
#define SEG_SIZE(x) ((x) << 0x02)
#define SEG_LONG(x) ((x) << 0x01)
#define SEG_SAVL(x) (x)

#define GDT_CODE_PL0_TYPE_FLAGS SEG_PRES(1) | SEG_PRIV(0) | SEG_DESCTYPE(1) | SEG_CODE_EXRD

#define GDT_CODE_PL0_FLAGS SEG_GRAN(1) | SEG_SIZE(1) | SEG_LONG(0) | SEG_SAVL(0)

#define GDT_DATA_PL0_TYPE_FLAGS SEG_PRES(1) | SEG_PRIV(0) | SEG_DESCTYPE(1) | SEG_CODE_RDWR

#define GDT_DATA_PL0_FLAGS SEG_GRAN(1) | SEG_SIZE(1) | SEG_LONG (0) | SEG_SAVL(0)

struct GDTDescriptor create_descriptor(unsigned int base, unsigned int limit, unsigned char type_flags, unsigned char flags)
{
    struct GDTDescriptor d;
    d.limit_first = limit & 0xFFFF;
    d.base_first = base & 0xFFFF;
    d.base_second = (base >> 16) & 0x00FF;
    d.type_flags = type_flags;
    d.limit_flags = ((flags << 4) & 0xF0) | ((limit >> 16) & 0xF);
    d.base_third = (base >> 24) & 0xFF;
    return d;
}

void configure_gdt()
{
    struct GDTDescriptor descriptors[DESCRIPTOR_COUNT];
    descriptors[0] = create_descriptor(0, 0, 0, 0);    
    descriptors[1] = create_descriptor(0, 0xFFFFFFFF, (GDT_CODE_PL0_TYPE_FLAGS), (GDT_CODE_PL0_FLAGS));
    descriptors[2] = create_descriptor(0, 0xFFFFFFFF, (GDT_DATA_PL0_TYPE_FLAGS), (GDT_DATA_PL0_FLAGS));
    
    struct GDT table;
    table.address = (int)&descriptors;
    table.size = (sizeof(struct GDTDescriptor) * DESCRIPTOR_COUNT) - 1;

    load_gdt(table);
    load_segment_registers();
}
