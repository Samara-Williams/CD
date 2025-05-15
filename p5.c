#include <stdio.h>
#include <string.h>
#include <ctype.h>

void printOperator(const char *op, int len) {
    char op_str[3] = {0};
    strncpy(op_str, op, len);
    
    switch (op_str[0]) {
        case '+':
            if (len == 1) {
                printf("Addition Operator: %s\n", op_str);
            }
            break;
        case '-':
            if (len == 1) {
                printf("Subtraction Operator: %s\n", op_str);
            }
            break;
        case '*':
            if (len == 1) {
                printf("Multiplication Operator: %s\n", op_str);
            }
            break;
        case '/':
            if (len == 1) {
                printf("Division Operator: %s\n", op_str);
            }
            break;
        case '=':
            if (len == 1) {
                printf("Assignment Operator: %s\n", op_str);
            } else if (len == 2 && op_str[1] == '=') {
                printf("Equality Operator: %s\n", op_str);
            }
            break;
        case '!':
            if (len == 2 && op_str[1] == '=') {
                printf("Not Equal Operator: %s\n", op_str);
            }
            break;
        case '<':
            if (len == 1) {
                printf("Less Than Operator: %s\n", op_str);
            } else if (len == 2 && op_str[1] == '=') {
                printf("Less Than or Equal Operator: %s\n", op_str);
            }
            break;
        case '>':
            if (len == 1) {
                printf("Greater Than Operator: %s\n", op_str);
            } else if (len == 2 && op_str[1] == '=') {
                printf("Greater Than or Equal Operator: %s\n", op_str);
            }
            break;
        default:
            printf("Unknown Operator: %s\n", op_str);
            break;
    }
}

void scanOperators(const char *input) {
    int pos = 0;
    while (input[pos] != '\0') {
        // Skip whitespace
        while (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n') {
            pos++;
        }

        // Skip comments
        if (input[pos] == '/' && input[pos + 1] == '*') {
            pos += 2;
            while (input[pos] != '\0' && !(input[pos] == '*' && input[pos + 1] == '/')) {
                pos++;
            }
            if (input[pos] == '*') {
                pos += 2; // Skip */
            }
            continue;
        }

        if (input[pos] == '\0') {
            break;
        }

        // Check for operators
        if (strchr("+-*/=<>", input[pos]) || input[pos] == '!') {
            if ((input[pos] == '=' && input[pos + 1] == '=') ||
                (input[pos] == '!' && input[pos + 1] == '=') ||
                (input[pos] == '<' && input[pos + 1] == '=') ||
                (input[pos] == '>' && input[pos + 1] == '=')) {
                printOperator(&input[pos], 2);
                pos += 2;
            } else {
                printOperator(&input[pos], 1);
                pos++;
            }
        } else {
            printf("Unknown: %c\n", input[pos]);
            pos++;
        }
    }
}

int main() {
    char input[256];
    printf("Enter input: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline

    scanOperators(input);
    return 0;
}