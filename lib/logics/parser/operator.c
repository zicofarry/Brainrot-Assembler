#include "parser.h"

// prosedur untuk memproses token operator
void parserOperator_asm(char* op1, char* op_token, char* op2, char* result_reg, char* asm_code) {
    char op[10];
    if(strcmp(op_token, "peak") == 0)        strcpy(op, "g");   // >
    else if(strcmp(op_token, "mid") == 0)    strcpy(op, "l");   // <
    else if(strcmp(op_token, "mogged") == 0) strcpy(op, "ge");  // >=
    else if(strcmp(op_token, "cooked") == 0) strcpy(op, "le");  // <=
    else if(strcmp(op_token, "match") == 0)  strcpy(op, "e");   // ==
    else if(strcmp(op_token, "hitdiff") == 0)strcpy(op, "ne");  // !=
    else return;

    // Hapus isi awal
    asm_code[0] = '\0';

    strcat(asm_code, "    ; compare ");
    strcat(asm_code, op1);
    strcat(asm_code, " ");
    strcat(asm_code, op_token);
    strcat(asm_code, " ");
    strcat(asm_code, op2);
    strcat(asm_code, "\n");

    // Pindah operand ke eax dan ebx
    if (isdigit(op1[0]))
        sprintf(asm_code + strlen(asm_code), "    mov eax, %s\n", op1);
    else
        sprintf(asm_code + strlen(asm_code), "    mov eax, [%s]\n", op1);

    if (isdigit(op2[0]))
        sprintf(asm_code + strlen(asm_code), "    mov ebx, %s\n", op2);
    else
        sprintf(asm_code + strlen(asm_code), "    mov ebx, [%s]\n", op2);

    strcat(asm_code, "    cmp eax, ebx\n");

    // Buat setX instruksi
    sprintf(asm_code + strlen(asm_code), "    set%s %s\n", op, result_reg);

    // Ubah register byte ke 32-bit register penuh jika ingin konsisten
    // (misal: al → eax, bl → ebx)
    // tapi kalau boolean cukup pakai `al`, tak masalah
}