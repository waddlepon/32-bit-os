#ifndef _KERNEL_INTERRUPTS_H
#define _KERNEL_INTERRUPTS_H

#include <stdint.h>

#define IRQ_BASE 32

struct interrupt_regs
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));

typedef void (*isr)(struct interrupt_regs *);

void enable_interrupts();
void disable_interrupts();

void register_interrupt(uint8_t i, isr handler);

#endif
