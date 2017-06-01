#include <stdio.h>

#include <kernel/panic.h>
#include <kernel/interrupts.h>
#include <kernel/tty.h>
#include <kernel/serial.h>
#include "vga.h"

void panic(char * msg)
{
    serial_writestring("PANIC: ");
    serial_writestring(msg);
    disable_interrupts();    
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE));
    terminal_clear();
    printf("An error has occured and AsbestOS has been halted to prevent damage to your computer.\n\n");
    printf("Technical Information:\n\n");
    printf("Error Description: ");
    printf(msg);
    for (;;)
    {
        asm("hlt");
    }
}
