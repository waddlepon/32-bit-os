#include <kernel/kmalloc.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>

uint32_t heap_start;
uint32_t heap_size;
uint32_t next;

void * kmalloc(size_t size, size_t align)
{

}

void kfree(void * address, size_t size, size_t align)
{
}
