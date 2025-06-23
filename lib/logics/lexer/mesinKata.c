#include "lexer.h"

// int idx;                // Index dari karakter terakhir pada kata
char cw[51];            // Kata saat ini (current word)

// cek eop
int eop(char pita[]){
    if(pita[idx] == '\0'){ // jika '\n' return 1
        return 1;
    }
    else{
        return 0; // jika bukan '\n' kembalikan 0
    }
}

// memulai mesin
void start(char pita[]){
    // set index dan panjang kata menjadi 0
    idx = 0;
    wlen = 0;
    while (pita[idx] == ' ' || pita[idx] == '\t') {
        idx++;
    }

    if(pita[idx] == '"'){ // kondisi jika nama mengandung spasi, maka harus diapit dengan tanda petik 2
        idx++;
        while(((pita[idx] != '"') && (eop(pita) == 0)) || ((pita[idx-1] == '\\') && (eop(pita) == 0))){
            if(pita[idx] == '\\' ){
                idx++;
            }
            
            cw[wlen] = pita[idx];
            wlen++;
            idx++;
        }
        cw[wlen] = '\0';
        idx++;
    }else{
        // masukkan kata baru
        while ((pita[idx] != ' ') && (eop(pita) == 0)){
            cw[wlen] = pita[idx];
            wlen++;
            idx++;
        }
        cw[wlen] = '\0'; // akhiri lagi
    }
}

// reset current word
void reset(){
    wlen = 0; // kembalikan panjang kata menjadi 0
    cw[wlen] = '\0'; // set current word menjadi null (siap diisi)
}


// pindah next kata
void inc(char pita[]){
    wlen = 0; // set panjang kata jadi 0 (memastikan)


    while (pita[idx] == ' '){ // ignore blank
        idx++;
    }
    
    if(pita[idx] == '"'){ // kondisi jika nama mengandung spasi, maka harus diapit dengan tanda petik 2
        idx++;
        while(((pita[idx] != '"') && (eop(pita) == 0)) || ((pita[idx-1] == '\\') && (eop(pita) == 0))){
            if(pita[idx] == '\\' ){
                idx++;
            }
            
            cw[wlen] = pita[idx];
            wlen++;
            idx++;
        }
        cw[wlen] = '\0';
        idx++;
    }else{ // kondisi kata biasa
        // masukkan kata baru
        while ((pita[idx] != ' ') && (eop(pita) == 0)){
            cw[wlen] = pita[idx];
            wlen++;
            idx++;
        }
        cw[wlen] = '\0'; // akhiri lagi
    }

}

// pindah ke kata sebelumnya
void dec(char pita[]){
    if(idx > 0){

        while (idx >= 0 && (pita[idx] == ' ' || pita[idx] == ';')){ // ignore blank
            idx--;
        }

        if(pita[idx] == '"'){
            idx--;
            //memundurkan indeks ke kata sebelumnya
            while (idx >= 0 && (pita[idx] != '"') && (eop(pita) == 0) || ((pita[idx-1] == '\\') && (eop(pita) == 0))){
                idx--;
            }
            idx--;
            idx--;
        }else{
            //memundurkan indeks ke kata sebelumnya
            while (idx >= 0 && (pita[idx] != ' ') && (eop(pita) == 0)){
                idx--;
            }
            idx--;
        }
        
        
        wlen = 0; // set panjang kata jadi 0 (memastikan)
        
        if(pita[idx] == '"'){ // kondisi jika nama mengandung spasi, maka harus diapit dengan tanda petik 2
            idx--;
            int lastc = idx;
            // menghitung panjang kata
            while (idx >= 0 && (pita[idx] != '"') && (eop(pita) == 0) || ((pita[idx-1] == '\\') && (eop(pita) == 0))){
                if(pita[idx] == '\\' ){
                    idx--;
                }
                
                wlen++;
                idx--;
            }
            // printf("|%d|", wlen);
            idx++; // masuk ke awal kata
            
            for(int i = 0; i < wlen; i++){
                if(pita[idx] == '\\' ){
                    idx++;
                }
                cw[i] = pita[idx++];
            }
            cw[wlen] = '\0'; // akhiri lagi
            idx++;
        }else{
            int lastc = idx;
            // menghitung panjang kata
            while (idx >= 0 && (pita[idx] != ' ') && (eop(pita) == 0)){
                wlen++;
                idx--;
            }
            idx++; // masuk ke awal kata
            
            for(int i = 0; i < wlen; i++){
                cw[i] = pita[idx++];
            }
            cw[wlen] = '\0'; // akhiri lagi
        }
    }
}


// mengembalikan current word
char* getcw(){
    return cw;
}


// mengembalikan panjang current word
int getlen(){
    return wlen;
}


// menghitung jumlah kata pada suatu string
int countw(char pita[]){
    char tmp[51];
    strcpy(tmp, getcw());

    int jkata = 0;
    start(pita);// mulai kata
    while(eop(pita) == 0){ // loop selama belum bertemu eop
        jkata++; // jumlah kata bertambah
        inc(pita); // maju ke kata selanjutnya
    }

    // menghitung sisa kata yang memiliki eop
    if(getlen() > 0){
        jkata++;
    }

    //mengembalikan indeks kata
    start(pita);
    while(strcmp(tmp, getcw()) != 0){
        inc(pita);
    }

    
    return jkata;
}

// menghitung sekarang ada di kata keberapa pada suatu string
int wordnow(char pita[]){
    char tmp[51];
    strcpy(tmp, getcw());

    int jkata = 1;
    start(pita);// mulai kata
    while(strcmp(getcw(), tmp) != 0){ // loop selama belum bertemu kata sekarang
        jkata++; // jumlah kata bertambah
        inc(pita); // maju ke kata selanjutnya
    }
    
    return jkata;
}

// Mengecek apakah dalam pita tersebut memiliki EOP (;) atau tidak
int cekEOP(char pita[]){
    START(pita);
    int i = 0;
    while (i < strlen(pita) - 1){
        INC(pita);
        i++;
    }

    if(GETCC() == ';'){
        return 1;
    }else{
        return 0;
    }
}

// Mengecek apakah kata saat ini merupakan kata terakhir
int lastw(char pita[]){
    if(wordnow(pita) == countw(pita)){
        return 1;
    }else{
        return 0;
    }
}

// Menghitung jumlah pita di dalam pita masukan 
int ctape(char pita[]){
    int i = 0;
    int tape = 0;
    while(i < strlen(pita) - 1){
        if(pita[i] == '\\' && pita[i + 1] == ';'){
            tape++;
        }
        i++;
    }

    return tape;
}

// Memisahkan Multi Pita Ke Dalam Array Of Pita
void pisahtape(char pita[], char tmp[51][101]){
    start(pita);
    int i = 0;
    while(eop(pita) == 0){
        strcpy(tmp[i], getcw());
        i++;
        inc(pita);
    }
    strcpy(tmp[i], getcw());
}

int is_single_word(const char *str) {
    while (*str) {
        if (isspace(*str)) {
            return 0; // Ada spasi, berarti lebih dari satu kata
        }
        str++;
    }
    return 1; // Tidak ada spasi, berarti hanya satu kata
}
