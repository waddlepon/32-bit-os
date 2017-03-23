#include <stdint.h>

#define FREE
#define USED

#define ERROR

static pageframe_t kalloc_frame_int()
{
    uint32_t i = 0;
    while (frame_map[i] != FREE)
    {
        i++;
        
        if (i == npages) return(ERROR);

    }
    frame_map[i] = USED;
    return (startframe + (i * 0x1000));
}

pageframe_t kalloc_frame()
{
    static uint8_t allocate = 1;
    static uint8_t pframe = 0;
    pageframe_t ret;

    if (pframe == 20) allocate = 1;

    if (allocate == 1)
    {
        for (int i = 0; i < 20; i++)
        {
            pre_frames[i] = kalloc_frame_int();
        }
        pframe = 0;
        allocate = 0;
    }
    ret = pre_frames[pframe];
    pframe++;
    return ret;
}
