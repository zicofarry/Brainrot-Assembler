#include "parser.h"
#include "../lexer/lexer.h"

// prosedur untuk memparsing token tipe data float
void skibidiToilet_asm(char* line) {
    start(line);        // mulai mesin kata
    inc(line);          // skip keyword "skibidiToilet"

    char var_name[100];
    strcpy(var_name, getcw()); // ambil nama variabel

    // Simpan ke daftar variabel sebagai float
    addVariable(var_name, "float");

    inc(line);

    if (strcmp(getcw(), "is") == 0) {
        inc(line);
        char rhs[256] = "";

        // Gabungkan semua token setelah "is"
        while (!eop(line)) {
            if (strlen(rhs) > 0) strcat(rhs, " ");
            strcat(rhs, getcw());
            inc(line);
        }

        if (strlen(rhs) > 0) strcat(rhs, " ");
        strcat(rhs, getcw());

        // Tambahkan ke .data: alokasikan space float
        char data_declaration[128];
        sprintf(data_declaration, "%s: dd 0.0\n", var_name);
        strcat(tape_data, data_declaration);

        // Tambahkan ke .text: assign nilai float (hardcoded)
        char text_code[256];
        sprintf(text_code,
            "    mov dword [%s], __float32__(%s)\n",
            var_name, rhs);
        strcat(tape_text, text_code);
    } else {
        // Hanya deklarasi tanpa nilai
        char data_declaration[128];
        sprintf(data_declaration, "%s: dd 0.0\n", var_name);
        strcat(tape_data, data_declaration);
    }
}