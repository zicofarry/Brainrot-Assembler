#include "parser.h"
#include "../lexer/lexer.h"

static int while_label_counter = 0;

void lingaguliguli_asm(char* line) {
    char start_label[64], end_label[64], true_label[64], temp[256];
    char cond_asm[2048] = "";
    char left[64], op_token[32], right[64], logic_op[16];
    int label_id = while_label_counter++;
    int is_last = 0, first_cond = 1;

    sprintf(start_label, "WHILE_START_%d", label_id);
    sprintf(end_label, "WHILE_END_%d", label_id);
    sprintf(true_label, "WHILE_TRUE_%d", label_id);

    strcat(tape_text, start_label);
    strcat(tape_text, ":\n");

    start(line);
    inc(line); // skip "lingaguliguli"

    while (!is_last) {
        memset(temp, 0, sizeof(temp));  // 🧼 Bersihkan temp
        char* token = getcw();
        int len = strlen(token);
        if (token[len - 1] == ':') {
            token[len - 1] = '\0';
            is_last = 1;
        }

        if (getType(token) != NULL && strcmp(getType(token), "string") == 0) {
            strcpy(left, token);
            inc(line);
            strcpy(op_token, getcw());
            inc(line);
            strcpy(right, getcw());
            if (right[strlen(right) - 1] == ':') {
                right[strlen(right) - 1] = '\0';
                is_last = 1;
            }

            // TODO: Tambahkan dukungan strcmp
            strcpy(temp, "    ; strcmp not yet supported\n");
        } else {
            strcpy(left, token);
            inc(line);
            strcpy(op_token, getcw());
            inc(line);
            strcpy(right, getcw());
            if (right[strlen(right) - 1] == ':') {
                right[strlen(right) - 1] = '\0';
                is_last = 1;
            }

            parserOperator_asm(left, op_token, right, "al", temp);
        }

        strcat(cond_asm, temp);
        strcat(cond_asm, "    cmp al, 0\n");

        if (first_cond) {
            sprintf(temp, "    je %s\n", end_label);
            strcat(cond_asm, temp);
        } else {
            if (strcmp(logic_op, "and") == 0) {
                sprintf(temp, "    je %s\n", end_label);
                strcat(cond_asm, temp);
            } else if (strcmp(logic_op, "or") == 0) {
                sprintf(temp, "    jne %s\n", true_label);
                strcat(cond_asm, temp);
            }
        }

        if (!is_last) {
            inc(line);
            char* logic_token = getcw();
            if (strcmp(logic_token, "orNahh") == 0) {
                strcpy(logic_op, "or");
                inc(line);
            } else if (strcmp(logic_token, "butAlso") == 0) {
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

    if (strstr(cond_asm, true_label) != NULL) {
        sprintf(temp, "%s:\n", true_label);
        strcat(cond_asm, temp);
    }

    strcat(tape_text, cond_asm);

    // strcat(tape_text, "    jmp ");
    // strcat(tape_text, start_label);
    // strcat(tape_text, "\n");


    pushLabel(start_label, end_label, "loop");

}
