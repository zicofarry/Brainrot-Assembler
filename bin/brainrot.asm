section .data
n: dd 0
i: dd 0
j: dd 0
str_0 db " ", 0
len_str_0 equ $ - str_0
k: dd 0
str_1 db "*", 0
len_str_1 equ $ - str_1
str_2 db "", 10, 0
len_str_2 equ $ - str_2

section .bss
input_buffer_n: resb 100

section .text
global _start

_start:
    ; gyatt input int ke n
    mov eax, 0
    mov edi, 0
    mov rsi, input_buffer_n
    mov edx, 100
    syscall
    xor rbx, rbx
    xor rcx, rcx
    mov rdx, 0
    mov al, [input_buffer_n + rcx]
    cmp al, '-'
    jne parse_int_loop_n
    mov rdx, 1
    inc rcx
parse_int_loop_n:
    mov al, [input_buffer_n + rcx]
    cmp al, 10
    je end_parse_int_n
    cmp al, 13
    je end_parse_int_n
    cmp al, '0'
    jb end_parse_int_n
    cmp al, '9'
    ja end_parse_int_n
    sub al, '0'
    imul rbx, rbx, 10
    add rbx, rax
    inc rcx
    jmp parse_int_loop_n
end_parse_int_n:
    cmp rdx, 1
    jne store_int_n
    neg rbx
store_int_n:
    mov dword [n], ebx
    ; declaration with operation: i is 1
    mov eax, 1
    mov [i], eax
WHILE_START_0:
    ; compare i cooked n
    mov eax, [i]
    mov ebx, [n]
    cmp eax, ebx
    setle al
    cmp al, 0
    je WHILE_END_0
    cmp al, 0
    ; declaration with operation: j is n - 1
    mov eax, [n]
    sub eax, 1
    mov [j], eax
WHILE_START_1:
    ; compare j mogged i
    mov eax, [j]
    mov ebx, [i]
    cmp eax, ebx
    setge al
    cmp al, 0
    je WHILE_END_1
    cmp al, 0
    mov eax, 4
    mov ebx, 1
    mov ecx, str_0
    mov edx, len_str_0
    int 0x80
    ; j -= 1
    mov eax, [j]
    sub eax, 1
    mov [j], eax
    jmp WHILE_START_1
WHILE_END_1:
    ; assignment j is 1  
    mov eax, 1
    mov [j], eax
    ; declaration with operation: k is 2 * i - 1
    mov eax, 2
    imul eax, [i]
    sub eax, 1
    mov [k], eax
WHILE_START_2:
    ; compare j cooked k
    mov eax, [j]
    mov ebx, [k]
    cmp eax, ebx
    setle al
    cmp al, 0
    je WHILE_END_2
    cmp al, 0
    mov eax, 4
    mov ebx, 1
    mov ecx, str_1
    mov edx, len_str_1
    int 0x80
    ; j += 1
    mov eax, [j]
    add eax, 1
    mov [j], eax
    jmp WHILE_START_2
WHILE_END_2:
    mov eax, 4
    mov ebx, 1
    mov ecx, str_2
    mov edx, len_str_2
    int 0x80
    ; i += 1
    mov eax, [i]
    add eax, 1
    mov [i], eax
    jmp WHILE_START_0
WHILE_END_0:
    mov eax, 1
    xor ebx, ebx
    int 0x80
