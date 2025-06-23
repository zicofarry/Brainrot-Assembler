#include "parser.h"
#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>

// Menyimpan deklarasi variabel integer (skibidi)
void skibidi_asm(char* line) {
    start(line);        // mulai mesin kata
    inc(line);          // skip keyword "skibidi"

    char var_name[100];
    strcpy(var_name, getcw()); // ambil nama variabel

    // Simpan ke daftar variabel
    addVariable(var_name, "int");

    inc(line);

     if (strcmp(getcw(), "is") == 0) {
        inc(line);

        char rhs_full[256] = "";
        char token_list[10][64];
        int token_count = 0;

        // Ambil semua token setelah "is"
        while (!eop(line)) {
            strcpy(token_list[token_count++], getcw());
            if (strlen(rhs_full) > 0) strcat(rhs_full, " ");
            strcat(rhs_full, getcw());
            inc(line);
        }

        // Ambil token terakhir (kalau tidak terambil karena EOP)
        if (strlen(getcw()) > 0) {
            strcpy(token_list[token_count++], getcw());
            if (strlen(rhs_full) > 0) strcat(rhs_full, " ");
            strcat(rhs_full, getcw());
        }

        // .data section
        char data_declaration[128];
        sprintf(data_declaration, "%s: dd 0\n", var_name);
        strcat(tape_data, data_declaration);

        // .text section
        char instr[1024] = "";
        sprintf(instr, "    ; declaration with operation: %s is %s\n", var_name, rhs_full);

        // ✳️ Handle satu token saja
        if (token_count == 1) {
            char* value = token_list[0];
            // if (isdigit(value[0]))
            if (value[0] == '-' && isdigit(value[1]) || isdigit(value[0]))
                sprintf(instr + strlen(instr), "    mov eax, %s\n", value);
            else
                sprintf(instr + strlen(instr), "    mov eax, [%s]\n", value);

            sprintf(instr + strlen(instr), "    mov [%s], eax\n", var_name);
            strcat(tape_text, instr);
            return;
        }

        // Tokenisasi ekspresi panjang
        char operand1[64] = "", operand2[64] = "", op[8] = "";
        int token_index = 0;

        for (int i = 0; i < token_count; i++) {
            char* token = token_list[i];

            if (token_index == 0)
                strcpy(operand1, token);
            else if (token_index % 2 == 1)
                strcpy(op, token);
            else
                strcpy(operand2, token);

            if (token_index >= 2 && strlen(op) > 0 && strlen(operand2) > 0) {
                if (token_index == 2) {
                    if (isdigit(operand1[0]))
                        sprintf(instr + strlen(instr), "    mov eax, %s\n", operand1);
                    else
                        sprintf(instr + strlen(instr), "    mov eax, [%s]\n", operand1);
                }

                if (strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
                    strcat(instr, "    xor edx, edx\n");
                    if (isdigit(operand2[0]))
                        sprintf(instr + strlen(instr), "    mov ecx, %s\n", operand2);
                    else
                        sprintf(instr + strlen(instr), "    mov ecx, [%s]\n", operand2);
                    strcat(instr, "    idiv ecx\n");
                    if (strcmp(op, "%") == 0)
                        strcat(instr, "    mov eax, edx\n");
                } else {
                    if (isdigit(operand2[0])) {
                        sprintf(instr + strlen(instr), "    %s eax, %s\n",
                            strcmp(op, "+") == 0 ? "add" :
                            strcmp(op, "-") == 0 ? "sub" :
                            strcmp(op, "*") == 0 ? "imul" : "nop",
                            operand2);
                    } else {
                        sprintf(instr + strlen(instr), "    %s eax, [%s]\n",
                            strcmp(op, "+") == 0 ? "add" :
                            strcmp(op, "-") == 0 ? "sub" :
                            strcmp(op, "*") == 0 ? "imul" : "nop",
                            operand2);
                    }
                }

                strcpy(operand1, "eax");
                op[0] = '\0';
                operand2[0] = '\0';
            }

            token_index++;
        }

        sprintf(instr + strlen(instr), "    mov [%s], eax\n", var_name);
        strcat(tape_text, instr);

    } else {
        char data_declaration[128];
        sprintf(data_declaration, "%s: dd 0\n", var_name);
        strcat(tape_data, data_declaration);
    }
}