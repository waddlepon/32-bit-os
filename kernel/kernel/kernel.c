#include <stdio.h>
#include <stdint.h>

#include <kernel/multiboot.h>
#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/interrupts.h>
#include <kernel/pic.h>
#include <kernel/keyboard.h>
#include <kernel/pmm.h>
#include <kernel/paging.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

void kernel_main(multiboot_info_t *mbi)
{
    serial_init();
    serial_writestring("Serial initialized\n");

    gdt_init();
    serial_writestring("GDT initialized\n");
    idt_init();
    serial_writestring("IDT initialized\n");

    //paging stuff HAS to go after GDT and IDT tables
    pmm_init((uint32_t) &_kernel_start, (uint32_t) &_kernel_end, mbi);
    serial_writestring("PMM initialized \n");
    paging_init((uint32_t) &_kernel_start, (uint32_t) &_kernel_end);
    serial_writestring("Paging initialized \n");

    PIC_init(); 
    serial_writestring("PIC initialized\n");

    keyboard_init();

    terminal_init();
    serial_writestring("Terminal initialized\n");

    enable_interrupts();
    
    printf("Welcome to asbestOS\n");
    for (;;)
    {
        asm("hlt");
    }
}
