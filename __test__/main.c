#include <stdio.h>   // deklarasi library yang akan digunakan
#include <string.h>  // deklarasi library yang akan digunakan
#include <stdlib.h>  // deklarasi library yang akan digunakan
#include <ctype.h>  // deklarasi library yang akan digunakan

#define typename(x) _Generic((x), \
    int: "int", \
    float: "float", \
    double: "double", \
    char: "char", \
    char*: "char*", \
    default: "unknown")

typedef struct {
    char name[50];
    char type[10];  // "int", "float", dll
} Variable;

Variable variables[100];
int var_count = 0;

// Tambahkan variabel ke tabel
void addVariable(char* name, char* type) {
    strcpy(variables[var_count].name, name);
    strcpy(variables[var_count].type, type);
    var_count++;
}

// Cari tipe berdasarkan nama
char* getType(char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].type;
        }
    }
    return NULL; // tidak ditemukan
}

// DEKLARASI VARIABEL GLOBAL
int idx;                // Index dari karakter terakhir pada kata
int wlen;               // Panjang kata (word length)
char cc;                // Karakter saat ini (current character)
char cw[51];            // Kata saat ini (current word)
int nonterm;            // Mode Saat ini (Terminal/ Non-Terminal)
char tape[1025];         // Pita Masukan

// mesin baris
char lines[200][1025];  // Maks 200 baris, masing-masing maksimal 1024 karakter
int line_count = 0;    // Total jumlah baris
int curr_line = 0;     // Baris saat ini

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

// mesin baris
void pisahbaris(char pita[]) {
    int i = 0, j = 0, k = 0;
    while (pita[i] != '\0') {
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
    line_count = j + 1;  // simpan total baris
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

void startLine() {
    curr_line = 0;
}

void nextLine() {
    if (curr_line < line_count - 1) {
        curr_line++;
    }
}

void prevLine() {
    if (curr_line > 0) {
        curr_line--;
    }
}

int isLastLine() {
    return curr_line == line_count - 1;
}

char* getLine() {
    return lines[curr_line];
}

void parserOperator(char *token, char* str){
    if(strcmp(token, "butAlso") == 0){
        strcpy(str, " && ");
    }else if(strcmp(token, "orNah") == 0){
        strcpy(str, " || ");
    }else if(strcmp(token, "peak") == 0){
        strcpy(str, " > ");
    }else if(strcmp(token, "mid") == 0){
        strcpy(str, " < ");
    }else if(strcmp(token, "mogged") == 0){
        strcpy(str, " >= ");
    }else if(strcmp(token, "cooked") == 0){
        strcpy(str, " <= ");
    }else if(strcmp(token, "match") == 0){
        strcpy(str, " == ");
    }else if(strcmp(token, "hitdiff") == 0){
        strcpy(str, " != ");
    }else if(strcmp(token, "is") == 0){
        strcpy(str, " = ");
    }else{
        // strcpy(str, token);
        sprintf(str, "%s", token);
    }
}

void parserAuraOperation(char pita[]) {
    start(pita);

    char varname[100];
    strcpy(varname, getcw()); // ambil nama variabel

    inc(pita); // ke token kedua (operator+angka)
    char opAndNum[100];
    strcpy(opAndNum, getcw());

    char op = opAndNum[0]; // '+' atau '-'
    int val = atoi(&opAndNum[1]); // ambil angkanya

    inc(pita); // ke token 'aura'
    if (strcmp(getcw(), "aura") == 0) {
        if (op == '+') {
            sprintf(pita, "%s += %d;", varname, val);
        } else if (op == '-') {
            sprintf(pita, "%s -= %d;", varname, val);
        } else {
            sprintf(pita, "// ERROR: operator aura tidak dikenali: %c", op);
        }
    } else {
        sprintf(pita, "// ERROR: harus ada keyword 'aura'!");
    }
}

void parseAssignment(char* line) {
    start(line);
    char lhs[100], rhs[100];
    strcpy(lhs, getcw());  // Dapatkan variabel kiri
    char* tipeLHS = getType(lhs);  // Dapatkan tipe dari lhs

    if (tipeLHS == NULL) {
        printf("Error: variabel '%s' belum dideklarasikan!\n", lhs);
        return;
    }

    inc(line); // ke 'is'
    if (strcmp(getcw(), "is") != 0) {
        printf("Error: sintaks assignment salah setelah '%s'\n", lhs);
        return;
    }

    inc(line); // ke value
    strcpy(rhs, getcw());  // Dapatkan nilai atau variabel kanan

    // Cek jika rhs adalah literal angka
    int isNumber = 1, isFloat = 0;
    for (int i = 0; i < strlen(rhs); i++) {
        if (!isdigit(rhs[i])) {
            if (rhs[i] == '.' && !isFloat) {
                isFloat = 1;
            } else {
                isNumber = 0;
                break;
            }
        }
    }

    if (isNumber) {
        if (isFloat && strcmp(tipeLHS, "float") != 0) {
            printf("Error: tidak bisa assign float ke '%s' bertipe %s\n", lhs, tipeLHS);
            return;
        }
        if (!isFloat && strcmp(tipeLHS, "int") != 0) {
            printf("Error: tidak bisa assign int ke '%s' bertipe %s\n", lhs, tipeLHS);
            return;
        }
        sprintf(line, "%s = %s;", lhs, rhs);
        return;
    }

    // Cek jika RHS adalah variabel
    char* tipeRHS = getType(rhs);
    if (tipeRHS == NULL) {
        printf("Error: variabel '%s' belum dideklarasikan!\n", rhs);
        return;
    }

    if (strcmp(tipeLHS, tipeRHS) != 0) {
        printf("Error: tidak bisa meng-assign variabel '%s' bertipe '%s' dengan '%s' bertipe '%s'\n",
               lhs, tipeLHS, rhs, tipeRHS);
        return;
    }

    // Cek khusus jika tipenya string
    if (strcmp(tipeLHS, "string") == 0) {
        sprintf(line, "strcpy(%s, %s);", lhs, rhs);
    } else {
        sprintf(line, "%s = %s;", lhs, rhs);
    }
}


void parser(char* line){
    start(line);
    // printf("|%s|\n", getcw());
    if (line[0] == '\0' || strcmp(getcw(), "") == 0) return;

    if (strcmp(getcw(), "skibidi") == 0) {
        inc(line);
        char var_name[100];
        strcpy(var_name, getcw());
        char* tipe = "int";
        addVariable(var_name, tipe);
        
        inc(line);
        if (strcmp(getcw(), "is") == 0) {
            inc(line);
            char* value = getcw();
            sprintf(line, "int %s = %s;", var_name, value);
        } else {
            sprintf(line, "int %s;", var_name);
        }
    
    } else if (strcmp(getcw(), "skibidiToilet") == 0) {
        inc(line);
        char var_name[100];
        strcpy(var_name, getcw());
        char* tipe = "float";
        addVariable(var_name, tipe);
    
        inc(line);
        if (strcmp(getcw(), "is") == 0) {
            inc(line);
            char* value = getcw();
            sprintf(line, "float %s = %s;", var_name, value);
        } else {
            sprintf(line, "float %s;", var_name);
        }
    
    } else if (strcmp(getcw(), "sigma") == 0) {
        inc(line);
        char var_name[100];
        strcpy(var_name, getcw());
        char* tipe = "char";
        addVariable(var_name, tipe);
    
        inc(line);
        if (strcmp(getcw(), "is") == 0) {
            inc(line);
            char* value = getcw();
            sprintf(line, "char %s = '%s';", var_name, value);
        } else {
            sprintf(line, "char %s;", var_name);
        }
    
    } else if (strcmp(getcw(), "sigmaBoy") == 0) {
        inc(line);
        char var_name[100];
        strcpy(var_name, getcw());
        char* tipe = "string";
        addVariable(var_name, tipe);
    
        inc(line);
        if (strcmp(getcw(), "is") == 0) {
            inc(line);
            char* value = getcw();
            sprintf(line, "char %s[101]; strcpy(%s, \"%s\");", var_name, var_name, value);
        } else {
            sprintf(line, "char %s[101];", var_name);
        }
    } else if (strcmp(getcw(), "gyatt") == 0) {
        inc(line);
        char* var_name = getcw();
        char* tipe = getType(var_name);
        if (tipe == NULL) {
            printf("Error: variabel '%s' belum dideklarasikan!\n", var_name);
        } else if (strcmp(tipe, "int") == 0) {
            sprintf(line, "scanf(\"%%d\", &%s);", var_name);
        } else if (strcmp(tipe, "float") == 0) {
            sprintf(line, "scanf(\"%%f\", &%s);", var_name);
        } else if (strcmp(tipe, "char") == 0) {
            sprintf(line, "scanf(\" %%c\", &%s);", var_name);
        } else if (strcmp(tipe, "string") == 0) {
            sprintf(line, "scanf(\"%%s\", %s);", var_name);
        }
        // tambahkan else if untuk tipe lainnya
    }else if(strcmp(getcw(), "bombaclat") == 0){
        inc(line);
        if(!is_single_word(getcw())){
            sprintf(line, "printf(\"%s\\n\");", getcw());
        }else{
            char* var_name = getcw();
            char* tipe = getType(var_name);
            if (tipe == NULL) {
                printf("Error: variabel '%s' belum dideklarasikan!\n", var_name);
            } else if (strcmp(tipe, "int") == 0) {
                sprintf(line, "printf(\"%%d\\n\", %s);", var_name);
            } else if (strcmp(tipe, "float") == 0) {
                sprintf(line, "printf(\"%%f\\n\", %s);", var_name);
            } else if (strcmp(tipe, "char") == 0) {
                sprintf(line, "printf(\"%%c\\n\", %s);", var_name);
            } else if (strcmp(tipe, "string") == 0) {
                sprintf(line, "printf(\"%%s\\n\", %s);", var_name);
            }
        }
    }else if(strcmp(getcw(), "tungtungtung") == 0){
        char output[256] = "";
        char temp[32];

        start(line);
        strcat(output, "for(");
        inc(line);
        if(strcmp(getcw(), "sahur") == 0){
            strcat(output, "int i = 0; i < ");
        }else{
            printf("Error sintaks\n");
        }
        
        inc(line);
        if(strcmp(getcw(), "tilz") == 0){
            inc(line);
            strcpy(temp, getcw());
            strcat(output, temp);
        }else{
            printf("Error sintaks\n");
        }


        strcat(output, "; i++){");
        strcpy(line, output);
    }else if(strcmp(getcw(), "lingaguliguli") == 0){
        char output[256] = "";
        char temp[32];

        strcat(output, "while(");
        start(line);
        inc(line);

        while (1) {
            char* token = getcw();
            int len = strlen(token);
    
            if (token[len - 1] == ':') {
                token[len - 1] = '\0'; // Hapus ':' dari akhir token
                parserOperator(token, temp);
                strcat(output, temp);
                break; // Akhiri loop karena sudah mencapai ':'
            } else {
                parserOperator(token, temp);
                strcat(output, temp);
                inc(line);
            }
        }
        strcat(output, "){");
        // printf("%s\n", line);
        strcpy(line, output);
    }else if(strcmp(getcw(), "goofy") == 0){
        char output[256] = "";
        char temp[32];

        strcat(output, "if(");
        start(line);
        inc(line);

        while (1) {
            char* token = getcw();
            int len = strlen(token);
    
            if (token[len - 1] == ':') {
                token[len - 1] = '\0'; // Hapus ':' dari akhir token
                parserOperator(token, temp);
                strcat(output, temp);
                break; // Akhiri loop karena sudah mencapai ':'
            } else {
                parserOperator(token, temp);
                strcat(output, temp);
                inc(line);
            }
        }
        strcat(output, "){");
        strcpy(line, output);
    }else if(strcmp(getcw(), "sus") == 0){
        char output[256] = "";
        char temp[32];

        strcat(output, "}else if(");
        start(line);
        inc(line);

        while (1) {
            char* token = getcw();
            int len = strlen(token);
    
            if (token[len - 1] == ':') {
                token[len - 1] = '\0'; // Hapus ':' dari akhir token
                parserOperator(token, temp);
                strcat(output, temp);
                break; // Akhiri loop karena sudah mencapai ':'
            } else {
                parserOperator(token, temp);
                strcat(output, temp);
                inc(line);
            }
        }
        strcat(output, "){");
        strcpy(line, output);
    }else if(strcmp(getcw(), "ahh") == 0){
        strcpy(line, "}else{");
    }else if (strstr(line, "bruh") != NULL) {
        strcpy(line, "break;");
    }else if (strstr(line, "unpause") != NULL) {
        strcpy(line, "continue;");
    }else if(strcmp(getcw(), "stahp") == 0){
        strcpy(line, "}");
    }else if(strcmp(getcw(), "stamp") == 0){
        strcpy(line, "}");
    }else if (strstr(line, "is") != NULL) {
        parseAssignment(line);
    }else if (strstr(line, "aura") != NULL) {
        parserAuraOperation(line);
    }else if(strcmp(getcw(), "danYappp") == 0){
        // lakukan sesuatu kalau perlu, atau skip aja
        line[0] = '\0'; // agar tidak mengganggu output akhir
    }else{
        if (line[0] == '\0' || strcmp(getcw(), "") == 0) return;
        printf("ERROR EUYY!!!\n");
        sprintf(line, "// ERROR: keyword tidak dikenali");
    }
    // printf("%s\n", line);
}

void readFromStdin() {
    tape[0] = '\0';  // kosongkan isi tape
    char temp[1024];

    // printf("Masukkan input (akhiri dengan 'danYappp' di satu baris):\n");

    while (fgets(temp, sizeof(temp), stdin)) {
        if (strstr(temp, "danYappp")) {
            break;  // berhenti kalau menemukan "danYappp"
        }
        strcat(tape, temp);
    }
}

// Prosedur Read Untuk Id Kasus
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

// Prosedur Write Untuk Kasus
void write(char fileName[]){
    FILE *fWrite;
    fWrite = fopen(fileName, "w");
    fprintf(fWrite, "#include <stdio.h>\n");
    fprintf(fWrite, "#include <string.h>\n\n");
    fprintf(fWrite, "#include <math.h>\n\n");
    fprintf(fWrite, "int main(){\n");
    fprintf(fWrite, "%s", tape);
    fprintf(fWrite, "return 0;\n}");

    fclose(fWrite);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <brainrot-file>\n", argv[0]);
        return 1;
    }
    read(argv[1]);
// int main(){
//     readFromStdin();

    pisahbaris(tape);
    tape[0] = '\0';
    startLine();
    while (1) {
        // printf("Baris %d: %s\n", curr_line + 1, getLine());
        // char tmp[51];
        // strcpy(tmp, getLine());
        parser(getLine());
        strcat(tape, getLine());
        strcat(tape, "\n");  // kalau tiap baris perlu dipisah newline
        // printf("%s|\n", getLine());
        

        if (isLastLine()) break;
        nextLine();
    }

    // printf("%s", tape);
    write("brainrot.c");
    return 0;
}
