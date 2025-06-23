#include "lib/logics/lexer/lexer.h"
#include "lib/logics/parser/parser.h"

char tape_data[BUFFER_SIZE];
char tape_bss[BUFFER_SIZE];
char tape_text[BUFFER_SIZE];

// kalau kamu mau ngoding di cmd, uncomment kode di bawah, dan int main yang arg sampe read, jadiin komen atau hapus aja    
// int main(){
//     readFromStdin();
//     char argv[2][101];
//     strcpy(argv[1], "brainrot");

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <brainrot-file>\n", argv[0]);
        return 1;
    }
    read(argv[1]);

    pisahbaris(tape);
    tape[0] = '\0';
    
    // Kosongkan buffers global
    tape_data[0] = '\0';
    tape_bss[0] = '\0';
    tape_text[0] = '\0';

    int text_section_started = 0;

    
    startLine();
    while (1) {
        // tokenisasi baris sebelum parsing
        tokenizeLine(argv[1], getLine(), curr_line + 1);
        
        // parsing line yang kita miliki dengan memanggil prosedur mainParser
        // mainParser(getLine());
        mainParserASM(getLine(), &text_section_started);  // versi ASM

        // setelah parsing dilakukan, simpan hasil parsing an ke dalam string tape
        // strcat(tape, getLine());
        // strcat(tape, "\n");

        // jika sudah line terakhir, break
        if (isLastLine()) break;

        nextLine();
    }

    // Sekarang gabungkan semua section dan tulis ke file asm
    FILE *asm_out = fopen("bin/brainrot.asm", "w");
    if (!asm_out) {
        perror("Failed to create ASM file");
        return 1;
    }

    // Tulis section data dan bss dulu
    fprintf(asm_out, "section .data\n%s\n", tape_data);
    fprintf(asm_out, "section .bss\n%s\n", tape_bss);

    // Tulis section text, pastikan awali dengan global _start dan label _start kalau belum ada
    if (!text_section_started) {
        fprintf(asm_out, "section .text\n");
        fprintf(asm_out, "global _start\n\n_start:\n");
    }
    fprintf(asm_out, "%s", tape_text);

    // Akhiri dengan syscall exit
    fprintf(asm_out,
    "    mov eax, 1\n"      // sys_exit
    "    xor ebx, ebx\n"    // return code 0
    "    int 0x80\n");

    fclose(asm_out);

    // Compile & Run
    if (!error) {
        printf(GREEN "Parsing successful!"RESET"\n");
        int compile = system("nasm -f elf64 bin/brainrot.asm -o bin/brainrot.o");
        int link = system("ld bin/brainrot.o -o bin/brainrot");
        if(!compile && !link){
            printf(GREEN "Compilation successful!"RESET"\n");
            system("./bin/brainrot");
        } else {
            printf(RED "Compilation failed!\n" RESET);
        }
    } else {
        printf(RED "Parsing failed!\n" RESET);
    }
    
    return 0;
}
