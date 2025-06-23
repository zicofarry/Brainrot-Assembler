#include "parser.h"
#include "../lexer/lexer.h"
#include <string.h>

// prosedur untuk melakukan parsing tiap barisnya ke Assembly
void mainParserASM(char* line, int* text_section_started) {
    start(line);
    if (line[0] == '\0' || strcmp(getcw(), "") == 0) return;

    if (!*text_section_started) {
        strcat(tape_text, "section .text\n");
        strcat(tape_text, "global _start\n\n_start:\n");
        *text_section_started = 1;
    }


    if(strcmp(getcw(), "skibidi") == 0) skibidi_asm(line);
    // else if(strcmp(getcw(), "skibidiToilet") == 0) skibidiToilet_asm(line);
    else if(strcmp(getcw(), "sigma") == 0) sigma_asm(line);
    else if(strcmp(getcw(), "sigmaBoy") == 0) sigmaBoy_asm(line);
    else if(strcmp(getcw(), "gyatt") == 0) gyatt_asm(line);
    else if(strcmp(getcw(), "bombaclat") == 0) bombaclat_asm(line);
    // else if(strcmp(getcw(), "tungtungtung") == 0) tungtungtung_asm(line);
    else if(strcmp(getcw(), "lingaguliguli") == 0) lingaguliguli_asm(line);
    else if(strcmp(getcw(), "goofy") == 0) goofy_asm(line);
    else if(strcmp(getcw(), "sus") == 0) sus_asm(line);
    // else if(strcmp(getcw(), "ahh") == 0) ahh_asm();
    // else if(strstr(line, "bruh") != NULL) {
    //     fprintf(asm_out, "    jmp .exit_loop\n"); // analogi "break"
    // }else if(strstr(line, "unpause") != NULL) {
    //     fprintf(asm_out, "    jmp .continue_loop\n"); // analogi "continue"
    else if(strcmp(getcw(), "stahp") == 0) stahp_asm();
    //     // end of block, bisa diskip dalam ASM (tidak perlu kurung kurawal)
    else if(strstr(line, "is") != NULL) parseAssignment_asm(line);
    else if(strstr(line, "aura") != NULL) parserAuraOperation_asm(line);
    // }else if(strcmp(getcw(), "danYappp") == 0) {
        // abaikan baris
    // }
    else {
        // fprintf(asm_out, "; ERROR: keyword tidak dikenali -> %s\n", line);
    }
}
