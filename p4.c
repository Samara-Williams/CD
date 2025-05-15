#include <stdio.h>
#include <ctype.h>
#include <string.h>

int isValidIdentifier(const char *str) {
    if (!isalpha(str[0]) && str[0] != '_') return 0; // Must start with letter or underscore
    for (int i = 1; i < strlen(str); i++) {
        if (!isalnum(str[i]) && str[i] != '_') return 0; // Only letters, digits, or underscores
    }
    return 1;
}

int main() {
    char id[100];
    printf("Enter an identifier: ");
    scanf("%s", id);

    if (isValidIdentifier(id)) {
        printf("Valid identifier\n");
    } else {
        printf("Invalid identifier\n");
    }

    return 0;
}