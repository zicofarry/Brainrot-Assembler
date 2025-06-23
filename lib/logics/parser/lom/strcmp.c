#include "parser.h"

// prosedur untuk memproses token operator comparation, tapi untuk string
void parserStrcmp_asm(char* left, char* op_token, char* right, char* result_reg, char* out, int id) {
    out[0] = '\0';

    // generate label untuk string literal left dan right
    char label_left[64], label_right[64];
    sprintf(label_left, "str_left_%d", id);
    sprintf(label_right, "str_right_%d", id);

    // buat section data string literal
    sprintf(out + strlen(out),
        "section .data\n"
        "%s db \"%s\", 0\n"
        "%s db \"%s\", 0\n"
        "section .text\n"
        "    lea rsi, [%s]\n"
        "    lea rdi, [%s]\n"
        ".__strcmp_loop_%d:\n"
        "    mov al, [rsi]\n"
        "    mov bl, [rdi]\n"
        "    cmp al, 0\n"
        "    je .__strcmp_check_end_%d\n"
        "    cmp bl, 0\n"
        "    je .__strcmp_check_end_%d\n"
        "    cmp al, bl\n"
        "    jne .__strcmp_not_equal_%d\n"
        "    inc rsi\n"
        "    inc rdi\n"
        "    jmp .__strcmp_loop_%d\n"
        ".__strcmp_check_end_%d:\n"
        "    cmp al, bl\n"
        "    jne .__strcmp_not_equal_%d\n"
        "    mov eax, 0\n"
        "    jmp .__strcmp_done_%d\n"
        ".__strcmp_not_equal_%d:\n"
        "    mov eax, 1\n"
        ".__strcmp_done_%d:\n",
        label_left, left,
        label_right, right,
        label_left, label_right,
        id, id, id, id, id, id, id, id, id, id
    );

    // perbandingan hasil strcmp
    if (strcmp(op_token, "match") == 0)
        strcat(out, "    sete al\n");
    else if (strcmp(op_token, "hitdiff") == 0)
        strcat(out, "    setne al\n");
    else if (strcmp(op_token, "peak") == 0)
        strcat(out, "    setg al\n");
    else if (strcmp(op_token, "mid") == 0)
        strcat(out, "    setl al\n");
    else if (strcmp(op_token, "mogged") == 0)
        strcat(out, "    setge al\n");
    else if (strcmp(op_token, "cooked") == 0)
        strcat(out, "    setle al\n");

    // pindahkan hasil perbandingan ke register tujuan
    sprintf(out + strlen(out), "    movzx %s, al\n", result_reg);
}