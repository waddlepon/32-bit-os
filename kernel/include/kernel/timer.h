#ifndef _KERNEL_TIMER_H
#define _KERNEL_TIMER_H

#include <stdint.h>

#include <kernel/interrupts.h>

void timer_init(uint32_t frequency);
void set_timer_divisor(uint32_t frequency);
void timer_handler(struct interrupt_regs * regs);
uint32_t get_ticks();

#endif
