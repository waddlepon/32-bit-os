.global gdt_flush
.extern gdt_ptr

gdt_flush:  
    lgdt gdt_ptr

    movl %cr0, %eax
    or $1, %eax
    movl %eax, %cr0

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $flush

flush:
    ret
