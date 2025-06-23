#include "parser.h"
#include "../lexer/lexer.h"

// prosedur untuk memparsing token keyword for
void tungtungtung(char* line){
    char output[256] = "";
    char temp[32];

    start(line);
    strcat(output, "for(");
    inc(line);
    if(strcmp(getcw(), "sahur") == 0){
        strcat(output, "int i = 0; i < ");
    }else{
        printf("Error sintaks (baru dibikin mode int i = 0; saja)\n");
    }
    
    inc(line);
    if(strcmp(getcw(), "tilz") == 0){
        inc(line);
        strcpy(temp, getcw());
        strcat(output, temp);
    }else{
        printf("Error sintaks (baru dibikin mode i < (variabel); saja\n");
    }


    strcat(output, "; i++){");
    strcpy(line, output);
}