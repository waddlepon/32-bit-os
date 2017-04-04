#include <kernel/keyboard.h>
#include <kernel/interrupts.h>
#include <kernel/tty.h>

#include "io.h"

int shift_down = 0;

static unsigned char scan_code_table_shift[] =
{
NULL, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
'\t', 'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   '{', '}', '\n', 0,
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,
F1,   F2, F3, F4, F5, F6, F7, F8, F9, F10, 0, 0,
HOME, UP, PGUP, '-', LEFT, '5',   RIGHT, '+', END, DOWN, PGDN, INS, DEL, 0, 0, 0, F11, F12
};

static unsigned char scan_code_table_noshift[] =
{
NULL, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
'\t', 'q', 'w',   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   '[', ']', '\n', 0,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0,
F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, 0, 0,
HOME, UP, PGUP,'-', LEFT, '5', RIGHT, '+', END, DOWN, PGDN, INS, DEL, 0, 0, 0, F11, F12
};

void keyboard_handle_char(struct interrupt_regs * regs)
{
    char scancode = inb(0x60);
    char key;

    if (scancode & 0x80)
    {
        scancode &= 0x7F;

        if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT)
        {
            shift_down = 0;
        }
        return;
    }
    else
    {
        if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT)
        {
            shift_down = 1;
            return;
        }
        
        if (shift_down)
        {
            key = scan_code_table_shift[scancode];
        }
        else
        {
            key = scan_code_table_noshift[scancode];
        }
    }

    terminal_putchar(key);
    terminal_update_cursor(terminal_get_row(), terminal_get_column());
}

void keyboard_init()
{
    register_interrupt(IRQ_BASE + 1,  &keyboard_handle_char);
}
