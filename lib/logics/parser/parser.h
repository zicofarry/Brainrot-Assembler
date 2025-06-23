#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lexer/lexer.h"

// typedef struct {
//     char label[32];
//     char type[8]; // "if", "for", "while"
// } ControlLabel;
typedef struct {
    char start_label[32];
    char end_label[32];
    char type[16]; // "loop", "if", dll
} ControlLabel;


extern ControlLabel label_stack[100];
extern int label_top;

#define BUFFER_SIZE 65536
extern char tape_data[BUFFER_SIZE];
extern char tape_bss[BUFFER_SIZE];
extern char tape_text[BUFFER_SIZE];
extern int if_count;

// Deklarasi prosedur 
void mainParserASM(char* line, int* text_section_started);                                      // untuk memproses string baris menjadi bahasa c (parsing)
void parserOperator_asm(char* op1, char* op_token, char* op2, char* result_reg, char* asm_code);
void parserStrcmp_asm(char* left, char* op_token, char* right, char* result_reg, char* out, int id);
void parserAuraOperation_asm(char pita[]);                                  // untuk parsing aura (increment/ decrement)
void parseAssignment_asm(char* line);                                       // untuk memparsing assignment
void skibidi_asm(char* line);                                               // untuk memparsing token tipe data int
// void skibidiToilet_asm(char* line);                                         // untuk memparsing token tipe data float
void sigma_asm(char* line);                                                 // untuk memparsing token tipe data char
void sigmaBoy_asm(char* line);                                              // untuk memparsing token tipe data string
void gyatt_asm(char* line);                                            // untuk memparsing token fungsi scanf
void bombaclat_asm(char* line);                                       // untuk memparsing token fungsi printf
// void tungtungtung(char* line);                                          // untuk memparsing token keyword for
void lingaguliguli_asm(char* line);                                         // untuk memparsing token keyword while
void goofy_asm(char* line);                                                 // untuk memparsing token keyword if
// void goofy_asm(char* line, const char* block_type);                                                 // untuk memparsing token keyword if
void sus_asm(char* line);                                                   // untuk memparsing token keyword else if
// void ahh_asm();                                                   // untuk memparsing token keyword else
void trim(char *str);
void stahp_asm();
// void pushLabel(char* label, char* type);
void pushLabel(char* start, char* end, char* type);
ControlLabel popLabel();