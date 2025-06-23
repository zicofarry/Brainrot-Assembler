#include "parser.h"
#include "../lexer/lexer.h"

// prosedur untuk memparsing tipe data char
void sigma_asm(char* line) {
    start(line);
    inc(line); // skip "sigma"

    char var_name[100];
    strcpy(var_name, getcw());

    // hapus tanda kutip jika ada
    if (var_name[0] == '\'') {
        memmove(var_name, var_name + 1, strlen(var_name));
        var_name[strlen(var_name) - 1] = '\0';
    }

    addVariable(var_name, "char");

    inc(line);

    char buffer[512];

    if (strcmp(getcw(), "is") == 0) {
        inc(line);
        char* value = getcw();
        char ch = value[0];
        if (ch == '\'') ch = value[1];

        sprintf(buffer,
            "%s: db '%c'\n",
            var_name, ch
        );
        strcat(tape_data, buffer);
    } else {
        sprintf(buffer,
            "%s: resb 1\n",
            var_name
        );
        strcat(tape_bss, buffer);
    }
}
