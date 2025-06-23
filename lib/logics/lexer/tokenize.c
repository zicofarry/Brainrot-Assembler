#include "lexer.h"

int error = 0;

// fungsi untuk mengidentifikasi jenis token dari lexeme
TokenType identifyTokenType(const char *lexeme) {
    if (strcmp(lexeme, "skibidi") == 0 || strcmp(lexeme, "skibidiToilet") == 0 ||
        strcmp(lexeme, "sigma") == 0 || strcmp(lexeme, "sigmaBoy") == 0)
        return DATA_TYPE;

    if (strcmp(lexeme, "gyatt") == 0 || strcmp(lexeme, "bombaclat") == 0)
        return FUNCTION;

    if (strcmp(lexeme, "goofy") == 0 || strcmp(lexeme, "ahh") == 0 ||
        strcmp(lexeme, "sus") == 0 || strcmp(lexeme, "tungtungtung") == 0 ||
        strcmp(lexeme, "lingaguliguli") == 0 || strcmp(lexeme, "bruh") == 0 ||
        strcmp(lexeme, "sahur") == 0 || strcmp(lexeme, "tilz") == 0 ||
        strcmp(lexeme, "unpause") == 0 || strcmp(lexeme, "bop") == 0)
        return KEYWORD;

    if (strcmp(lexeme, "is") == 0)
        return ASSIGNMENT;

    if (strcmp(lexeme, "butAlso") == 0 || strcmp(lexeme, "orNahh") == 0 ||
        strcmp(lexeme, "peak") == 0 || strcmp(lexeme, "mid") == 0 ||
        strcmp(lexeme, "mogged") == 0 || strcmp(lexeme, "cooked") == 0 ||
        strcmp(lexeme, "match") == 0 || strcmp(lexeme, "hitdiff") == 0 ||
        strcmp(lexeme, "aura") == 0 || (lexeme[0] == '+' || lexeme[0] == '-') && isdigit(lexeme[1])||
        strcmp(lexeme, "+") == 0 || strcmp(lexeme, "-") == 0 ||
        strcmp(lexeme, "(") == 0 || strcmp(lexeme, ")") == 0 ||
        strcmp(lexeme, "*") == 0 || strcmp(lexeme, "/") == 0 ||
        strcmp(lexeme, "%") == 0)
        return OPERATOR;

    int len = strlen(lexeme);
    if (len > 0 && lexeme[0] == '"' && lexeme[len - 1] == '"') {
        printf("Detected string: %s\n", lexeme);  // Debug print
        return STRING;
    }

    if (len == 3 && lexeme[0] == '\'' && lexeme[2] == '\'')
        return CHAR;

    int has_dot = 0;
    int is_number = 1;
    for (int i = 0; lexeme[i]; i++) {
        if (isdigit(lexeme[i])) continue;
        if (lexeme[i] == '.' && !has_dot) {
            has_dot = 1;
            continue;
        }
        is_number = 0;
        break;
    }
    if (is_number) {
        return has_dot ? FLOAT_NUM : INTEGER_NUM;
    }
    if (getType(lexeme) != NULL) return VARIABLE;

    if (strcmp(lexeme, ":") == 0) return OPEN_COL;
    if (strcmp(lexeme, "stahp") == 0 ) return CLOSE_COL;

    if (strcmp(lexeme, "danYappp") == 0) return TOKEN_EOP;

    // kalau masih belum ke return berarti itu UNKNOWN Token
    return UNKNOWN;
}

// fungsi untuk mendapatkan string dari token untuk ditampilkan
const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case DATA_TYPE: return "DATA_TYPE";
        case VARIABLE: return "VARIABLE";
        case ASSIGNMENT: return "ASSIGNMENT";
        case INTEGER_NUM: return "INTEGER_NUM";
        case FLOAT_NUM: return "FLOAT_NUM";
        case CHAR: return "CHAR";
        case STRING: return "STRING";
        case FUNCTION: return "FUNCTION";
        case IDENTIFIER: return "IDENTIFIER";
        case KEYWORD: return "KEYWORD";
        case OPERATOR: return "OPERATOR";
        case OPEN_COL: return "OPEN_COL";
        case CLOSE_COL: return "CLOSE_COL";
        case TOKEN_EOP: return "TOKEN_EOP";
        default: return "UNKNOWN";  // Jika token tidak diketahui
    }
}

// prosedure untuk melakukan tokenisasi dari huruf pertama sampai akhir (perbaris)
void tokenizeLine(const char* filename, char* line, int lineNumber) {
    START(line);
    int isExpectingVar = 0;
    char currentType[20];
    
    while (!EOP()) {
        int startCol = GETIdx();
        char token[256];
        int i = 0;
        
        // Cek apakah token dimulai dengan tanda kutip (string)
        if (GETCC() == '"') {
            INC(line); // Lewati tanda kutip pertama
            while (!EOP() && GETCC() != '"') { // Terus baca hingga ketemu tanda kutip kedua
                token[i++] = GETCC();
                INC(line);
            }
            token[i] = '\0'; // Akhiri token dengan null terminator
            
            // Jika token berakhir dengan tanda kutip
            if (GETCC() == '"') {
                INC(line); // Lewati tanda kutip kedua
                TokenType type = STRING;
                printToken(filename, lineNumber, startCol, token, type);
                continue; // Lanjut ke token berikutnya
            }
        }

        // Jika bukan string, lanjutkan tokenisasi normal
        while (!EOP() && GETCC() != ' ' && GETCC() != '\t') {
            token[i++] = GETCC();
            INC(line);
        }
        token[i] = '\0';
        
        while ((GETCC() == ' ' || GETCC() == '\t') && !EOP()) {
            INC(line);
        }

        TokenType type;

        if (isExpectingVar) {
            addVariable(token, currentType); // Gunakan tipe data yang terakhir dikenali
            type = VARIABLE;
            isExpectingVar = 0;
        } else {
            type = identifyTokenType(token);
            if (type == DATA_TYPE) {
                isExpectingVar = 1;
                if (strcmp(token, "skibidi") == 0) strcpy(currentType, "int");
                else if (strcmp(token, "skibidiToilet") == 0) strcpy(currentType, "float");
                else if (strcmp(token, "sigma") == 0) strcpy(currentType, "char");
                else if (strcmp(token, "sigmaBoy") == 0) strcpy(currentType, "string");
                else strcpy(currentType, token);
            } else if (type == IDENTIFIER && getType(token) != NULL) {
                type = VARIABLE;
            }
        }

        // Mencetak token dengan warna yang sesuai
        printToken(filename, lineNumber, startCol, token, type);
    }
}

// prosedur untuk mencetak hasil token yang dikenali
void printToken(const char* filename, int lineNumber, int startCol, const char* token, TokenType type) {
    // Fungsi untuk mewarnai token berdasarkan jenisnya
    const char* colorizeToken(TokenType type) {
        switch (type) {
            case DATA_TYPE: return "\033[38;5;20m";     // Biru Tua
            case INTEGER_NUM: return "\033[93m";        // Kuning Terang
            case FLOAT_NUM: return "\033[93m";          // Kuning Terang
            case CHAR: return "\033[38;5;214m";         // Oren
            case STRING: return "\033[38;5;214m";       // Oren
            case VARIABLE: return "\033[38;5;111m";     // Biru Muda
            case ASSIGNMENT: return "\033[38;5;226m";   // Kuning
            case OPERATOR: return "\033[38;5;226m";     // Kuning
            case FUNCTION: return "\033[93m";           // Kuning Terang
            case IDENTIFIER: return "\033[38;5;198m";   // Pink Tua
            case KEYWORD: return "\033[38;5;198m";      // Pink Tua
            case OPEN_COL: return "\033[38;5;198m";     // Pink Tua
            case CLOSE_COL: return "\033[38;5;198m";    // Pink Tua
            case TOKEN_EOP: return "\033[38;5;46m";     // Hijau
            case UNKNOWN: return "\033[1;31m";          // Merah
            default: return "\033[0m";                  // Reset
        }
    }

    // Mencetak token dengan warna dan tipe yang sesuai
    if(type == UNKNOWN){
        printf(WHITE"%s:%d:%d:"RESET" %sERROR:%s \""WHITE"%s\""RESET" is not Recognized as a Token\n",
            filename, lineNumber, startCol + 1, colorizeToken(type), "\033[0m", token);
        error = 1;
    }else{
        // Uncomment kode dibawah jika kamu ingin mendebug lexeme token nya
        if(type == ASSIGNMENT || type == OPERATOR || type == OPEN_COL){
            printf(WHITE"%s:%d:%d:"RESET" \"%s%s%s\" is Recognized as an \"%s\" Token\n",
                filename, lineNumber, startCol + 1,
                colorizeToken(type), token, "\033[0m",
                tokenTypeToString(type));
        }else{
            printf(WHITE"%s:%d:%d:"RESET" \"%s%s%s\" is Recognized as a \"%s\" Token\n",
                filename, lineNumber, startCol + 1,
                colorizeToken(type), token, "\033[0m",
                tokenTypeToString(type));
        }    
    }
}