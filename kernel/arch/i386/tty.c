#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"
#include "io.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC00B8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_init(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_clear()
{
    terminal_row = 0;
    terminal_column = 0;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll()
{
    for (size_t y = 0; y < VGA_HEIGHT; ++y)
    {
        for (size_t x = 0; x < VGA_WIDTH; ++x)
        {
            if (y)
            {
                terminal_buffer[(y-1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
            }
            terminal_putentryat(' ', terminal_color, x, y);
        }
    }
    terminal_row = VGA_HEIGHT-1;
}

char terminal_get_char(int row, int col)
{
    const size_t index = row * VGA_WIDTH + col;
    return (char) (terminal_buffer[index] & 0x00FF);
}

size_t terminal_get_row()
{
    return terminal_row;
}

size_t terminal_get_column()
{
    return terminal_column;
}

uint32_t terminal_line_last_character(int row)
{
    for (size_t i = VGA_WIDTH - 2; i > 0 ; i--)
    {
        if (terminal_get_char(row, i) != ' ')
        {
            return i + 1;
        }
    }
    return 0;
}

void terminal_cursor_up()
{
    if (terminal_row == 0) return;

    terminal_row--;
    uint32_t last_character = terminal_line_last_character(terminal_row);
    if (last_character < terminal_column)
    {
        terminal_column = last_character;
    }
}

void terminal_cursor_down()
{
    if (terminal_row == VGA_HEIGHT - 1) return;

    terminal_row++;
    uint32_t last_character = terminal_line_last_character(terminal_row);
    if (last_character < terminal_column)
    {
        terminal_column = last_character;
    }
}

void terminal_cursor_left()
{
    if (terminal_column == 0) return;
    
    terminal_column--;
}

void terminal_cursor_right()
{
    if (terminal_column == VGA_WIDTH - 1) return;
    
    terminal_column++;
}

void terminal_update_cursor(int row, int col)
{
    unsigned short position = (row * VGA_WIDTH) + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) (position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        if (++terminal_row >= VGA_HEIGHT)
        {
            terminal_scroll();
        }
        terminal_column = 0;
        return;
    }

    if (c == '\b')
    {
        if (terminal_column == 0)
        {
            if (terminal_row == 0)
            {
                return;
            }
            else
            {
                terminal_row--;
                terminal_column = terminal_line_last_character(terminal_row); 
            }
        }
        else
        {
            terminal_column--;
        }
        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    if (++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT)
        {
            terminal_scroll();
        }
    }
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++) terminal_putchar(data[i]);
    terminal_update_cursor(terminal_row, terminal_column);
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}
