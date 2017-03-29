#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/paging.h>

void kernel_main(void)
{
    paging_init();
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

    uint32_t a = alloc_frame();
    uint32_t b = alloc_frame();
    uint32_t c = alloc_frame();

    printf("Welcome to asbestOS\n");
    printf("start address: %d\n", a);
    printf("next address: %d\n", b);
    printf("next address: %d\n", c);
    free_frame(b);
    uint32_t d = alloc_frame();
    printf("hole address: %d\n", d);
    for (;;)
    {
        asm("hlt");
    }
}
