#include "lexer.h"

// deklarasi array variabel
Variable variables[100];
int var_count = 0;

// prosedur untuk menambahkan variabel ke array bungkusan
void addVariable(char* name, char* type){
    strcpy(variables[var_count].name, name);
    strcpy(variables[var_count].type, type);
    var_count++;
}

// fungsi untuk mencari tipe data
char* getType(const char* name){
    for(int i = 0; i < var_count; i++){
        if(strcmp(variables[i].name, name) == 0){
            return variables[i].type;
        }
    }
    return NULL; // jika tidak ditemukan
}