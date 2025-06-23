#include "parser.h"
#include "../lexer/lexer.h"

// prosedur untuk memproses token tipe data string
void sigmaBoy_asm(char* line){
    start(line);
    inc(line);  // ambil nama variabel

    char var_name[100];
    strcpy(var_name, getcw());

    char* tipe = "string";
    addVariable(var_name, tipe);

    inc(line);

    if(strcmp(getcw(), "is") == 0){
        // Inisialisasi string: what the sigma rifa is "mama"
        inc(line);
        char* value = getcw();  // harus tanpa tanda kutip!

        int len = strlen(value);

        char asm_line[256];
        sprintf(asm_line,
            "%s db \"%s\", 0\n"
            "strlen_string_%s: equ %d\n",  // tambahkan panjang string
            var_name, value,
            var_name, len
        );

        strcat(tape_data, asm_line);
    } else {
        // Hanya deklarasi: what the sigma rifa
        char asm_line[128];
        sprintf(asm_line, "%s resb 101\n", var_name);
        strcat(tape_bss, asm_line);
    }
}