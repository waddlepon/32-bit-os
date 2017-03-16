#ifndef SEGMENT_H
#define SEGMENT_H
struct GDT
{
    unsigned int address;
    unsigned short size;
} __attribute__((packed));

struct GDTDescriptor
{
    unsigned short limit_first;
    unsigned short base_first;
    unsigned char base_second;
    unsigned char type_flags;
    unsigned char limit_flags;
    unsigned char base_third;
} __attribute__((packed));

void configure_gdt();

void load_gdt(struct GDT table);
void load_segment_registers();

#endif
