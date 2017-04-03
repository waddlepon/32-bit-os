.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set MAGIC, 0x1BADB002
.set FLAGS, ALIGN | MEMINFO
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.align 0x1000

.section .stack, "aw", @nobits
stack_bottom:
.skip 16384
stack_top:

.set KERNEL_VIRTUAL_BASE, 0xC0000000
.set KERNEL_PAGE_NUM, (KERNEL_VIRTUAL_BASE >> 22);

.section .data
.align 0x1000
.global BootPageDirectory
BootPageDirectory:
    .long 0x00000083
    .fill (KERNEL_PAGE_NUM - 1), 4, 0x00000000
    .long 0x00000083
    .fill (1024 - KERNEL_PAGE_NUM - 1), 4, 0x00000000

.section .setup, "ax", @progbits
.global _start
_start:
    lea (BootPageDirectory - KERNEL_VIRTUAL_BASE), %ecx
    movl %ecx, %cr3

    #set PSE, 4mb pages
    movl %cr4, %ecx
    orl $0x00000010, %ecx
    movl %ecx, %cr4

    movl %cr0, %ecx
    orl $0x80000000, %ecx
    movl %ecx, %cr0

    lea (_startHigherHalf), %ecx
    jmp *%ecx
    

.section .text
_startHigherHalf:
    movl $0x00000000, (BootPageDirectory)
    invlpg (0)

    movl $stack_top, %esp

    call _init

    add $KERNEL_VIRTUAL_BASE, %ebx
    push %ebx

    cli

    call kernel_main

    .halt:
    jmp .halt
