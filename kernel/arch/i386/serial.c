#include <string.h>
#include <stddef.h>

#include <kernel/serial.h>

#include "io.h"

#define PORT 0x3f8

void serial_init()
{
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x80);
    outb(PORT + 0, 0x03);
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x03);
    outb(PORT + 2, 0xC7);
    outb(PORT + 4, 0x0B);
}

static int is_transmit_empty()
{
    return inb(PORT + 5) & 0x20;
}

void serial_putchar(char c)
{
    while (is_transmit_empty() == 0);

    outb(PORT, c);
}

void serial_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++) serial_putchar(data[i]);
}

void serial_writestring(const char* data)
{
    serial_write(data, strlen(data));
}
