#include "parser.h"

ControlLabel label_stack[100];
int label_top = -1;

void stahp_asm() {
    ControlLabel label = popLabel();

    if (strcmp(label.type, "loop") == 0) {
        // untuk while/loop: lompat balik ke start
        char buf[128];
        sprintf(buf,
            "    jmp %s\n"
            "%s:\n",
            label.start_label,
            label.end_label
        );
        strcat(tape_text, buf);
    } else if (strcmp(label.type, "if") == 0) {
        // untuk if: cukup kasih label akhir
        char buf[64];
        sprintf(buf, "%s:\n", label.end_label);
        strcat(tape_text, buf);
    }
}


void pushLabel(char* start, char* end, char* type) {
    ++label_top;
    strcpy(label_stack[label_top].start_label, start);
    strcpy(label_stack[label_top].end_label, end);
    strcpy(label_stack[label_top].type, type);
}

ControlLabel popLabel() {
    ControlLabel empty = {"", "", ""};
    if (label_top < 0) return empty;
    return label_stack[label_top--];
}

// void stahp_asm() {
//     ControlLabel label = popLabel();
//     // if(strcmp(label.type, "loop") == 0){
//     //     strcat(tape_text, "    jmp ");
//     //     strcat(tape_text, start_label);
//     //     strcat(tape_text, "\n");
//     // }
//     if (strcmp(label.label, "") != 0) {
//         char buf[64];
//         sprintf(buf, "%s:\n", label.label);
//         strcat(tape_text, buf);
//     }
// }

// void pushLabel(char* label, char* type) {
//     ++label_top;
//     strcpy(label_stack[label_top].label, label);
//     strcpy(label_stack[label_top].type, type);
// }

// ControlLabel popLabel() {
//     ControlLabel empty = {"", ""};
//     if (label_top < 0) return empty;
//     return label_stack[label_top--];
// }