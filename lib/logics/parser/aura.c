#include "parser.h"

// prosedur untuk memparsing lexeme aura
void parserAuraOperation_asm(char pita[]) {
    start(pita);

    char varname[100];
    strcpy(varname, getcw()); // ambil nama variabel

    inc(pita); // ke token kedua (operator+angka)
    char opAndNum[100];
    strcpy(opAndNum, getcw());

    char op = opAndNum[0]; // '+' atau '-'
    int val = atoi(&opAndNum[1]); // ambil angkanya

    inc(pita); // ke token 'aura'
    if (strcmp(getcw(), "aura") == 0) {
        // generate assembly untuk varname += val atau varname -= val
        char buf[256];
        if (op == '+') {
            sprintf(buf,
                "    ; %s += %d\n"
                "    mov eax, [%s]\n"
                "    add eax, %d\n"
                "    mov [%s], eax\n",
                varname, val, varname, val, varname);
            strcat(tape_text, buf);
        } else if (op == '-') {
            sprintf(buf,
                "    ; %s -= %d\n"
                "    mov eax, [%s]\n"
                "    sub eax, %d\n"
                "    mov [%s], eax\n",
                varname, val, varname, val, varname);
            strcat(tape_text, buf);
        } else {
            sprintf(buf, "    ; ERROR: operator aura tidak dikenali: %c\n", op);
            strcat(tape_text, buf);
        }
        
    } else {
        char buf[256];
        sprintf(buf, "    ; ERROR: harus ada keyword 'aura'!\n");
        strcat(tape_text, buf);
    }
}