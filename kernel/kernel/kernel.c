#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>

void kernel_main(void)
{
    serial_init();
    serial_writestring("Serial initialized\n");
    terminal_init();
    serial_writestring("Terminal initialized\n");
    gdt_init();
    serial_writestring("GDT initialized\n");
    idt_init();
    serial_writestring("IDT initialized\n");
    PIC_init(); 
    serial_writestring("PIC initialized\n");
    enable_interrupts();

    printf("Welcome to asbestOS\n");
    for (;;)
    {
        asm("hlt");
    }
}
