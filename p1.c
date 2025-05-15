#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_ID_LEN 32

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char lexeme[100];
};

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}

void getNextToken(const char *input, int *pos, struct Token *token) {
    while (input[*pos] == ' ' || input[*pos] == '\t' || input[*pos] == '\n') {
        (*pos)++; // Skip whitespace
    }

    // Handle comments
    if (input[*pos] == '/') {
        if (input[*pos + 1] == '/') {
            while (input[*pos] != '\n' && input[*pos] != '\0') (*pos)++; // Skip line comment
            getNextToken(input, pos, token); // Recurse for next token
            return;
        } else if (input[*pos + 1] == '*') {
            (*pos) += 2;
            while (input[*pos] != '\0' && !(input[*pos] == '*' && input[*pos + 1] == '/')) (*pos)++;
            if (input[*pos] == '*') (*pos) += 2; // Skip */
            getNextToken(input, pos, token); // Recurse for next token
            return;
        }
    }

    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->lexeme, "");
        return;
    }

    if (isalpha(input[*pos])) {
        int i = 0;
        token->lexeme[i++] = input[(*pos)++];
        while (isalnum(input[*pos]) && i < MAX_ID_LEN - 1) {
            token->lexeme[i++] = input[(*pos)++];
        }
        token->lexeme[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return;
    }

    if (isdigit(input[*pos])) {
        int i = 0;
        while (isdigit(input[*pos])) {
            token->lexeme[i++] = input[(*pos)++];
        }
        token->lexeme[i] = '\0';
        token->type = TOKEN_NUMBER;
        return;
    }

    if (isOperator(input[*pos])) {
        token->lexeme[0] = input[(*pos)++];
        token->lexeme[1] = '\0';
        token->type = TOKEN_OPERATOR;
        return;
    }

    token->lexeme[0] = input[(*pos)++];
    token->lexeme[1] = '\0';
    token->type = TOKEN_UNKNOWN;
}

int main() {
    const char *input = "int x = 42; // comment\n y = x + 5; /* multi-line\ncomment */";
    int pos = 0;
    struct Token token;

    do {
        getNextToken(input, &pos, &token);
        switch (token.type) {
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s\n", token.lexeme);
                break;
            case TOKEN_NUMBER:
                printf("Number: %s\n", token.lexeme);
                break;
            case TOKEN_OPERATOR:
                printf("Operator: %s\n", token.lexeme);
                break;
            case TOKEN_UNKNOWN:
                printf("Unknown: %s\n", token.lexeme);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    } while (token.type != TOKEN_EOF);

    return 0;
}