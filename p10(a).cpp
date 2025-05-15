#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

// Operator precedence table
//        +   -   *   /   i   (   )   $
// index  0   1   2   3   4   5   6   7
char precedence[8][8] = {
    // +   -   *   /   i   (   )   $
    { '>', '>', '<', '<', '<', '<', '>', '>' }, // +
    { '>', '>', '<', '<', '<', '<', '>', '>' }, // -
    { '>', '>', '>', '>', '<', '<', '>', '>' }, // *
    { '>', '>', '>', '>', '<', '<', '>', '>' }, // /
    { '>', '>', '>', '>', 'e', 'e', '>', '>' }, // i (id)
    { '<', '<', '<', '<', '<', '<', '=', 'e' }, // (
    { '>', '>', '>', '>', 'e', 'e', '>', '>' }, // )
    { '<', '<', '<', '<', '<', '<', 'e', 'a' }  // $
};

// Symbol list
char symbols[] = { '+', '-', '*', '/', 'i', '(', ')', '$' };

// Stack for parser
char stack[MAX];
int top = -1;

// Get index of symbol in table
int getIndex(char c) {
    for (int i = 0; i < 8; i++) {
        if (symbols[i] == c)
            return i;
    }
    return -1;
}

// Push symbol to stack
void push(char c) {
    stack[++top] = c;
}

// Pop symbol from stack
char pop() {
    return stack[top--];
}

// Get top terminal from stack (for precedence lookup)
char topTerminal() {
    for (int i = top; i >= 0; i--) {
        if (strchr(symbols, stack[i])) {
            return stack[i];
        }
    }
    return '\0';
}

// Try to reduce stack based on grammar
int reduceStack() {
    // i → E
    if (stack[top] == 'i') {
        pop();
        push('E');
        return 1;
    }

    // E + E or E - E or E * E or E / E → E
    if (top >= 2 && stack[top] == 'E' && strchr("+-*/", stack[top - 1]) && stack[top - 2] == 'E') {
        pop(); // E
        pop(); // operator
        pop(); // E
        push('E');
        return 1;
    }

    // ( E ) → E
    if (top >= 2 && stack[top] == ')' && stack[top - 1] == 'E' && stack[top - 2] == '(') {
        pop(); // )
        pop(); // E
        pop(); // (
        push('E');
        return 1;
    }

    return 0; // no valid reduction
}

void parse(char *input) {
    int i = 0;
    char curr;

    // Initialize stack with $
    stack[++top] = '$';
    curr = input[i++];

    while (1) {
        char top_sym = topTerminal();
        int row = getIndex(top_sym);
        int col = getIndex(curr);

        if (row == -1 || col == -1) {
            printf("Invalid symbol '%c'.\n", curr);
            return;
        }

        char relation = precedence[row][col];

        // Display state
        printf("Stack: ");
        for (int j = 0; j <= top; j++) printf("%c", stack[j]);
        printf("\tInput: %s\tAction: ", &input[i - 1]);

        if (relation == '<' || relation == '=') {
            printf("Shift %c\n", curr);
            push(curr);
            curr = input[i++];
        } else if (relation == '>') {
            printf("Reduce\n");
            if (!reduceStack()) {
                printf("Syntax Error during reduction.\n");
                return;
            }
        } else if (relation == 'a') {
            if (stack[top] == 'E' && stack[top - 1] == '$') {
                printf("Accept\n");
                break;
            } else {
                printf("Syntax Error: stack not in final state.\n");
                return;
            }
        } else {
            printf("Syntax Error: no rule for %c and %c\n", top_sym, curr);
            return;
        }
    }
}

int main() {
    char input[MAX];
    printf("Enter expression (use 'i' for id, end with $): ");
    scanf("%s", input);

    parse(input);
    return 0;
}
