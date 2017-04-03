.global enable_interrupts

enable_interrupts:
    sti
    ret

.global disable_interrupts

disable_interrupts:
    cli
    ret

.extern isr_handler

.macro no_error_isr num
    .global isr\num
    isr\num:
        cli
        push $0
        push $\num
        jmp common_isr
.endm

.macro error_isr num
    .global isr\num
    isr\num:
        cli
        push $\num
        jmp common_isr
.endm

no_error_isr 0
no_error_isr 1
no_error_isr 2
no_error_isr 3
no_error_isr 4
no_error_isr 5
no_error_isr 6
no_error_isr 7
error_isr 8
no_error_isr 9
error_isr 10
error_isr 11
error_isr 12
error_isr 13
error_isr 14
no_error_isr 15
no_error_isr 16
error_isr 17
no_error_isr 18
no_error_isr 19
no_error_isr 20
no_error_isr 21
no_error_isr 22
no_error_isr 23
no_error_isr 24
no_error_isr 25
no_error_isr 26
no_error_isr 27
no_error_isr 28
no_error_isr 29
no_error_isr 30
no_error_isr 31

no_error_isr 48
no_error_isr 49

common_isr:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    call isr_handler
    popl %eax
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa
    add $8, %esp
    iret

.extern iqr_handler

.macro irq_entry irq num
    .global irq\irq
    irq\irq:
        cli
        push $0
        push $\num
        jmp common_irq
.endm

irq_entry 0, 32
irq_entry 1, 33
irq_entry 2, 34
irq_entry 3, 35
irq_entry 4, 36
irq_entry 5, 37
irq_entry 6, 38
irq_entry 7, 39
irq_entry 8, 40
irq_entry 9, 41
irq_entry 10, 42
irq_entry 11, 43
irq_entry 12, 44
irq_entry 13, 45
irq_entry 14, 46
irq_entry 15, 47

common_irq:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    call irq_handler
    popl %eax
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa
    add $8, %esp
    iret
