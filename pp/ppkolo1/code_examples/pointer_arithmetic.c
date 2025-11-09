#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int a[5] = {1, 4324, -382103, 3213, 321};

    printf("Wartość    | Adres          | Indeks\n");

    for (int i = 0; i < 5; i++) {
        printf("%-10d | %-10p | %-d\n", a[i], &a[i], i);
    }

    printf("------------------------------------\n");

    char *b = &a; // To jest zły pomysł!
    for (int i = 0; i < 20; i++) {
        printf("%-10d | %-10p | %-d\n", b[i], &b[i], i);
    }

    return EXIT_SUCCESS;
}
