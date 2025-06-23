#include "parser.h"
#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>

void gyatt_asm(char* line) {
    START(line);
    for (int i = 0; i < 5; i++) ADV(line); // skip "gyatt"

    while (!EOP()) {
        while (cc == ' ' && !EOP()) INC(line);
        if (cc == '+') {
            INC(line);
            continue;
        }

        char temp[100] = "";
        int i = 0;
        while (cc != ' ' && cc != '+' && !EOP()) {
            temp[i++] = cc;
            INC(line);
        }
        temp[i] = '\0';

        char* tipe = getType(temp);
        if (tipe == NULL) {
            char err[128];
            sprintf(err, "; ERROR: variabel '%s' belum dideklarasikan!\n", temp);
            strcat(tape_text, err);
            continue;
        }

        // Tambahkan deklarasi buffer di .bss jika belum ada
        char bss_decl[128];
        sprintf(bss_decl, "input_buffer_%s: resb 100\n", temp);
        if (!strstr(tape_bss, bss_decl)) {
            strcat(tape_bss, bss_decl);
        }

        if (strcmp(tipe, "int") == 0) {
            char kode[2048];
            sprintf(kode,
                "    ; gyatt input int ke %s\n"
                "    mov eax, 0\n"
                "    mov edi, 0\n"
                "    mov rsi, input_buffer_%s\n"
                "    mov edx, 100\n"
                "    syscall\n"
                "    xor rbx, rbx\n"
                "    xor rcx, rcx\n"
                "    mov rdx, 0\n"  // flag negatif = 0
                "    mov al, [input_buffer_%s + rcx]\n"
                "    cmp al, '-'\n"
                "    jne parse_int_loop_%s\n"
                "    mov rdx, 1\n"
                "    inc rcx\n"
                "parse_int_loop_%s:\n"
                "    mov al, [input_buffer_%s + rcx]\n"
                "    cmp al, 10\n"
                "    je end_parse_int_%s\n"
                "    cmp al, 13\n"
                "    je end_parse_int_%s\n"
                "    cmp al, '0'\n"
                "    jb end_parse_int_%s\n"
                "    cmp al, '9'\n"
                "    ja end_parse_int_%s\n"
                "    sub al, '0'\n"
                "    imul rbx, rbx, 10\n"
                "    add rbx, rax\n"
                "    inc rcx\n"
                "    jmp parse_int_loop_%s\n"
                "end_parse_int_%s:\n"
                "    cmp rdx, 1\n"
                "    jne store_int_%s\n"
                "    neg rbx\n"
                "store_int_%s:\n"
                "    mov dword [%s], ebx\n",
                temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp);

            // sprintf(kode,
            //     "    ; gyatt input int ke %s\n"
            //     "    mov eax, 0\n"
            //     "    mov edi, 0\n"
            //     "    mov rsi, input_buffer_%s\n"
            //     "    mov edx, 100\n"
            //     "    syscall\n"
            //     "    xor rbx, rbx\n"
            //     "    xor rcx, rcx\n"
            //     "parse_int_%s:\n"
            //     "    mov al, [input_buffer_%s + rcx]\n"
            //     "    cmp al, 10\n"
            //     "    je end_parse_int_%s\n"
            //     "    cmp al, 13\n"
            //     "    je end_parse_int_%s\n"
            //     "    cmp al, '0'\n"
            //     "    jb end_parse_int_%s\n"
            //     "    cmp al, '9'\n"
            //     "    ja end_parse_int_%s\n"
            //     "    sub al, '0'\n"
            //     "    imul rbx, rbx, 10\n"
            //     "    add rbx, rax\n"
            //     "    inc rcx\n"
            //     "    jmp parse_int_%s\n"
            //     "end_parse_int_%s:\n"
            //     "    mov dword [%s], ebx\n",
            //     temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp);
            strcat(tape_text, kode);
        }
        else if (strcmp(tipe, "float") == 0) {
            // Tidak implementasi parsing float manual karena kompleks,
            // disarankan tetap pakai input buffer + nanti diformat C dahulu.
            char kode[1024];
            sprintf(kode,
                "    ; gyatt input float ke %s (DISARANKAN parse manual di C lalu mov dword [var], 0xHEX)\n"
                "    mov eax, 0\n"
                "    mov edi, 0\n"
                "    mov rsi, input_buffer_%s\n"
                "    mov edx, 100\n"
                "    syscall\n"
                "    ; [PARSE FLOAT DI C DULU -> ganti dengan nilai hex bit pattern nanti]\n",
                temp, temp
            );
            strcat(tape_text, kode);
        } else if (strcmp(tipe, "char") == 0) {
            char kode[512];
            sprintf(kode,
                "    ; gyatt input char ke %s\n"
                "    mov eax, 0\n"
                "    mov edi, 0\n"
                "    mov rsi, input_buffer_%s\n"
                "    mov edx, 2\n"
                "    syscall\n"
                "    mov al, [input_buffer_%s]\n"
                "    mov [%s], al\n",
                temp, temp, temp, temp
            );
            strcat(tape_text, kode);
        } else if (strcmp(tipe, "string") == 0) {
            char kode[2048];
            sprintf(kode,
                "    ; gyatt input string ke %s\n"
                "    mov eax, 0        ; syscall read\n"
                "    mov edi, 0        ; stdin\n"
                "    mov rsi, input_buffer_%s\n"
                "    mov edx, 100\n"
                "    syscall\n"
                "    ; cari newline dan ganti dengan null terminator\n"
                "    xor rcx, rcx\n"
                "find_newline_%s:\n"
                "    mov al, [input_buffer_%s + rcx]\n"
                "    cmp al, 0\n"
                "    je end_find_newline_%s\n"
                "    cmp al, 10\n"
                "    je replace_newline_%s\n"
                "    inc rcx\n"
                "    jmp find_newline_%s\n"
                "replace_newline_%s:\n"
                "    mov byte [input_buffer_%s + rcx], 0\n"
                "end_find_newline_%s:\n"
                "    ; salin input_buffer ke variabel string\n"
                "    mov esi, input_buffer_%s\n"
                "    mov edi, %s\n"
                "copy_%s:\n"
                "    lodsb\n"
                "    stosb\n"
                "    test al, al\n"
                "    jnz copy_%s\n",
                temp, temp,
                temp, temp, temp,
                temp, temp,
                temp, temp, temp,
                temp, temp,
                temp, temp
            );
            strcat(tape_text, kode);
        }
    }
}


// void gyatt_asm(char* line) {
//     START(line);
//     for (int i = 0; i < 5; i++) ADV(line); // skip "gyatt"

//     while (!EOP()) {
//         while (cc == ' ' && !EOP()) INC(line);
//         if (cc == '+') {
//             INC(line);
//             continue;
//         }

//         char temp[100] = "";
//         int i = 0;
//         while (cc != ' ' && cc != '+' && !EOP()) {
//             temp[i++] = cc;
//             INC(line);
//         }
//         temp[i] = '\0';

//         char* tipe = getType(temp);
//         if (tipe == NULL) {
//             char err[128];
//             sprintf(err, "; ERROR: variabel '%s' belum dideklarasikan!\n", temp);
//             strcat(tape_text, err);
//             continue;
//         }

//         // Baca input ke buffer (pseudo kode, karena assembly syscall)
//         char input_buf[100] = {0};
//         // Misal ambil input dari stdin pakai fgets (simulasi)
//         fgets(input_buf, 100, stdin);
//         // Hapus newline jika ada
//         size_t len = strlen(input_buf);
//         if (len > 0 && input_buf[len - 1] == '\n') input_buf[len - 1] = '\0';

//         // Parsing sesuai tipe
//         if (strcmp(tipe, "int") == 0) {
//             int val = atoi(input_buf);
//             char kode[128];
//             sprintf(kode, "    mov dword [%s], %d\n", temp, val);
//             strcat(tape_text, kode);
//         } else if (strcmp(tipe, "float") == 0) {
//             float val = atof(input_buf);
//             // float disimpan sebagai bit pattern ke memori (butuh cast)
//             union {
//                 float f;
//                 unsigned int u;
//             } fu;
//             fu.f = val;
//             char kode[128];
//             sprintf(kode, "    mov dword [%s], 0x%x\n", temp, fu.u);
//             strcat(tape_text, kode);
//         } else if (strcmp(tipe, "char") == 0) {
//             char val = input_buf[0];
//             char kode[128];
//             sprintf(kode, "    mov byte [%s], '%c'\n", temp, val);
//             strcat(tape_text, kode);
//         } else if (strcmp(tipe, "string") == 0) {
//             // char bss_name[64];
//             // sprintf(bss_name, "input_buffer_%s", temp);
//             // if (!strstr(tape_bss, bss_name)) {
//             //     char bss_decl[128];
//             //     sprintf(bss_decl, "input_buffer_%s: resb 100\n", temp);
//             //     strcat(tape_bss, bss_decl);
//             // }

//              // Tambahkan ke section .data
//             char data_decl[256];
//             sprintf(data_decl, "input_buffer_%s: db \"%s\", 0\n", temp, input_buf);
//             strcat(tape_data, data_decl);

//             // Salin string secara manual
//             char kode[1024];
//             sprintf(kode,
//                 "    ; copy string ke %s\n"
//                 "    mov esi, input_buffer_%s\n"
//                 "    mov edi, %s\n"
//                 "copy_%s:\n"
//                 "    lodsb\n"
//                 "    stosb\n"
//                 "    test al, al\n"
//                 "    jnz copy_%s\n",
//                 temp, temp, temp, temp, temp);
//             strcat(tape_text, kode);
//         }
//     }
// }