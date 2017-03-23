.global outb
.global inb
.global io_wait


outb:
    mov 8(%esp), %al
    mov 4(%esp), %dx
    out %al, %dx
    ret

inb:
    mov 4(%esp), %dx
    in %dx, %al
    ret

io_wait:
    mov $0x00, %al
    out %al, $0x80
    ret
