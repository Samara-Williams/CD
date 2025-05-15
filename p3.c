#include <stdio.h>
#include <string.h>

int isA(const char *str) {
    return strlen(str) == 1 && str[0] == 'a';
}

int isAStarBPlus(const char *str) {
    int i = 0;
    while (str[i] == 'a') i++; // Consume zero or more 'a's
    if (i == strlen(str)) return 0; // No 'b's
    while (str[i] == 'b') i++; // Consume one or more 'b's
    return i == strlen(str) && str[i - 1] == 'b';
}

int isABB(const char *str) {
    return strlen(str) == 3 && str[0] == 'a' && str[1] == 'b' && str[2] == 'b';
}

int main() {
    char str[100];
    printf("Enter a string: ");
    scanf("%s", str);

    if (isA(str)) {
        printf("Matches 'a'\n");
    } else if (isAStarBPlus(str)) {
        printf("Matches 'a*b+'\n");
    } else if (isABB(str)) {
        printf("Matches 'abb'\n");
    } else {
        printf("Does not match any pattern\n");
    }

    return 0;
}