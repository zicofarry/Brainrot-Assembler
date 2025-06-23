#include "lexer.h"

int idx;                // Index dari karakter terakhir pada kata
int wlen;               // Panjang kata (word length)
char cc;                // Karakter saat ini (current character)

// fungsi untuk memeriksa akhir dari proses (End of Process)
int EOP(){
    if(cc == '\0'){ // jika cc saat ini '\n'
        return 1; // kembalikan nilai 1 (marking eop true)
    }
    else{ // jika tidak
        return 0; // kembalikan nilai 0
    }
}

// prosedur untuk memulai mesin karakter
void START(char pita[]){
    // memulai pembacaan mesin karakter
    idx = 0; // index pasti dimulai dari 0
    cc = pita[idx]; // cc adalah pita inputan ke idx = 0
    // Skip semua whitespace di awal (spasi dan tab)
    while ((cc == ' ' || cc == '\t') && cc != '\0') {
        idx++;
        cc = pita[idx];
    }
}

// prosedur untuk memajukan karakter pada pita
void INC(char pita[]){
    // menambah nilai indeks dan mengatur karakter yang dibaca
    idx++;
    cc = pita[idx];
}

// prosedur untuk memundurkan karakter pada pita
void DEC(char pita[]){
    // mengurangi nilai indeks dan mengatur karakter yang dibaca
    idx--;
    cc = pita[idx];
}

// prosedur untuk memajukan karakter hingga melewati spasi
void ADV(char pita[]){
    // menambah nilai indeks dan mengatur karakter yang dibaca
    idx++;
    cc = pita[idx];

    // proses looping jika karakter saat ini masih berupa spasi
    while((cc == ' ') && (EOP() == 0)){
        idx++;
        cc = pita[idx];
    }
}

// fungsi untuk mendapatkan karakter pita saat ini
char GETCC(){
    return cc;
}

// fungsi untuk mendapatkan indeks pita saat ini
int GETIdx(){
    return idx;
}