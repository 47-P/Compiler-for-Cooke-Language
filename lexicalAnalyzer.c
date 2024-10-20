#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// Enumeration for token types
typedef enum {
    ASSIGN_OP, ADD_OP, SUB_OP, MULT_OP, DIV_OP, MOD_OP,
    LESSER_OP, GREATER_OP, EQUAL_OP, NEQUAL_OP, LEQUAL_OP, GEQUAL_OP,
    BOOL_AND, BOOL_OR, BOOL_NOT,
    IDENT, INT_LIT, KEY_IN, KEY_OUT, KEY_IF, KEY_ELSE,
    OPEN_PAREN, CLOSE_PAREN, OPEN_CURL, CLOSE_CURL, SEMICOLON, UNKNOWN
} TokenType;

// Structure to hold lexeme and token
typedef struct {
    char lexeme[MAX_TOKEN_LENGTH];
    TokenType token;
} LexemeToken;

// Token names corresponding to enum values
const char* tokenNames[] = {
    "ASSIGN_OP", "ADD_OP", "SUB_OP", "MULT_OP", "DIV_OP", "MOD_OP",
    "LESSER_OP", "GREATER_OP", "EQUAL_OP", "NEQUAL_OP", "LEQUAL_OP", "GEQUAL_OP",
    "BOOL_AND", "BOOL_OR", "BOOL_NOT",
    "IDENT", "INT_LIT", "KEY_IN", "KEY_OUT", "KEY_IF", "KEY_ELSE",
    "OPEN_PAREN", "CLOSE_PAREN", "OPEN_CURL", "CLOSE_CURL", "SEMICOLON", "UNKNOWN"
};

// Function to check if a string is an integer
int isInteger(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// Function to classify lexeme as a token
TokenType classifyLexeme(const char* lexeme) {
    if (strcmp(lexeme, "=") == 0) return ASSIGN_OP;
    if (strcmp(lexeme, "+") == 0) return ADD_OP;
    if (strcmp(lexeme, "-") == 0) return SUB_OP;
    if (strcmp(lexeme, "*") == 0) return MULT_OP;
    if (strcmp(lexeme, "/") == 0) return DIV_OP;
    if (strcmp(lexeme, "%") == 0) return MOD_OP;
    if (strcmp(lexeme, "<") == 0) return LESSER_OP;
    if (strcmp(lexeme, ">") == 0) return GREATER_OP;
    if (strcmp(lexeme, "==") == 0) return EQUAL_OP;
    if (strcmp(lexeme, "!=") == 0) return NEQUAL_OP;
    if (strcmp(lexeme, "<=") == 0) return LEQUAL_OP;
    if (strcmp(lexeme, ">=") == 0) return GEQUAL_OP;
    if (strcmp(lexeme, "&&") == 0) return BOOL_AND;
    if (strcmp(lexeme, "||") == 0) return BOOL_OR;
    if (strcmp(lexeme, "!") == 0) return BOOL_NOT;
    if (strcmp(lexeme, "input") == 0) return KEY_IN;
    if (strcmp(lexeme, "output") == 0) return KEY_OUT;
    if (strcmp(lexeme, "if") == 0) return KEY_IF;
    if (strcmp(lexeme, "else") == 0) return KEY_ELSE;
    if (strcmp(lexeme, "(") == 0) return OPEN_PAREN;
    if (strcmp(lexeme, ")") == 0) return CLOSE_PAREN;
    if (strcmp(lexeme, "{") == 0) return OPEN_CURL;
    if (strcmp(lexeme, "}") == 0) return CLOSE_CURL;
    if (strcmp(lexeme, ";") == 0) return SEMICOLON;
    if (isInteger(lexeme)) return INT_LIT;
    if (isalpha(lexeme[0])) return IDENT;
    return UNKNOWN;
}

// Main lexical analyzer function
void lexicalAnalyzer(FILE *file) {
    char lexeme[MAX_TOKEN_LENGTH];
    printf("Cooke Analyzer ::\n");

    while (fscanf(file, "%s", lexeme) != EOF) {
        TokenType token = classifyLexeme(lexeme);
        printf("%s %s\n", lexeme, tokenNames[token]);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No input file provided.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    lexicalAnalyzer(file);
    fclose(file);
    return 0;
}
