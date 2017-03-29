#include <kernel/paging.h>
#include <stdio.h>

#define PAGE_SIZE 0x1000
#define INDEXES 32

extern uint32_t _kernel_end;

struct pageframe
{
    uint16_t index;
    uint8_t bit;
};

uint16_t hole = 0;
uint32_t framemap[INDEXES];
uint32_t startframe;
struct pageframe next_page;

void paging_init()
{
    startframe = _kernel_end;
    next_page.index = 0;
    next_page.bit = 0;
}

uint32_t alloc_frame()
{
    uint16_t index = 0;
    uint8_t bit = 0;

    if (hole)
    {
        int i = 0;
        while((framemap[i] & 0xFFFFFFFF) == 0xFFFFFFFF)
        {
            i++;
            if (i == INDEXES)
            {
                return 0;
            }
        }
        uint32_t b = 0x80000000;
        uint32_t test = framemap[i] ^ 1;
        int j = 0; 
        while((b & test) == 0)
        {
            j++;
            b >>= 1;
        }

        if (bit == 31)
        {
            index = i+1;
            bit = 0;
        }
        else
        {
            index = i;
            bit = j;
        }
        hole--;
    }
    else
    {
        index = next_page.index;
        bit = next_page.bit;

        if (bit == 31)
        {
            next_page.index = index + 1;
            next_page.bit = 0;
        }
        else
        {
            next_page.index = index;
            next_page.bit = bit + 1;
        }
    }

    framemap[index] |= 0x80000000 >> bit;
    return (uint32_t) startframe + (32 * index + bit) * PAGE_SIZE;
}

void free_frame(uint32_t a)
{
    a = (a - startframe)/PAGE_SIZE;
    uint8_t bit = a % 32;
    uint16_t index = (a - bit)/32;
    
    uint8_t test_bit;
    uint16_t test_index;

    if (next_page.bit == 0)
    {
        test_bit = 31;
        test_index = next_page.index - 1;
    }
    else
    {
        test_bit = next_page.bit - 1;
        test_index = next_page.index;
    }

    if (!(bit == test_bit && index == test_index))
    {
        hole++;
    }

    framemap[index] &= ((0x80000000 >> bit) ^ 1);
}
