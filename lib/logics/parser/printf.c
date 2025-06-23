#include "parser.h"
#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
int print_counter = 0;

void bombaclat_asm(char* line) {
    static int string_id = 0;
    static int added_temp_str = 0;
    char temp[1024] = "";
    char strLabel[64];
    int noNewline = 0;

    START(line);
    for (int i = 0; i < 9; i++) ADV(line); // skip "bombaclat"

    // Cek "- bop"
    char* bop = strstr(line, "- bop");
    if (bop != NULL) {
        noNewline = 1;
        bop[0] = '\0'; // potong - bop
    }

    if (strchr(line, '"') != NULL) {
        // --- MODE STRING: tidak diubah sama sekali ---
        // skip sampai ketemu kutipan pertama
        while (cc != '"' && !EOP()) INC(line);
        if (cc == '"') INC(line);

        int i = 0;
        while (cc != '"' && !EOP()) {
            temp[i++] = cc;
            INC(line);
        }

        temp[i] = '\0';

        // Label unik
        sprintf(strLabel, "str_%d", string_id++);

        // Tulis ke tape_data
        char buffer[2048];
        if (noNewline) {
            sprintf(buffer,
                "%s db \"%s\", 0\n"
                "len_%s equ $ - %s\n",
                strLabel, temp, strLabel, strLabel
            );
        } else {
            sprintf(buffer,
                "%s db \"%s\", 10, 0\n"
                "len_%s equ $ - %s\n",
                strLabel, temp, strLabel, strLabel
            );
        }
        strcat(tape_data, buffer);

        // Tulis ke tape_text
        sprintf(buffer,
            "    mov eax, 4\n"
            "    mov ebx, 1\n"
            "    mov ecx, %s\n"
            "    mov edx, len_%s\n"
            "    int 0x80\n", strLabel, strLabel);
        strcat(tape_text, buffer);
 } else {
        // --- MODE VARIABEL ---
        ADV(line); // skip whitespace
        char var[64] = "";
        int vi = 0;
        DEC(line);
        while (cc != ' ' && cc != '\n' && !EOP()) {
            var[vi++] = cc;
            INC(line);
        }
        var[vi] = '\0';

        char* type = getType(var);

        if (type == NULL) {
            fprintf(stderr, "Error: variable '%s' not declared.\n", var);
            return; // atau exit(1)
        }

        // Tambahkan deklarasi temp_str sekali saja ke tape_bss
        if (!added_temp_str) {
            strcat(tape_bss, "temp_str: resb 16\n");
            added_temp_str = 1;
        }

        char buffer[4096];

        if (strcmp(type, "int") == 0) {
            // Print integer
            sprintf(buffer,
                "    ; print integer value of variable: %s\n"
                "    mov eax, [%s]\n"
                "    mov ebx, 10\n"
                "    mov ecx, temp_str\n"
                "    add ecx, 12\n"
                "    mov byte [ecx], 0\n"
                "    ; check if negative\n"
                "    cmp eax, 0\n"
                "    jge print_int_%s_%d\n"
                "    neg eax\n"
                "    dec ecx\n"
                "    mov byte [ecx], '-'\n"
                "print_int_%s_%d:\n"
                "    dec ecx\n"
                "    xor edx, edx\n"
                "    div ebx\n"
                "    add dl, '0'\n"
                "    mov [ecx], dl\n"
                "    test eax, eax\n"
                "    jnz print_int_%s_%d\n"
                "    mov eax, 4\n"
                "    mov ebx, 1\n"
                "    mov edx, 12\n"
                "    sub edx, ecx\n"
                "    mov ecx, ecx\n"
                "    int 0x80\n",
                var, var,
                var, print_counter++,
                var, print_counter,
                var, print_counter);

            // sprintf(buffer,
            //     "    ; print integer value of variable: %s\n"
            //     "    mov eax, [%s]\n"
            //     "    mov ebx, 10\n"
            //     "    mov ecx, temp_str\n"
            //     "    add ecx, 12\n"
            //     "    mov byte [ecx], 0\n"
            //     "print_int_%s_%d:\n"
            //     "    dec ecx\n"
            //     "    xor edx, edx\n"
            //     "    div ebx\n"
            //     "    add dl, '0'\n"
            //     "    mov [ecx], dl\n"
            //     "    test eax, eax\n"
            //     "    jnz print_int_%s_%d\n"
            //     "    mov eax, 4\n"
            //     "    mov ebx, 1\n"
            //     "    mov edx, 12\n"
            //     "    sub edx, ecx\n"
            //     "    mov ecx, ecx\n"
            //     "    int 0x80\n",
            //     var, var, var, print_counter++, var, print_counter);
            strcat(tape_text, buffer);

        } else if (strcmp(type, "float") == 0) {
            sprintf(buffer,
                "    ; print float value of variable: %s with 2 decimals\n"
                "    movss xmm0, dword [%s]\n"
                "    cvttss2si eax, xmm0\n"
                "    mov ebx, 10\n"
                "    mov ecx, temp_str\n"
                "    add ecx, 12\n"
                "    mov byte [ecx], 0\n"
                "print_int_%s:\n"
                "    dec ecx\n"
                "    xor edx, edx\n"
                "    div ebx\n"
                "    add dl, '0'\n"
                "    mov [ecx], dl\n"
                "    test eax, eax\n"
                "    jnz print_int_%s\n"
                "    mov eax, 4\n"
                "    mov ebx, 1\n"
                "    mov edx, 12\n"
                "    sub edx, ecx\n"
                "    mov ecx, ecx\n"
                "    int 0x80\n"
                "    ; print dot\n"
                "    mov eax, 4\n"
                "    mov ebx, 1\n"
                "    mov ecx, dot\n"
                "    mov edx, 1\n"
                "    int 0x80\n"
                "    ; print decimal part\n"
                "    movss xmm0, dword [%s]\n"
                "    subss xmm0, dword [int_part]\n"
                "    mulss xmm0, dword [hundred]\n"
                "    cvttss2si eax, xmm0\n"
                "    mov ecx, temp_str\n"
                "    add ecx, 12\n"
                "    mov byte [ecx], 0\n"
                "print_dec_%s:\n"
                "    dec ecx\n"
                "    xor edx, edx\n"
                "    div ebx\n"
                "    add dl, '0'\n"
                "    mov [ecx], dl\n"
                "    test eax, eax\n"
                "    jnz print_dec_%s\n"
                "    mov eax, 4\n"
                "    mov ebx, 1\n"
                "    mov edx, 12\n"
                "    sub edx, ecx\n"
                "    mov ecx, ecx\n"
                "    int 0x80\n"
                ,
                var,
                var, var, var,
                var,
                var, var
            );
            strcat(tape_text, buffer);

            // Pastikan juga di section .data ada deklarasi
            strcat(tape_data,
                "dot: db '.', 0\n"
                "hundred: dd 100.0\n"
                "int_part: dd 0.0\n"
            );
        }
        else if (strcmp(type, "char") == 0) {
            sprintf(buffer,
                "    ; print char value of variable: %s\n"
                "    mov eax, 4\n"
                "    mov ebx, 1\n"
                "    mov ecx, %s\n"
                "    mov edx, 1\n"
                "    int 0x80\n",
                var, var);
            strcat(tape_text, buffer);
        }
        else if (strcmp(type, "char*") == 0 || strcmp(type, "string") == 0) {
            sprintf(buffer,
                "    ; print string value of variable: %s\n"
                "    mov rsi, %s\n"
                "    xor rcx, rcx\n"
                "strlen_loop_%s:\n"
                "    mov al, [rsi + rcx]\n"
                "    cmp al, 0\n"
                "    je strlen_done_%s\n"
                "    inc rcx\n"
                "    jmp strlen_loop_%s\n"
                "strlen_done_%s:\n"
                "    mov rax, 1\n"
                "    mov rdi, 1\n"
                "    mov rsi, %s\n"
                "    mov rdx, rcx\n"
                "    syscall\n",
                var, var, var, var, var, var, var, var);
            strcat(tape_text, buffer);
        } else {
            // fallback print integer
            sprintf(buffer,
                "    ; print value of variable %s (unknown type '%s'), print as int\n"
                "    mov eax, [%s]\n"
                "    mov ebx, 10\n"
                "    mov ecx, temp_str\n"
                "    add ecx, 12\n"
                "    mov byte [ecx], 0\n"
                "print_int_%s:\n"
                "    dec ecx\n"
                "    xor edx, edx\n"
                "    div ebx\n"
                "    add dl, '0'\n"
                "    mov [ecx], dl\n"
                "    test eax, eax\n"
                "    jnz print_int_%s\n"
                "    mov eax, 4\n"
                "    mov ebx, 1\n"
                "    mov edx, 12\n"
                "    sub edx, ecx\n"
                "    mov ecx, ecx\n"
                "    int 0x80\n",
                var, type, var, var, var);
            strcat(tape_text, buffer);
        }

        // Tambahkan newline jika tidak ada flag - bop
        if (!noNewline) {
            if (!strstr(tape_data, "newline: db 10"))
                strcat(tape_data, "newline: db 10\n");

            strcat(tape_text,
                "    ; print newline\n"
                "    mov eax, 4\n"
                "    mov ebx, 1\n"
                "    mov ecx, newline\n"
                "    mov edx, 1\n"
                "    int 0x80\n"
            );
        }
    }
}