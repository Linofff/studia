#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int a = 5;
    int *b = &a;

    printf("Wartość        | Adres\n");
    printf("%-14d | %-14p\n", a, &a);
    printf("%-14p | %-14p\n", b, &b);

    return EXIT_SUCCESS;
}
