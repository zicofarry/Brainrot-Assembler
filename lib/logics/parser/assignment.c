#include "parser.h"

void trim(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) start++;

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    if (start != str)
        memmove(str, start, end - start + 2);
}

// prosedur untuk memparsing assignment
void parseAssignment_asm(char* line) {
    start(line);
    char lhs[100], rhs[256] = "", buffer[1024] = "";
    char instr[1024] = "";

    strcpy(lhs, getcw());  // ambil variabel kiri
    char* tipeLHS = getType(lhs);

    inc(line); // skip 'is'
    inc(line); // ke awal ekspresi

    // Kumpulkan RHS
    while (!eop(line)) {
        if (strlen(rhs) > 0) strcat(rhs, " ");
        strcat(rhs, getcw());
        inc(line);
    }
    if (strlen(rhs) > 0) strcat(rhs, " ");
    strcat(rhs, getcw());

    trim(rhs);  // buang spasi depan belakang jika ada
    // printf("%s\n" , rhs);

    // Deteksi char literal
    if (rhs[0] == '\'' && rhs[2] == '\'') {
        char val = rhs[1];
        sprintf(instr,
            "    ; assignment %s is '%c'\n"
            "    mov byte [%s], '%c'\n",
            lhs, val, lhs, val
        );
        strcat(tape_text, instr);
        return;
    }

    START(line);
    int ada = 0;
    while(!EOP()){
        if(GETCC() == '\"'){
            // printf("masok\n");
            ada = 1;
            break;
        }
        INC(line);
    }
    // Deteksi string literal
    if (ada) {
        char label[100];
        sprintf(label, "str_%s", lhs);
        // Hapus kutip dari string
        char strVal[256];
        strcpy(strVal, rhs);
        // strncpy(strVal, rhs + 1, strlen(rhs) - 2);
        // strVal[strlen(rhs) - 2] = '\0';

        // Tambahkan ke data section
        char datadecl[256];
        sprintf(datadecl, "%s: db \"%s\", 0\n", label, strVal);
        strcat(tape_data, datadecl);

        // Tambahkan instruksi
       sprintf(instr,
            "    ; assignment %s is \"%s\"\n"
            "    lea rsi, [rel str_%s]\n"
            "    lea rdi, [rel %s]\n"
            "copy_%s:\n"
            "    mov al, [rsi]\n"
            "    mov [rdi], al\n"
            "    inc rsi\n"
            "    inc rdi\n"
            "    test al, al\n"
            "    jnz copy_%s\n",
            lhs, strVal, lhs, lhs, lhs, lhs);


        // sprintf(instr,
        //     "    ; assignment %s is \"%s\"\n"
        //     "    mov dword [%s], %s\n", lhs, strVal, lhs, label);
        strcat(tape_text, instr);
        return;
    }

    // Ekspresi matematika atau variabel
    char* token = strtok(rhs, " ");
    int token_index = 0;
    char op[8] = "";
    char operand1[64] = "", operand2[64] = "";

    while (token != NULL) {
        if (token_index == 0)
            strcpy(operand1, token);
        else if (token_index == 1)
            strcpy(op, token);
        else if (token_index == 2)
            strcpy(operand2, token);
        token_index++;
        token = strtok(NULL, " ");
    }

    sprintf(instr, "    ; assignment %s is %s %s %s\n", lhs, operand1, op, operand2);

    if (strlen(operand1) == 0) {
        strcat(instr, "    ; ERROR: no operand1\n");
        strcat(tape_text, instr);
        return;
    }

    // Operand pertama
    if (isdigit(operand1[0])) {
        sprintf(buffer, "    mov eax, %s\n", operand1);
    } else {
        sprintf(buffer, "    mov eax, [%s]\n", operand1);
    }
    strcat(instr, buffer);

    // Operasi jika ada
    if (strlen(op) > 0 && strlen(operand2) > 0) {
        if (strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
            strcat(instr, "    xor edx, edx\n");
            if (isdigit(operand2[0])) {
                sprintf(buffer, "    mov ecx, %s\n", operand2);
            } else {
                sprintf(buffer, "    mov ecx, [%s]\n", operand2);
            }
            strcat(instr, buffer);
            strcat(instr, "    idiv ecx\n");

            if (strcmp(op, "%") == 0) {
                strcat(instr, "    mov eax, edx\n");
            }
        } else {
            // Operasi biasa
            if (isdigit(operand2[0])) {
                sprintf(buffer, "    %s eax, %s\n",
                    strcmp(op, "+") == 0 ? "add" :
                    strcmp(op, "-" ) == 0 ? "sub" :
                    strcmp(op, "*" ) == 0 ? "imul" : "nop",
                    operand2);
            } else {
                sprintf(buffer, "    %s eax, [%s]\n",
                    strcmp(op, "+" ) == 0 ? "add" :
                    strcmp(op, "-" ) == 0 ? "sub" :
                    strcmp(op, "*" ) == 0 ? "imul" : "nop",
                    operand2);
            }
            strcat(instr, buffer);
        }
    }

    // Simpan hasil ke LHS
    sprintf(buffer, "    mov [%s], eax\n", lhs);
    strcat(instr, buffer);

    // Tambahkan instruksi
    strcat(tape_text, instr);
}


// void parseAssignment_asm(char* line) {
//     start(line);
//     char lhs[100], rhs[256] = "", buffer[1024] = "";
//     char instr[1024] = "";
    
//     strcpy(lhs, getcw());  // ambil variabel kiri
//     char* tipeLHS = getType(lhs);

//     inc(line); // skip 'is'
//     inc(line); // ke awal ekspresi

//     // Kumpulkan ekspresi RHS jadi satu string
//     while (!eop(line)) {
//         if (strlen(rhs) > 0) strcat(rhs, " ");
//         strcat(rhs, getcw());
//         inc(line);
//     }
//     if(strlen(rhs) > 0) strcat(rhs, " ");
//     strcat(rhs, getcw());

//     // Tokenisasi RHS: contoh "3 + y"
//     char* token = strtok(rhs, " ");
//     int token_index = 0;
//     char op[8] = "";
//     char operand1[64] = "", operand2[64] = "";

//     while (token != NULL) {
//         if (token_index == 0)
//             strcpy(operand1, token);
//         else if (token_index == 1)
//             strcpy(op, token);
//         else if (token_index == 2)
//             strcpy(operand2, token);
//         token_index++;
//         token = strtok(NULL, " ");
//     }

//     // Komentar
//     sprintf(instr, "    ; assignment %s is %s %s %s\n", lhs, operand1, op, operand2);

//     // Operand pertama
//     if (isdigit(operand1[0])) {
//         sprintf(buffer, "    mov eax, %s\n", operand1);
//     } else {
//         sprintf(buffer, "    mov eax, [%s]\n", operand1);
//     }
//     strcat(instr, buffer);

//     // Operasi jika ada
//     if (strlen(op) > 0 && strlen(operand2) > 0) {
//         if (strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
//             strcat(instr, "    xor edx, edx\n");
//             if (isdigit(operand2[0])) {
//                 sprintf(buffer, "    mov ecx, %s\n", operand2);
//             } else {
//                 sprintf(buffer, "    mov ecx, [%s]\n", operand2);
//             }
//             strcat(instr, buffer);
//             strcat(instr, "    idiv ecx\n");

//             if (strcmp(op, "%") == 0) {
//                 // Ambil sisa bagi dari edx
//                 strcat(instr, "    mov eax, edx\n");
//             }
//         } else {
//             // Operasi biasa: + - *
//             if (isdigit(operand2[0])) {
//                 sprintf(buffer, "    %s eax, %s\n",
//                     strcmp(op, "+") == 0 ? "add" :
//                     strcmp(op, "-" ) == 0 ? "sub" :
//                     strcmp(op, "*" ) == 0 ? "imul" : "nop",
//                     operand2);
//             } else {
//                 sprintf(buffer, "    %s eax, [%s]\n",
//                     strcmp(op, "+" ) == 0 ? "add" :
//                     strcmp(op, "-" ) == 0 ? "sub" :
//                     strcmp(op, "*" ) == 0 ? "imul" : "nop",
//                     operand2);
//             }
//             strcat(instr, buffer);
//         }
//     }

//     // Simpan hasil ke LHS
//     sprintf(buffer, "    mov [%s], eax\n", lhs);
//     strcat(instr, buffer);

//     // Tambahkan instruksi ke tape_text
//     strcat(tape_text, instr);
// }