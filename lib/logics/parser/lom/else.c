#include "parser.h"
#include "../lexer/lexer.h"

// prosedur untuk memparsing token keyword else
void ahh_asm() {
    // Pop label dari if sebelumnya
    ControlLabel prev = popLabel();

    // Label lompat setelah else
    char skip_label[64];
    sprintf(skip_label, "SKIP_IF_%d", if_count);
    pushLabel("", skip_label, "if");

    // Lompat ke akhir
    char temp[128];
    sprintf(temp, "    jmp %s\n", skip_label);
    strcat(tape_text, temp);
    
    // Label untuk bagian else
    sprintf(temp, "%s:\n", prev.end_label);
    strcat(tape_text, temp);
}
