#include <stdint.h>
#include <string.h>

#include <kernel/idt.h>
#include <kernel/pic.h>

#include "keyboard.h"

#define IRQ_BASE 32

struct idt_entry
{
    uint16_t base_low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_struct
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

extern void idt_flush();

static struct idt_entry create_idt_entry(uint32_t base, uint16_t sel, uint8_t flags)
{
    struct idt_entry d;
    d.base_low = base & 0xFFFF;
    d.base_high = (base >> 16) & 0xFFFF;

    d.sel = sel;
    d.zero = 0;
    d.flags = flags;

    return d;
}

struct idt_ptr_struct idt_ptr;
struct idt_entry idt_entries[256];

void idt_init()
{
    idt_ptr.size = (sizeof(struct idt_entry) * 256) - 1;
    idt_ptr.offset = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

    idt_entries[0] = create_idt_entry((uint32_t)isr0, 0x08, 0x8E);
    idt_entries[1] = create_idt_entry((uint32_t)isr1, 0x08, 0x8E);
    idt_entries[2] = create_idt_entry((uint32_t)isr2, 0x08, 0x8E);
    idt_entries[3] = create_idt_entry((uint32_t)isr3, 0x08, 0x8E);
    idt_entries[4] = create_idt_entry((uint32_t)isr4, 0x08, 0x8E);
    idt_entries[5] = create_idt_entry((uint32_t)isr5, 0x08, 0x8E);
    idt_entries[6] = create_idt_entry((uint32_t)isr6, 0x08, 0x8E);
    idt_entries[7] = create_idt_entry((uint32_t)isr7, 0x08, 0x8E);
    idt_entries[8] = create_idt_entry((uint32_t)isr8, 0x08, 0x8E);
    idt_entries[9] = create_idt_entry((uint32_t)isr9, 0x08, 0x8E);
    idt_entries[10] = create_idt_entry((uint32_t)isr10, 0x08, 0x8E);
    idt_entries[11] = create_idt_entry((uint32_t)isr11, 0x08, 0x8E);
    idt_entries[12] = create_idt_entry((uint32_t)isr12, 0x08, 0x8E);
    idt_entries[13] = create_idt_entry((uint32_t)isr13, 0x08, 0x8E);
    idt_entries[14] = create_idt_entry((uint32_t)isr14, 0x08, 0x8E);
    idt_entries[15] = create_idt_entry((uint32_t)isr15, 0x08, 0x8E);
    idt_entries[16] = create_idt_entry((uint32_t)isr16, 0x08, 0x8E);
    idt_entries[17] = create_idt_entry((uint32_t)isr17, 0x08, 0x8E);
    idt_entries[18] = create_idt_entry((uint32_t)isr18, 0x08, 0x8E);
    idt_entries[19] = create_idt_entry((uint32_t)isr19, 0x08, 0x8E);
    idt_entries[20] = create_idt_entry((uint32_t)isr20, 0x08, 0x8E);
    idt_entries[21] = create_idt_entry((uint32_t)isr21, 0x08, 0x8E);
    idt_entries[22] = create_idt_entry((uint32_t)isr22, 0x08, 0x8E);
    idt_entries[23] = create_idt_entry((uint32_t)isr23, 0x08, 0x8E);
    idt_entries[24] = create_idt_entry((uint32_t)isr24, 0x08, 0x8E);
    idt_entries[25] = create_idt_entry((uint32_t)isr25, 0x08, 0x8E);
    idt_entries[26] = create_idt_entry((uint32_t)isr26, 0x08, 0x8E);
    idt_entries[27] = create_idt_entry((uint32_t)isr27, 0x08, 0x8E);
    idt_entries[28] = create_idt_entry((uint32_t)isr28, 0x08, 0x8E);
    idt_entries[29] = create_idt_entry((uint32_t)isr29, 0x08, 0x8E);
    idt_entries[30] = create_idt_entry((uint32_t)isr30, 0x08, 0x8E);
    idt_entries[31] = create_idt_entry((uint32_t)isr31, 0x08, 0x8E);

    idt_entries[32] = create_idt_entry((uint32_t)irq0, 0x08, 0x8E);
    idt_entries[33] = create_idt_entry((uint32_t)irq1, 0x08, 0x8E);
    idt_entries[34] = create_idt_entry((uint32_t)irq2, 0x08, 0x8E);
    idt_entries[35] = create_idt_entry((uint32_t)irq3, 0x08, 0x8E);
    idt_entries[36] = create_idt_entry((uint32_t)irq4, 0x08, 0x8E);
    idt_entries[37] = create_idt_entry((uint32_t)irq5, 0x08, 0x8E);
    idt_entries[38] = create_idt_entry((uint32_t)irq6, 0x08, 0x8E);
    idt_entries[39] = create_idt_entry((uint32_t)irq7, 0x08, 0x8E);
    idt_entries[40] = create_idt_entry((uint32_t)irq8, 0x08, 0x8E);
    idt_entries[41] = create_idt_entry((uint32_t)irq9, 0x08, 0x8E);
    idt_entries[42] = create_idt_entry((uint32_t)irq10, 0x08, 0x8E);
    idt_entries[43] = create_idt_entry((uint32_t)irq11, 0x08, 0x8E);
    idt_entries[44] = create_idt_entry((uint32_t)irq12, 0x08, 0x8E);
    idt_entries[45] = create_idt_entry((uint32_t)irq13, 0x08, 0x8E);
    idt_entries[46] = create_idt_entry((uint32_t)irq14, 0x08, 0x8E);
    idt_entries[47] = create_idt_entry((uint32_t)irq15, 0x08, 0x8E);

    idt_entries[48] = create_idt_entry((uint32_t)isr48, 0x08, 0x8E);
    idt_entries[49] = create_idt_entry((uint32_t)isr49, 0x08, 0x8E);

    idt_flush();
}

struct interrupt_regs
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));

void isr_handler(struct interrupt_regs *regs)
{
}

void irq_handler(struct interrupt_regs *regs)
{
    int irq = regs->int_no - IRQ_BASE;

    if (irq == 1)
    {
        keyboard_handle_char();
    }
    PIC_send_EOI(irq);
}
