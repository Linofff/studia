#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char t[128] = {"GdanskUniversity"}, i = 15;

    printf("Wartości: |");
    for (int j = 0; t[j] != '\0'; j++) {
        printf(" %-2c |", t[j]);
    }

    printf(" \\0 \nIndeksy:  |");
    for (int j = 0; t[j] != '\0'; j++) {
        printf(" %-2d |", j);
    }

    printf(" 16\n-------------\n");

    for (; i; i /= 2) {
        printf("Indeks (i): %d, znak: %c\n", i, *(t + i));
    }

    return EXIT_SUCCESS;
}
