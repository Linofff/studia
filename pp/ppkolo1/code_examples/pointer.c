#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int a;

    printf("Wartość    | Adres\n");
    printf("%-10d | %-10p\n", a, &a);

    return EXIT_SUCCESS;
}
