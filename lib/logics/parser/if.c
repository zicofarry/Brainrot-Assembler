#include "parser.h"
#include "../lexer/lexer.h"

int if_count = 0;


void goofy_asm(char* line) {
    static int if_count = 0;
    char cmp_code[2048] = "";
    char temp[256];
    char left[64], op_token[32], right[64];
    char logic_op[16];

    char skip_label[64];
    char true_label[64];
    // char end_label[64];
    sprintf(skip_label, "SKIP_IF_%d", if_count);
    sprintf(true_label, "TRUE_IF_%d", if_count);
    // sprintf(end_label, "END_IF_%d", if_count);
    if_count++;

    start(line);
    inc(line); // skip "goofy"

    int is_last = 0;
    int first_cond = 1;

    while (!is_last) {
        // Ambil token left
        char* token = getcw();
        int len = strlen(token);
        if(token[len - 1] == ':') {
            token[len - 1] = '\0';
            is_last = 1;
        }

        // Cek tipe string atau bukan
        if(getType(token) != NULL && strcmp(getType(token), "string") == 0) {
            strcpy(left, token);
            inc(line);
            strcpy(op_token, getcw());
            inc(line);
            strcpy(right, getcw());
            if(right[strlen(right) - 1] == ':') {
                right[strlen(right) - 1] = '\0';
                is_last = 1;
            }

            // parserStrcmp_asm(left, op_token, right, "al", temp, strcmp_id++);
            // For now pakai stub dummy
            strcpy(temp, "    ; strcmp dummy\n");
        } else {
            strcpy(left, token);
            inc(line);
            strcpy(op_token, getcw());
            inc(line);
            strcpy(right, getcw());
            if(right[strlen(right) - 1] == ':') {
                right[strlen(right) - 1] = '\0';
                is_last = 1;
            }

            parserOperator_asm(left, op_token, right, "al", temp);
        }

        strcat(cmp_code, temp);
        strcat(cmp_code, "    cmp al, 0\n");

        if(first_cond) {
            // Kondisi pertama → langsung lompat kalau gagal
            sprintf(temp, "    je %s\n", skip_label);
            strcat(cmp_code, temp);
        } else {
            // Logika selanjutnya
            if(strcmp(logic_op, "and") == 0) {
                sprintf(temp, "    je %s\n", skip_label);
                strcat(cmp_code, temp);
            } else if(strcmp(logic_op, "or") == 0) {
                sprintf(temp, "    jne %s\n", true_label);
                strcat(cmp_code, temp);
            }
        }

        // Ambil operator logika jika ada
        if (!is_last) {
            inc(line); // maju ke operator logika
            char* logic_token = getcw();
            if(strcmp(logic_token, "orNahh") == 0) {
                strcpy(logic_op, "or");
                inc(line);
            } else if(strcmp(logic_token, "butAlso") == 0) {
                strcpy(logic_op, "and");
                inc(line);
            } else {
                strcpy(logic_op, "none");
            }
        } else {
            strcpy(logic_op, "none");
        }

        first_cond = 0;
    }

    //  // Kalau ada OR, pasang label TRUE_IF di sini
    // if (strstr(cmp_code, true_label) != NULL) {
    //     sprintf(temp, "%s:\n", true_label);
    //     strcat(cmp_code, temp);
    // }

    // // Di sini kamu harus memasukkan kode blok IF TRUE secara manual di caller (atau tambahkan parameter fungsi)

    // // Setelah blok IF TRUE selesai, lompat ke END_IF
    // sprintf(temp, "    jmp %s\n", end_label);
    // strcat(cmp_code, temp);

    // // Label SKIP_IF (target lompat jika kondisi false)
    // sprintf(temp, "%s:\n", skip_label);
    // strcat(cmp_code, temp);

    // // Push label skip untuk ELSE IF atau ELSE nanti
    // pushLabel(skip_label, "if");

    // // Push label end untuk lompat ke sini setelah blok if benar selesai
    // pushLabel(end_label, "end_if");

    // strcat(tape_text, cmp_code);

    // Kalau ada OR, label true dipasang di sini
    if (strstr(cmp_code, true_label) != NULL) {
        sprintf(temp, "%s:\n", true_label);
        strcat(cmp_code, temp);
    }

    strcat(tape_text, cmp_code);
    pushLabel("", skip_label, "if");

    
}