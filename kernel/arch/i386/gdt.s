global load_gdt
global load_segment_registers

load_gdt:
    lgdt [esp + 4]
    ret

load_segment_registers:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    jmp 0x08:flush_cs

flush_cs:
    ret
