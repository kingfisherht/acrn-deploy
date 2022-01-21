;dispstr.asm
[section .text]
global dispstr
dispstr:
    push ebp
    mov ebp,esp
    sub esp,0F0h
    mov edx,[ebp+12] ;字符串的长度
    mov ecx,[ebp+8];首字符的地址
    ;因为寄存器中存的是地址，所以要加方括号
    mov ebx,1
    mov eax,4
    int 80h
    leave ;作用等于下面两句
    ;mov esp,ebp
    ;pop ebp
    ret
