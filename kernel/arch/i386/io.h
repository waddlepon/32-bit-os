#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H 1

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void io_wait(void);

#endif
