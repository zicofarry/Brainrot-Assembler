#include "parser.h"
#include "../lexer/lexer.h"

// prosedur untuk memparsing token keyword else if

void sus_asm(char* line) {
    // Pop label dari if sebelumnya
    ControlLabel prev = popLabel();

    // Label lompat setelah else-if
    char skip_label[64];
    sprintf(skip_label, "SKIP_IF_%d", if_count);
    pushLabel("",skip_label, "if");

    // Lompat ke bagian setelah blok else if
    char temp[128];
    sprintf(temp, "    jmp %s\n", skip_label);
    strcat(tape_text, temp);

    // Label dari kondisi if/else if sebelumnya ditaruh di sini
    sprintf(temp, "%s:\n", prev.end_label);
    strcat(tape_text, temp);

    // Lanjut parsing else if seolah-olah ini kondisi baru
    goofy_asm(line); // panggil ulang goofy_asm untuk kondisi else if
}
