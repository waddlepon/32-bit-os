#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_init(void);
void terminal_clear();
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
char terminal_get_char(int row, int col);
size_t terminal_get_row();
size_t terminal_get_column();
uint32_t terminal_line_last_character(int row);
void terminal_cursor_up();
void terminal_cursor_down();
void terminal_cursor_left();
void terminal_cursor_right();
void terminal_update_cursor(int row, int col);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
