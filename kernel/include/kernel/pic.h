#ifndef KERNEL_PIC_H
#define KERNEL_PIC_H

void PIC_init();
void PIC_remap(int offset1, int offset2);
void PIC_send_EOI(unsigned char irq);
void IRQ_set_mask(unsigned char irq_line);
void IRQ_clear_mask(unsigned char irq_line);
#endif
