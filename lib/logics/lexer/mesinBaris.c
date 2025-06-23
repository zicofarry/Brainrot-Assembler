#include "lexer.h"

int line_count = 0;
int curr_line = 0;
char lines[200][5005];

// prosedur untuk memisahkan string tape menjadi beberapa baris
void pisahbaris(char pita[]) {
    int i = 0, j = 0, k = 0;
    while (pita[i] != '\0') {
        if (pita[i] == '\r') {
            // Abaikan karakter carriage return
            i++;
            continue;
        }
        if (pita[i] == '\n') {
            lines[j][k] = '\0';
            j++;
            k = 0;
        } else {
            lines[j][k] = pita[i];
            k++;
        }
        i++;
    }
    lines[j][k] = '\0';
    line_count = j + 1;
}

// prosedur untuk memulai baris
void startLine() {
    curr_line = 0;
}

// prosedur untuk maju ke baris selanjutnya
void nextLine() {
    if (curr_line < line_count - 1) {
        curr_line++;
    }
}

// prosedur untuk mundur ke baris sebelumnya
void prevLine() {
    if (curr_line > 0) {
        curr_line--;
    }
}

// prosedur untuk mengecek jika ini baris terakhir
int isLastLine() {
    return curr_line == line_count - 1;
}

// prosedur untuk mendapatkan indeks baris saat ini
char* getLine() {
    return lines[curr_line];
}