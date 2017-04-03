ENTRY(_start)
SECTIONS {
    . = 1M;

    .setup : ALIGN (4K)
    {
        *(.multiboot)
        *(.setup)
    }

    . += 0xC0000000;

    _kernel_start = .;

    .text : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
    }

    .rodata : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata)
    }

    .data : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
    }

    .bss : AT(ADDR(.bss) - 0xC0000000)
    {
        *(.bss)
        *(.stack)
    }

    _kernel_end = .;
}
