#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
    ASSIGN_OP, ADD_OP, SUB_OP, MULT_OP, DIV_OP, MOD_OP,
    LESSER_OP, GREATER_OP, EQUAL_OP, NEQUAL_OP, LEQUAL_OP, GEQUAL_OP,
    BOOL_AND, BOOL_OR, BOOL_NOT,
    IDENT, INT_LIT, KEY_IN, KEY_OUT, KEY_IF, KEY_ELSE,
    OPEN_PAREN, CLOSE_PAREN, OPEN_CURL, CLOSE_CURL, SEMICOLON, UNKNOWN
} TokenType;

const char* tokenNames[] = {
    "ASSIGN_OP", "ADD_OP", "SUB_OP", "MULT_OP", "DIV_OP", "MOD_OP",
    "LESSER_OP", "GREATER_OP", "EQUAL_OP", "NEQUAL_OP", "LEQUAL_OP", "GEQUAL_OP",
    "BOOL_AND", "BOOL_OR", "BOOL_NOT",
    "IDENT", "INT_LIT", "KEY_IN", "KEY_OUT", "KEY_IF", "KEY_ELSE",
    "OPEN_PAREN", "CLOSE_PAREN", "OPEN_CURL", "CLOSE_CURL", "SEMICOLON", "UNKNOWN"
};

typedef struct {
    char lexeme[MAX_TOKEN_LENGTH];
    TokenType token;
} LexemeToken;

// Parser state
LexemeToken currentToken;
FILE* inputFile;
int currentLine = 1;
int errorCount = 0;

// Forward declarations
void nextToken();
void parseProgram();
void parseStatement();
void parseExpression();
void parseCondition();
void syntaxError(const char* expected);

// Function to get the next token
void nextToken() {
    char lexeme[MAX_TOKEN_LENGTH];
    if (fscanf(inputFile, "%s", lexeme) != EOF) {
        currentToken.token = classifyLexeme(lexeme);
        strcpy(currentToken.lexeme, lexeme);

        // Track line numbers
        if (strchr(lexeme, '\n')) {
            currentLine++;
        }
    } else {
        currentToken.token = UNKNOWN;
        strcpy(currentToken.lexeme, "");
    }
}

// Error handling
void syntaxError(const char* expected) {
    fprintf(stderr, "Error on line %d: Expected %s but found %s (%s)\n",
            currentLine, expected, currentToken.lexeme, tokenNames[currentToken.token]);
    errorCount++;
    nextToken();  // Skip the problematic token to continue parsing
}

// Parse Program (P ::= S)
void parseProgram() {
    parseStatement();
}

// Parse Statement (S rules)
void parseStatement() {
    if (currentToken.token == IDENT) {
        nextToken();
        if (currentToken.token == ASSIGN_OP) {
            nextToken();
            parseExpression();
            if (currentToken.token != SEMICOLON) {
                syntaxError("SEMICOLON");
            } else {
                nextToken();
            }
        } else {
            syntaxError("ASSIGN_OP");
        }
    } else if (currentToken.token == KEY_IF) {
        nextToken();
        if (currentToken.token == OPEN_PAREN) {
            nextToken();
            parseCondition();
            if (currentToken.token == CLOSE_PAREN) {
                nextToken();
                if (currentToken.token == OPEN_CURL) {
                    nextToken();
                    parseStatement();
                    if (currentToken.token == CLOSE_CURL) {
                        nextToken();
                        if (currentToken.token == KEY_ELSE) {
                            nextToken();
                            if (currentToken.token == OPEN_CURL) {
                                nextToken();
                                parseStatement();
                                if (currentToken.token != CLOSE_CURL) {
                                    syntaxError("CLOSE_CURL");
                                } else {
                                    nextToken();
                                }
                            } else {
                                syntaxError("OPEN_CURL");
                            }
                        }
                    } else {
                        syntaxError("CLOSE_CURL");
                    }
                } else {
                    syntaxError("OPEN_CURL");
                }
            } else {
                syntaxError("CLOSE_PAREN");
            }
        } else {
            syntaxError("OPEN_PAREN");
        }
    } else if (currentToken.token == KEY_IN || currentToken.token == KEY_OUT) {
        nextToken();
        if (currentToken.token == OPEN_PAREN) {
            nextToken();
            if (currentToken.token == IDENT) {
                nextToken();
                if (currentToken.token == CLOSE_PAREN) {
                    nextToken();
                    if (currentToken.token != SEMICOLON) {
                        syntaxError("SEMICOLON");
                    } else {
                        nextToken();
                    }
                } else {
                    syntaxError("CLOSE_PAREN");
                }
            } else {
                syntaxError("IDENT");
            }
        } else {
            syntaxError("OPEN_PAREN");
        }
    } else {
        syntaxError("valid statement");
    }
}

// Parse Expression (E rules)
void parseExpression() {
    // Implementation of E ::= T | E + T | E - T
    if (currentToken.token == INT_LIT || currentToken.token == IDENT) {
        nextToken();
        while (currentToken.token == ADD_OP || currentToken.token == SUB_OP) {
            nextToken();
            if (currentToken.token == INT_LIT || currentToken.token == IDENT) {
                nextToken();
            } else {
                syntaxError("INT_LIT or IDENT");
            }
        }
    } else {
        syntaxError("INT_LIT or IDENT");
    }
}

// Parse Condition (C rules)
void parseCondition() {
    // Implementation of C ::= E < E | E > E | etc.
    parseExpression();
    if (currentToken.token == LESSER_OP || currentToken.token == GREATER_OP ||
        currentToken.token == EQUAL_OP || currentToken.token == NEQUAL_OP ||
        currentToken.token == LEQUAL_OP || currentToken.token == GEQUAL_OP) {
        nextToken();
        parseExpression();
    } else {
        syntaxError("comparison operator");
    }
}

// Main parser function
void syntaxParser(const char* fileName) {
    inputFile = fopen(fileName, "r");
    if (!inputFile) {
        fprintf(stderr, "Error: Could not open file %s\n", fileName);
        exit(3);
    }

    printf("Cooke Parser ::\n");
    nextToken();
    parseProgram();

    if (errorCount == 0) {
        printf("Syntax Validated\n");
        fclose(inputFile);
        exit(0);
    } else {
        fclose(inputFile);
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No input file provided.\n");
        return 2;
    }

    syntaxParser(argv[1]);
    return 0;
}
