#include <kernel/interrupts.h>
#include <kernel/pic.h>

isr interrupt_handlers[256];

void register_interrupt(uint8_t i, isr handler)
{
    interrupt_handlers[i] = handler;
}

void isr_handler(struct interrupt_regs *regs)
{
    if (interrupt_handlers[regs->int_no] != 0)
    {
        isr handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }
}

void irq_handler(struct interrupt_regs *regs)
{
    int irq = regs->int_no - IRQ_BASE;

    if (interrupt_handlers[regs->int_no] != 0)
    {
        isr handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }

    PIC_send_EOI(irq);
}
