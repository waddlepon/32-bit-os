#include <kernel/timer.h>

#include "io.h"

#define PIT_CHANNEL_0 0x40
#define PIT_CHANNEL_1 0x41
#define PIT_CHANNEL_2 0x42
#define PIT_COMMAND 0x43

uint32_t ticks = 0;

void timer_init(uint32_t frequency)
{
    register_interrupt(IRQ_BASE,  &timer_handler);
    
    set_timer_divisor(frequency); 
}

void set_timer_divisor(uint32_t frequency)
{
    uint32_t divisor = 1193180 / frequency;
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL_0, divisor & 0xFF);
    outb(PIT_CHANNEL_0, (divisor >> 8) & 0xFF);
}

void timer_handler(struct interrupt_regs * regs)
{
    ticks++;
}

uint32_t get_ticks()
{
    return ticks;
}
