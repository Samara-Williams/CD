#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK 100
#define MAX_INPUT 100

// Token types
enum TokenType { ID, PLUS, MULT, LPAREN, RPAREN, END, ERROR };

// Token structure
struct Token {
    enum TokenType type;
    char value;
};

// Stack structure
char stack[MAX_STACK];
int stack_top = -1;

// Input tokens
struct Token input[MAX_INPUT];
int input_pos = 0;

void push(char c) {
    if (stack_top < MAX_STACK - 1) {
        stack[++stack_top] = c;
    } else {
        printf("Stack overflow\n");
        exit(1);
    }
}

char pop() {
    if (stack_top >= 0) {
        return stack[stack_top--];
    }
    printf("Stack underflow\n");
    exit(1);
    return 0;
}

void print_stack() {
    printf("$");
    for (int i = 0; i <= stack_top; i++) {
        printf("%c", stack[i]);
    }
    printf("        ");
}

void print_input() {
    for (int i = input_pos; input[i].type != END; i++) {
        printf("%c", input[i].value);
    }
    printf("$");
}

void display_step() {
    print_stack();
    print_input();
    printf("\n");
}

int get_non_terminal_index(char symbol) {
    switch (symbol) {
        case 'E': return 0;
        case 'e': return 1; // E'
        case 'T': return 2;
        case 't': return 3; // T'
        case 'F': return 4;
        default: return -1;
    }
}

struct Token get_next_token(const char *str, int *pos) {
    struct Token token;
    while (str[*pos] == ' ' || str[*pos] == '\t' || str[*pos] == '\n') (*pos)++;

    if (str[*pos] == '\0') {
        token.type = END;
        token.value = '$';
    } else if (str[*pos] == 'i') {
        token.type = ID;
        token.value = 'i';
        (*pos)++;
    } else if (str[*pos] == '+') {
        token.type = PLUS;
        token.value = '+';
        (*pos)++;
    } else if (str[*pos] == '*') {
        token.type = MULT;
        token.value = '*';
        (*pos)++;
    } else if (str[*pos] == '(') {
        token.type = LPAREN;
        token.value = '(';
        (*pos)++;
    } else if (str[*pos] == ')') {
        token.type = RPAREN;
        token.value = ')';
        (*pos)++;
    } else {
        token.type = ERROR;
        token.value = str[*pos];
        (*pos)++;
    }
    return token;
}

void tokenize(const char *str) {
    int pos = 0;
    int token_idx = 0;
    while (str[pos] != '\0') {
        input[token_idx] = get_next_token(str, &pos);
        if (input[token_idx].type == ERROR) {
            printf("Invalid token: %c\n", input[token_idx].value);
            exit(1);
        }
        token_idx++;
    }
    input[token_idx].type = END;
    input[token_idx].value = '$';
}

void parse() {
    push('$');
    push('E');

    printf("Stack        INPUT\n");

    while (stack_top >= 0) {
        display_step();

        char top = stack[stack_top];
        struct Token current_token = input[input_pos];

        if (top == '$' && current_token.type == END) {
            printf("success\n");
            break;
        }

        if (top == 'i' || top == '+' || top == '*' || top == '(' || top == ')' || top == '$') {
            if (top == current_token.value) {
                pop();
                input_pos++;
            } else {
                printf("Error: Expected %c, found %c\n", top, current_token.value);
                exit(1);
            }
        } else {
            int nt = get_non_terminal_index(top);
            if (nt == -1) {
                printf("Error: Invalid symbol on stack\n");
                exit(1);
            }

            pop(); // Pop the non-terminal
            switch (nt) {
                case 0: // E -> T e
                    if (current_token.value == 'i' || current_token.value == '(') {
                        push('e'); push('T');
                    } else {
                        printf("Error: No production for E on %c\n", current_token.value);
                        exit(1);
                    }
                    break;
                case 1: // E' -> + T e | ε
                    if (current_token.value == '+') {
                        push('e'); push('T'); push('+');
                    } else if (current_token.value == ')' || current_token.value == '$') {
                        // ε production, do nothing
                    } else {
                        printf("Error: No production for E' on %c\n", current_token.value);
                        exit(1);
                    }
                    break;
                case 2: // T -> F t
                    if (current_token.value == 'i' || current_token.value == '(') {
                        push('t'); push('F');
                    } else {
                        printf("Error: No production for T on %c\n", current_token.value);
                        exit(1);
                    }
                    break;
                case 3: // T' -> * F t | ε
                    if (current_token.value == '*') {
                        push('t'); push('F'); push('*');
                    } else if (current_token.value == '+' || current_token.value == ')' || current_token.value == '$') {
                        // ε production, do nothing
                    } else {
                        printf("Error: No production for T' on %c\n", current_token.value);
                        exit(1);
                    }
                    break;
                case 4: // F -> i | ( E )
                    if (current_token.value == 'i') {
                        push('i');
                    } else if (current_token.value == '(') {
                        push(')'); push('E'); push('(');
                    } else {
                        printf("Error: No production for F on %c\n", current_token.value);
                        exit(1);
                    }
                    break;
            }
        }
    }
}

int main() {
    char input_str[MAX_INPUT];
    printf("Enter the input string: ");
    fgets(input_str, MAX_INPUT, stdin);
    input_str[strcspn(input_str, "\n")] = '\0';

    tokenize(input_str);
    parse();
    return 0;
}
