#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isComment(const char *line) {
    int i = 0;
    while (isspace(line[i])) i++; // Skip leading whitespace

    if (line[i] == '/' && line[i + 1] == '/') {
        return 1; // Line comment
    }
    if (line[i] == '/' && line[i + 1] == '*') {
        return 1; // Block comment start
    }
    return 0;
}

int main() {
    char line[256];
    printf("Enter a line: ");
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = '\0'; // Remove newline

    if (isComment(line)) {
        printf("This is a comment.\n");
    } else {
        printf("This is not a comment.\n");
    }

    return 0;
}