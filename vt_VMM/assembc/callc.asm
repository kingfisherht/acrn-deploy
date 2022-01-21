extern func
section .text
    global _start
_start:
    push 1
    push 2
    call func
    add esp,8

    mov eax,1
    int 0x80

