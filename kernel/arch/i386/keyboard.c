#include <kernel/tty.h>

#include "io.h"

unsigned char scan_code_table_2[128] =
{
    //fill next
};		

char keyboard_get_scancode()
{
    char c = 0;
    do
    {
        if (inb(0x60) != c)
        {
            c = inb(0x60);
            if (c > 0) return c;
        }
    } while(1);
}

char keyboard_get_char()
{
    return scan_code_table_2[(int) keyboard_get_scancode()];
}

void keyboard_handle_char()
{
    char key = keyboard_get_char();
    terminal_putchar(key);
    terminal_update_cursor(terminal_get_row(), terminal_get_column());
}
