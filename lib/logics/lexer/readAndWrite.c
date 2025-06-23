#include "lexer.h"

char tape[5005]; // pita dari file .brainrot

// prosedur untuk membaca inputan dari cmd
void readFromStdin() {
    tape[0] = '\0';  // kosongkan isi tape
    char temp[1024];

    while (fgets(temp, sizeof(temp), stdin)) {
        if (strstr(temp, "danYappp")) {
            break;  // berhenti jika menemukan "danYappp"
        }
        strcat(tape, temp);
    }
}

// prosedur untuk membaca inputan dari file .brainrot
void read(char fileName[]){
    FILE *fRead;
    fRead = fopen(fileName, "r");//r untuk operator read file

    tape[0] = '\0';  // kosongkan isi tape dulu
    char temp[1024];

    while (fgets(temp, sizeof(temp), fRead)) {
        if (strstr(temp, "danYappp")) {
            // berhenti kalau sudah ketemu "danYappp"
            break;
        }
        strcat(tape, temp);
    }

    fclose(fRead);
}

// prosedur untuk write dari tape ke file dalam bahasa c
void write(char fileName[]){
    FILE *fWrite;
    fWrite = fopen(fileName, "w");
    fprintf(fWrite, "#include <stdio.h>\n");
    fprintf(fWrite, "#include <string.h>\n");
    fprintf(fWrite, "#include <math.h>\n\n");
    fprintf(fWrite, "int main(){\n");
    fprintf(fWrite, "%s", tape);
    fprintf(fWrite, "return 0;\n}");
    fclose(fWrite);
}